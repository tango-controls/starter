DROP PROCEDURE IF EXISTS tango.ds_start;
DROP PROCEDURE IF EXISTS tango.import_event;
DROP PROCEDURE IF EXISTS tango.import_device;
DROP PROCEDURE IF EXISTS tango.class_prop;
DROP PROCEDURE IF EXISTS tango.dev_prop;
DROP PROCEDURE IF EXISTS tango.class_att_prop;
DROP PROCEDURE IF EXISTS tango.get_dev_list;
DROP PROCEDURE IF EXISTS tango.dev_att_prop;
DROP PROCEDURE IF EXISTS tango.obj_prop;


#########################################################
#														#
#			MAIN PROCEDURE								#
#														#
# Procedure input parameters:							#
# 1 - Device server name (executable/inst_name)			#
# 2 - Host name											#
# Procedure output parameters:							#
# 1 - A huge string with several elements and a			#
#	  separator set to 0 (binary 0)						#
#														#
#########################################################


#
# If you change something in these procedures, do not forget
# to also change the COMMENT part of the ds_start procedure
# CREATE command
#

DELIMITER |
CREATE PROCEDURE tango.ds_start 
(IN ds_name VARCHAR(255),
 IN host VARCHAR(255),
 OUT res_str BLOB) READS SQL DATA COMMENT 'release 1.3'
proc: BEGIN

	DECLARE notifd_event_name VARCHAR(255) DEFAULT 'notifd/factory/';
	DECLARE adm_dev_name VARCHAR(255) DEFAULT 'dserver/';
	DECLARE done, dev_nb,class_nb INT DEFAULT 0;
	DECLARE tmp_class,d_name VARCHAR(255);
	DECLARE dev_list BLOB;
	DECLARE start,pos INT DEFAULT 1;
	DECLARE class_nb_pos INT;
	DECLARE ca_dev_name VARCHAR(255);

	DECLARE cur_class_list CURSOR FOR 
	SELECT DISTINCT class 
	FROM tango.device 
	WHERE server = ds_name;
		
	DECLARE CONTINUE HANDLER FOR SQLSTATE '02000' SET done = 1;	
	DECLARE EXIT HANDLER FOR SQLEXCEPTION SET res_str = CONCAT_WS(CHAR(0),res_str,'MySQL Error');

	SET adm_dev_name = CONCAT(adm_dev_name,ds_name);

#
# import admin device
#

	CALL tango.import_device(adm_dev_name,res_str);

	IF LOCATE('Not Found',res_str) != 0 OR LOCATE('MySQL Error',res_str) != 0 THEN
		LEAVE proc;
	END IF;
	
#
# import event factory for notification service running on that host
#

	SET notifd_event_name = CONCAT(notifd_event_name,host);
	CALL tango.import_event(notifd_event_name,res_str);
	
	IF LOCATE('MySQL Error',res_str) != 0 THEN
		SET res_str = 'MySQL ERROR during import_event procedure for event factory';
		LEAVE proc;
	END IF;

#
# import event channel for this server
#
	
	CALL tango.import_event(adm_dev_name,res_str);
	
	IF LOCATE('MySQL Error',res_str) != 0 THEN
		SET res_str = 'MySQL ERROR during import_event procedure for DS event channel';
		LEAVE proc;
	END IF;
	
#
# Get all class properties for DServer class
#

	CALL tango.class_prop('DServer',res_str);
	
	IF LOCATE('MySQL Error',res_str) != 0 THEN
		SET res_str = 'MySQL ERROR while getting DServer class property(ies)';
		LEAVE proc;
	END IF;

#
# Get all class properties for Default class
#

	CALL tango.class_prop('Default',res_str);
	
	IF LOCATE('MySQL Error',res_str) != 0 THEN
		SET res_str = 'MySQL ERROR while getting Default class property(ies)';
		LEAVE proc;
	END IF;
	
#
# Get all device properties for admin device
#

	CALL tango.dev_prop(adm_dev_name,res_str);
#
#
#
	SET res_str = CONCAT_WS(CHAR(0),res_str,ds_name);
	SET class_nb_pos = LENGTH(res_str);		
#
# A loop for each class embedded within the server
#
	
	OPEN cur_class_list;

	REPEAT
		FETCH cur_class_list INTO tmp_class;
		IF NOT done THEN

			IF tmp_class != 'dserver' THEN
				SET class_nb = class_nb + 1;
						
				CALL tango.class_prop(tmp_class,res_str);
				
				IF LOCATE('MySQL Error',res_str) != 0 THEN
					SET res_str = 'MySQL ERROR while getting DS class(es) property(ies)';
					CLOSE cur_class_list;
					LEAVE proc;
				END IF;
				
				CALL tango.class_att_prop(tmp_class,res_str);
				
				IF LOCATE('MySQL Error',res_str) != 0 THEN
					SET res_str = 'MySQL ERROR while getting DS class(es) attribute(s) property(ies)';
					CLOSE cur_class_list;
					LEAVE proc;
				END IF;
				
				CALL tango.get_dev_list(tmp_class,ds_name,res_str,dev_list,dev_nb);
				
				IF LOCATE('MySQL Error',res_str) != 0 THEN
					SET res_str = 'MySQL ERROR while getting DS class(es) device list';
					CLOSE cur_class_list;
					LEAVE proc;
				END IF;

#
# A loop for each device in the class
#
				
				WHILE dev_nb > 0 DO
					SET pos = LOCATE(CHAR(0),dev_list,start);
					IF pos = 0 THEN
						SET d_name = SUBSTRING(dev_list,start);
					ELSE
						SET d_name = SUBSTRING(dev_list,start,pos-start);
						SET start = pos + 1;
					END IF;
#					select dev_list,d_name,pos,start;
										
					CALL tango.dev_prop(d_name,res_str);
					
					IF LOCATE('MySQL Error',res_str) != 0 THEN
						SET res_str = 'MySQL ERROR while getting device property(ies)';
						CLOSE cur_class_list;
						LEAVE proc;
					END IF;
					
					CALL tango.dev_att_prop(d_name,res_str);
					
					IF LOCATE('MySQL Error',res_str) != 0 THEN
						SET res_str = 'MySQL ERROR while getting device attribute property(ies)';
						CLOSE cur_class_list;
						LEAVE proc;
					END IF;
					
					SET dev_nb = dev_nb - 1;
				END WHILE;
				SET start = 1;
			END IF;
		END IF;
	UNTIL done END REPEAT;
	
	CLOSE cur_class_list;
	
	SET res_str = INSERT(res_str,class_nb_pos+1,1,CONCAT_WS(CONCAT(class_nb),CHAR(0),CHAR(0)));
	
#
# Get service(s) property
#

	SET ca_dev_name = 'Empty';
	
	CALL tango.obj_prop('CtrlSystem',ca_dev_name,res_str);
	
	IF ca_dev_name != 'Empty' THEN

#
# import control access service device
#

		CALL tango.import_device(ca_dev_name,res_str);

	END IF;
	
	
END proc|

#########################################################
#														#
#		IMPORT EVENT PROCEDURE							#
#														#
#########################################################


CREATE PROCEDURE tango.import_event 
(IN ev_name VARCHAR(255),
 INOUT res_str BLOB) READS SQL DATA
BEGIN
	DECLARE tmp_ior TEXT;
	DECLARE tmp_version VARCHAR(8);
	DECLARE tmp_host VARCHAR(255);
	DECLARE tmp_ev_name VARCHAR(255);
	DECLARE tmp_exp, tmp_pid INT;
	DECLARE not_found INT DEFAULT 0;
	
	DECLARE CONTINUE HANDLER FOR NOT FOUND SET not_found = 1;
	DECLARE EXIT HANDLER FOR SQLEXCEPTION SET res_str = CONCAT_WS(CHAR(0),res_str,'MySQL Error');

	SET tmp_ev_name = ev_name;
	SET tmp_ev_name = REPLACE(tmp_ev_name,'_','\_');
		
	SELECT exported,ior,version,pid,host
	INTO tmp_exp,tmp_ior,tmp_version,tmp_pid,tmp_host
	FROM tango.event
	WHERE name = tmp_ev_name;
	
	IF not_found = 1 THEN
		SET res_str = CONCAT_WS(CHAR(0),res_str,ev_name,'Not Found');
	ELSE
		SET res_str = CONCAT_WS(CHAR(0),res_str,ev_name,tmp_ior,tmp_version,tmp_host,CONCAT(tmp_exp),CONCAT(tmp_pid));
	END IF;
END |


#########################################################
#														#
#		IMPORT DEVICE PROCEDURE							#
#														#
#########################################################


CREATE PROCEDURE tango.import_device 
(IN dev_name VARCHAR(255),
 INOUT res_str BLOB) READS SQL DATA
imp_proc: BEGIN
	DECLARE tmp_ior TEXT;
	DECLARE tmp_version VARCHAR(8);
	DECLARE tmp_host,tmp_server,tmp_class VARCHAR(255);
	DECLARE tmp_exp, tmp_pid INT;
	DECLARE not_found INT DEFAULT 0;
	
	DECLARE CONTINUE HANDLER FOR NOT FOUND SET not_found = 1;
	DECLARE EXIT HANDLER FOR SQLEXCEPTION SET res_str = CONCAT_WS(CHAR(0),res_str,'MySQL Error');
		
	SELECT exported,ior,version,pid,server,host,class
	INTO tmp_exp,tmp_ior,tmp_version,tmp_pid,tmp_server,tmp_host,tmp_class
	FROM tango.device 
	WHERE name = dev_name;
	
	SET res_str = CONCAT_WS(CHAR(0),res_str,dev_name);
	
	IF not_found = 1 THEN
		SET res_str = CONCAT_WS(CHAR(0),res_str,'Not Found');
		LEAVE imp_proc;
	END IF;
	
	IF tmp_ior IS NULL THEN
		SET res_str = CONCAT_WS(CHAR(0),res_str,'');
	ELSE
		SET res_str = CONCAT_WS(CHAR(0),res_str,tmp_ior);
	END IF;

	IF tmp_version IS NULL THEN
		SET res_str = CONCAT_WS(CHAR(0),res_str,'');
	ELSE
		SET res_str = CONCAT_WS(CHAR(0),res_str,tmp_version);
	END IF;
			
	SET res_str = CONCAT_WS(CHAR(0),res_str,tmp_server,tmp_host,CONCAT(tmp_exp));
	
	IF tmp_pid IS NULL THEN
		SET res_str = CONCAT_WS(CHAR(0),res_str,'');
	ELSE
		SET res_str = CONCAT_WS(CHAR(0),res_str,CONCAT(tmp_pid));
	END IF;

	SET res_str = CONCAT_WS(CHAR(0),res_str,tmp_class);	
END imp_proc |


#########################################################
#														#
#		GET CLASS PROPERTIES PROCEDURE					#
#														#
#########################################################


CREATE PROCEDURE tango.class_prop 
(IN class_name VARCHAR(255),
 INOUT res_str BLOB) READS SQL DATA
BEGIN
	DECLARE tmp_name VARCHAR(255);
	DECLARE tmp_value TEXT;
	DECLARE tmp_count INT;
	DECLARE done,prop_nb,prop_elt_nb INT DEFAULT 0;
	DECLARE class_name_pos,prop_name_pos INT;
	
	DECLARE cur_class CURSOR FOR 
	SELECT name,count,value 
	FROM tango.property_class 
	WHERE class = class_name ORDER BY name,count;
	
	DECLARE CONTINUE HANDLER FOR SQLSTATE '02000' SET done = 1;
	DECLARE EXIT HANDLER FOR SQLEXCEPTION SET res_str = CONCAT_WS(CHAR(0),res_str,'MySQL Error');

	SET res_str = CONCAT_WS(CHAR(0),res_str,class_name);
	SET class_name_pos = LENGTH(res_str);
	OPEN cur_class;

	REPEAT
		FETCH cur_class INTO tmp_name,tmp_count,tmp_value;
		IF NOT done THEN
		
			IF tmp_count = 1 THEN
				IF prop_elt_nb != 0 THEN
					SET res_str = INSERT(res_str,prop_name_pos+1,1,CONCAT_WS(CONCAT(prop_elt_nb),CHAR(0),CHAR(0)));
				END IF;
				SET res_str = CONCAT_WS(CHAR(0),res_str,tmp_name);
				SET prop_name_pos = LENGTH(res_str);
				SET prop_nb = prop_nb + 1;
				SET prop_elt_nb = 0;
			END IF;
						
			SET res_str = CONCAT_WS(CHAR(0),res_str,tmp_value);
			SET prop_elt_nb = prop_elt_nb + 1;
		END IF;
	UNTIL done END REPEAT;
	
	CLOSE cur_class;

	IF prop_nb != 0 THEN	
		SET res_str = INSERT(res_str,prop_name_pos+1,1,CONCAT_WS(CONCAT(prop_elt_nb),CHAR(0),CHAR(0)));
	END IF;

	IF prop_nb != 0 THEN	
		SET res_str = INSERT(res_str,class_name_pos+1,1,CONCAT_WS(CONCAT(prop_nb),CHAR(0),CHAR(0)));
	ELSE
		SET res_str = CONCAT_WS(CHAR(0),res_str,0);
	END IF;
END |


#########################################################
#														#
#		GET DEVICE PROPERTIES PROCEDURE					#
#														#
#########################################################


CREATE PROCEDURE tango.dev_prop 
(IN dev_name VARCHAR(255),
 INOUT res_str BLOB) READS SQL DATA
BEGIN
	DECLARE tmp_name VARCHAR(255);
	DECLARE tmp_value TEXT;
	DECLARE tmp_count INT;
	DECLARE done,prop_nb,prop_elt_nb INT DEFAULT 0;
	DECLARE dev_name_pos,prop_name_pos INT;
	
	DECLARE cur_dev CURSOR FOR 
	SELECT name,count,value 
	FROM tango.property_device 
	WHERE device = dev_name ORDER BY name,count;
	
	DECLARE CONTINUE HANDLER FOR SQLSTATE '02000' SET done = 1;
	DECLARE EXIT HANDLER FOR SQLEXCEPTION SET res_str = CONCAT_WS(CHAR(0),res_str,'MySQL Error');
	
	SET res_str = CONCAT_WS(CHAR(0),res_str,dev_name);
	SET dev_name_pos = LENGTH(res_str);

	OPEN cur_dev;

	REPEAT
		FETCH cur_dev INTO tmp_name,tmp_count,tmp_value;
		IF NOT done THEN
		
			IF tmp_count = 1 THEN
				IF prop_elt_nb != 0 THEN
					SET res_str = INSERT(res_str,prop_name_pos+1,1,CONCAT_WS(CONCAT(prop_elt_nb),CHAR(0),CHAR(0)));
				END IF;
				SET res_str = CONCAT_WS(CHAR(0),res_str,tmp_name);
				SET prop_name_pos = LENGTH(res_str);
				SET prop_nb = prop_nb + 1;
				SET prop_elt_nb = 0;
			END IF;
						
			SET res_str = CONCAT_WS(CHAR(0),res_str,tmp_value);
			SET prop_elt_nb = prop_elt_nb + 1;
		END IF;
	UNTIL done END REPEAT;
	
	CLOSE cur_dev;

	IF prop_nb != 0 THEN	
		SET res_str = INSERT(res_str,prop_name_pos+1,1,CONCAT_WS(CONCAT(prop_elt_nb),CHAR(0),CHAR(0)));
	END IF;

	IF prop_nb != 0 THEN	
		SET res_str = INSERT(res_str,dev_name_pos+1,1,CONCAT_WS(CONCAT(prop_nb),CHAR(0),CHAR(0)));
	ELSE
		SET res_str = CONCAT_WS(CHAR(0),res_str,0);
	END IF;
END |


#########################################################
#														#
#		GET CLASS ATTRIBUTE PROPERTIES PROCEDURE		#
#														#
#########################################################


CREATE PROCEDURE tango.class_att_prop 
(IN class_name VARCHAR(255),
 INOUT res_str BLOB) READS SQL DATA
BEGIN
	DECLARE tmp_name,tmp_attribute VARCHAR(255);
	DECLARE known_att VARCHAR(255) DEFAULT '';
	DECLARE tmp_value TEXT;
	DECLARE tmp_count INT;
	DECLARE done,prop_nb,att_nb,prop_elt_nb INT DEFAULT 0;
	DECLARE class_name_pos,att_name_pos,prop_name_pos INT;
	
	DECLARE cur_class_att_prop CURSOR FOR 
	SELECT attribute,name,count,value 
	FROM tango.property_attribute_class 
	WHERE class = class_name ORDER BY attribute,name,count;
	
	DECLARE CONTINUE HANDLER FOR SQLSTATE '02000' SET done = 1;
	DECLARE EXIT HANDLER FOR SQLEXCEPTION SET res_str = CONCAT_WS(CHAR(0),res_str,'MySQL Error');

	SET res_str = CONCAT_WS(CHAR(0),res_str,class_name);
	SET class_name_pos = LENGTH(res_str);

	OPEN cur_class_att_prop;

	REPEAT
		FETCH cur_class_att_prop INTO tmp_attribute,tmp_name,tmp_count,tmp_value;
		IF NOT done THEN
		
			IF tmp_attribute != known_att THEN
				IF prop_nb != 0 THEN
					SET res_str = INSERT(res_str,att_name_pos+1,1,CONCAT_WS(CONCAT(prop_nb),CHAR(0),CHAR(0)));
					IF prop_nb < 10 THEN
						SET prop_name_pos = prop_name_pos + 2;
					ELSEIF prop_nb < 100 THEN
						SET prop_name_pos = prop_name_pos + 3;
					ELSE
						SET prop_name_pos = prop_name_pos + 4;
					END IF;
				END IF;
				SET known_att = tmp_attribute;
				SET res_str = CONCAT_WS(CHAR(0),res_str,tmp_attribute);
				SET att_name_pos = LENGTH(res_str);
				SET att_nb = att_nb + 1;
				SET prop_nb = 0;
			END IF;
			
			IF tmp_count = 1 THEN
				IF prop_elt_nb != 0 THEN
					SET res_str = INSERT(res_str,prop_name_pos+1,1,CONCAT_WS(CONCAT(prop_elt_nb),CHAR(0),CHAR(0)));
					IF prop_nb = 0 THEN
						IF prop_elt_nb < 10 THEN
							SET att_name_pos = att_name_pos + 2;
						ELSEIF prop_elt_nb < 100 THEN
							SET att_name_pos = att_name_pos + 3;
						ELSE
							SET att_name_pos = att_name_pos + 4;
						END IF;
					END IF;
				END IF;
				SET res_str = CONCAT_WS(CHAR(0),res_str,tmp_name);
				SET prop_name_pos = LENGTH(res_str);
				SET prop_nb = prop_nb + 1;
				SET prop_elt_nb = 0;
			END IF;
						
			SET res_str = CONCAT_WS(CHAR(0),res_str,tmp_value);
			SET prop_elt_nb = prop_elt_nb + 1;
		END IF;
	UNTIL done END REPEAT;
	
	CLOSE cur_class_att_prop;

	IF prop_nb != 0 THEN	
		SET res_str = INSERT(res_str,att_name_pos+1,1,CONCAT_WS(CONCAT(prop_nb),CHAR(0),CHAR(0)));
		IF prop_nb < 10 THEN
			SET prop_name_pos = prop_name_pos + 2;
		ELSEIF prop_nb < 100 THEN
			SET prop_name_pos = prop_name_pos + 3;
		ELSE
			SET prop_name_pos = prop_name_pos + 4;
		END IF;	
		IF prop_elt_nb != 0 THEN
			SET res_str = INSERT(res_str,prop_name_pos+1,1,CONCAT_WS(CONCAT(prop_elt_nb),CHAR(0),CHAR(0)));
		END IF;
	END IF;

	IF att_nb != 0 THEN	
		SET res_str = INSERT(res_str,class_name_pos+1,1,CONCAT_WS(CONCAT(att_nb),CHAR(0),CHAR(0)));
	ELSE
		SET res_str = CONCAT_WS(CHAR(0),res_str,0);
	END IF;
END |


#########################################################
#														#
#		GET DEVICE ATTRIBUTE PROPERTIES PROCEDURE		#
#														#
#########################################################


CREATE PROCEDURE tango.dev_att_prop 
(IN dev_name VARCHAR(255),
 INOUT res_str BLOB) READS SQL DATA
BEGIN
	DECLARE tmp_name,tmp_attribute VARCHAR(255);
	DECLARE known_att VARCHAR(255) DEFAULT '';
	DECLARE tmp_value TEXT;
	DECLARE tmp_count INT;
	DECLARE done,prop_nb,att_nb,prop_elt_nb INT DEFAULT 0;
	DECLARE dev_name_pos,att_name_pos,prop_name_pos INT;
	
	DECLARE cur_dev_att_prop CURSOR FOR 
	SELECT attribute,name,count,value 
	FROM tango.property_attribute_device 
	WHERE device = dev_name ORDER BY attribute,name,count;
	
	DECLARE CONTINUE HANDLER FOR SQLSTATE '02000' SET done = 1;
	DECLARE EXIT HANDLER FOR SQLEXCEPTION SET res_str = CONCAT_WS(CHAR(0),res_str,'MySQL Error');

	SET res_str = CONCAT_WS(CHAR(0),res_str,dev_name);
	SET dev_name_pos = LENGTH(res_str);

	OPEN cur_dev_att_prop;

	REPEAT
		FETCH cur_dev_att_prop INTO tmp_attribute,tmp_name,tmp_count,tmp_value;
		IF NOT done THEN
		
			IF tmp_attribute != known_att THEN
				IF prop_nb != 0 THEN
					SET res_str = INSERT(res_str,att_name_pos+1,1,CONCAT_WS(CONCAT(prop_nb),CHAR(0),CHAR(0)));
					IF prop_nb < 10 THEN
						SET prop_name_pos = prop_name_pos + 2;
					ELSEIF prop_nb < 100 THEN
						SET prop_name_pos = prop_name_pos + 3;
					ELSE
						SET prop_name_pos = prop_name_pos + 4;
					END IF;
				END IF;
				SET known_att = tmp_attribute;
				SET res_str = CONCAT_WS(CHAR(0),res_str,tmp_attribute);
				SET att_name_pos = LENGTH(res_str);
				SET att_nb = att_nb + 1;
				SET prop_nb = 0;
			END IF;
			
			IF tmp_count = 1 THEN
				IF prop_elt_nb != 0 THEN
					SET res_str = INSERT(res_str,prop_name_pos+1,1,CONCAT_WS(CONCAT(prop_elt_nb),CHAR(0),CHAR(0)));
					IF prop_nb = 0 THEN
						IF prop_elt_nb < 10 THEN
							SET att_name_pos = att_name_pos + 2;
						ELSEIF prop_elt_nb < 100 THEN
							SET att_name_pos = att_name_pos + 3;
						ELSE
							SET att_name_pos = att_name_pos + 4;
						END IF;
					END IF;
				END IF;
				SET res_str = CONCAT_WS(CHAR(0),res_str,tmp_name);
				SET prop_name_pos = LENGTH(res_str);
				SET prop_nb = prop_nb + 1;
				SET prop_elt_nb = 0;
			END IF;
						
			SET res_str = CONCAT_WS(CHAR(0),res_str,tmp_value);
			SET prop_elt_nb = prop_elt_nb + 1;
		END IF;
	UNTIL done END REPEAT;
	
	CLOSE cur_dev_att_prop;

	IF prop_nb != 0 THEN	
		SET res_str = INSERT(res_str,att_name_pos+1,1,CONCAT_WS(CONCAT(prop_nb),CHAR(0),CHAR(0)));
		IF prop_nb < 10 THEN
			SET prop_name_pos = prop_name_pos + 2;
		ELSEIF prop_nb < 100 THEN
			SET prop_name_pos = prop_name_pos + 3;
		ELSE
			SET prop_name_pos = prop_name_pos + 4;
		END IF;	
		IF prop_elt_nb != 0 THEN
			SET res_str = INSERT(res_str,prop_name_pos+1,1,CONCAT_WS(CONCAT(prop_elt_nb),CHAR(0),CHAR(0)));
		END IF;
	END IF;

	IF att_nb != 0 THEN	
		SET res_str = INSERT(res_str,dev_name_pos+1,1,CONCAT_WS(CONCAT(att_nb),CHAR(0),CHAR(0)));
	ELSE
		SET res_str = CONCAT_WS(CHAR(0),res_str,0);
	END IF;
END |


#########################################################
#														#
#		GET DEVICE LIST PROCEDURE						#
#														#
#########################################################


CREATE PROCEDURE tango.get_dev_list 
(IN class_name VARCHAR(255), IN serv VARCHAR(255),
 INOUT res_str BLOB, OUT d_list TEXT, OUT d_num INT) READS SQL DATA
BEGIN
	DECLARE tmp_name VARCHAR(255);
	DECLARE done INT DEFAULT 0;
	DECLARE nb_dev INT DEFAULT 0;
	DECLARE class_name_pos INT;
		
	DECLARE cur_dev_list CURSOR FOR 
	SELECT name 
	FROM tango.device 
	WHERE class = class_name AND server = serv;
	
	DECLARE CONTINUE HANDLER FOR SQLSTATE '02000' SET done = 1;
	DECLARE EXIT HANDLER FOR SQLEXCEPTION SET res_str = CONCAT_WS(CHAR(0),res_str,'MySQL Error');

	SET res_str = CONCAT_WS(CHAR(0),res_str,class_name);
	SET class_name_pos = LENGTH(res_str);
	
	OPEN cur_dev_list;

	REPEAT
		FETCH cur_dev_list INTO tmp_name;
		IF NOT done THEN						
			SET res_str = CONCAT_WS(CHAR(0),res_str,tmp_name);
			IF nb_dev = 0 THEN
				SET d_list = CONCAT_WS("",d_list,tmp_name);
			ELSE
				SET d_list = CONCAT_WS(CHAR(0),d_list,tmp_name);
			END IF;
			SET nb_dev = nb_dev + 1;
		END IF;
	UNTIL done END REPEAT;
	
	CLOSE cur_dev_list;
	
	SET res_str = INSERT(res_str,class_name_pos+1,1,CONCAT_WS(CONCAT(nb_dev),CHAR(0),CHAR(0)));
	SET d_num = nb_dev;
END |

#########################################################
#														#
#		GET OBJECT PROPERTIES PROCEDURE					#
#														#
#########################################################


CREATE PROCEDURE tango.obj_prop 
(IN obj_name VARCHAR(255),OUT serv_dev_name VARCHAR(255),
 INOUT res_str BLOB) READS SQL DATA
BEGIN
	DECLARE tmp_name VARCHAR(255);
	DECLARE tmp_value TEXT;
	DECLARE tmp_count INT;
	DECLARE done,prop_nb,prop_elt_nb INT DEFAULT 0;
	DECLARE dev_name_pos,prop_name_pos INT;
	DECLARE serv_defined INT DEFAULT 0;
	
	DECLARE cur_dev CURSOR FOR 
	SELECT name,count,value 
	FROM tango.property 
	WHERE object = obj_name ORDER BY count;
	
	DECLARE CONTINUE HANDLER FOR SQLSTATE '02000' SET done = 1;
	DECLARE EXIT HANDLER FOR SQLEXCEPTION SET res_str = CONCAT_WS(CHAR(0),res_str,'MySQL Error');

	SET res_str = CONCAT_WS(CHAR(0),res_str,obj_name);
	SET dev_name_pos = LENGTH(res_str);
	
	OPEN cur_dev;

	REPEAT
		FETCH cur_dev INTO tmp_name,tmp_count,tmp_value;
		IF NOT done THEN
		
			IF tmp_count = 1 THEN
				IF prop_elt_nb != 0 THEN
					SET res_str = INSERT(res_str,prop_name_pos+1,1,CONCAT_WS(CONCAT(prop_elt_nb),CHAR(0),CHAR(0)));
				END IF;
				SET res_str = CONCAT_WS(CHAR(0),res_str,tmp_name);
				SET prop_name_pos = LENGTH(res_str);
				SET prop_nb = prop_nb + 1;
				SET prop_elt_nb = 0;
				IF tmp_name = 'Services' THEN
					SET serv_defined = 1;
				ELSE
					SET serv_defined = 0;
				END IF;
			END IF;

			IF serv_defined = 1 THEN
				IF LOCATE('AccessControl/tango:',tmp_value) != 0 THEN
					SET serv_dev_name = SUBSTRING(tmp_value,21);
				END IF;
			END IF;
								
			SET res_str = CONCAT_WS(CHAR(0),res_str,tmp_value);
			SET prop_elt_nb = prop_elt_nb + 1;
		END IF;
	UNTIL done END REPEAT;
	
	CLOSE cur_dev;

	IF prop_nb != 0 THEN	
		SET res_str = INSERT(res_str,prop_name_pos+1,1,CONCAT_WS(CONCAT(prop_elt_nb),CHAR(0),CHAR(0)));
	END IF;

	IF prop_nb != 0 THEN	
		SET res_str = INSERT(res_str,dev_name_pos+1,1,CONCAT_WS(CONCAT(prop_nb),CHAR(0),CHAR(0)));
	ELSE
		SET res_str = CONCAT_WS(CHAR(0),res_str,0);
	END IF;
END |

DELIMITER ;
