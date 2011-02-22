USE tango;
#
# Table structure for table 'access_address'
#
DROP TABLE IF EXISTS access_address;
CREATE TABLE access_address (
  user varchar(255) default NULL,
  address varchar(255) default NULL,
  netmask varchar(255) default 'FF.FF.FF.FF',
  updated timestamp(14) NOT NULL,
  accessed timestamp(14) NOT NULL default '00000000000000'
) TYPE=MyISAM;



#
# Table structure for table 'access_device'
#
DROP TABLE IF EXISTS access_device;
CREATE TABLE access_device (
  user varchar(255) default NULL,
  device varchar(255) default NULL,
  rights varchar(255) default NULL,
  updated timestamp(14) NOT NULL,
  accessed timestamp(14) NOT NULL default '00000000000000'
) TYPE=MyISAM;



#
#	Create default user access
#
INSERT INTO access_address VALUES ('*','*.*.*.*','FF.FF.FF.FF',20060824131221,00000000000000);
INSERT INTO access_device VALUES ('*','*/*/*','write',20060824131221,00000000000000);



#
#	Create default DServer clsss access
#
DELETE FROM property_class WHERE class = 'DServer' and name = 'AllowedAccessCmd';

INSERT INTO property_class VALUES('DServer','AllowedAccessCmd',1,'QueryClass',NULL,NULL,NULL);
INSERT INTO property_class VALUES('DServer','AllowedAccessCmd',2,'QueryDevice',NULL,NULL,NULL);
INSERT INTO property_class VALUES('DServer','AllowedAccessCmd',3,'EventSubscriptionChange',NULL,NULL,NULL);
INSERT INTO property_class VALUES('DServer','AllowedAccessCmd',4,'DevPollStatus',NULL,NULL,NULL);
INSERT INTO property_class VALUES('DServer','AllowedAccessCmd',5,'GetLoggingLevel',NULL,NULL,NULL);
INSERT INTO property_class VALUES('DServer','AllowedAccessCmd',6,'GetLoggingTarget',NULL,NULL,NULL);
INSERT INTO property_class VALUES('DServer','AllowedAccessCmd',7,'QueryWizardDevProperty',NULL,NULL,NULL);
INSERT INTO property_class VALUES('DServer','AllowedAccessCmd',8,'QueryWizardClassProperty',NULL,NULL,NULL);



#
#	Create default Starter clsss access
#
DELETE FROM property_class WHERE class = 'Starter' and name = 'AllowedAccessCmd';

INSERT INTO property_class VALUES ('Starter','AllowedAccessCmd',1,'DevReadLog',NULL,NULL,NULL);
INSERT INTO property_class VALUES ('Starter','AllowedAccessCmd',2,'DevStart',NULL,NULL,NULL);
INSERT INTO property_class VALUES ('Starter','AllowedAccessCmd',3,'DevGetRunningServers',NULL,NULL,NULL);
INSERT INTO property_class VALUES ('Starter','AllowedAccessCmd',4,'DevGetStopServers',NULL,NULL,NULL);



#
#	Create default Database clsss access
#
DELETE FROM property_class WHERE class = 'Database' and name = 'AllowedAccessCmd';


INSERT INTO property_class VALUES('Database','AllowedAccessCmd',1,'DbGetServerInfo',NULL,NULL,NULL);
INSERT INTO property_class VALUES('Database','AllowedAccessCmd',2,'DbGetServerNameList',NULL,NULL,NULL);
INSERT INTO property_class VALUES('Database','AllowedAccessCmd',3,'DbGetInstanceNameList',NULL,NULL,NULL);
INSERT INTO property_class VALUES('Database','AllowedAccessCmd',4,'DbGetDeviceServerClassList',NULL,NULL,NULL);
INSERT INTO property_class VALUES('Database','AllowedAccessCmd',5,'DbGetDeviceList',NULL,NULL,NULL);
INSERT INTO property_class VALUES('Database','AllowedAccessCmd',6,'DbGetDeviceDomainList',NULL,NULL,NULL);
INSERT INTO property_class VALUES('Database','AllowedAccessCmd',7,'DbGetDeviceFamilyList',NULL,NULL,NULL);
INSERT INTO property_class VALUES('Database','AllowedAccessCmd',8,'DbGetDeviceMemberList',NULL,NULL,NULL);
INSERT INTO property_class VALUES('Database','AllowedAccessCmd',9,'DbGetClassList',NULL,NULL,NULL);
INSERT INTO property_class VALUES('Database','AllowedAccessCmd',10,'DbGetDeviceAliasList',NULL,NULL,NULL);
INSERT INTO property_class VALUES('Database','AllowedAccessCmd',11,'DbGetObjectList',NULL,NULL,NULL);
INSERT INTO property_class VALUES('Database','AllowedAccessCmd',12,'DbGetPropertyList',NULL,NULL,NULL);
INSERT INTO property_class VALUES('Database','AllowedAccessCmd',13,'DbGetProperty',NULL,NULL,NULL);
INSERT INTO property_class VALUES('Database','AllowedAccessCmd',14,'DbGetClassPropertyList',NULL,NULL,NULL);
INSERT INTO property_class VALUES('Database','AllowedAccessCmd',15,'DbGetClasProperty',NULL,NULL,NULL);
INSERT INTO property_class VALUES('Database','AllowedAccessCmd',16,'DbGetDevicePropertyList',NULL,NULL,NULL);
INSERT INTO property_class VALUES('Database','AllowedAccessCmd',17,'DbGetDeviceProperty',NULL,NULL,NULL);
INSERT INTO property_class VALUES('Database','AllowedAccessCmd',18,'DbGetClassAttributeList',NULL,NULL,NULL);
INSERT INTO property_class VALUES('Database','AllowedAccessCmd',19,'DbGetDeviceAttributeProperty',NULL,NULL,NULL);
INSERT INTO property_class VALUES('Database','AllowedAccessCmd',20,'DbGetDeviceAttributeProperty2',NULL,NULL,NULL);
INSERT INTO property_class VALUES('Database','AllowedAccessCmd',21,'DbGetLoggingLevel',NULL,NULL,NULL);
INSERT INTO property_class VALUES('Database','AllowedAccessCmd',22,'DbGetAliasDevice',NULL,NULL,NULL);
INSERT INTO property_class VALUES('Database','AllowedAccessCmd',23,'DbGetClassForDevice',NULL,NULL,NULL);
INSERT INTO property_class VALUES('Database','AllowedAccessCmd',24,'DbGetClassInheritanceForDevice',NULL,NULL,NULL);
INSERT INTO property_class VALUES('Database','AllowedAccessCmd',25,'DbGetDataForServerCache',NULL,NULL,NULL);
INSERT INTO property_class VALUES('Database','AllowedAccessCmd',26,'DbInfo',NULL,NULL,NULL);
INSERT INTO property_class VALUES('Database','AllowedAccessCmd',27,'DbGetClassAttributeProperty',NULL,NULL,NULL);
INSERT INTO property_class VALUES('Database','AllowedAccessCmd',28,'DbGetClassAttributeProperty2',NULL,NULL,NULL);
INSERT INTO property_class VALUES('Database','AllowedAccessCmd',29,'DbGetDeviceInfo',NULL,NULL,NULL);



#
#	Create default TangoAccessControl clsss access
#
DELETE FROM property_class WHERE class = 'TangoAccessControl' and name = 'AllowedAccessCmd';

INSERT INTO property_class VALUES ('TangoAccessControl','AllowedAccessCmd',1,'GetUsers',NULL,NULL,NULL);
INSERT INTO property_class VALUES ('TangoAccessControl','AllowedAccessCmd',2,'GetAddressByUser',NULL,NULL,NULL);
INSERT INTO property_class VALUES ('TangoAccessControl','AllowedAccessCmd',3,'GetDeviceByUser',NULL,NULL,NULL);
INSERT INTO property_class VALUES ('TangoAccessControl','AllowedAccessCmd',4,'GetAccess',NULL,NULL,NULL);
INSERT INTO property_class VALUES ('TangoAccessControl','AllowedAccessCmd',5,'GetAllowedCommandClassList',NULL,NULL,NULL);


#
#	Create TangoAccessControl server  instance
#
INSERT INTO device VALUES ('dserver/TangoAccessControl/1',NULL,'dserver','TangoAccessControl','1',0,'','','TangoAccessControl/1',14891,'DServer','3','2006-09-14 16:06:20','2006-09-14 16:04:32',NULL);
INSERT INTO device VALUES ('sys/access-control/1',NULL,'sys','access','control',0,'','','TangoAccessControl/1',0,'TangoAccessControl','3','2006-09-14 16:06:20','2006-09-14 16:04:32',NULL);
INSERT INTO server VALUES ('tangoaccesscontrol/1','',1,1);
