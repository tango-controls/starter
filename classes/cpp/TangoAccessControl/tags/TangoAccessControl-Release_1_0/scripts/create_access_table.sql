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
#	Delete default DServer clsss access if exist
#
DELETE FROM property_class where class='DServer' AND name='AllowedAccessCmd' AND value='EventSubscriptionChange';
DELETE FROM property_class where class='DServer' AND name='AllowedAccessCmd' AND value='QueryDevice';
DELETE FROM property_class where class='DServer' AND name='AllowedAccessCmd' AND value='QueryClass';
DELETE FROM property_class where class='DServer' AND name='AllowedAccessCmd' AND value='DevPollStatus';
DELETE FROM property_class where class='DServer' AND name='AllowedAccessCmd' AND value='GetLoggingLevel';
DELETE FROM property_class where class='DServer' AND name='AllowedAccessCmd' AND value='GetLoggingTarget';
DELETE FROM property_class where class='DServer' AND name='AllowedAccessCmd' AND value='QueryWizardDevProperty';
DELETE FROM property_class where class='DServer' AND name='AllowedAccessCmd' AND value='QueryWizardClassProperty';
#
#	Create default DServer clsss access
#
INSERT INTO property_class VALUES ('DServer','AllowedAccessCmd',3,'EventSubscriptionChange',20060912161031,20060912161031,NULL);
INSERT INTO property_class VALUES ('DServer','AllowedAccessCmd',2,'QueryDevice',20060912161031,20060912161031,NULL);
INSERT INTO property_class VALUES ('DServer','AllowedAccessCmd',1,'QueryClass',20060912161031,20060912161031,NULL);
INSERT INTO property_class VALUES ('DServer','AllowedAccessCmd',4,'DevPollStatus',20060912161031,20060912161031,NULL);
INSERT INTO property_class VALUES ('DServer','AllowedAccessCmd',5,'GetLoggingLevel',20060912161031,20060912161031,NULL);
INSERT INTO property_class VALUES ('DServer','AllowedAccessCmd',6,'GetLoggingTarget',20060912161031,20060912161031,NULL);
INSERT INTO property_class VALUES ('DServer','AllowedAccessCmd',7,'QueryWizardDevProperty',20060912161031,20060912161031,NULL);
INSERT INTO property_class VALUES ('DServer','AllowedAccessCmd',8,'QueryWizardClassProperty',20060912161031,20060912161031,NULL);




#
#	Delete default Starter clsss access if exist
#
DELETE FROM property_class where class='Starter' AND name='AllowedAccessCmd' AND value='DevStart';
DELETE FROM property_class where class='Starter' AND name='AllowedAccessCmd' AND value='DevGetRunningServers';
DELETE FROM property_class where class='Starter' AND name='AllowedAccessCmd' AND value='DevReadLog';
DELETE FROM property_class where class='Starter' AND name='AllowedAccessCmd' AND value='State';
DELETE FROM property_class where class='Starter' AND name='AllowedAccessCmd' AND value='DevGetStopServers';
#
#	Create default Starter clsss access
#
INSERT INTO property_class VALUES ('Starter','AllowedAccessCmd',3,'DevStart',20060912083524,20060912083524,NULL);
INSERT INTO property_class VALUES ('Starter','AllowedAccessCmd',4,'DevGetRunningServers',20060912083524,20060912083524,NULL);
INSERT INTO property_class VALUES ('Starter','AllowedAccessCmd',2,'DevReadLog',20060912083524,20060912083524,NULL);
INSERT INTO property_class VALUES ('Starter','AllowedAccessCmd',1,'State',20060912083524,20060912083524,NULL);
INSERT INTO property_class VALUES ('Starter','AllowedAccessCmd',5,'DevGetStopServers',20060912083524,20060912083524,NULL);




#
#	Delete default Database clsss access if exist
#
DELETE FROM property_class where class='DataBase' AND name='AllowedAccessCmd' AND value='GetLoggingTarget';
DELETE FROM property_class where class='DataBase' AND name='AllowedAccessCmd' AND value='GetLoggingLevel';
DELETE FROM property_class where class='DataBase' AND name='AllowedAccessCmd' AND value='DbGetDeviceAttributeProperty2';
DELETE FROM property_class where class='DataBase' AND name='AllowedAccessCmd' AND value='DbGetDeviceAttributeProperty';
DELETE FROM property_class where class='DataBase' AND name='AllowedAccessCmd' AND value='DbGetClassAttributeList';
DELETE FROM property_class where class='DataBase' AND name='AllowedAccessCmd' AND value='DbGetDeviceProperty';
DELETE FROM property_class where class='DataBase' AND name='AllowedAccessCmd' AND value='DbGetDevicePropertyList';
DELETE FROM property_class where class='DataBase' AND name='AllowedAccessCmd' AND value='DbGetClassProperty';
DELETE FROM property_class where class='DataBase' AND name='AllowedAccessCmd' AND value='DbGetClassPropertyList';
DELETE FROM property_class where class='DataBase' AND name='AllowedAccessCmd' AND value='DbGetProperty';
DELETE FROM property_class where class='DataBase' AND name='AllowedAccessCmd' AND value='DbGetPropertyList';
DELETE FROM property_class where class='DataBase' AND name='AllowedAccessCmd' AND value='DbGetDeviceAliasList';
DELETE FROM property_class where class='DataBase' AND name='AllowedAccessCmd' AND value='DbGetObjectList';
DELETE FROM property_class where class='DataBase' AND name='AllowedAccessCmd' AND value='DbGetClassList';
DELETE FROM property_class where class='DataBase' AND name='AllowedAccessCmd' AND value='DbGetDeviceMemberList';
DELETE FROM property_class where class='DataBase' AND name='AllowedAccessCmd' AND value='DbGetDeviceFamilyList';
DELETE FROM property_class where class='DataBase' AND name='AllowedAccessCmd' AND value='DbGetDeviceDomainList';
DELETE FROM property_class where class='DataBase' AND name='AllowedAccessCmd' AND value='DbGetDeviceList';
DELETE FROM property_class where class='DataBase' AND name='AllowedAccessCmd' AND value='DbGetDeviceServerClassList';
DELETE FROM property_class where class='DataBase' AND name='AllowedAccessCmd' AND value='DbGetInstanceNameList';
DELETE FROM property_class where class='DataBase' AND name='AllowedAccessCmd' AND value='DbGetServerNameList';
DELETE FROM property_class where class='DataBase' AND name='AllowedAccessCmd' AND value='DbGetServerInfo';
DELETE FROM property_class where class='DataBase' AND name='AllowedAccessCmd' AND value='DbGetAliasDevice';

#
#	Create default Database clsss access
#
INSERT INTO property_class VALUES ('DataBase','AllowedAccessCmd',22,'GetLoggingTarget',20060913093236,20060913093236,NULL);
INSERT INTO property_class VALUES ('DataBase','AllowedAccessCmd',21,'GetLoggingLevel',20060913093236,20060913093236,NULL);
INSERT INTO property_class VALUES ('DataBase','AllowedAccessCmd',20,'DbGetDeviceAttributeProperty2',20060913093236,20060913093236,NULL);
INSERT INTO property_class VALUES ('DataBase','AllowedAccessCmd',19,'DbGetDeviceAttributeProperty',20060913093236,20060913093236,NULL);
INSERT INTO property_class VALUES ('DataBase','AllowedAccessCmd',18,'DbGetClassAttributeList',20060913093236,20060913093236,NULL);
INSERT INTO property_class VALUES ('DataBase','AllowedAccessCmd',17,'DbGetDeviceProperty',20060913093236,20060913093236,NULL);
INSERT INTO property_class VALUES ('DataBase','AllowedAccessCmd',16,'DbGetDevicePropertyList',20060913093236,20060913093236,NULL);
INSERT INTO property_class VALUES ('DataBase','AllowedAccessCmd',15,'DbGetClassProperty',20060913093236,20060913093236,NULL);
INSERT INTO property_class VALUES ('DataBase','AllowedAccessCmd',14,'DbGetClassPropertyList',20060913093236,20060913093236,NULL);
INSERT INTO property_class VALUES ('DataBase','AllowedAccessCmd',13,'DbGetProperty',20060913093236,20060913093236,NULL);
INSERT INTO property_class VALUES ('DataBase','AllowedAccessCmd',12,'DbGetPropertyList',20060913093236,20060913093236,NULL);
INSERT INTO property_class VALUES ('DataBase','AllowedAccessCmd',10,'DbGetDeviceAliasList',20060913093236,20060913093236,NULL);
INSERT INTO property_class VALUES ('DataBase','AllowedAccessCmd',11,'DbGetObjectList',20060913093236,20060913093236,NULL);
INSERT INTO property_class VALUES ('DataBase','AllowedAccessCmd',9,'DbGetClassList',20060913093236,20060913093236,NULL);
INSERT INTO property_class VALUES ('DataBase','AllowedAccessCmd',8,'DbGetDeviceMemberList',20060913093236,20060913093236,NULL);
INSERT INTO property_class VALUES ('DataBase','AllowedAccessCmd',7,'DbGetDeviceFamilyList',20060913093236,20060913093236,NULL);
INSERT INTO property_class VALUES ('DataBase','AllowedAccessCmd',6,'DbGetDeviceDomainList',20060913093236,20060913093236,NULL);
INSERT INTO property_class VALUES ('DataBase','AllowedAccessCmd',5,'DbGetDeviceList',20060913093236,20060913093236,NULL);
INSERT INTO property_class VALUES ('DataBase','AllowedAccessCmd',4,'DbGetDeviceServerClassList',20060913093236,20060913093236,NULL);
INSERT INTO property_class VALUES ('DataBase','AllowedAccessCmd',3,'DbGetInstanceNameList',20060913093236,20060913093236,NULL);
INSERT INTO property_class VALUES ('DataBase','AllowedAccessCmd',2,'DbGetServerNameList',20060913093236,20060913093236,NULL);
INSERT INTO property_class VALUES ('DataBase','AllowedAccessCmd',1,'DbGetServerInfo',20060913093236,20060913093236,NULL);
INSERT INTO property_class VALUES ('DataBase','AllowedAccessCmd',23,'DbGetAliasDevice',20060913093236,20060913093236,NULL);





#
#	Delete default TangoAccessControl clsss access if exist
#
DELETE FROM property_class where class='TangoAccessControl' AND name='AllowedAccessCmd' AND value='GetUsers';
DELETE FROM property_class where class='TangoAccessControl' AND name='AllowedAccessCmd' AND value='GetAddressByUser';
DELETE FROM property_class where class='TangoAccessControl' AND name='AllowedAccessCmd' AND value='GetDeviceByUser';
DELETE FROM property_class where class='TangoAccessControl' AND name='AllowedAccessCmd' AND value='GetAccess';
#
#	Create default TangoAccessControl clsss access
#
INSERT INTO property_class VALUES ('TangoAccessControl','AllowedAccessCmd',1,'GetUsers',20060915102208,20060915102208,NULL);
INSERT INTO property_class VALUES ('TangoAccessControl','AllowedAccessCmd',2,'GetAddressByUser',20060915102208,20060915102208,NULL);
INSERT INTO property_class VALUES ('TangoAccessControl','AllowedAccessCmd',3,'GetDeviceByUser',20060915102208,20060915102208,NULL);
INSERT INTO property_class VALUES ('TangoAccessControl','AllowedAccessCmd',4,'GetAccess',20060915102208,20060915102208,NULL);

#
#	Create TangoAccessControl server  instance
#
INSERT INTO device VALUES ('dserver/TangoAccessControl/1',NULL,'dserver','TangoAccessControl','1',0,'','','TangoAccessControl/1',14891,'DServer','3','2006-09-14 16:06:20','2006-09-14 16:04:32',NULL);
INSERT INTO device VALUES ('sys/access-control/1',NULL,'sys','access','control',0,'','','TangoAccessControl/1',0,'TangoAccessControl','3','2006-09-14 16:06:20','2006-09-14 16:04:32',NULL);
INSERT INTO server VALUES ('tangoaccesscontrol/1','',1,1);
