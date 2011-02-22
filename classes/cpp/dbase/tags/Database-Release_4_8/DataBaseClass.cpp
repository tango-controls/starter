static const char *ClassId    = "$Id$";
static const char *CvsPath    = "$Source$";
static const char *SvnPath    = "$HeadURL: $";
static const char *TagName    = "$Name$";
static const char *HttpServer = "http://www.esrf.fr/computing/cs/tango/tango_doc/ds_doc/";
static const char *RcsId = "$Header$";
//+=============================================================================
//
// file :        DataBaseClass.cpp
//
// description : C++ source for the DataBaseClass. A singleton
//               class derived from DeviceClass. It implements the
//               command list and all properties and methods required
//               by the DataBase once per process.
//
// project :     TANGO Device Server
//
// author(s) :   A.Gotz, JL Pons, P Verdier
//
// Copyright (C) :      2004,2005,2006,2007,2008,2009,2010
//						European Synchrotron Radiation Facility
//                      BP 220, Grenoble 38043
//                      FRANCE
//
// This file is part of Tango.
//
// Tango is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// Tango is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with Tango.  If not, see <http://www.gnu.org/licenses/>.
//
// $Version$
//
// $Log$
// Revision 2.38  2009/04/06 13:11:08  pascal_verdier
// DbMySqlSelect command added.
//
// Revision 2.37  2009/01/30 08:17:51  taurel
// - Added DbGetDeviceWideList command
//
// Revision 2.36  2008/03/26 17:31:00  taurel
// - Committed after merge with the Release 4 branch
//
// Revision 2.35.2.3  2008/03/26 17:15:36  taurel
// - Add some code in the device-factory to set the info returned by the Tango "info" call
//
// Revision 2.35.2.2  2008/02/28 17:07:37  taurel
// - Added the StoredProcedureRelease attribute
//
// Revision 2.35.2.1  2008/01/25 12:29:50  taurel
// - Add a new command DbDeleteAllDeviceAttributeProperty
// - The DbImportDevice now also returns the device class
// - Fix bug in the DbGetHostServerList command
//
// Revision 2.35  2007/11/06 07:24:06  taurel
// - Added the DbGetDataForServerCache command (with timing stats)
// - Add timing stats for the DbPutClassProperty command
//
// Revision 2.34  2007/04/25 06:01:03  pascal_verdier
// IOR NULL checked in DbGetDeviceInfo() and DbImportDevice().
//
// Revision 2.33  2006/10/10 14:29:00  jensmeyer
// Deleted db_bench.cpp
//
// Revision 2.32  2006/09/28 11:18:20  pascal_verdier
// DbGetClassForDevice and DbGetClassInheritanceForDevice commands added.
//
// Revision 2.31  2006/08/23 08:57:34  pascal_verdier
// Header changed.
//
// Revision 2.30  2006/06/22 15:25:32  jlpons
// Added history commands
//
// Revision 2.29  2005/10/05 07:35:54  pascal_verdier
// Bug fixed in Timing_Info attribute read.
// ResetTimingValues command added.
// Windows compatibility added for timing attributes.
//
// Revision 2.28  2005/07/25 08:49:17  andy_gotz
// replaced the timing attributes for the import device with a spectrum of timing
// stats for a list of commands.
//
// Revision 2.27  2005/07/21 20:12:23  andy_gotz
// added attributes to return timing information for the DbImportDevice command
//
// Revision 2.26  2005/06/06 09:10:55  pascal_verdier
// Warning removed at compiling.
//
// Revision 2.25  2005/02/15 10:03:30  pascal_verdier
// Bug fixed in DbGetDeviceInfo command
//
// Revision 2.24  2005/02/11 12:58:49  pascal_verdier
// Is now a TANGO 5 server.
// DbGetDeviceInfo command added.
//
// Revision 2.23  2005/01/10 10:35:25  pascal_verdier
// Updated to Tango 5.
//
// Revision 2.22  2004/10/22 10:33:55  taurel
// Added four new commands DbPutClassAttributeProperty2,
// DbGetClassAttributeProperty2, DbPutDeviceAttributeProperty2
// and DbGetDeviceAttributeProperty2. These commands support array attribute
// properties. For compatibility reason, keep the old commands.
//
// Revision 2.21  2004/09/23 08:34:44  pascal_verdier
// TemplateCmd in message replaced by the real command name.
//
// Revision 2.20  2004/06/21 12:20:12  pascal_verdier
// DbGetExportdDeviceListForClass and DbGetDeviceAlias commands added.
//
// Revision 2.19  2004/03/08 12:41:56  pascal_verdier
// Attribute alias management added.
//
// Revision 2.18  2004/01/26 13:25:00  pascal_verdier
// DbGetServerNameList and DbGetInstanceNameList commands added.
//
// Revision 2.17  2003/12/11 10:22:10  pascal_verdier
// DbGetHostServersInfo command added.
//
// Revision 2.16  2003/08/13 12:02:23  andy_gotz
// added DbPutAttributeAlias and DbGetAttributeAlias commands needed by
// the AttributeProxy object. Note this needs a new table attribute_alias
// in mysql.
//
// Revision 2.15  2003/03/20 14:55:13  andy_gotz
// added support for device aliases; fixed bug in updating server info; tagged server name onto end of import info; implemented support for importing and exporting events;
//
// Revision 2.14  2003/01/16 14:30:40  goetz
// ported Makefile to omniorb
//
// Revision 2.13  2002/11/26 10:00:58  goetz
// added delete_class_attribute_property; changed Solaris to CC; added pid to import_device
//
// Revision 2.12  2002/09/16 08:39:36  goetz
// added GetObjectList and GetPropertyList commands
//
// Revision 2.11  2002/02/04 17:09:09  goetz
// updated Windows port
//
// Revision 2.9  2001/07/12 12:15:18  goetz
// changed db_get_class_list() and db_delete_device_attribute_property()
//
// Revision 2.8  2001/07/04 05:17:03  goetz
// dserver device domain,family,member corrected; wildcards for DbGetClassList
//
// Revision 2.7  2001/07/04 04:42:25  goetz
// delete all properties before updating them
//
// Revision 2.6  2001/03/22 12:56:52  goetz
// fixed bug in DbAddDevice command, device name now unique in device table
//
// Revision 2.5  2001/03/06 12:05:44  goetz
// added DbGetDeviceExportedList; DbExportDevice updates host info in server table
//
// Revision 2.4  2001/03/06 11:01:56  goetz
// added DbGetDeviceExportedList command; DbExportDevice updates host in server table
//
// Revision 2.3  2001/03/05 12:10:53  goetz
// checking in before going to add new command(s)
//
// Revision 2.2  2001/01/03 11:58:29  goetz
// E.Taurel modified version for new TACO exception class
//
// Revision 2.1  2000/11/02 14:35:23  goetz
// added commands for server info
//
// Revision 2.0  2000/10/19 07:31:08  goetz
// changed major version number to 2
//
// Revision 1.16  2000/10/19 07:30:28  goetz
// ported Database to TANGO V2.0
//
// Revision 1.15  2000/10/02 08:37:16  goetz
// going to port to TANGO V2 i.e. OB V4
//
// Revision 1.14  2000/07/31 07:55:51  goetz
// added commands DbDeleteDevice and DbDeleteServer
//
// Revision 1.13  2000/07/29 13:33:04  goetz
// added commands DbDeleteDevice and DbDeleteServer, bug fixes
//
// Revision 1.12  2000/07/27 13:36:17  goetz
// checking in before going to add new commands with pogo
//
// Revision 1.11  2000/06/29 11:38:26  goetz
// intermediate checkin
//
// Revision 1.10  2000/06/06 11:04:56  goetz
// added DbGetDeviceAttributeList; modified DbGetDevicePropertyList; migrated to pogo
//
// Revision 1.8  2000/05/25 08:32:06  goetz
// added browsing commands for Jive browser
//
// Revision 1.7  2000/05/17 14:41:24  goetz
// get domain, family and member commands converted to use database columns
//
// Revision 1.6  2000/05/16 11:47:09  goetz
// added get commands for alias, domain, family + member
//
// Revision 1.5  2000/03/13 17:36:44  goetz
// fixed bugs in Device Attirbute Properties commands
//
// Revision 1.4  2000/02/16 14:12:51  goetz
// added file DataBase.pogo
//
// Revision 1.3  2000/02/16 14:12:03  goetz
// converted DataBaseds to POGO
//
//
//-=============================================================================
//
//  This file was generated by POGO
//
//                     (c) - Pascal Verdier - ESRF
//
//=============================================================================


#include <tango.h>

#include <DataBase.h>
#include <DataBaseClass.h>

namespace DataBase_ns {


//+----------------------------------------------------------------------------
//
// method : 		DbMySqlSelectCmd::execute()
// 
// description : 	method to trigger the execution of the command.
//                PLEASE DO NOT MODIFY this method core without pogo   
//
// in : - device : The device on which the command must be executed
//		- in_any : The command input data
//
// returns : The command output data (packed in the Any object)
//
//-----------------------------------------------------------------------------
CORBA::Any *DbMySqlSelectCmd::execute(Tango::DeviceImpl *device,const CORBA::Any &in_any)
{

	cout2 << "DbMySqlSelectCmd::execute(): arrived" << endl;

	Tango::DevString	argin;
	extract(in_any, argin);

	return insert((static_cast<DataBase *>(device))->db_my_sql_select(argin));
}


//+----------------------------------------------------------------------------
//
// method : 		DbGetDeviceWideListCmd::execute()
// 
// description : 	method to trigger the execution of the command.
//                PLEASE DO NOT MODIFY this method core without pogo   
//
// in : - device : The device on which the command must be excuted
//		- in_any : The command input data
//
// returns : The command output data (packed in the Any object)
//
//-----------------------------------------------------------------------------
CORBA::Any *DbGetDeviceWideListCmd::execute(Tango::DeviceImpl *device,const CORBA::Any &in_any)
{

	cout2 << "DbGetDeviceWideListCmd::execute(): arrived" << endl;

	Tango::DevString	argin;
	extract(in_any, argin);

	return insert((static_cast<DataBase *>(device))->db_get_device_wide_list(argin));
}

//+----------------------------------------------------------------------------
//
// method : 		DbGetDeviceListCmd::execute()
// 
// description : 	method to trigger the execution of the command.
//                PLEASE DO NOT MODIFY this method core without pogo   
//
// in : - device : The device on which the command must be excuted
//		- in_any : The command input data
//
// returns : The command output data (packed in the Any object)
//
//-----------------------------------------------------------------------------
CORBA::Any *DbGetDeviceListCmd::execute(Tango::DeviceImpl *device,const CORBA::Any &in_any)
{

	cout2 << "DbGetDeviceListCmd::execute(): arrived" << endl;

	const Tango::DevVarStringArray	*argin;
	extract(in_any, argin);

	return insert((static_cast<DataBase *>(device))->db_get_device_list(argin));
}

//+----------------------------------------------------------------------------
//
// method : 		DbDeleteAllDeviceAttributePropertyClass::execute()
// 
// description : 	method to trigger the execution of the command.
//                PLEASE DO NOT MODIFY this method core without pogo   
//
// in : - device : The device on which the command must be executed
//		- in_any : The command input data
//
// returns : The command output data (packed in the Any object)
//
//-----------------------------------------------------------------------------
CORBA::Any *DbDeleteAllDeviceAttributePropertyClass::execute(Tango::DeviceImpl *device,const CORBA::Any &in_any)
{

	cout2 << "DbDeleteAllDeviceAttributePropertyClass::execute(): arrived" << endl;

	const Tango::DevVarStringArray	*argin;
	extract(in_any, argin);

	((static_cast<DataBase *>(device))->db_delete_all_device_attribute_property(argin));
	return new CORBA::Any();
}

//+----------------------------------------------------------------------------
//
// method : 		DbGetDataForServerCacheCmd::execute()
// 
// description : 	method to trigger the execution of the command.
//                PLEASE DO NOT MODIFY this method core without pogo   
//
// in : - device : The device on which the command must be excuted
//		- in_any : The command input data
//
// returns : The command output data (packed in the Any object)
//
//-----------------------------------------------------------------------------
CORBA::Any *DbGetDataForServerCacheCmd::execute(Tango::DeviceImpl *device,const CORBA::Any &in_any)
{

	cout2 << "DbGetDataForServerCacheCmd::execute(): arrived" << endl;

	const Tango::DevVarStringArray	*argin;
	extract(in_any, argin);

	return insert((static_cast<DataBase *>(device))->db_get_data_for_server_cache(argin));
}
//+----------------------------------------------------------------------------
//
// method : 		DbGetClassInheritanceForDeviceClass::execute()
// 
// description : 	method to trigger the execution of the command.
//                PLEASE DO NOT MODIFY this method core without pogo   
//
// in : - device : The device on which the command must be excuted
//		- in_any : The command input data
//
// returns : The command output data (packed in the Any object)
//
//-----------------------------------------------------------------------------
CORBA::Any *DbGetClassInheritanceForDeviceClass::execute(Tango::DeviceImpl *device,const CORBA::Any &in_any)
{

	cout2 << "DbGetClassInheritanceForDeviceClass::execute(): arrived" << endl;

	Tango::DevString	argin;
	extract(in_any, argin);

	return insert((static_cast<DataBase *>(device))->db_get_class_inheritance_for_device(argin));
}

//+----------------------------------------------------------------------------
//
// method : 		DbGetClassForDeviceClass::execute()
// 
// description : 	method to trigger the execution of the command.
//                PLEASE DO NOT MODIFY this method core without pogo   
//
// in : - device : The device on which the command must be excuted
//		- in_any : The command input data
//
// returns : The command output data (packed in the Any object)
//
//-----------------------------------------------------------------------------
CORBA::Any *DbGetClassForDeviceClass::execute(Tango::DeviceImpl *device,const CORBA::Any &in_any)
{

	cout2 << "DbGetClassForDeviceClass::execute(): arrived" << endl;

	Tango::DevString	argin;
	extract(in_any, argin);

	return insert((static_cast<DataBase *>(device))->db_get_class_for_device(argin));
}


//+----------------------------------------------------------------------------
//
// method : 		DbGetClassAttributePropertyHistCmd::execute()
// 
// description : 	method to trigger the execution of the command.
//                PLEASE DO NOT MODIFY this method core without pogo   
//
// in : - device : The device on which the command must be excuted
//		- in_any : The command input data
//
// returns : The command output data (packed in the Any object)
//
//-----------------------------------------------------------------------------
CORBA::Any *DbGetClassAttributePropertyHistCmd::execute(Tango::DeviceImpl *device,const CORBA::Any &in_any)
{

	cout2 << "DbGetClassAttributePropertyHistCmd::execute(): arrived" << endl;

	const Tango::DevVarStringArray	*argin;
	extract(in_any, argin);

	return insert((static_cast<DataBase *>(device))->db_get_class_attribute_property_hist(argin));
}

//+----------------------------------------------------------------------------
//
// method : 		DbGetDeviceAttributePropertyHistCmd::execute()
// 
// description : 	method to trigger the execution of the command.
//                PLEASE DO NOT MODIFY this method core without pogo   
//
// in : - device : The device on which the command must be excuted
//		- in_any : The command input data
//
// returns : The command output data (packed in the Any object)
//
//-----------------------------------------------------------------------------
CORBA::Any *DbGetDeviceAttributePropertyHistCmd::execute(Tango::DeviceImpl *device,const CORBA::Any &in_any)
{

	cout2 << "DbGetDeviceAttributePropertyHistCmd::execute(): arrived" << endl;

	const Tango::DevVarStringArray	*argin;
	extract(in_any, argin);

	return insert((static_cast<DataBase *>(device))->db_get_device_attribute_property_hist(argin));
}

//+----------------------------------------------------------------------------
//
// method : 		DbGetPropertyHistClass::execute()
// 
// description : 	method to trigger the execution of the command.
//                PLEASE DO NOT MODIFY this method core without pogo   
//
// in : - device : The device on which the command must be excuted
//		- in_any : The command input data
//
// returns : The command output data (packed in the Any object)
//
//-----------------------------------------------------------------------------
CORBA::Any *DbGetPropertyHistClass::execute(Tango::DeviceImpl *device,const CORBA::Any &in_any)
{

	cout2 << "DbGetPropertyHistClass::execute(): arrived" << endl;

	const Tango::DevVarStringArray	*argin;
	extract(in_any, argin);

	return insert((static_cast<DataBase *>(device))->db_get_property_hist(argin));
}

//+----------------------------------------------------------------------------
//
// method : 		DbGetClassPropertyHistClass::execute()
// 
// description : 	method to trigger the execution of the command.
//                PLEASE DO NOT MODIFY this method core without pogo   
//
// in : - device : The device on which the command must be excuted
//		- in_any : The command input data
//
// returns : The command output data (packed in the Any object)
//
//-----------------------------------------------------------------------------
CORBA::Any *DbGetClassPropertyHistClass::execute(Tango::DeviceImpl *device,const CORBA::Any &in_any)
{

	cout2 << "DbGetClassPropertyHistClass::execute(): arrived" << endl;

	const Tango::DevVarStringArray	*argin;
	extract(in_any, argin);

	return insert((static_cast<DataBase *>(device))->db_get_class_property_hist(argin));
}



//+----------------------------------------------------------------------------
//
// method : 		DbGetDevicePropertyHistClass::execute()
// 
// description : 	method to trigger the execution of the command.
//                PLEASE DO NOT MODIFY this method core without pogo   
//
// in : - device : The device on which the command must be excuted
//		- in_any : The command input data
//
// returns : The command output data (packed in the Any object)
//
//-----------------------------------------------------------------------------
CORBA::Any *DbGetDevicePropertyHistClass::execute(Tango::DeviceImpl *device,const CORBA::Any &in_any)
{

	cout2 << "DbGetDevicePropertyHistClass::execute(): arrived" << endl;

	const Tango::DevVarStringArray	*argin;
	extract(in_any, argin);

	return insert((static_cast<DataBase *>(device))->db_get_device_property_hist(argin));
}

//+----------------------------------------------------------------------------
//
// method : 		ResetTimingValuesClass::execute()
// 
// description : 	method to trigger the execution of the command.
//                PLEASE DO NOT MODIFY this method core without pogo   
//
// in : - device : The device on which the command must be excuted
//		- in_any : The command input data
//
// returns : The command output data (packed in the Any object)
//
//-----------------------------------------------------------------------------
CORBA::Any *ResetTimingValuesClass::execute(Tango::DeviceImpl *device,const CORBA::Any &in_any)
{

	cout2 << "ResetTimingValuesClass::execute(): arrived" << endl;

	((static_cast<DataBase *>(device))->reset_timing_values());
	return new CORBA::Any();
}

//+----------------------------------------------------------------------------
//
// method : 		DbGetDeviceInfoClass::execute()
// 
// description : 	method to trigger the execution of the command.
//                PLEASE DO NOT MODIFY this method core without pogo   
//
// in : - device : The device on which the command must be excuted
//		- in_any : The command input data
//
// returns : The command output data (packed in the Any object)
//
//-----------------------------------------------------------------------------
CORBA::Any *DbGetDeviceInfoClass::execute(Tango::DeviceImpl *device,const CORBA::Any &in_any)
{

	cout2 << "DbGetDeviceInfoClass::execute(): arrived" << endl;

	Tango::DevString	argin;
	extract(in_any, argin);

	return insert((static_cast<DataBase *>(device))->db_get_device_info(argin));
}

//+----------------------------------------------------------------------------
//
// method : 		DbGetClassAttributeProperty2Cmd::execute()
// 
// description : 	method to trigger the execution of the command.
//                PLEASE DO NOT MODIFY this method core without pogo   
//
// in : - device : The device on which the command must be excuted
//		- in_any : The command input data
//
// returns : The command output data (packed in the Any object)
//
//-----------------------------------------------------------------------------
CORBA::Any *DbGetClassAttributeProperty2Cmd::execute(Tango::DeviceImpl *device,const CORBA::Any &in_any)
{

	cout2 << "DbGetClassAttributeProperty2Cmd::execute(): arrived" << endl;

	const Tango::DevVarStringArray	*argin;
	extract(in_any, argin);

	return insert((static_cast<DataBase *>(device))->db_get_class_attribute_property2(argin));
}

//+----------------------------------------------------------------------------
//
// method : 		DbPutClassAttributeProperty2Cmd::execute()
// 
// description : 	method to trigger the execution of the command.
//                PLEASE DO NOT MODIFY this method core without pogo   
//
// in : - device : The device on which the command must be excuted
//		- in_any : The command input data
//
// returns : The command output data (packed in the Any object)
//
//-----------------------------------------------------------------------------
CORBA::Any *DbPutClassAttributeProperty2Cmd::execute(Tango::DeviceImpl *device,const CORBA::Any &in_any)
{

	cout2 << "DbPutClassAttributeProperty2Cmd::execute(): arrived" << endl;

	const Tango::DevVarStringArray	*argin;
	extract(in_any, argin);

	((static_cast<DataBase *>(device))->db_put_class_attribute_property2(argin));
	return new CORBA::Any();
}

//+----------------------------------------------------------------------------
//
// method : 		DbGetDeviceAttributeProperty2Cmd::execute()
// 
// description : 	method to trigger the execution of the command.
//                PLEASE DO NOT MODIFY this method core without pogo   
//
// in : - device : The device on which the command must be excuted
//		- in_any : The command input data
//
// returns : The command output data (packed in the Any object)
//
//-----------------------------------------------------------------------------
CORBA::Any *DbGetDeviceAttributeProperty2Cmd::execute(Tango::DeviceImpl *device,const CORBA::Any &in_any)
{

	cout2 << "DbGetDeviceAttributeProperty2Cmd::execute(): arrived" << endl;

	const Tango::DevVarStringArray	*argin;
	extract(in_any, argin);

	return insert((static_cast<DataBase *>(device))->db_get_device_attribute_property2(argin));
}

//+----------------------------------------------------------------------------
//
// method : 		DbPutDeviceAttributeProperty2Cmd::execute()
// 
// description : 	method to trigger the execution of the command.
//                PLEASE DO NOT MODIFY this method core without pogo   
//
// in : - device : The device on which the command must be excuted
//		- in_any : The command input data
//
// returns : The command output data (packed in the Any object)
//
//-----------------------------------------------------------------------------
CORBA::Any *DbPutDeviceAttributeProperty2Cmd::execute(Tango::DeviceImpl *device,const CORBA::Any &in_any)
{

	cout2 << "DbPutDeviceAttributeProperty2Cmd::execute(): arrived" << endl;

	const Tango::DevVarStringArray	*argin;
	extract(in_any, argin);

	((static_cast<DataBase *>(device))->db_put_device_attribute_property2(argin));
	return new CORBA::Any();
}

//+----------------------------------------------------------------------------
//
// method : 		DbGetExportdDeviceListForClassCmd::execute()
// 
// description : 	method to trigger the execution of the command.
//                PLEASE DO NOT MODIFY this method core without pogo   
//
// in : - device : The device on which the command must be excuted
//		- in_any : The command input data
//
// returns : The command output data (packed in the Any object)
//
//-----------------------------------------------------------------------------
CORBA::Any *DbGetExportdDeviceListForClassCmd::execute(Tango::DeviceImpl *device,const CORBA::Any &in_any)
{

	cout2 << "DbGetExportdDeviceListForClassCmd::execute(): arrived" << endl;

	Tango::DevString	argin;
	extract(in_any, argin);

	return insert((static_cast<DataBase *>(device))->db_get_exportd_device_list_for_class(argin));
}

//+----------------------------------------------------------------------------
//
// method : 		DbGetDeviceAliasCmd::execute()
// 
// description : 	method to trigger the execution of the command.
//                PLEASE DO NOT MODIFY this method core without pogo   
//
// in : - device : The device on which the command must be excuted
//		- in_any : The command input data
//
// returns : The command output data (packed in the Any object)
//
//-----------------------------------------------------------------------------
CORBA::Any *DbGetDeviceAliasCmd::execute(Tango::DeviceImpl *device,const CORBA::Any &in_any)
{

	cout2 << "DbGetDeviceAliasCmd::execute(): arrived" << endl;

	Tango::DevString	argin;
	extract(in_any, argin);

	return insert((static_cast<DataBase *>(device))->db_get_device_alias(argin));
}



//+----------------------------------------------------------------------------
//
// method : 		DbDeleteAttributeAliasCmd::execute()
// 
// description : 	method to trigger the execution of the command.
//                PLEASE DO NOT MODIFY this method core without pogo   
//
// in : - device : The device on which the command must be excuted
//		- in_any : The command input data
//
// returns : The command output data (packed in the Any object)
//
//-----------------------------------------------------------------------------
CORBA::Any *DbDeleteAttributeAliasCmd::execute(Tango::DeviceImpl *device,const CORBA::Any &in_any)
{

	cout2 << "DbDeleteAttributeAliasCmd::execute(): arrived" << endl;

	Tango::DevString	argin;
	extract(in_any, argin);

	((static_cast<DataBase *>(device))->db_delete_attribute_alias(argin));
	return new CORBA::Any();
}

//+----------------------------------------------------------------------------
//
// method : 		DbDeleteDeviceAliasCmd::execute()
// 
// description : 	method to trigger the execution of the command.
//                PLEASE DO NOT MODIFY this method core without pogo   
//
// in : - device : The device on which the command must be excuted
//		- in_any : The command input data
//
// returns : The command output data (packed in the Any object)
//
//-----------------------------------------------------------------------------
CORBA::Any *DbDeleteDeviceAliasCmd::execute(Tango::DeviceImpl *device,const CORBA::Any &in_any)
{

	cout2 << "DbDeleteDeviceAliasCmd::execute(): arrived" << endl;

	Tango::DevString	argin;
	extract(in_any, argin);

	((static_cast<DataBase *>(device))->db_delete_device_alias(argin));
	return new CORBA::Any();
}

//+----------------------------------------------------------------------------
//
// method : 		DbGetInstanceNameListCmd::execute()
// 
// description : 	method to trigger the execution of the command.
//                PLEASE DO NOT MODIFY this method core without pogo   
//
// in : - device : The device on which the command must be excuted
//		- in_any : The command input data
//
// returns : The command output data (packed in the Any object)
//
//-----------------------------------------------------------------------------
CORBA::Any *DbGetInstanceNameListCmd::execute(Tango::DeviceImpl *device,const CORBA::Any &in_any)
{

	cout2 << "DbGetInstanceNameListCmd::execute(): arrived" << endl;

	Tango::DevString	argin;
	extract(in_any, argin);

	return insert((static_cast<DataBase *>(device))->db_get_instance_name_list(argin));
}

//+----------------------------------------------------------------------------
//
// method : 		DbGetServerNameListCmd::execute()
// 
// description : 	method to trigger the execution of the command.
//                PLEASE DO NOT MODIFY this method core without pogo   
//
// in : - device : The device on which the command must be excuted
//		- in_any : The command input data
//
// returns : The command output data (packed in the Any object)
//
//-----------------------------------------------------------------------------
CORBA::Any *DbGetServerNameListCmd::execute(Tango::DeviceImpl *device,const CORBA::Any &in_any)
{

	cout2 << "DbGetServerNameListCmd::execute(): arrived" << endl;

	Tango::DevString	argin;
	extract(in_any, argin);

	return insert((static_cast<DataBase *>(device))->db_get_server_name_list(argin));
}

//+----------------------------------------------------------------------------
//
// method : 		DbGetAliasDeviceCmd::execute()
// 
// description : 	method to trigger the execution of the command.
//                PLEASE DO NOT MODIFY this method core without pogo   
//
// in : - device : The device on which the command must be excuted
//		- in_any : The command input data
//
// returns : The command output data (packed in the Any object)
//
//-----------------------------------------------------------------------------
CORBA::Any *DbGetAliasDeviceCmd::execute(Tango::DeviceImpl *device,const CORBA::Any &in_any)
{

	cout2 << "DbGetAliasDeviceCmd::execute(): arrived" << endl;

	Tango::DevString	argin;
	extract(in_any, argin);

	return insert((static_cast<DataBase *>(device))->db_get_alias_device(argin));
}

//+----------------------------------------------------------------------------
//
// method : 		DbGetHostServersInfoCmd::execute()
// 
// description : 	method to trigger the execution of the command.
//                PLEASE DO NOT MODIFY this method core without pogo   
//
// in : - device : The device on which the command must be excuted
//		- in_any : The command input data
//
// returns : The command output data (packed in the Any object)
//
//-----------------------------------------------------------------------------
CORBA::Any *DbGetHostServersInfoCmd::execute(Tango::DeviceImpl *device,const CORBA::Any &in_any)
{

	cout2 << "DbGetHostServersInfoCmd::execute(): arrived" << endl;

	Tango::DevString	argin;
	extract(in_any, argin);

	return insert((static_cast<DataBase *>(device))->db_get_host_servers_info(argin));
}

//+----------------------------------------------------------------------------
//
// method : 		DbGetAttributeAliasCmd::execute()
// 
// description : 	method to trigger the execution of the command.
//                PLEASE DO NOT MODIFY this method core without pogo   
//
// in : - device : The device on which the command must be excuted
//		- in_any : The command input data
//
// returns : The command output data (packed in the Any object)
//
//-----------------------------------------------------------------------------
CORBA::Any *DbGetAttributeAliasCmd::execute(Tango::DeviceImpl *device,const CORBA::Any &in_any)
{

	cout2 << "DbGetAttributeAliasCmd::execute(): arrived" << endl;

	Tango::DevString	argin;
	extract(in_any, argin);

	return insert((static_cast<DataBase *>(device))->db_get_attribute_alias(argin));
}

//+----------------------------------------------------------------------------
//
// method : 		DbPutAttributeAliasCmd::execute()
// 
// description : 	method to trigger the execution of the command.
//                PLEASE DO NOT MODIFY this method core without pogo   
//
// in : - device : The device on which the command must be excuted
//		- in_any : The command input data
//
// returns : The command output data (packed in the Any object)
//
//-----------------------------------------------------------------------------
CORBA::Any *DbPutAttributeAliasCmd::execute(Tango::DeviceImpl *device,const CORBA::Any &in_any)
{

	cout2 << "DbPutAttributeAliasCmd::execute(): arrived" << endl;

	const Tango::DevVarStringArray	*argin;
	extract(in_any, argin);

	((static_cast<DataBase *>(device))->db_put_attribute_alias(argin));
	return new CORBA::Any();
}

//+----------------------------------------------------------------------------
//
// method : 		DbPutDeviceAliasCmd::execute()
// 
// description : 	method to trigger the execution of the command.
//                PLEASE DO NOT MODIFY this method core without pogo   
//
// in : - device : The device on which the command must be excuted
//		- in_any : The command input data
//
// returns : The command output data (packed in the Any object)
//
//-----------------------------------------------------------------------------
CORBA::Any *DbPutDeviceAliasCmd::execute(Tango::DeviceImpl *device,const CORBA::Any &in_any)
{

	cout2 << "DbPutDeviceAliasCmd::execute(): arrived" << endl;

	const Tango::DevVarStringArray	*argin;
	extract(in_any, argin);

	((static_cast<DataBase *>(device))->db_put_device_alias(argin));
	return new CORBA::Any();
}

//+----------------------------------------------------------------------------
//
// method : 		DbUnExportEventCmd::execute()
// 
// description : 	method to trigger the execution of the command.
//                PLEASE DO NOT MODIFY this method core without pogo   
//
// in : - device : The device on which the command must be excuted
//		- in_any : The command input data
//
// returns : The command output data (packed in the Any object)
//
//-----------------------------------------------------------------------------
CORBA::Any *DbUnExportEventCmd::execute(Tango::DeviceImpl *device,const CORBA::Any &in_any)
{

	cout2 << "DbUnExportEventCmd::execute(): arrived" << endl;

	Tango::DevString	argin;
	extract(in_any, argin);

	((static_cast<DataBase *>(device))->db_un_export_event(argin));
	return new CORBA::Any();
}

//+----------------------------------------------------------------------------
//
// method : 		DbImportEventCmd::execute()
// 
// description : 	method to trigger the execution of the command.
//                PLEASE DO NOT MODIFY this method core without pogo   
//
// in : - device : The device on which the command must be excuted
//		- in_any : The command input data
//
// returns : The command output data (packed in the Any object)
//
//-----------------------------------------------------------------------------
CORBA::Any *DbImportEventCmd::execute(Tango::DeviceImpl *device,const CORBA::Any &in_any)
{

	cout2 << "DbImportEventCmd::execute(): arrived" << endl;

	Tango::DevString	argin;
	extract(in_any, argin);

	return insert((static_cast<DataBase *>(device))->db_import_event(argin));
}

//+----------------------------------------------------------------------------
//
// method : 		DbExportEventCmd::execute()
// 
// description : 	method to trigger the execution of the command.
//                PLEASE DO NOT MODIFY this method core without pogo   
//
// in : - device : The device on which the command must be excuted
//		- in_any : The command input data
//
// returns : The command output data (packed in the Any object)
//
//-----------------------------------------------------------------------------
CORBA::Any *DbExportEventCmd::execute(Tango::DeviceImpl *device,const CORBA::Any &in_any)
{

	cout2 << "DbExportEventCmd::execute(): arrived" << endl;

	const Tango::DevVarStringArray	*argin;
	extract(in_any, argin);

	((static_cast<DataBase *>(device))->db_export_event(argin));
	return new CORBA::Any();
}

//+----------------------------------------------------------------------------
//
// method : 		DbGetObjectListCmd::execute()
// 
// description : 	method to trigger the execution of the command.
//                PLEASE DO NOT MODIFY this method core without pogo   
//
// in : - device : The device on which the command must be excuted
//		- in_any : The command input data
//
// returns : The command output data (packed in the Any object)
//
//-----------------------------------------------------------------------------
CORBA::Any *DbGetObjectListCmd::execute(Tango::DeviceImpl *device,const CORBA::Any &in_any)
{

	cout2 << "DbGetObjectListCmd::execute(): arrived" << endl;

	Tango::DevString	argin;
	extract(in_any, argin);

	return insert((static_cast<DataBase *>(device))->db_get_object_list(argin));
}

//+----------------------------------------------------------------------------
//
// method : 		DbGetDeviceServerClassListCmd::execute()
// 
// description : 	method to trigger the execution of the command.
//                PLEASE DO NOT MODIFY this method core without pogo   
//
// in : - device : The device on which the command must be excuted
//		- in_any : The command input data
//
// returns : The command output data (packed in the Any object)
//
//-----------------------------------------------------------------------------
CORBA::Any *DbGetDeviceServerClassListCmd::execute(Tango::DeviceImpl *device,const CORBA::Any &in_any)
{

	cout2 << "DbGetDeviceServerClassListCmd::execute(): arrived" << endl;

	Tango::DevString	argin;
	extract(in_any, argin);

	return insert((static_cast<DataBase *>(device))->db_get_device_server_class_list(argin));
}
//+----------------------------------------------------------------------------
//
// method : 		DbGetDeviceExportedListCmd::execute()
// 
// description : 	method to trigger the execution of the command.
//                PLEASE DO NOT MODIFY this method core without pogo   
//
// in : - device : The device on which the command must be excuted
//		- in_any : The command input data
//
// returns : The command output data (packed in the Any object)
//
//-----------------------------------------------------------------------------
CORBA::Any *DbGetDeviceExportedListCmd::execute(Tango::DeviceImpl *device,const CORBA::Any &in_any)
{

	cout2 << "DbGetDeviceExportedListCmd::execute(): arrived" << endl;

	Tango::DevString	argin;
	extract(in_any, argin);

	return insert((static_cast<DataBase *>(device))->db_get_device_exported_list(argin));
}

//+----------------------------------------------------------------------------
//
// method : 		DbDeleteServerInfoCmd::execute()
// 
// description : 	method to trigger the execution of the command.
//                PLEASE DO NOT MODIFY this method core without pogo   
//
// in : - device : The device on which the command must be excuted
//		- in_any : The command input data
//
// returns : The command output data (packed in the Any object)
//
//-----------------------------------------------------------------------------
CORBA::Any *DbDeleteServerInfoCmd::execute(Tango::DeviceImpl *device,const CORBA::Any &in_any)
{

	cout2 << "DbDeleteServerInfoCmd::execute(): arrived" << endl;

	Tango::DevString	argin;
	extract(in_any, argin);

	((static_cast<DataBase *>(device))->db_delete_server_info(argin));
	return new CORBA::Any();
}

//+----------------------------------------------------------------------------
//
// method : 		DbPutServerInfoCmd::execute()
// 
// description : 	method to trigger the execution of the command.
//                PLEASE DO NOT MODIFY this method core without pogo   
//
// in : - device : The device on which the command must be excuted
//		- in_any : The command input data
//
// returns : The command output data (packed in the Any object)
//
//-----------------------------------------------------------------------------
CORBA::Any *DbPutServerInfoCmd::execute(Tango::DeviceImpl *device,const CORBA::Any &in_any)
{

	cout2 << "DbPutServerInfoCmd::execute(): arrived" << endl;

	const Tango::DevVarStringArray	*argin;
	extract(in_any, argin);

	((static_cast<DataBase *>(device))->db_put_server_info(argin));
	return new CORBA::Any();
}

//+----------------------------------------------------------------------------
//
// method : 		DbGetServerInfoCmd::execute()
// 
// description : 	method to trigger the execution of the command.
//                PLEASE DO NOT MODIFY this method core without pogo   
//
// in : - device : The device on which the command must be excuted
//		- in_any : The command input data
//
// returns : The command output data (packed in the Any object)
//
//-----------------------------------------------------------------------------
CORBA::Any *DbGetServerInfoCmd::execute(Tango::DeviceImpl *device,const CORBA::Any &in_any)
{

	cout2 << "DbGetServerInfoCmd::execute(): arrived" << endl;

	Tango::DevString	argin;
	extract(in_any, argin);

	return insert((static_cast<DataBase *>(device))->db_get_server_info(argin));
}

//+----------------------------------------------------------------------------
//
// method : 		DbDeleteDeviceAttributePropertyCmd::execute()
// 
// description : 	method to trigger the execution of the command
//
// in : - device : The device on which the command must be excuted
//		- in_any : The command input data
//
// returns : The command output data (packed in the Any object)
//
//-----------------------------------------------------------------------------
CORBA::Any *DbDeleteDeviceAttributePropertyCmd::execute(Tango::DeviceImpl *device,const CORBA::Any &in_any)
{

	cout2 << "DbDeleteDeviceAttributePropertyCmd::execute(): arrived" << endl;

	const Tango::DevVarStringArray	*argin;
	extract(in_any, argin);

	((static_cast<DataBase *>(device))->db_delete_device_attribute_property(argin));
	return new CORBA::Any();
}


//+----------------------------------------------------------------------------
//
// method : 		DbDeleteDeviceAttributeCmd::execute()
// 
// description : 	method to trigger the execution of the command
//
// in : - device : The device on which the command must be excuted
//		- in_any : The command input data
//
// returns : The command output data (packed in the Any object)
//
//-----------------------------------------------------------------------------
CORBA::Any *DbDeleteDeviceAttributeCmd::execute(Tango::DeviceImpl *device,const CORBA::Any &in_any)
{

	cout2 << "DbDeleteDeviceAttributeCmd::execute(): arrived" << endl;

	const Tango::DevVarStringArray	*argin;
	extract(in_any, argin);

	((static_cast<DataBase *>(device))->db_delete_device_attribute(argin));
	return new CORBA::Any();
}

//+----------------------------------------------------------------------------
//
// method : 		DbDeleteClassAttributePropertyCmd::execute()
// 
// description : 	method to trigger the execution of the command
//
// in : - device : The device on which the command must be excuted
//		- in_any : The command input data
//
// returns : The command output data (packed in the Any object)
//
//-----------------------------------------------------------------------------
CORBA::Any *DbDeleteClassAttributePropertyCmd::execute(Tango::DeviceImpl *device,const CORBA::Any &in_any)
{

	cout2 << "DbDeleteClassAttributePropertyCmd::execute(): arrived" << endl;

	const Tango::DevVarStringArray	*argin;
	extract(in_any, argin);

	((static_cast<DataBase *>(device))->db_delete_class_attribute_property(argin));
	return new CORBA::Any();
}


//+----------------------------------------------------------------------------
//
// method : 		DbDeleteClassAttributeCmd::execute()
// 
// description : 	method to trigger the execution of the command
//
// in : - device : The device on which the command must be excuted
//		- in_any : The command input data
//
// returns : The command output data (packed in the Any object)
//
//-----------------------------------------------------------------------------
CORBA::Any *DbDeleteClassAttributeCmd::execute(Tango::DeviceImpl *device,const CORBA::Any &in_any)
{

	cout2 << "DbDeleteClassAttributeCmd::execute(): arrived" << endl;

	const Tango::DevVarStringArray	*argin;
	extract(in_any, argin);

	((static_cast<DataBase *>(device))->db_delete_class_attribute(argin));
	return new CORBA::Any();
}


//+----------------------------------------------------------------------------
//
// method : 		DbDeleteServerCmd::execute()
// 
// description : 	method to trigger the execution of the command
//
// in : - device : The device on which the command must be excuted
//		- in_any : The command input data
//
// returns : The command output data (packed in the Any object)
//
//-----------------------------------------------------------------------------
CORBA::Any *DbDeleteServerCmd::execute(Tango::DeviceImpl *device,const CORBA::Any &in_any)
{

	cout2 << "DbDeleteServerCmd::execute(): arrived" << endl;

	Tango::DevString	argin;
	extract(in_any, argin);

	((static_cast<DataBase *>(device))->db_delete_server(argin));
	return new CORBA::Any();
}


//+----------------------------------------------------------------------------
//
// method : 		DbDeleteDeviceCmd::execute()
// 
// description : 	method to trigger the execution of the command
//
// in : - device : The device on which the command must be excuted
//		- in_any : The command input data
//
// returns : The command output data (packed in the Any object)
//
//-----------------------------------------------------------------------------
CORBA::Any *DbDeleteDeviceCmd::execute(Tango::DeviceImpl *device,const CORBA::Any &in_any)
{

	cout2 << "DbDeleteDeviceCmd::execute(): arrived" << endl;

	Tango::DevString	argin;
	extract(in_any, argin);

	((static_cast<DataBase *>(device))->db_delete_device(argin));
	return new CORBA::Any();
}


//+----------------------------------------------------------------------------
//
// method : 		DbGetDeviceAttributeList::execute()
// 
// description : 	method to trigger the execution of the command
//
// in : - device : The device on which the command must be excuted
//		- in_any : The command input data
//
// returns : The command output data (packed in the Any object)
//
//-----------------------------------------------------------------------------
CORBA::Any *DbGetDeviceAttributeList::execute(Tango::DeviceImpl *device,const CORBA::Any &in_any)
{

	cout2 << "DbGetDeviceAttributeList::execute(): arrived" << endl;

	const Tango::DevVarStringArray	*argin;
	extract(in_any, argin);

	return insert((static_cast<DataBase *>(device))->db_get_device_attribute_list(argin));
}

//+----------------------------------------------------------------------------
//
// method : 		DbGetPropertyList::execute()
// 
// description : 	method to trigger the execution of the command
//
// in : - device : The device on which the command must be excuted
//		- in_any : The command input data
//
// returns : The command output data (packed in the Any object)
//
//-----------------------------------------------------------------------------
CORBA::Any *DbGetPropertyList::execute(Tango::DeviceImpl *device,const CORBA::Any &in_any)
{

	cout2 << "DbGetPropertyList::execute(): arrived" << endl;

	const Tango::DevVarStringArray	*argin;
	extract(in_any, argin);

	return insert((static_cast<DataBase *>(device))->db_get_property_list(argin));
}

//+----------------------------------------------------------------------------
//
// method : 		DbGetProperty::execute()
// 
// description : 	method to trigger the execution of the command
//
// in : - device : The device on which the command must be excuted
//		- in_any : The command input data
//
// returns : The command output data (packed in the Any object)
//
//-----------------------------------------------------------------------------
CORBA::Any *DbGetProperty::execute(Tango::DeviceImpl *device,const CORBA::Any &in_any)
{

	cout2 << "DbGetProperty::execute(): arrived" << endl;

	const Tango::DevVarStringArray	*argin;
	extract(in_any, argin);

	return insert((static_cast<DataBase *>(device))->db_get_property(argin));
}


//+----------------------------------------------------------------------------
//
// method : 		DbPutProperty::execute()
// 
// description : 	method to trigger the execution of the command
//
// in : - device : The device on which the command must be excuted
//		- in_any : The command input data
//
// returns : The command output data (packed in the Any object)
//
//-----------------------------------------------------------------------------
CORBA::Any *DbPutProperty::execute(Tango::DeviceImpl *device,const CORBA::Any &in_any)
{

	cout2 << "DbPutProperty::execute(): arrived" << endl;

	const Tango::DevVarStringArray	*argin;
	extract(in_any, argin);

	((static_cast<DataBase *>(device))->db_put_property(argin));
	return new CORBA::Any();
}

//+----------------------------------------------------------------------------
//
// method : 		DbDeleteProperty::execute()
// 
// description : 	method to trigger the execution of the command
//
// in : - device : The device on which the command must be excuted
//		- in_any : The command input data
//
// returns : The command output data (packed in the Any object)
//
//-----------------------------------------------------------------------------
CORBA::Any *DbDeleteProperty::execute(Tango::DeviceImpl *device,const CORBA::Any &in_any)
{

	cout2 << "DbDeleteProperty::execute(): arrived" << endl;

	const Tango::DevVarStringArray	*argin;
	extract(in_any, argin);

	((static_cast<DataBase *>(device))->db_delete_property(argin));
	return new CORBA::Any();
}


//+----------------------------------------------------------------------------
//
// method : 		DbGetHostServerList::execute()
// 
// description : 	method to trigger the execution of the command
//
// in : - device : The device on which the command must be excuted
//		- in_any : The command input data
//
// returns : The command output data (packed in the Any object)
//
//-----------------------------------------------------------------------------
CORBA::Any *DbGetHostServerList::execute(Tango::DeviceImpl *device,const CORBA::Any &in_any)
{

	cout2 << "DbGetHostServerList::execute(): arrived" << endl;

	Tango::DevString	argin;
	extract(in_any, argin);

	return insert((static_cast<DataBase *>(device))->db_get_host_server_list(argin));
}

//+----------------------------------------------------------------------------
//
// method : 		DbGetHostList::execute()
// 
// description : 	method to trigger the execution of the command
//
// in : - device : The device on which the command must be excuted
//		- in_any : The command input data
//
// returns : The command output data (packed in the Any object)
//
//-----------------------------------------------------------------------------
CORBA::Any *DbGetHostList::execute(Tango::DeviceImpl *device,const CORBA::Any &in_any)
{

	cout2 << "DbGetHostList::execute(): arrived" << endl;

	Tango::DevString	argin;
	extract(in_any, argin);

	return insert((static_cast<DataBase *>(device))->db_get_host_list(argin));
}

//+----------------------------------------------------------------------------
//
// method : 		DbGetServerList::execute()
// 
// description : 	method to trigger the execution of the command
//
// in : - device : The device on which the command must be excuted
//		- in_any : The command input data
//
// returns : The command output data (packed in the Any object)
//
//-----------------------------------------------------------------------------
CORBA::Any *DbGetServerList::execute(Tango::DeviceImpl *device,const CORBA::Any &in_any)
{

	cout2 << "DbGetServerList::execute(): arrived" << endl;

	Tango::DevString	argin;
	extract(in_any, argin);

	return insert((static_cast<DataBase *>(device))->db_get_server_list(argin));
}

//+----------------------------------------------------------------------------
//
// method : 		DbGetClassPropertyList::execute()
// 
// description : 	method to trigger the execution of the command
//
// in : - device : The device on which the command must be excuted
//		- in_any : The command input data
//
// returns : The command output data (packed in the Any object)
//
//-----------------------------------------------------------------------------
CORBA::Any *DbGetClassPropertyList::execute(Tango::DeviceImpl *device,const CORBA::Any &in_any)
{

	cout2 << "DbGetClassPropertyList::execute(): arrived" << endl;

	Tango::DevString	argin;
	extract(in_any, argin);

	return insert((static_cast<DataBase *>(device))->db_get_class_property_list(argin));
}

//+----------------------------------------------------------------------------
//
// method : 		DbGetDevicePropertyList::execute()
// 
// description : 	method to trigger the execution of the command
//
// in : - device : The device on which the command must be excuted
//		- in_any : The command input data
//
// returns : The command output data (packed in the Any object)
//
//-----------------------------------------------------------------------------
CORBA::Any *DbGetDevicePropertyList::execute(Tango::DeviceImpl *device,const CORBA::Any &in_any)
{

	cout2 << "DbGetDevicePropertyList::execute(): arrived" << endl;

	const Tango::DevVarStringArray	*argin;
	extract(in_any, argin);

	return insert((static_cast<DataBase *>(device))->db_get_device_property_list(argin));
}

//+----------------------------------------------------------------------------
//
// method : 		DbGetDeviceAliasList::execute()
// 
// description : 	method to trigger the execution of the command
//
// in : - device : The device on which the command must be excuted
//		- in_any : The command input data
//
// returns : The command output data (packed in the Any object)
//
//-----------------------------------------------------------------------------
CORBA::Any *DbGetDeviceAliasList::execute(Tango::DeviceImpl *device,const CORBA::Any &in_any)
{

	cout2 << "DbGetDeviceAliasList::execute(): arrived" << endl;

	Tango::DevString	argin;
	extract(in_any, argin);

	return insert((static_cast<DataBase *>(device))->db_get_device_alias_list(argin));
}

//+----------------------------------------------------------------------------
//
// method : 		DbGetDeviceMemberList::execute()
// 
// description : 	method to trigger the execution of the command
//
// in : - device : The device on which the command must be excuted
//		- in_any : The command input data
//
// returns : The command output data (packed in the Any object)
//
//-----------------------------------------------------------------------------
CORBA::Any *DbGetDeviceMemberList::execute(Tango::DeviceImpl *device,const CORBA::Any &in_any)
{

	cout2 << "DbGetDeviceMemberList::execute(): arrived" << endl;

	Tango::DevString	argin;
	extract(in_any, argin);

	return insert((static_cast<DataBase *>(device))->db_get_device_member_list(argin));
}

//+----------------------------------------------------------------------------
//
// method : 		DbGetDeviceFamilyList::execute()
// 
// description : 	method to trigger the execution of the command
//
// in : - device : The device on which the command must be excuted
//		- in_any : The command input data
//
// returns : The command output data (packed in the Any object)
//
//-----------------------------------------------------------------------------
CORBA::Any *DbGetDeviceFamilyList::execute(Tango::DeviceImpl *device,const CORBA::Any &in_any)
{

	cout2 << "DbGetDeviceFamilyList::execute(): arrived" << endl;

	Tango::DevString	argin;
	extract(in_any, argin);

	return insert((static_cast<DataBase *>(device))->db_get_device_family_list(argin));
}

//+----------------------------------------------------------------------------
//
// method : 		DbGetDeviceDomainList::execute()
// 
// description : 	method to trigger the execution of the command
//
// in : - device : The device on which the command must be excuted
//		- in_any : The command input data
//
// returns : The command output data (packed in the Any object)
//
//-----------------------------------------------------------------------------
CORBA::Any *DbGetDeviceDomainList::execute(Tango::DeviceImpl *device,const CORBA::Any &in_any)
{

	cout2 << "DbGetDeviceDomainList::execute(): arrived" << endl;

	Tango::DevString	argin;
	extract(in_any, argin);

	return insert((static_cast<DataBase *>(device))->db_get_device_domain_list(argin));
}

//+----------------------------------------------------------------------------
//
// method : 		DbInfo::execute()
// 
// description : 	method to trigger the execution of the command
//
// in : - device : The device on which the command must be excuted
//		- in_any : The command input data
//
// returns : The command output data (packed in the Any object)
//
//-----------------------------------------------------------------------------
CORBA::Any *DbInfo::execute(Tango::DeviceImpl *device,const CORBA::Any &in_any)
{

	cout2 << "DbInfo::execute(): arrived" << endl;

	return insert((static_cast<DataBase *>(device))->db_info());
}




//+----------------------------------------------------------------------------
//
// method : 		DbGetClassList::execute()
// 
// description : 	method to trigger the execution of the command
//
// in : - device : The device on which the command must be excuted
//		- in_any : The command input data
//
// returns : The command output data (packed in the Any object)
//
//-----------------------------------------------------------------------------
CORBA::Any *DbGetClassList::execute(Tango::DeviceImpl *device,const CORBA::Any &in_any)
{

	cout2 << "DbGetClassList::execute(): arrived" << endl;

	Tango::DevString	argin;
	extract(in_any, argin);

	return insert((static_cast<DataBase *>(device))->db_get_class_list(argin));
}


//+----------------------------------------------------------------------------
//
// method : 		DbGetDeviceClassList::execute()
// 
// description : 	method to trigger the execution of the command
//
// in : - device : The device on which the command must be excuted
//		- in_any : The command input data
//
// returns : The command output data (packed in the Any object)
//
//-----------------------------------------------------------------------------
CORBA::Any *DbGetDeviceClassList::execute(Tango::DeviceImpl *device,const CORBA::Any &in_any)
{

	cout2 << "DbGetDeviceClassList::execute(): arrived" << endl;

	Tango::DevString	argin;
	extract(in_any, argin);

	return insert((static_cast<DataBase *>(device))->db_get_device_class_list(argin));
}


//+----------------------------------------------------------------------------
//
// method : 		DbGetDeviceProperty::execute()
// 
// description : 	method to trigger the execution of the command
//
// in : - device : The device on which the command must be excuted
//		- in_any : The command input data
//
// returns : The command output data (packed in the Any object)
//
//-----------------------------------------------------------------------------
CORBA::Any *DbGetDeviceProperty::execute(Tango::DeviceImpl *device,const CORBA::Any &in_any)
{

	cout2 << "DbGetDeviceProperty::execute(): arrived" << endl;

	const Tango::DevVarStringArray	*argin;
	extract(in_any, argin);

	return insert((static_cast<DataBase *>(device))->db_get_device_property(argin));
}


//+----------------------------------------------------------------------------
//
// method : 		DbPutDeviceProperty::execute()
// 
// description : 	method to trigger the execution of the command
//
// in : - device : The device on which the command must be excuted
//		- in_any : The command input data
//
// returns : The command output data (packed in the Any object)
//
//-----------------------------------------------------------------------------
CORBA::Any *DbPutDeviceProperty::execute(Tango::DeviceImpl *device,const CORBA::Any &in_any)
{

	cout2 << "DbPutDeviceProperty::execute(): arrived" << endl;

	const Tango::DevVarStringArray	*argin;
	extract(in_any, argin);

	((static_cast<DataBase *>(device))->db_put_device_property(argin));
	return new CORBA::Any();
}

//+----------------------------------------------------------------------------
//
// method : 		DbDeleteDeviceProperty::execute()
// 
// description : 	method to trigger the execution of the command
//
// in : - device : The device on which the command must be excuted
//		- in_any : The command input data
//
// returns : The command output data (packed in the Any object)
//
//-----------------------------------------------------------------------------
CORBA::Any *DbDeleteDeviceProperty::execute(Tango::DeviceImpl *device,const CORBA::Any &in_any)
{

	cout2 << "DbDeleteDeviceProperty::execute(): arrived" << endl;

	const Tango::DevVarStringArray	*argin;
	extract(in_any, argin);

	((static_cast<DataBase *>(device))->db_delete_device_property(argin));
	return new CORBA::Any();
}


//+----------------------------------------------------------------------------
//
// method : 		DbGetDeviceAttributeProperty::execute()
// 
// description : 	method to trigger the execution of the command
//
// in : - device : The device on which the command must be excuted
//		- in_any : The command input data
//
// returns : The command output data (packed in the Any object)
//
//-----------------------------------------------------------------------------
CORBA::Any *DbGetDeviceAttributeProperty::execute(Tango::DeviceImpl *device,const CORBA::Any &in_any)
{

	cout2 << "DbGetDeviceAttributeProperty::execute(): arrived" << endl;

	const Tango::DevVarStringArray	*argin;
	extract(in_any, argin);

	return insert((static_cast<DataBase *>(device))->db_get_device_attribute_property(argin));
}


//+----------------------------------------------------------------------------
//
// method : 		DbPutDeviceAttributeProperty::execute()
// 
// description : 	method to trigger the execution of the command
//
// in : - device : The device on which the command must be excuted
//		- in_any : The command input data
//
// returns : The command output data (packed in the Any object)
//
//-----------------------------------------------------------------------------
CORBA::Any *DbPutDeviceAttributeProperty::execute(Tango::DeviceImpl *device,const CORBA::Any &in_any)
{

	cout2 << "DbPutDeviceAttributeProperty::execute(): arrived" << endl;

	const Tango::DevVarStringArray	*argin;
	extract(in_any, argin);

	((static_cast<DataBase *>(device))->db_put_device_attribute_property(argin));
	return new CORBA::Any();
}

//+----------------------------------------------------------------------------
//
// method : 		DbGetClassAttributeProperty::execute()
// 
// description : 	method to trigger the execution of the command
//
// in : - device : The device on which the command must be excuted
//		- in_any : The command input data
//
// returns : The command output data (packed in the Any object)
//
//-----------------------------------------------------------------------------
CORBA::Any *DbGetClassAttributeProperty::execute(Tango::DeviceImpl *device,const CORBA::Any &in_any)
{

	cout2 << "DbGetClassAttributeProperty::execute(): arrived" << endl;

	const Tango::DevVarStringArray	*argin;
	extract(in_any, argin);

	return insert((static_cast<DataBase *>(device))->db_get_class_attribute_property(argin));
}


//+----------------------------------------------------------------------------
//
// method : 		DbPutClassAttributeProperty::execute()
// 
// description : 	method to trigger the execution of the command
//
// in : - device : The device on which the command must be excuted
//		- in_any : The command input data
//
// returns : The command output data (packed in the Any object)
//
//-----------------------------------------------------------------------------
CORBA::Any *DbPutClassAttributeProperty::execute(Tango::DeviceImpl *device,const CORBA::Any &in_any)
{

	cout2 << "DbPutClassAttributeProperty::execute(): arrived" << endl;

	const Tango::DevVarStringArray	*argin;
	extract(in_any, argin);

	((static_cast<DataBase *>(device))->db_put_class_attribute_property(argin));
	return new CORBA::Any();
}

//+----------------------------------------------------------------------------
//
// method : 		DbGetClassAttributeList::execute()
// 
// description : 	method to trigger the execution of the command
//
// in : - device : The device on which the command must be excuted
//		- in_any : The command input data
//
// returns : The command output data (packed in the Any object)
//
//-----------------------------------------------------------------------------
CORBA::Any *DbGetClassAttributeList::execute(Tango::DeviceImpl *device,const CORBA::Any &in_any)
{

	cout2 << "DbGetClassAttributeList::execute(): arrived" << endl;

	const Tango::DevVarStringArray	*argin;
	extract(in_any, argin);

	return insert((static_cast<DataBase *>(device))->db_get_class_attribute_list(argin));
}


//+----------------------------------------------------------------------------
//
// method : 		DbGetClassProperty::execute()
// 
// description : 	method to trigger the execution of the command
//
// in : - device : The device on which the command must be excuted
//		- in_any : The command input data
//
// returns : The command output data (packed in the Any object)
//
//-----------------------------------------------------------------------------
CORBA::Any *DbGetClassProperty::execute(Tango::DeviceImpl *device,const CORBA::Any &in_any)
{

	cout2 << "DbGetClassProperty::execute(): arrived" << endl;

	const Tango::DevVarStringArray	*argin;
	extract(in_any, argin);

	return insert((static_cast<DataBase *>(device))->db_get_class_property(argin));
}


//+----------------------------------------------------------------------------
//
// method : 		DbPutClassProperty::execute()
// 
// description : 	method to trigger the execution of the command
//
// in : - device : The device on which the command must be excuted
//		- in_any : The command input data
//
// returns : The command output data (packed in the Any object)
//
//-----------------------------------------------------------------------------
CORBA::Any *DbPutClassProperty::execute(Tango::DeviceImpl *device,const CORBA::Any &in_any)
{

	cout2 << "DbPutClassProperty::execute(): arrived" << endl;

	const Tango::DevVarStringArray	*argin;
	extract(in_any, argin);

	((static_cast<DataBase *>(device))->db_put_class_property(argin));
	return new CORBA::Any();
}

//+----------------------------------------------------------------------------
//
// method : 		DbDeleteClassProperty::execute()
// 
// description : 	method to trigger the execution of the command
//
// in : - device : The device on which the command must be excuted
//		- in_any : The command input data
//
// returns : The command output data (packed in the Any object)
//
//-----------------------------------------------------------------------------
CORBA::Any *DbDeleteClassProperty::execute(Tango::DeviceImpl *device,const CORBA::Any &in_any)
{

	cout2 << "DbDeleteClassProperty::execute(): arrived" << endl;

	const Tango::DevVarStringArray	*argin;
	extract(in_any, argin);

	((static_cast<DataBase *>(device))->db_delete_class_property(argin));
	return new CORBA::Any();
}


//+----------------------------------------------------------------------------
//
// method : 		DbExportDevice::execute()
// 
// description : 	method to trigger the execution of the command
//
// in : - device : The device on which the command must be excuted
//		- in_any : The command input data
//
// returns : The command output data (packed in the Any object)
//
//-----------------------------------------------------------------------------
CORBA::Any *DbExportDevice::execute(Tango::DeviceImpl *device,const CORBA::Any &in_any)
{

	cout2 << "DbExportDevice::execute(): arrived" << endl;

	const Tango::DevVarStringArray	*argin;
	extract(in_any, argin);

	((static_cast<DataBase *>(device))->db_export_device(argin));
	return new CORBA::Any();
}


//+----------------------------------------------------------------------------
//
// method : 		DbImportDevice::execute()
// 
// description : 	method to trigger the execution of the command
//
// in : - device : The device on which the command must be excuted
//		- in_any : The command input data
//
// returns : The command output data (packed in the Any object)
//
//-----------------------------------------------------------------------------
CORBA::Any *DbImportDevice::execute(Tango::DeviceImpl *device,const CORBA::Any &in_any)
{

	cout2 << "DbImportDevice::execute(): arrived" << endl;

	Tango::DevString	argin;
	extract(in_any, argin);

	return insert((static_cast<DataBase *>(device))->db_import_device(argin));
}


//+----------------------------------------------------------------------------
//
// method : 		DbUnExportDevice::execute()
// 
// description : 	method to trigger the execution of the command
//
// in : - device : The device on which the command must be excuted
//		- in_any : The command input data
//
// returns : The command output data (packed in the Any object)
//
//-----------------------------------------------------------------------------
CORBA::Any *DbUnExportDevice::execute(Tango::DeviceImpl *device,const CORBA::Any &in_any)
{

	cout2 << "DbUnExportDevice::execute(): arrived" << endl;

	Tango::DevString	argin;
	extract(in_any, argin);

	((static_cast<DataBase *>(device))->db_un_export_device(argin));
	return new CORBA::Any();
}


//+----------------------------------------------------------------------------
//
// method : 		DbUnExportServer::execute()
// 
// description : 	method to trigger the execution of the command
//
// in : - device : The device on which the command must be excuted
//		- in_any : The command input data
//
// returns : The command output data (packed in the Any object)
//
//-----------------------------------------------------------------------------
CORBA::Any *DbUnExportServer::execute(Tango::DeviceImpl *device,const CORBA::Any &in_any)
{

	cout2 << "DbUnExportServer::execute(): arrived" << endl;

	Tango::DevString	argin;
	extract(in_any, argin);

	((static_cast<DataBase *>(device))->db_un_export_server(argin));
	return new CORBA::Any();
}


//+----------------------------------------------------------------------------
//
// method : 		DbAddServer::execute()
// 
// description : 	method to trigger the execution of the command
//
// in : - device : The device on which the command must be excuted
//		- in_any : The command input data
//
// returns : The command output data (packed in the Any object)
//
//-----------------------------------------------------------------------------
CORBA::Any *DbAddServer::execute(Tango::DeviceImpl *device,const CORBA::Any &in_any)
{

	cout2 << "DbAddServer::execute(): arrived" << endl;

	const Tango::DevVarStringArray	*argin;
	extract(in_any, argin);

	((static_cast<DataBase *>(device))->db_add_server(argin));
	return new CORBA::Any();
}


//+----------------------------------------------------------------------------
//
// method : 		DbAddDevice::execute()
// 
// description : 	method to trigger the execution of the command
//
// in : - device : The device on which the command must be excuted
//		- in_any : The command input data
//
// returns : The command output data (packed in the Any object)
//
//-----------------------------------------------------------------------------
CORBA::Any *DbAddDevice::execute(Tango::DeviceImpl *device,const CORBA::Any &in_any)
{

	cout2 << "DbAddDevice::execute(): arrived" << endl;

	const Tango::DevVarStringArray	*argin;
	extract(in_any, argin);

	((static_cast<DataBase *>(device))->db_add_device(argin));
	return new CORBA::Any();
}


//+----------------------------------------------------------------------------
//
// method : 		DbGetAttributeAliasList::execute()
// 
// description : 	method to trigger the execution of the command
//
// in : - device : The device on which the command must be excuted
//		- in_any : The command input data
//
// returns : The command output data (packed in the Any object)
//
//-----------------------------------------------------------------------------
CORBA::Any *DbGetAttributeAliasList::execute(Tango::DeviceImpl *device,const CORBA::Any &in_any)
{

	cout2 << "DbGetAttributeAliasList::execute(): arrived" << endl;

	Tango::DevString	argin;
	extract(in_any, argin);

	return insert((static_cast<DataBase *>(device))->db_get_attribute_alias_list(argin));
}

//
//----------------------------------------------------------------
//	Initialize pointer for singleton pattern
//----------------------------------------------------------------
//
DataBaseClass *DataBaseClass::_instance = NULL;

//+----------------------------------------------------------------------------
//
// method : 		DataBaseClass::DataBaseClass(string &s)
// 
// description : 	constructor for the DataBaseClass
//
// in : - s : The class name
//
//-----------------------------------------------------------------------------
DataBaseClass::DataBaseClass(string &s):Tango::DeviceClass(s)
{

	cout2 << "Entering DataBaseClass constructor" << endl;
	get_class_property();
	set_default_property();
	write_class_property();
	
	cout2 << "Leaving DataBaseClass constructor" << endl;

}
//+----------------------------------------------------------------------------
//
// method : 		DataBaseClass::~DataBaseClass()
// 
// description : 	destructor for the DataBaseClass
//
//-----------------------------------------------------------------------------
DataBaseClass::~DataBaseClass()
{
	_instance = NULL;
}

//+----------------------------------------------------------------------------
//
// method : 		DataBaseClass::instance
// 
// description : 	Create the object if not already done. Otherwise, just
//			return a pointer to the object
//
// in : - name : The class name
//
//-----------------------------------------------------------------------------
DataBaseClass *DataBaseClass::init(const char *name)
{
	if (_instance == NULL)
	{
		try
		{
			string s(name);
			_instance = new DataBaseClass(s);
		}
		catch (bad_alloc)
		{
			throw;
		}		
	}		
	return _instance;
}

DataBaseClass *DataBaseClass::instance()
{
	if (_instance == NULL)
	{
		cerr << "Class is not initialised !!" << endl;
		exit(-1);
	}
	return _instance;
}

//+----------------------------------------------------------------------------
//
// method : 		DataBaseClass::command_factory
// 
// description : 	Create the command object(s) and store them in the 
//			command list
//
//-----------------------------------------------------------------------------
void DataBaseClass::command_factory()
{
	command_list.push_back(new DbAddDevice("DbAddDevice",
		Tango::DEVVAR_STRINGARRAY, Tango::DEV_VOID,
		"",
		"",
		Tango::OPERATOR));
	command_list.push_back(new DbAddServer("DbAddServer",
		Tango::DEVVAR_STRINGARRAY, Tango::DEV_VOID,
		"",
		"",
		Tango::OPERATOR));
	command_list.push_back(new DbDeleteAttributeAliasCmd("DbDeleteAttributeAlias",
		Tango::DEV_STRING, Tango::DEV_VOID,
		"alias name.",
		"",
		Tango::OPERATOR));
	command_list.push_back(new DbDeleteClassAttributeCmd("DbDeleteClassAttribute",
		Tango::DEVVAR_STRINGARRAY, Tango::DEV_VOID,
		"device",
		"attribute",
		Tango::OPERATOR));
	command_list.push_back(new DbDeleteClassAttributePropertyCmd("DbDeleteClassAttributeProperty",
		Tango::DEVVAR_STRINGARRAY, Tango::DEV_VOID,
		"device",
		"attribute",
		Tango::OPERATOR));
	command_list.push_back(new DbDeleteClassProperty("DbDeleteClassProperty",
		Tango::DEVVAR_STRINGARRAY, Tango::DEV_VOID,
		"",
		"",
		Tango::OPERATOR));
	command_list.push_back(new DbDeleteDeviceCmd("DbDeleteDevice",
		Tango::DEV_STRING, Tango::DEV_VOID,
		"device name",
		"",
		Tango::OPERATOR));
	command_list.push_back(new DbDeleteDeviceAliasCmd("DbDeleteDeviceAlias",
		Tango::DEV_STRING, Tango::DEV_VOID,
		"alias name",
		"",
		Tango::OPERATOR));
	command_list.push_back(new DbDeleteDeviceAttributeCmd("DbDeleteDeviceAttribute",
		Tango::DEVVAR_STRINGARRAY, Tango::DEV_VOID,
		"device",
		"attribute",
		Tango::OPERATOR));
	command_list.push_back(new DbDeleteDeviceAttributePropertyCmd("DbDeleteDeviceAttributeProperty",
		Tango::DEVVAR_STRINGARRAY, Tango::DEV_VOID,
		"device",
		"attribute",
		Tango::OPERATOR));
	command_list.push_back(new DbDeleteDeviceProperty("DbDeleteDeviceProperty",
		Tango::DEVVAR_STRINGARRAY, Tango::DEV_VOID,
		"",
		"",
		Tango::OPERATOR));
	command_list.push_back(new DbDeleteProperty("DbDeleteProperty",
		Tango::DEVVAR_STRINGARRAY, Tango::DEV_VOID,
		"",
		"",
		Tango::OPERATOR));
	command_list.push_back(new DbDeleteServerCmd("DbDeleteServer",
		Tango::DEV_STRING, Tango::DEV_VOID,
		"server name",
		"",
		Tango::OPERATOR));
	command_list.push_back(new DbDeleteServerInfoCmd("DbDeleteServerInfo",
		Tango::DEV_STRING, Tango::DEV_VOID,
		"server name",
		"",
		Tango::OPERATOR));
	command_list.push_back(new DbExportDevice("DbExportDevice",
		Tango::DEVVAR_STRINGARRAY, Tango::DEV_VOID,
		"",
		"",
		Tango::OPERATOR));
	command_list.push_back(new DbExportEventCmd("DbExportEvent",
		Tango::DEVVAR_STRINGARRAY, Tango::DEV_VOID,
		"event channel or factory",
		"IOR",
		Tango::OPERATOR));
	command_list.push_back(new DbGetAliasDeviceCmd("DbGetAliasDevice",
		Tango::DEV_STRING, Tango::DEV_STRING,
		"specified alias.",
		"Device name found.",
		Tango::OPERATOR));
	command_list.push_back(new DbGetAttributeAliasCmd("DbGetAttributeAlias",
		Tango::DEV_STRING, Tango::DEV_STRING,
		"alias",
		"attribute name",
		Tango::OPERATOR));
	command_list.push_back(new DbGetAttributeAliasList("DbGetAttributeAliasList",
		Tango::DEV_STRING, Tango::DEVVAR_STRINGARRAY,
		"attribute alias",
		"attribute name",
		Tango::OPERATOR));
	command_list.push_back(new DbGetClassAttributeList("DbGetClassAttributeList",
		Tango::DEVVAR_STRINGARRAY, Tango::DEVVAR_STRINGARRAY,
		"",
		"",
		Tango::OPERATOR));
	command_list.push_back(new DbGetClassAttributeProperty("DbGetClassAttributeProperty",
		Tango::DEVVAR_STRINGARRAY, Tango::DEVVAR_STRINGARRAY,
		"",
		"",
		Tango::OPERATOR));
	command_list.push_back(new DbGetClassAttributeProperty2Cmd("DbGetClassAttributeProperty2",
		Tango::DEVVAR_STRINGARRAY, Tango::DEVVAR_STRINGARRAY,
		"",
		"",
		Tango::OPERATOR));
	command_list.push_back(new DbGetClassAttributePropertyHistCmd("DbGetClassAttributePropertyHist",
		Tango::DEVVAR_STRINGARRAY, Tango::DEVVAR_STRINGARRAY,
		"",
		"",
		Tango::OPERATOR));
	command_list.push_back(new DbGetClassForDeviceClass("DbGetClassForDevice",
		Tango::DEV_STRING, Tango::DEV_STRING,
		"Device name",
		"Class off the specified device",
		Tango::OPERATOR));
	command_list.push_back(new DbGetClassInheritanceForDeviceClass("DbGetClassInheritanceForDevice",
		Tango::DEV_STRING, Tango::DEVVAR_STRINGARRAY,
		"Device name",
		"Classes off the specified device.\n[0] - is the class of the device.\n[1] - is the class from the device class is inherited.\n........and so on",
		Tango::OPERATOR));
	command_list.push_back(new DbGetClassList("DbGetClassList",
		Tango::DEV_STRING, Tango::DEVVAR_STRINGARRAY,
		"",
		"",
		Tango::OPERATOR));
	command_list.push_back(new DbGetClassProperty("DbGetClassProperty",
		Tango::DEVVAR_STRINGARRAY, Tango::DEVVAR_STRINGARRAY,
		"",
		"",
		Tango::OPERATOR));
	command_list.push_back(new DbGetClassPropertyHistClass("DbGetClassPropertyHist",
		Tango::DEVVAR_STRINGARRAY, Tango::DEVVAR_STRINGARRAY,
		"",
		"",
		Tango::OPERATOR));
	command_list.push_back(new DbGetClassPropertyList("DbGetClassPropertyList",
		Tango::DEV_STRING, Tango::DEVVAR_STRINGARRAY,
		"",
		"",
		Tango::OPERATOR));
	command_list.push_back(new DbGetDeviceAliasCmd("DbGetDeviceAlias",
		Tango::DEV_STRING, Tango::DEV_STRING,
		"The device name",
		"The alias found",
		Tango::OPERATOR));
	command_list.push_back(new DbGetDeviceAliasList("DbGetDeviceAliasList",
		Tango::DEV_STRING, Tango::DEVVAR_STRINGARRAY,
		"",
		"",
		Tango::OPERATOR));
	command_list.push_back(new DbGetDeviceAttributeList("DbGetDeviceAttributeList",
		Tango::DEVVAR_STRINGARRAY, Tango::DEVVAR_STRINGARRAY,
		"device name",
		"attribute wildcard",
		Tango::OPERATOR));
	command_list.push_back(new DbGetDeviceAttributeProperty("DbGetDeviceAttributeProperty",
		Tango::DEVVAR_STRINGARRAY, Tango::DEVVAR_STRINGARRAY,
		"",
		"",
		Tango::OPERATOR));
	command_list.push_back(new DbGetDeviceAttributeProperty2Cmd("DbGetDeviceAttributeProperty2",
		Tango::DEVVAR_STRINGARRAY, Tango::DEVVAR_STRINGARRAY,
		"",
		"",
		Tango::OPERATOR));
	command_list.push_back(new DbGetDeviceAttributePropertyHistCmd("DbGetDeviceAttributePropertyHist",
		Tango::DEVVAR_STRINGARRAY, Tango::DEVVAR_STRINGARRAY,
		"",
		"",
		Tango::OPERATOR));
	command_list.push_back(new DbGetDeviceClassList("DbGetDeviceClassList",
		Tango::DEV_STRING, Tango::DEVVAR_STRINGARRAY,
		"",
		"",
		Tango::OPERATOR));
	command_list.push_back(new DbGetDeviceDomainList("DbGetDeviceDomainList",
		Tango::DEV_STRING, Tango::DEVVAR_STRINGARRAY,
		"",
		"",
		Tango::OPERATOR));
	command_list.push_back(new DbGetDeviceExportedListCmd("DbGetDeviceExportedList",
		Tango::DEV_STRING, Tango::DEVVAR_STRINGARRAY,
		"filter",
		"list of exported devices",
		Tango::OPERATOR));
	command_list.push_back(new DbGetDeviceFamilyList("DbGetDeviceFamilyList",
		Tango::DEV_STRING, Tango::DEVVAR_STRINGARRAY,
		"",
		"",
		Tango::OPERATOR));
	command_list.push_back(new DbGetDeviceInfoClass("DbGetDeviceInfo",
		Tango::DEV_STRING, Tango::DEVVAR_LONGSTRINGARRAY,
		"Device name",
		"Info from DbImportDevice and started/stopped dates.",
		Tango::OPERATOR));
	command_list.push_back(new DbGetDeviceListCmd("DbGetDeviceList",
		Tango::DEVVAR_STRINGARRAY, Tango::DEVVAR_STRINGARRAY,
		"argin[0] : server name\nargin[1] : class name",
		"The list of devices for specified server and class.",
		Tango::OPERATOR));
	command_list.push_back(new DbGetDeviceWideListCmd("DbGetDeviceWideList",
		Tango::DEV_STRING, Tango::DEVVAR_STRINGARRAY,
		"filter",
		"list of exported devices",
		Tango::OPERATOR));
	command_list.push_back(new DbGetDeviceMemberList("DbGetDeviceMemberList",
		Tango::DEV_STRING, Tango::DEVVAR_STRINGARRAY,
		"",
		"",
		Tango::OPERATOR));
	command_list.push_back(new DbGetDeviceProperty("DbGetDeviceProperty",
		Tango::DEVVAR_STRINGARRAY, Tango::DEVVAR_STRINGARRAY,
		"",
		"",
		Tango::OPERATOR));
	command_list.push_back(new DbGetDevicePropertyHistClass("DbGetDevicePropertyHist",
		Tango::DEVVAR_STRINGARRAY, Tango::DEVVAR_STRINGARRAY,
		"",
		"",
		Tango::OPERATOR));
	command_list.push_back(new DbGetDevicePropertyList("DbGetDevicePropertyList",
		Tango::DEVVAR_STRINGARRAY, Tango::DEVVAR_STRINGARRAY,
		"",
		"",
		Tango::OPERATOR));
	command_list.push_back(new DbGetDeviceServerClassListCmd("DbGetDeviceServerClassList",
		Tango::DEV_STRING, Tango::DEVVAR_STRINGARRAY,
		"device server instance name",
		"list of classes for this device server",
		Tango::OPERATOR));
	command_list.push_back(new DbGetExportdDeviceListForClassCmd("DbGetExportdDeviceListForClass",
		Tango::DEV_STRING, Tango::DEVVAR_STRINGARRAY,
		"Class name",
		"Device exported list",
		Tango::OPERATOR));
	command_list.push_back(new DbGetHostList("DbGetHostList",
		Tango::DEV_STRING, Tango::DEVVAR_STRINGARRAY,
		"",
		"",
		Tango::OPERATOR));
	command_list.push_back(new DbGetHostServerList("DbGetHostServerList",
		Tango::DEV_STRING, Tango::DEVVAR_STRINGARRAY,
		"",
		"",
		Tango::OPERATOR));
	command_list.push_back(new DbGetHostServersInfoCmd("DbGetHostServersInfo",
		Tango::DEV_STRING, Tango::DEVVAR_STRINGARRAY,
		"host name",
		"server info for all servers running on specified host",
		Tango::OPERATOR));
	command_list.push_back(new DbGetInstanceNameListCmd("DbGetInstanceNameList",
		Tango::DEV_STRING, Tango::DEVVAR_STRINGARRAY,
		"server name",
		"The instance names found for specified server.",
		Tango::OPERATOR));
	command_list.push_back(new DbGetObjectListCmd("DbGetObjectList",
		Tango::DEV_STRING, Tango::DEVVAR_STRINGARRAY,
		"wild card",
		"list of object names",
		Tango::OPERATOR));
	command_list.push_back(new DbGetProperty("DbGetProperty",
		Tango::DEVVAR_STRINGARRAY, Tango::DEVVAR_STRINGARRAY,
		"",
		"",
		Tango::OPERATOR));
	command_list.push_back(new DbGetPropertyHistClass("DbGetPropertyHist",
		Tango::DEVVAR_STRINGARRAY, Tango::DEVVAR_STRINGARRAY,
		"",
		"",
		Tango::OPERATOR));
	command_list.push_back(new DbGetPropertyList("DbGetPropertyList",
		Tango::DEVVAR_STRINGARRAY, Tango::DEVVAR_STRINGARRAY,
		"",
		"",
		Tango::OPERATOR));
	command_list.push_back(new DbGetServerInfoCmd("DbGetServerInfo",
		Tango::DEV_STRING, Tango::DEVVAR_STRINGARRAY,
		"server name",
		"server info",
		Tango::OPERATOR));
	command_list.push_back(new DbGetServerList("DbGetServerList",
		Tango::DEV_STRING, Tango::DEVVAR_STRINGARRAY,
		"",
		"",
		Tango::OPERATOR));
	command_list.push_back(new DbGetServerNameListCmd("DbGetServerNameList",
		Tango::DEV_STRING, Tango::DEVVAR_STRINGARRAY,
		"wildcard for server names.",
		"server names found.",
		Tango::OPERATOR));
	command_list.push_back(new DbImportDevice("DbImportDevice",
		Tango::DEV_STRING, Tango::DEVVAR_LONGSTRINGARRAY,
		"",
		"",
		Tango::OPERATOR));
	command_list.push_back(new DbImportEventCmd("DbImportEvent",
		Tango::DEV_STRING, Tango::DEVVAR_LONGSTRINGARRAY,
		"name of event channel or factory",
		"export information e.g. IOR",
		Tango::OPERATOR));
	command_list.push_back(new DbInfo("DbInfo",
		Tango::DEV_VOID, Tango::DEVVAR_STRINGARRAY,
		"",
		"",
		Tango::OPERATOR));
	command_list.push_back(new DbPutAttributeAliasCmd("DbPutAttributeAlias",
		Tango::DEVVAR_STRINGARRAY, Tango::DEV_VOID,
		"attribute name, alias",
		"",
		Tango::OPERATOR));
	command_list.push_back(new DbPutClassAttributeProperty("DbPutClassAttributeProperty",
		Tango::DEVVAR_STRINGARRAY, Tango::DEV_VOID,
		"",
		"",
		Tango::OPERATOR));
	command_list.push_back(new DbPutClassAttributeProperty2Cmd("DbPutClassAttributeProperty2",
		Tango::DEVVAR_STRINGARRAY, Tango::DEV_VOID,
		"",
		"",
		Tango::OPERATOR));
	command_list.push_back(new DbPutClassProperty("DbPutClassProperty",
		Tango::DEVVAR_STRINGARRAY, Tango::DEV_VOID,
		"",
		"",
		Tango::OPERATOR));
	command_list.push_back(new DbPutDeviceAliasCmd("DbPutDeviceAlias",
		Tango::DEVVAR_STRINGARRAY, Tango::DEV_VOID,
		"device name",
		"alias",
		Tango::OPERATOR));
	command_list.push_back(new DbPutDeviceAttributeProperty("DbPutDeviceAttributeProperty",
		Tango::DEVVAR_STRINGARRAY, Tango::DEV_VOID,
		"",
		"",
		Tango::OPERATOR));
	command_list.push_back(new DbPutDeviceAttributeProperty2Cmd("DbPutDeviceAttributeProperty2",
		Tango::DEVVAR_STRINGARRAY, Tango::DEV_VOID,
		"",
		"",
		Tango::OPERATOR));
	command_list.push_back(new DbPutDeviceProperty("DbPutDeviceProperty",
		Tango::DEVVAR_STRINGARRAY, Tango::DEV_VOID,
		"",
		"",
		Tango::OPERATOR));
	command_list.push_back(new DbPutProperty("DbPutProperty",
		Tango::DEVVAR_STRINGARRAY, Tango::DEV_VOID,
		"",
		"",
		Tango::OPERATOR));
	command_list.push_back(new DbPutServerInfoCmd("DbPutServerInfo",
		Tango::DEVVAR_STRINGARRAY, Tango::DEV_VOID,
		"server info",
		"",
		Tango::OPERATOR));
	command_list.push_back(new DbUnExportDevice("DbUnExportDevice",
		Tango::DEV_STRING, Tango::DEV_VOID,
		"",
		"",
		Tango::OPERATOR));
	command_list.push_back(new DbUnExportEventCmd("DbUnExportEvent",
		Tango::DEV_STRING, Tango::DEV_VOID,
		"name of event channel or factory to unexport",
		"none",
		Tango::OPERATOR));
	command_list.push_back(new DbUnExportServer("DbUnExportServer",
		Tango::DEV_STRING, Tango::DEV_VOID,
		"",
		"",
		Tango::OPERATOR));
	command_list.push_back(new ResetTimingValuesClass("ResetTimingValues",
		Tango::DEV_VOID, Tango::DEV_VOID,
		"",
		"",
		Tango::OPERATOR));
	command_list.push_back(new DbGetDataForServerCacheCmd("DbGetDataForServerCache",
		Tango::DEVVAR_STRINGARRAY, Tango::DEVVAR_STRINGARRAY,
		"Elt[0] = DS name (exec_name/inst_name), Elt[1] = Host name",
		"All the data needed by the device server during its startup sequence. Precise list depend on the device server",
		Tango::OPERATOR));
	command_list.push_back(new DbDeleteAllDeviceAttributePropertyClass("DbDeleteAllDeviceAttributeProperty",
		Tango::DEVVAR_STRINGARRAY, Tango::DEV_VOID,
		"str[0] = device name, str[1]...str[n] = attribute name(s)",
		"",
		Tango::OPERATOR));
	command_list.push_back(new DbMySqlSelectCmd("DbMySqlSelect",
		Tango::DEV_STRING, Tango::DEVVAR_LONGSTRINGARRAY,
		"MySql Select command",
		"MySql Select command result\n - svalues : select results\n - lvalue[n] : =0 if svalue[n] is null else =1\n (last lvalue -1) is number of rows, (last lvalue) is number of fields",
		Tango::OPERATOR));

	//	add polling if any
	for (unsigned int i=0 ; i<command_list.size(); i++)
	{
	}
}

//+----------------------------------------------------------------------------
//
// method : 		DataBaseClass::get_class_property
// 
// description : 	Get the class property for specified name.
//
// in :		string	name : The property name
//
//+----------------------------------------------------------------------------
Tango::DbDatum DataBaseClass::get_class_property(string &prop_name)
{
	for (unsigned int i=0 ; i<cl_prop.size() ; i++)
		if (cl_prop[i].name == prop_name)
			return cl_prop[i];
	//	if not found, return  an empty DbDatum
	return Tango::DbDatum(prop_name);
}
//+----------------------------------------------------------------------------
//
// method : 		DataBaseClass::get_default_device_property()
// 
// description : 	Return the default value for device property.
//
//-----------------------------------------------------------------------------
Tango::DbDatum DataBaseClass::get_default_device_property(string &prop_name)
{
	for (unsigned int i=0 ; i<dev_def_prop.size() ; i++)
		if (dev_def_prop[i].name == prop_name)
			return dev_def_prop[i];
	//	if not found, return  an empty DbDatum
	return Tango::DbDatum(prop_name);
}

//+----------------------------------------------------------------------------
//
// method : 		DataBaseClass::get_default_class_property()
// 
// description : 	Return the default value for class property.
//
//-----------------------------------------------------------------------------
Tango::DbDatum DataBaseClass::get_default_class_property(string &prop_name)
{
	for (unsigned int i=0 ; i<cl_def_prop.size() ; i++)
		if (cl_def_prop[i].name == prop_name)
			return cl_def_prop[i];
	//	if not found, return  an empty DbDatum
	return Tango::DbDatum(prop_name);
}
//+----------------------------------------------------------------------------
//
// method : 		DataBaseClass::device_factory
// 
// description : 	Create the device object(s) and store them in the 
//			device list
//
// in :		Tango::DevVarStringArray *devlist_ptr : The device name list
//
//-----------------------------------------------------------------------------
void DataBaseClass::device_factory(const Tango::DevVarStringArray *devlist_ptr)
{


	cout << "DataBaseClass::DataBaseClass(): create database device " << DataBase::db_name.c_str() << endl;
	device_list.push_back(new DataBase(this, DataBase::db_name.c_str(),
                                    	"TANGO database device server"));

	export_device(device_list[0],"database");
}
//+----------------------------------------------------------------------------
//	Method: DataBaseClass::attribute_factory(vector<Tango::Attr *> &att_list)
//-----------------------------------------------------------------------------
void DataBaseClass::attribute_factory(vector<Tango::Attr *> &att_list)
{
	//	Attribute : StoredProcedureRelease
	StoredProcedureReleaseAttrib	*stored_procedure_release = new StoredProcedureReleaseAttrib();
	att_list.push_back(stored_procedure_release);

	//	Attribute : Timing_average
	Timing_averageAttrib	*timing_average = new Timing_averageAttrib();
	att_list.push_back(timing_average);

	//	Attribute : Timing_minimum
	Timing_minimumAttrib	*timing_minimum = new Timing_minimumAttrib();
	att_list.push_back(timing_minimum);

	//	Attribute : Timing_maximum
	Timing_maximumAttrib	*timing_maximum = new Timing_maximumAttrib();
	att_list.push_back(timing_maximum);

	//	Attribute : Timing_calls
	Timing_callsAttrib	*timing_calls = new Timing_callsAttrib();
	att_list.push_back(timing_calls);

	//	Attribute : Timing_index
	Timing_indexAttrib	*timing_index = new Timing_indexAttrib();
	att_list.push_back(timing_index);

	//	Attribute : Timing_info
	Timing_infoAttrib	*timing_info = new Timing_infoAttrib();
	att_list.push_back(timing_info);

	//	End of Automatic code generation
	//-------------------------------------------------------------
}

//+----------------------------------------------------------------------------
//
// method : 		DataBaseClass::get_class_property()
// 
// description : 	Read the class properties from database.
//
//-----------------------------------------------------------------------------
void DataBaseClass::get_class_property()
{
	//	Initialize your default values here (if not done with  POGO).
	//------------------------------------------------------------------

	//	Read class properties from database.(Automatic code generation)
	//------------------------------------------------------------------

	//	Call database and extract values
	//--------------------------------------------
	if (Tango::Util::instance()->_UseDb==true)
		get_db_class()->get_property(cl_prop);
	Tango::DbDatum	def_prop;
	int	i = -1;


	//	End of Automatic code generation
	//------------------------------------------------------------------

}

//+----------------------------------------------------------------------------
//
// method : 	DataBaseClass::set_default_property
// 
// description: Set default property (class and device) for wizard.
//              For each property, add to wizard property name and description
//              If default value has been set, add it to wizard property and
//              store it in a DbDatum.
//
//-----------------------------------------------------------------------------
void DataBaseClass::set_default_property()
{
	string	prop_name;
	string	prop_desc;
	string	prop_def;

	vector<string>	vect_data;
	//	Set Default Class Properties
	//	Set Default Device Properties
}
//+----------------------------------------------------------------------------
//
// method : 		DataBaseClass::write_class_property
// 
// description : 	Set class description as property in database
//
//-----------------------------------------------------------------------------
void DataBaseClass::write_class_property()
{
	//	First time, check if database used
	//--------------------------------------------
	if (Tango::Util::_UseDb == false)
		return;

	Tango::DbData	data;
	string	classname = get_name();
	string	header;
	string::size_type	start, end;

	//	Put title
	Tango::DbDatum	title("ProjectTitle");
	string	str_title("TANGO");
	title << str_title;
	data.push_back(title);

	//	Put Description
	Tango::DbDatum	description("Description");
	vector<string>	str_desc;
	str_desc.push_back("This class manage the TANGO database.");
	description << str_desc;
	data.push_back(description);
		
	//	put cvs or svn location
	string	filename(classname);
	filename += "Class.cpp";
	
	// Create a string with the class ID to
	// get the string into the binary
	string	class_id(ClassId);
	
	// check for cvs information
	string	src_path(CvsPath);
	start = src_path.find("/");
	if (start!=string::npos)
	{
		end   = src_path.find(filename);
		if (end>start)
		{
			string	strloc = src_path.substr(start, end-start);
			//	Check if specific repository
			start = strloc.find("/cvsroot/");
			if (start!=string::npos && start>0)
			{
				string	repository = strloc.substr(0, start);
				if (repository.find("/segfs/")!=string::npos)
					strloc = "ESRF:" + strloc.substr(start, strloc.length()-start);
			}
			Tango::DbDatum	cvs_loc("cvs_location");
			cvs_loc << strloc;
			data.push_back(cvs_loc);
		}
	}
	// check for svn information
	else
	{
		string	src_path(SvnPath);
		start = src_path.find("://");
		if (start!=string::npos)
		{
			end = src_path.find(filename);
			if (end>start)
			{
				header = "$HeadURL: ";
				start = header.length();
				string	strloc = src_path.substr(start, (end-start));
				
				Tango::DbDatum	svn_loc("svn_location");
				svn_loc << strloc;
				data.push_back(svn_loc);
			}
		}
	}

	//	Get CVS or SVN revision tag
	
	// CVS tag
	string	tagname(TagName);
	header = "$Name: ";
	start = header.length();
	string	endstr(" $");
	
	end   = tagname.find(endstr);
	if (end!=string::npos && end>start)
	{
		string	strtag = tagname.substr(start, end-start);
		Tango::DbDatum	cvs_tag("cvs_tag");
		cvs_tag << strtag;
		data.push_back(cvs_tag);
	}
	
	// SVN tag
	string	svnpath(SvnPath);
	header = "$HeadURL: ";
	start = header.length();
	
	end   = svnpath.find(endstr);
	if (end!=string::npos && end>start)
	{
		string	strloc = svnpath.substr(start, end-start);
		
		string tagstr ("/tags/");
		start = strloc.find(tagstr);
		if ( start!=string::npos )
		{
			start = start + tagstr.length();
			end   = strloc.find(filename);
			string	strtag = strloc.substr(start, end-start-1);
			
			Tango::DbDatum	svn_tag("svn_tag");
			svn_tag << strtag;
			data.push_back(svn_tag);
		}
	}

	//	Get URL location
	string	httpServ(HttpServer);
	if (httpServ.length()>0)
	{
		Tango::DbDatum	db_doc_url("doc_url");
		db_doc_url << httpServ;
		data.push_back(db_doc_url);
	}

	//  Put inheritance
	Tango::DbDatum	inher_datum("InheritedFrom");
	vector<string> inheritance;
	inheritance.push_back("Device_4Impl");
	inher_datum << inheritance;
	data.push_back(inher_datum);

	//	Call database and and values
	//--------------------------------------------
	get_db_class()->put_property(data);
}

}	// namespace
