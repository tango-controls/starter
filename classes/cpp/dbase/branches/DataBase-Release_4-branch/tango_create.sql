-- MySQL dump 8.22
--
-- Host: localhost    Database: tango
---------------------------------------------------------
-- Server version	3.23.55-log
CREATE DATABASE tango;
USE tango;

--
-- Table structure for table 'attribute_class'
--

CREATE TABLE attribute_class (
  class varchar(255) NOT NULL default '',
  name varchar(255) NOT NULL default '',
  updated timestamp(14) NOT NULL,
  accessed timestamp(14) NOT NULL,
  comment text,
  KEY index_attribute_class (class(64),name(64))
) TYPE=MyISAM;

--
-- Dumping data for table 'attribute_class'
--



--
-- Table structure for table 'device'
--

CREATE TABLE device (
  name varchar(255) default NULL,
  alias varchar(255) default NULL,
  domain varchar(85) default NULL,
  family varchar(85) default NULL,
  member varchar(85) default NULL,
  exported int(11) default NULL,
  ior text,
  host varchar(255) default NULL,
  server varchar(255) default NULL,
  pid int(11) default NULL,
  class varchar(255) default NULL,
  version varchar(8) default NULL,
  started datetime default NULL,
  stopped datetime default NULL,
  comment text,
  KEY index_name (name(64),alias(64))
) TYPE=MyISAM;

--
-- Dumping data for table 'device'
--


INSERT INTO device VALUES ('sys/database/2',NULL,'sys','database','2',1,'nada','wow','DataBaseds/2',1,'DataBase','2','2003-07-11 09:43:17',NULL,NULL);
INSERT INTO device VALUES ('dserver/DataBaseds/2',NULL,'dserver','DataBaseds','2',1,'nada','wow','DataBaseds/2',1,'DServer','2','2003-07-11 09:43:17',NULL,NULL);

--
-- Table structure for table 'property'
--

CREATE TABLE property (
  object varchar(255) default NULL,
  name varchar(255) default NULL,
  count int(11) default NULL,
  value text default NULL,
  updated timestamp(14) NOT NULL,
  accessed timestamp(14) NOT NULL,
  comment text,
  KEY index_name (object(64),name(64))
) TYPE=MyISAM;

--
-- Dumping data for table 'property'
--



--
-- Table structure for table 'property_attribute_class'
--

CREATE TABLE property_attribute_class (
  class varchar(255) NOT NULL default '',
  attribute varchar(255) NOT NULL default '',
  name varchar(255) NOT NULL default '',
  count int(11) NOT NULL default '0',
  value text default NULL,
  updated timestamp(14) NOT NULL,
  accessed timestamp(14) NOT NULL,
  comment text,
  KEY index_property_attribute_class (class(64),attribute(64),name(64),count)
) TYPE=MyISAM;

--
-- Dumping data for table 'property_attribute_class'
--



--
-- Table structure for table 'property_attribute_device'
--

CREATE TABLE property_attribute_device (
  device varchar(255) NOT NULL default '',
  attribute varchar(255) NOT NULL default '',
  name varchar(255) NOT NULL default '',
  count int(11) NOT NULL default '0',
  value text default NULL,
  updated timestamp(14) NOT NULL,
  accessed timestamp(14) NOT NULL,
  comment text,
  KEY index_property_attribute_device (device(64),attribute(64),name(64),count)
) TYPE=MyISAM;

--
-- Dumping data for table 'property_attribute_device'
--



--
-- Table structure for table 'property_class'
--

CREATE TABLE property_class (
  class varchar(255) NOT NULL default '',
  name varchar(255) NOT NULL default '',
  count int(11) NOT NULL default '0',
  value text default NULL,
  updated timestamp(14) NOT NULL,
  accessed timestamp(14) NOT NULL,
  comment text,
  KEY index_property (class(64),name(64),count)
) TYPE=MyISAM;

--
-- Dumping data for table 'property_class'
--


--
-- Table structure for table 'property_device'
--

CREATE TABLE property_device (
  device varchar(255) NOT NULL default '',
  name varchar(255) NOT NULL default '',
  domain varchar(255) NOT NULL default '',
  family varchar(255) NOT NULL default '',
  member varchar(255) NOT NULL default '',
  count int(11) NOT NULL default '0',
  value text default NULL,
  updated timestamp(14) NOT NULL,
  accessed timestamp(14) NOT NULL,
  comment text,
  KEY index_resource (device(64),name(64),count)
) TYPE=MyISAM;

--
-- Dumping data for table 'property_device'
--


--
-- Table structure for table 'server'
--

CREATE TABLE server (
  name varchar(255) NOT NULL default '',
  host varchar(255) NOT NULL default '',
  mode int(11) default '0',
  level int(11) default '0',
  KEY index_name (name(64))
) TYPE=MyISAM;

--
-- Dumping data for table 'server'
--

CREATE TABLE attribute_alias (
  alias varchar(255) DEFAULT '' NOT NULL,
  name varchar(255) DEFAULT '' NOT NULL,
  device varchar(255) DEFAULT '' NOT NULL,
  attribute varchar(255) DEFAULT '' NOT NULL,
  updated timestamp(14),
  accessed timestamp(14),
  comment text,
  KEY index_attribute_alias (alias(64),name(64))
);

CREATE TABLE event (
  name varchar(255),
  exported int(11),
  ior text,
  host varchar(255),
  server varchar(255),
  pid int(11),
  version varchar(8),
  started datetime,
  stopped datetime,
  KEY index_name (name(64))
);

CREATE TABLE history_ids (
  name varchar(255) NOT NULL default '',
  id int(10) unsigned NOT NULL default '0'
) TYPE=MyISAM;

INSERT INTO history_ids VALUES ('device',0);
INSERT INTO history_ids VALUES ('device_attribute',0);
INSERT INTO history_ids VALUES ('class',0);
INSERT INTO history_ids VALUES ('class_attribute',0);
INSERT INTO history_ids VALUES ('object',0);

CREATE TABLE property_hist (
  id int(10) unsigned NOT NULL default '0',
  date timestamp(14) NOT NULL,
  object varchar(255) NOT NULL default '',
  name varchar(255) NOT NULL default '',
  count int(11) NOT NULL default '0',
  value text,
  KEY index_id (id),
  KEY index_object (object),
  KEY index_name (name)
) TYPE=MyISAM;

CREATE TABLE property_device_hist (
  id int(10) unsigned NOT NULL default '0',
  date timestamp(14) NOT NULL,
  device varchar(255) NOT NULL default '',
  name varchar(255) NOT NULL default '',
  count int(11) NOT NULL default '0',
  value text,
  KEY index_id (id),
  KEY index_device (device),
  KEY index_name (name)
) TYPE=MyISAM;

CREATE TABLE property_class_hist (
  id int(10) unsigned NOT NULL default '0',
  date timestamp(14) NOT NULL,
  class varchar(255) NOT NULL default '',
  name varchar(255) NOT NULL default '',
  count int(11) NOT NULL default '0',
  value text,
  KEY index_id (id),
  KEY index_class (class),
  KEY index_name (name)
) TYPE=MyISAM;

CREATE TABLE property_attribute_class_hist (
  id int(10) unsigned NOT NULL default '0',
  date timestamp(14) NOT NULL,
  class varchar(255) NOT NULL default '',
  attribute varchar(255) NOT NULL default '',
  name varchar(255) NOT NULL default '',
  count int(11) NOT NULL default '0',
  value text,
  KEY index_id (id),
  KEY index_class (class),
  KEY index_attribute (attribute),
  KEY index_name (name)  
) TYPE=MyISAM;

CREATE TABLE property_attribute_device_hist (
  id int(10) unsigned NOT NULL default '0',
  date timestamp(14) NOT NULL,
  device varchar(255) NOT NULL default '',
  attribute varchar(255) NOT NULL default '',
  name varchar(255) NOT NULL default '',
  count int(11) NOT NULL default '0',
  value text,
  KEY index_id (id),
  KEY index_device (device),
  KEY index_attribute (attribute),
  KEY index_name (name)  
) TYPE=MyISAM;

