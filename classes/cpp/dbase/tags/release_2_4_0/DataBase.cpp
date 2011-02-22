static const char *RcsId = "$Header$";
//+=============================================================================
//
// file :         DataBase.cpp
//
// description :  C++ source for the DataBase and its commands. 
//                The class is derived from Device. It represents the
//                CORBA servant object which will be accessed from the
//                network. All commands which can be executed on the
//                DataBase are implemented in this file.
//
// project :      TANGO Device Servers
//
// author(s) :    A.Gotz, P.Verdier
//
// $Version$
//
// $Log$
// Revision 2.37  2004/10/07 09:11:05  pascal_verdier
// Error in a DevFailed Exception message corrected.
//
// Revision 2.36  2004/10/07 08:53:56  pascal_verdier
// Memory leak on DbGetDeviceHost call fixed.
//
// Revision 2.35  2004/09/29 08:39:53  pascal_verdier
// Memory leak in db_import_device fixed.
//
// Revision 2.34  2004/07/06 13:27:18  pascal_verdier
// DbGetDeviceAlias command added.
//
// Revision 2.33  2004/06/21 14:29:07  pascal_verdier
// Stream problem fixed.
//
// Revision 2.32  2004/06/21 12:20:11  pascal_verdier
// DbGetExportdDeviceListForClass and DbGetDeviceAlias commands added.
//
// Revision 2.31  2004/05/19 07:26:20  pascal_verdier
// Windows project added.
//
// Revision 2.30  2004/04/20 13:58:35  pascal_verdier
// Bug on databaseds name for device name fixed.
//
// Revision 2.29  2004/03/30 14:24:53  pascal_verdier
// Bug on backslash on property name fixed.
//
// Revision 2.28  2004/03/08 14:11:01  nleclercq
// Minor changes in order to compile without warnings under msvc6
//
// Revision 2.27  2004/03/08 12:41:56  pascal_verdier
// Attribute alias management added.
//
// Revision 2.26  2004/02/27 12:54:21  pascal_verdier
// Check if escape sequence has not been already treated by client.
//
// Revision 2.25  2004/02/27 09:43:07  pascal_verdier
// Bug on escape sequence (quotes and double quotes) fixed.
//
// Revision 2.24  2004/02/23 10:24:37  pascal_verdier
// A thread has been added.
// This thread is in charge to warn concerned Starter servers to execute DbGetServerInfo.
// Because a new server has been exported (DbExportDevice command has been received),
// or server info have been modified (DbPutServerInfo command has been received).
//
// Revision 2.23  2004/01/26 13:25:00  pascal_verdier
// DbGetServerNameList and DbGetInstanceNameList commands added.
//
// Revision 2.22  2003/12/11 10:22:10  pascal_verdier
// DbGetHostServersInfo command added.
//
// Revision 2.21  2003/11/24 13:13:47  andy_gotz
// added support for environment variables MYSQL_USER and MYSQL_PASSWORD
// added db_bench, a program to bench the performance of importing a device
//
// Revision 2.20  2003/08/13 12:02:23  andy_gotz
// added DbPutAttributeAlias and DbGetAttributeAlias commands needed by
// the AttributeProxy object. Note this needs a new table attribute_alias
// in mysql.
//
// Revision 2.19  2003/04/25 09:12:42  taurel
// Change algo. used in the DbAddDevice command. It now create a DServer device entry only if it does not already exist. Otherwise, leave it unchanged.
// Fixes bugs in the parameter passed to some logging calls.
//
// Revision 2.18  2003/04/23 10:48:51  taurel
// Ported to gcc 3.2 and remove unused local variables which generates warnings when compiled under Windows VC++6
//
// Revision 2.17  2003/04/08 12:39:16  andy_gotz
// fixed bugs with use of underscores in server and device names; added commands for event table
//
// Revision 2.16  2003/03/20 14:55:10  andy_gotz
// added support for device aliases; fixed bug in updating server info; tagged server name onto end of import info; implemented support for importing and exporting events;
//
// Revision 2.15  2003/01/16 14:30:40  goetz
// ported Makefile to omniorb
//
// Revision 2.14  2002/11/26 10:00:58  goetz
// added delete_class_attribute_property; changed Solaris to CC; added pid to import_device
//
// Revision 2.13  2002/09/16 08:39:36  goetz
// added GetObjectList and GetPropertyList commands
//
// Revision 2.12  2002/02/04 17:09:08  goetz
// updated Windows port
//
// Revision 2.10  2001/07/12 12:15:18  goetz
// changed db_get_class_list() and db_delete_device_attribute_property()
//
// Revision 2.9  2001/07/04 05:17:03  goetz
// dserver device domain,family,member corrected; wildcards for DbGetClassList
//
// Revision 2.8  2001/07/04 04:42:24  goetz
// delete all properties before updating them
//
// Revision 2.6  2001/03/22 12:56:52  goetz
// fixed bug in DbAddDevice command, device name now unique in device table
//
// Revision 2.5  2001/03/06 12:05:44  goetz
// added DbGetDeviceExportedList; DbExportDevice updates host info in server table
//
// Revision 2.3  2001/03/05 12:10:52  goetz
// checking in before going to add new command(s)
//
// Revision 2.2  2001/01/03 11:58:28  goetz
// E.Taurel modified version for new TACO exception class
//
// Revision 2.1  2000/11/02 14:35:22  goetz
// added commands for server info
//
// Revision 2.0  2000/10/19 07:31:07  goetz
// changed major version number to 2
//
// Revision 1.16  2000/10/19 07:30:27  goetz
// ported Database to TANGO V2.0
//
// Revision 1.15  2000/10/02 08:37:15  goetz
// going to port to TANGO V2 i.e. OB V4
//
// Revision 1.14  2000/07/31 07:55:50  goetz
// added commands DbDeleteDevice and DbDeleteServer
//
// Revision 1.13  2000/07/29 13:33:03  goetz
// added commands DbDeleteDevice and DbDeleteServer, bug fixes
//
// Revision 1.12  2000/07/27 13:36:16  goetz
// checking in before going to add new commands with pogo
//
// Revision 1.11  2000/06/29 11:38:25  goetz
// intermediate checkin
//
// Revision 1.10  2000/06/06 11:04:56  goetz
// added DbGetDeviceAttributeList; modified DbGetDevicePropertyList; migrated to pogo
//
// Revision 1.9  2000/05/31 11:06:33  goetz
// added new commands; wildcard changed to *; servant name = database for all instances
//
// Revision 1.8  2000/05/25 08:32:05  goetz
// added browsing commands for Jive browser
//
// Revision 1.7  2000/05/17 14:41:23  goetz
// get domain, family and member commands converted to use database columns
//
// Revision 1.6  2000/05/16 11:47:09  goetz
// added get commands for alias, domain, family + member
//
// Revision 1.5  2000/03/13 17:36:43  goetz
// fixed bugs in Device Attribute Properties commands
//
// Revision 1.4  2000/02/16 14:12:51  goetz
// added file DataBase.pogo
//
// Revision 1.3  2000/02/16 14:12:02  goetz
// converted DataBaseds to POGO
//
//
// copyleft :     European Synchrotron Radiation Facility
//                BP 220, Grenoble 38043
//                FRANCE
//
//-=============================================================================
//
//  This file has been partially generated by POGO
//
//                     (c) - Pascal Verdier - ESRF
//
//=============================================================================


//===================================================================
//
//	The folowing table gives the correspondance
//	between commands and method's name.
//
//	Command's name	|	Method's name
//	----------------------------------------
//	State	|	dev_state()
//	Status	|	dev_status()
//	DbGetPropertyList	|	db_get_property_list()
//	DbGetProperty	|	db_get_property()
//	DbPutProperty	|	db_put_property()
//	DbDeleteProperty	|	db_delete_property()
//	DbGetHostServerList	|	db_get_host_server_list()
//	DbGetHostList	|	db_get_host_list()
//	DbGetServerList	|	db_get_server_list()
//	DbGetServerNameList	|	db_get_server_name_list()
//	DbGetInstanceNameList	|	db_get_instance_name_list()
//	DbGetClassPropertyList	|	db_get_class_property_list()
//	DbGetDevicePropertyList	|	db_get_device_property_list()
//	DbGetDeviceAlias	|	db_get_device_alias()
//	DbGetDeviceAliasList	|	db_get_device_alias_list()
//	DbGetDeviceMemberList	|	db_get_device_member_list()
//	DbGetDeviceFamilyList	|	db_get_device_family_list()
//	DbGetDeviceDomainList	|	db_get_device_domain_list()
//	DbInfo	|	db_info()
//	DbGetDeviceList	|	db_get_device_list()
//	DbGetClassList	|	db_get_class_list()
//	DbGetDeviceClassList	|	db_get_device_class_list()
//	DbGetDeviceProperty	|	db_get_device_property()
//	DbPutDeviceProperty	|	db_put_device_property()
//	DbDeleteDeviceProperty	|	db_delete_device_property()
//	DbGetDeviceAttributeProperty	|	db_get_device_attribute_property()
//	DbPutDeviceAttributeProperty	|	db_put_device_attribute_property()
//	DbGetClassAttributeProperty	|	db_get_class_attribute_property()
//	DbPutClassAttributeProperty	|	db_put_class_attribute_property()
//	DbGetClassProperty	|	db_get_class_property()
//	DbPutClassProperty	|	db_put_class_property()
//	DbDeleteClassProperty	|	db_delete_class_property()
//	DbExportDevice	|	db_export_device()
//	DbImportDevice	|	db_import_device()
//	DbUnExportDevice	|	db_un_export_device()
//	DbUnExportServer	|	db_un_export_server()
//	DbAddServer	|	db_add_server()
//	DbAddDevice	|	db_add_device()
//	DbGetClassAttributeList	|	db_get_class_attribute_list()
//	DbGetDeviceAttributeList	|	db_get_device_attribute_list()
//	DbDeleteDevice	|	db_delete_device()
//	DbDeleteServer	|	db_delete_server()
//	DbDeleteDeviceAttribute	|	db_delete_device_attribute()
//	DbDeleteDeviceAttributeProperty	|	db_delete_device_attribute_property()
//	DbDeleteClassAttribute	|	db_delete_class_attribute()
//	DbDeleteClassAttributeProperty	|	db_delete_class_attribute_property()
//	DbGetHostServersInfo	|	db_get_host_servers_info()
//	DbGetServerInfo	|	dev_get_server_info()
//	DbPutServerInfo	|	db_put_server_info()
//	DbDeleteServerInfo	|	db_delete_server_info()
//	DbGetDeviceExportedList	|	db_get_device_exported_list()
//	DbGetDeviceServerClassList	|	db_get_device_server_class_list()
//	DbGetObjectList	|	db_get_object_list()
//	DbGetAttributeAliasList	|	db_get_attribute_alias_list()
//	DbExportEvent	|	db_export_event()
//	DbImportEvent	|	db_import_event()
//	DbUnExportEvent	|	db_un_export_event()
//	DbPutDeviceAlias	|	db_put_device_alias()
//	DbPutAttributeAlias	|	db_put_attribute_alias()
//	DbGetAttributeAlias	|	db_get_attribute_alias()
//	DbGetAliasDevice	|	db_get_alias_device()
//	DbDeleteDeviceAlias	|	db_delete_device_alias()
//	DbDeleteAttributeAlias	|	db_delete_attribute_alias()
//	DbGetExportdDeviceListForClass	|	db_get_exportd_device_list_for_class()
//	DbPutDeviceAttributeProperty2	|	db_put_device_attribute_property2()
//	DbGetDeviceAttributeProperty2	|	db_get_device_attribute_property2()
//	DbPutClassAttributeProperty2	|	db_put_class_attribute_property2()
//	DbGetClassAttributeProperty2	|	db_get_class_attribute_property2()
//
//===================================================================



#include <tango.h>
#include <DataBase.h>
#include <mysql.h>
#include <stdio.h>
//#include <sys/time.h>

using namespace std;

namespace DataBase {

string DataBase::db_name("sys/database/1");

//+----------------------------------------------------------------------------
//
// method : 		DataBase::DataBase(string &s)
// 
// description : 	constructors for TANGO Database device server
//
// in : - cl : Pointer to the DeviceClass object
//      - s : Device name 
//
//-----------------------------------------------------------------------------
DataBase::DataBase(Tango::DeviceClass *cl,string &s):Device_2Impl(cl,s.c_str())
{
	init_device();
}

DataBase::DataBase(Tango::DeviceClass *cl,const char *s):Device_2Impl(cl,s)
{
	init_device();
}

DataBase::DataBase(Tango::DeviceClass *cl,const char *s,const char *d)
:Device_2Impl(cl,s,d)
{
	init_device();
}

void DataBase::init_device()
{
	char *database = (char *)"tango";
	char *mysql_user = (char *)"root";
	char *mysql_password = (char *)"";

        WARN_STREAM << "DataBase::DataBase() create database device " << device_name << endl;

// Initialise mysql database structure and connect to TANGO database

        mysql_init(&mysql);

	if (getenv("MYSQL_USER") != NULL)
	{
		mysql_user = getenv("MYSQL_USER");
	}
	if (getenv("MYSQL_PASSWORD") != NULL)
	{
		mysql_password = getenv("MYSQL_PASSWORD");
	}
        WARN_STREAM << "DataBase::DataBase() mysql database user =  " << mysql_user << " , password = " << mysql_password << endl;

//        if (!mysql_real_connect(&mysql,NULL,"root","",database,0,NULL,0))
	if (!mysql_real_connect(&mysql, "localhost", mysql_user, mysql_password, database, 0, NULL, 0))
        {
           WARN_STREAM << "DataBase::init_device(): failed to connect to TANGO database (error = " ;
           WARN_STREAM << mysql_error(&mysql) << endl;
        }

	try
	{
		//	Check if controlled server list modification must be fired to starter
		Tango::DevVarStringArray *argin = new Tango::DevVarStringArray();
		argin->length(2);
		(*argin)[0] = CORBA::string_dup("Default");
		(*argin)[1] = CORBA::string_dup("FireToStarter");
		Tango::DevVarStringArray *argout = db_get_property(argin);

		if ((*argout)[3] == 0)	//	Not defined
			fireToStarter = true;
		else
		{
			//	Get property value
			string	value((*argout)[4]);
			transform(value.begin(), value.end(), value.begin(), ::tolower);
			if (value=="false")	
				fireToStarter = false;
			else
				fireToStarter = true;
		}
		delete argin;
		delete argout;
	}
	catch(Tango::DevFailed &)
	{
		fireToStarter = true;
	}
	WARN_STREAM << "fireToStarter = " << fireToStarter << endl;

	//	If fire to starter is true
	if (fireToStarter==true)
	{
		//	Build shared data and thread to update Starter in case of
		//	change of controlled servers conditions
		starter_shared = new UpdStarterData();
		upd_starter_thread = new UpdateStarter(starter_shared);
		upd_starter_thread->start();
	}
}

//+----------------------------------------------------------------------------
//
// method : 		DataBase::always_executed_hook()
// 
// description : 	method always executed before any command is executed
//
//-----------------------------------------------------------------------------
void DataBase::always_executed_hook()
{

	INFO_STREAM << "In always_executed_hook method" << endl;

}

//+----------------------------------------------------------------------------
//
// method : 		DataBase::state_cmd()
// 
// description : 	command to read the device state
//
// out :		device state
//
//-----------------------------------------------------------------------------
Tango::DevState DataBase::dev_state()
{

	INFO_STREAM << "In DataBase state command" << endl;
	
	return device_state;
}

//+----------------------------------------------------------------------------
//
// method : 		DataBase::status_cmd()
// 
// description : 	command to read the device status
//
// out :		device status
//
//-----------------------------------------------------------------------------
const char* DataBase::dev_status()
{

	INFO_STREAM << "In DataBase status command" << endl;

	return device_status.c_str();
}

//+------------------------------------------------------------------
/**
 *	method:	DataBase::db_get_object_list
 *
 *	description:	method to execute "DbGetObjectList"
 *	DataBase methods prototypes
 *
 * @param	argin	wild card
 * @return	list of object names
 *
 */
//+------------------------------------------------------------------
Tango::DevVarStringArray *DataBase::db_get_object_list(Tango::DevString wildcard)
{
	static char sql_query[256];
	static char error_mess[256];
	MYSQL_RES *result;
	MYSQL_ROW row;
	int n_rows, i;
	string tmp_wildcard;

	INFO_STREAM << "DataBase::db_get_object_list(): object " << wildcard << endl;

	tmp_wildcard = replace_wildcard(wildcard);
	sprintf(sql_query,"SELECT DISTINCT object FROM property WHERE object LIKE \"%s\" ORDER BY object",
	        tmp_wildcard.c_str());
	DEBUG_STREAM << "DataBase::db_get_object_list(): sql_query " << sql_query << endl;

	if (mysql_real_query(&mysql, sql_query, strlen(sql_query)) != 0)
	{
	   WARN_STREAM << "DataBase::db_get_object_list(): failed to query TANGO database for list of objects";
	   WARN_STREAM << wildcard << " (error=" << mysql_error(&mysql) << ")" << endl;
	   sprintf(error_mess,"failed to query TANGO database for list of objects (error=%s)",mysql_error(&mysql));
	   Tango::Except::throw_exception((const char *)DB_SQLError,
	   				  (const char *)error_mess,
					  (const char *)"DataBase::db_get_object_list()");
	}

	if ((result = mysql_store_result(&mysql)) == NULL)
	{
	   WARN_STREAM << "DataBase::db_get_object_list(): mysql_store_result() failed  (error=" << mysql_error(&mysql) << ")" << endl;
	   sprintf(error_mess,"mysql_store_result() failed (error=%s)",mysql_error(&mysql));
	   Tango::Except::throw_exception((const char *)DB_SQLError,
	   				  (const char *)error_mess,
					  (const char *)"DataBase::db_get_object_list()");
	}

	n_rows = mysql_num_rows(result);
	DEBUG_STREAM << "DataBase::db_get_object_list(): mysql_num_rows() " << n_rows << endl;
	Tango::DevVarStringArray *object_list = new Tango::DevVarStringArray;

	if (n_rows > 0)
	{
	   object_list->length(n_rows);

	   for (i=0; i<n_rows; i++)
	   {
	      if ((row = mysql_fetch_row(result)) != NULL)
	      {
	         DEBUG_STREAM << "DataBase::db_get_object_list(): object[ "<< i << "] object " << row[0] << endl;
	         (*object_list)[i]   = CORBA::string_dup(row[0]);
	      }
	   }
	}
	else
		object_list->length(0);
	mysql_free_result(result);

	return(object_list);
}

//+------------------------------------------------------------------
/**
 *	method:	DataBase::db_get_property_list
 *
 *	description:	method to execute "DbGetPropertyList"
 *	DataBase methods prototypes
 *
 * @param	argin	
 * @return	
 *
 */
//+------------------------------------------------------------------
Tango::DevVarStringArray *DataBase::db_get_property_list(const Tango::DevVarStringArray *object_wildcard)
{
	static char sql_query[256];
	static char error_mess[256];
	MYSQL_RES *result;
	MYSQL_ROW row;
	int n_rows, i;
	const char *object, *wildcard;
	string tmp_wildcard;

	if (object_wildcard->length() != 2)
	{
	   WARN_STREAM << "DataBase::db_get_property_list(): incorrect number of input arguments " << endl;
	   sprintf(error_mess,"incorrect no. of input arguments, needs 2 (object,wildcard)");
	   Tango::Except::throw_exception((const char *)DB_IncorrectArguments,
	   				  (const char *)error_mess,
					  (const char *)"DataBase::GetPropertyList()");
	}
	object = (*object_wildcard)[0];
	wildcard = (*object_wildcard)[1];

	INFO_STREAM << "DataBase::db_get_property_list(): object " << object << endl;

	if (object == NULL)
	{
		sprintf(sql_query,"SELECT DISTINCT name FROM property WHERE object LIKE \"%\" ORDER BY name");
	}
	else
	{
		tmp_wildcard = replace_wildcard(wildcard);
		sprintf(sql_query,"SELECT DISTINCT name FROM property WHERE object LIKE \"%s\" AND name LIKE \"%s\" ORDER BY name",
	        object, tmp_wildcard.c_str());
	}
	DEBUG_STREAM << "DataBase::db_get_property_list(): sql_query " << sql_query << endl;

	if (mysql_real_query(&mysql, sql_query, strlen(sql_query)) != 0)
	{
	   WARN_STREAM << "DataBase::db_get_property_list(): failed to query TANGO database for list of properties";
	   WARN_STREAM << object << " (error=" << mysql_error(&mysql) << ")" << endl;
	   sprintf(error_mess,"failed to query TANGO database for list of properties (error=%s)",mysql_error(&mysql));
	   Tango::Except::throw_exception((const char *)DB_SQLError,
	   				  (const char *)error_mess,
					  (const char *)"DataBase::db_get_property_list()");
	}

	if ((result = mysql_store_result(&mysql)) == NULL)
	{
	   WARN_STREAM << "DataBase::db_get_property_list(): mysql_store_result() failed  (error=" << mysql_error(&mysql) << ")" << endl;
	   sprintf(error_mess,"mysql_store_result() failed (error=%s)",mysql_error(&mysql));
	   Tango::Except::throw_exception((const char *)DB_SQLError,
	   				  (const char *)error_mess,
					  (const char *)"DataBase::db_get_property_list()");
	}

	n_rows = mysql_num_rows(result);
	DEBUG_STREAM << "DataBase::db_get_property_list(): mysql_num_rows() " << n_rows << endl;
	Tango::DevVarStringArray *property_list = new Tango::DevVarStringArray;

	if (n_rows > 0)
	{
	   property_list->length(n_rows);

	   for (i=0; i<n_rows; i++)
	   {
	      if ((row = mysql_fetch_row(result)) != NULL)
	      {
	         DEBUG_STREAM << "DataBase::db_get_property_list(): property[ "<< i << "] property " << row[0] << endl;
	         (*property_list)[i]   = CORBA::string_dup(row[0]);
	      }
	   }
	}
	else
		property_list->length(0);
	mysql_free_result(result);

	return(property_list);
}

//+------------------------------------------------------------------
/**
 *	method:	DataBase::db_get_property
 *
 *	description:	method to execute "DbGetProperty"
 *	DataBase methods prototypes
 *
 * @param	argin	
 * @return	
 *
 */
//+------------------------------------------------------------------
Tango::DevVarStringArray *DataBase::db_get_property(const Tango::DevVarStringArray *property_names)
{
	static char sql_query[256];
	static char error_mess[256];
	static char n_properties_str[256];
	static char n_rows_str[256];
	MYSQL_RES *result;
	MYSQL_ROW row;
	int n_properties=0, n_rows=0, n_props=0, i, j;
	Tango::DevVarStringArray *property_list = new Tango::DevVarStringArray;
	const char *tmp_object;
	string tmp_name;

	INFO_STREAM << "DataBase::db_get_property(): get " << property_names->length()-1 << " properties for object " << (*property_names)[0] << endl;

	tmp_object = (*property_names)[0];
	sprintf(n_properties_str, "%d", property_names->length()-1);
	property_list->length(2);
	(*property_list)[0] = CORBA::string_dup(tmp_object);
	(*property_list)[1] = CORBA::string_dup(n_properties_str);
        n_props = 2;

	for (i=1; i<property_names->length(); i++)
	{
	   tmp_name = replace_wildcard((*property_names)[i]);
	   sprintf(sql_query,"SELECT count,value,name FROM property WHERE object = \"%s\" AND name LIKE \"%s\" ORDER BY count",
	           tmp_object, tmp_name.c_str());
	   DEBUG_STREAM << "DataBase::db_get_property(): sql_query " << sql_query << endl;

	   if (mysql_real_query(&mysql, sql_query, strlen(sql_query)) != 0)
	   {
	      WARN_STREAM << "DataBase::db_get_property(): failed to query TANGO database for list of properties ";
	      WARN_STREAM << "for " << (*property_names)[0] << "/" << (*property_names)[i] << " (error=" << mysql_error(&mysql) << ")" << endl;
	      sprintf(error_mess,"failed to query TANGO database for list of properties for %s/%s (error=%s)",tmp_object,tmp_name.c_str(),mysql_error(&mysql));
	      Tango::Except::throw_exception((const char *)DB_SQLError,
	      				     (const char *)error_mess,
					     (const char *)"DataBase::db_get_property()");
	   }

	   if ((result = mysql_store_result(&mysql)) == NULL)
	   {
	      WARN_STREAM << "DataBase::db_get_property(): mysql_store_result() failed  (error=" << mysql_error(&mysql) << ")" << endl;
	      sprintf(error_mess,"mysql_store_result() failed (error=%s)",mysql_error(&mysql));
	      Tango::Except::throw_exception((const char *)DB_SQLError,
	      				     (const char *)error_mess,
					     (const char *)"DataBase::db_get_property()");
	   }

	   n_rows = mysql_num_rows(result);
	   DEBUG_STREAM << "DataBase::db_get_property(): mysql_num_rows() " << n_rows << endl;
	   sprintf(n_rows_str,"%d",n_rows);
	   n_props = n_props+2;
	   property_list->length(n_props);
	   (*property_list)[n_props-2] = CORBA::string_dup(tmp_name.c_str());
           (*property_list)[n_props-1] = CORBA::string_dup(n_rows_str);
	   if (n_rows > 0)
	   {
	      for (j=0; j<n_rows; j++)
	      {
	         if ((row = mysql_fetch_row(result)) != NULL)
	         {
	            DEBUG_STREAM << "DataBase::db_get_property(): property[ "<< i << "] count " << row[0] << " value " << row[1] << endl;
		    n_props++;
	   	    property_list->length(n_props);
	            (*property_list)[n_props-1] = CORBA::string_dup(row[1]);
	         }
	      }
	      n_properties += n_rows;
	   }
	   else
	   {
	      n_props++;
	      property_list->length(n_props);
	      (*property_list)[n_props-1] = CORBA::string_dup(" ");
	   }
	   mysql_free_result(result);
	}

	DEBUG_STREAM << "DataBase::db_get_property(): property_list->length() "<< property_list->length() << endl;

	return(property_list);
}

//+------------------------------------------------------------------
/**
 *	method:	DataBase::db_put_property
 *
 *	description:	method to execute "DbPutProperty"
 *
 * @param	argin	
 *
 */
//+------------------------------------------------------------------
void DataBase::db_put_property(const Tango::DevVarStringArray *property_list)
{
	static char sql_query[512];
	static char error_mess[256];
	static char tmp_count_str[256];
	int n_properties=0, n_rows=0, i, j, k;
	const char *tmp_object, *tmp_count;
	string tmp_name;

	sscanf((*property_list)[1],"%d",&n_properties);
	INFO_STREAM << "DataBase::db_put_property(): put " << n_properties << " properties for object " << (*property_list)[0] << endl;

	k = 2;
	for (i=0; i<n_properties; i++)
	{
	   tmp_count = 0;
	   sscanf((*property_list)[k+1], "%d", &n_rows);
	   for (j=k+2; j<k+n_rows+2; j++)
	   {
	      tmp_object = (*property_list)[0];
//	      tmp_name = replace_wildcard((*property_list)[k]);
	      tmp_name = (*property_list)[k];
          string tmp_escaped_string = escape_string((*property_list)[j]);
	      tmp_count++; sprintf(tmp_count_str, "%d", tmp_count);

// first delete the tuple (device,name,count) from the property table

	      sprintf(sql_query,"DELETE FROM property WHERE object=\"%s\" AND name=\"%s\" AND count=\"%s\"",
	              tmp_object, tmp_name.c_str(), tmp_count_str);
	      DEBUG_STREAM << "DataBase::db_put_property(): sql_query " << sql_query << endl;
	      if (mysql_real_query(&mysql, sql_query, strlen(sql_query)) != 0)
	      {
	         WARN_STREAM << "DataBase::db_put_property(): failed to delete property from TANGO database ";
	         WARN_STREAM << "for " << tmp_object << "/" << tmp_name.c_str() << " (error=" << mysql_error(&mysql) << ")" << endl;
	         sprintf(error_mess,"failed to delete property from TANGO database  for %s/%s (error=%s)",tmp_object,tmp_name.c_str(),mysql_error(&mysql));
	         Tango::Except::throw_exception((const char *)DB_SQLError,
		 				(const char *)error_mess,
						(const char *)"DataBase::db_put_property()");
	      }

// then insert the new value for this tuple

	      sprintf(sql_query,"INSERT INTO property SET object=\'%s\',name=\'%s\',count=\'%s\',value=\'%s\',updated=NULL,accessed=NULL",
	              tmp_object, tmp_name.c_str(), tmp_count_str, tmp_escaped_string.c_str());
	      DEBUG_STREAM << "DataBase::db_put_property(): sql_query " << sql_query << endl;

	      if (mysql_real_query(&mysql, sql_query, strlen(sql_query)) != 0)
	      {
	         WARN_STREAM << "DataBase::db_put_property(): failed to insert property into TANGO database ";
	         WARN_STREAM << "for " << tmp_object << "/" << tmp_name.c_str() << " (error=" << mysql_error(&mysql) << ")" << endl;
	         sprintf(error_mess,"failed to insert property into TANGO database  for %s/%s (error=%s)",tmp_object,tmp_name.c_str(),mysql_error(&mysql));
	         Tango::Except::throw_exception((const char *)DB_SQLError,
		 				(const char *)error_mess,
						(const char *)"DataBase::db_put_property()");
	      }
	   }
	   k = k+n_rows+2;
	}

	return;
}

//+------------------------------------------------------------------
/**
 *	method:	DataBase::db_delete_property
 *
 *	description:	method to execute "DbDeleteProperty"
 *
 * @param	argin	
 *
 */
//+------------------------------------------------------------------
void DataBase::db_delete_property(const Tango::DevVarStringArray *property_list)
{
	static char sql_query[512];
	static char error_mess[256];
	static char tmp_count_str[256];
	int n_properties=0, i;
	const char *tmp_object;
	string tmp_name;

	n_properties = property_list->length() - 1;
	INFO_STREAM << "DataBase::db_delete_property(): put " << n_properties << " properties for device " << (*property_list)[0] << endl;

	for (i=0; i<n_properties; i++)
	{
	      tmp_object = (*property_list)[0];
	      tmp_name = replace_wildcard((*property_list)[i+1]);

// delete the tuple (device,name,count) from the property table

	      sprintf(sql_query,"DELETE FROM property WHERE object=\"%s\" AND name = \"%s\"",
	              tmp_object, tmp_name.c_str());
	      DEBUG_STREAM << "DataBase::db_delete_property(): sql_query " << sql_query << endl;
	      if (mysql_real_query(&mysql, sql_query, strlen(sql_query)) != 0)
	      {
	         WARN_STREAM << "DataBase::db_delete_property(): failed to delete property from TANGO database ";
	         WARN_STREAM << "for " << tmp_object << "/" << tmp_name << " (error=" << mysql_error(&mysql) << ")" << endl;
	         sprintf(error_mess,"failed to delete property from TANGO database  for %s/%s (error=%s)",tmp_object,tmp_name.c_str(),mysql_error(&mysql));
	         Tango::Except::throw_exception((const char *)DB_SQLError,
		 				(const char *)error_mess,
						(const char *)"DataBase::db_delete_property()");
	      }

	}

	return;
}
//+------------------------------------------------------------------
/**
 *	method:	DataBase::db_get_host_server_list
 *
 *	description:	method to execute "DbGetHostServerList"
 *
 * @param	argin	
 * @return	
 *
 */
//+------------------------------------------------------------------
Tango::DevVarStringArray *DataBase::db_get_host_server_list(Tango::DevString wildcard)
{
	static char sql_query[256];
	static char error_mess[256];
	string tmp_wildcard;
	MYSQL_RES *result;
	MYSQL_ROW row;
	int n_rows, i;

	INFO_STREAM << "DataBase::db_get_host_server_list(): wild card " << wildcard << endl;

	if (wildcard == NULL)
	{
		sprintf(sql_query,"SELECT DISTINCT server FROM device WHERE host LIKE \"%\" ORDER BY server");
	}
	else
	{
		tmp_wildcard = replace_wildcard(wildcard);
		sprintf(sql_query,"SELECT DISTINCT server FROM device WHERE host LIKE \"%s\" ORDER BY server",
	        tmp_wildcard.c_str());
	}
	DEBUG_STREAM << "DataBase::db_get_host_server_list(): sql_query " << sql_query << endl;

	if (mysql_real_query(&mysql, sql_query, strlen(sql_query)) != 0)
	{
	   WARN_STREAM << "DataBase::db_get_host_server_list(): failed to query TANGO database for list of servers";
	   WARN_STREAM << wildcard << " (error=" << mysql_error(&mysql) << ")" << endl;
	   sprintf(error_mess,"failed to query TANGO database for list of servers (error=%s)",mysql_error(&mysql));
	   Tango::Except::throw_exception((const char *)DB_SQLError,
	   				  (const char *)error_mess,
					  (const char *)"DataBase::db_get_host_server_list()");
	}

	if ((result = mysql_store_result(&mysql)) == NULL)
	{
	   WARN_STREAM << "DataBase::db_get_host_server_list(): mysql_store_result() failed  (error=" << mysql_error(&mysql) << ")" << endl;
	   sprintf(error_mess,"mysql_store_result() failed (error=%s)",mysql_error(&mysql));
	   Tango::Except::throw_exception((const char *)DB_SQLError,
	   				  (const char *)error_mess,
					  (const char *)"DataBase::db_get_host_server_list()");
	}

	n_rows = mysql_num_rows(result);
	DEBUG_STREAM << "DataBase::db_get_host_server_list(): mysql_num_rows() " << n_rows << endl;
	Tango::DevVarStringArray *server_list = new Tango::DevVarStringArray;

	if (n_rows > 0)
	{
	   server_list->length(n_rows);

	   for (i=0; i<n_rows; i++)
	   {
	      if ((row = mysql_fetch_row(result)) != NULL)
	      {
	         DEBUG_STREAM << "DataBase::db_get_host_server_list(): row[ "<< i << "] alias " << row[0] << endl;
	         (*server_list)[i]   = CORBA::string_dup(row[0]);
	      }
	   }
	}
	else
		server_list->length(0);
	mysql_free_result(result);

	return(server_list);
}

//+------------------------------------------------------------------
/**
 *	method:	DataBase::db_get_host_list
 *
 *	description:	method to execute "DbGetHostList"
 *
 * @param	argin	
 * @return	
 *
 */
//+------------------------------------------------------------------
Tango::DevVarStringArray *DataBase::db_get_host_list(Tango::DevString wildcard)
{
	static char sql_query[256];
	static char error_mess[256];
	string tmp_wildcard;
	MYSQL_RES *result;
	MYSQL_ROW row;
	int n_rows, i;

	INFO_STREAM << "DataBase::db_get_host_list(): wild card " << wildcard << endl;

	if (wildcard == NULL)
	{
		sprintf(sql_query,"SELECT DISTINCT host FROM device WHERE host LIKE \"%\" ORDER BY host");
	}
	else
	{
		tmp_wildcard = replace_wildcard(wildcard);
		sprintf(sql_query,"SELECT DISTINCT host FROM device WHERE host LIKE \"%s\" ORDER BY host",
	        tmp_wildcard.c_str());
	}
	DEBUG_STREAM << "DataBase::db_get_host_list(): sql_query " << sql_query << endl;

	if (mysql_real_query(&mysql, sql_query, strlen(sql_query)) != 0)
	{
	   WARN_STREAM << "DataBase::db_get_host_list(): failed to query TANGO database for list of hosts";
	   WARN_STREAM << wildcard << " (error=" << mysql_error(&mysql) << ")" << endl;
	   sprintf(error_mess,"failed to query TANGO database for list of hosts (error=%s)",mysql_error(&mysql));
	   Tango::Except::throw_exception((const char *)DB_SQLError,
	   				  (const char *)error_mess,
					  (const char *)"DataBase::db_get_host_list()");
	}

	if ((result = mysql_store_result(&mysql)) == NULL)
	{
	   WARN_STREAM << "DataBase::db_get_host_list(): mysql_store_result() failed  (error=" << mysql_error(&mysql) << ")" << endl;
	   sprintf(error_mess,"mysql_store_result() failed (error=%s)",mysql_error(&mysql));
	   Tango::Except::throw_exception((const char *)DB_SQLError,
	   				  (const char *)error_mess,
					  (const char *)"DataBase::db_get_host_list()");
	}

	n_rows = mysql_num_rows(result);
	DEBUG_STREAM << "DataBase::db_get_host_list(): mysql_num_rows() " << n_rows << endl;
	Tango::DevVarStringArray *host_list = new Tango::DevVarStringArray;

	if (n_rows > 0)
	{
	   host_list->length(n_rows);

	   for (i=0; i<n_rows; i++)
	   {
	      if ((row = mysql_fetch_row(result)) != NULL)
	      {
	         DEBUG_STREAM << "DataBase::db_get_host_list(): host[ "<< i << "] alias " << row[0] << endl;
	         (*host_list)[i]   = CORBA::string_dup(row[0]);
	      }
	   }
	}
	else
		host_list->length(0);
	mysql_free_result(result);

	return(host_list);
}

//+------------------------------------------------------------------
/**
 *	method:	DataBase::db_get_server_list
 *
 *	description:	method to execute "DbGetServerList"
 *
 * @param	argin	
 * @return	
 *
 */
//+------------------------------------------------------------------
Tango::DevVarStringArray *DataBase::db_get_server_list(Tango::DevString wildcard)
{
	static char sql_query[256];
	static char error_mess[256];
	string tmp_wildcard;
	MYSQL_RES *result;
	MYSQL_ROW row;
	int n_rows, i;

	INFO_STREAM << "DataBase::db_get_server_list(): wild card " << wildcard << endl;

	if (wildcard == NULL)
	{
		sprintf(sql_query,"SELECT DISTINCT server FROM device WHERE server LIKE \"%\" ORDER BY server");
	}
	else
	{
		tmp_wildcard = replace_wildcard(wildcard);
		sprintf(sql_query,"SELECT DISTINCT server FROM device WHERE server LIKE \"%s\" ORDER BY server",
	        tmp_wildcard.c_str());
	}
	DEBUG_STREAM << "DataBase::db_get_server_list(): sql_query " << sql_query << endl;

	if (mysql_real_query(&mysql, sql_query, strlen(sql_query)) != 0)
	{
	   WARN_STREAM << "DataBase::db_get_server_list(): failed to query TANGO database for list of servers";
	   WARN_STREAM << wildcard << " (error=" << mysql_error(&mysql) << ")" << endl;
	   sprintf(error_mess,"failed to query TANGO database for list of servers (error=%s)",mysql_error(&mysql));
	   Tango::Except::throw_exception((const char *)DB_SQLError,
	   				  (const char *)error_mess,
					  (const char *)"DataBase::db_get_server_list()");
	}

	if ((result = mysql_store_result(&mysql)) == NULL)
	{
	   WARN_STREAM << "DataBase::db_get_server_list(): mysql_store_result() failed  (error=" << mysql_error(&mysql) << ")" << endl;
	   sprintf(error_mess,"mysql_store_result() failed (error=%s)",mysql_error(&mysql));
	   Tango::Except::throw_exception((const char *)DB_SQLError,
	   				  (const char *)error_mess,
					  (const char *)"DataBase::db_get_server_list()");
	}

	n_rows = mysql_num_rows(result);
	DEBUG_STREAM << "DataBase::db_get_server_list(): mysql_num_rows() " << n_rows << endl;
	Tango::DevVarStringArray *server_list = new Tango::DevVarStringArray;

	if (n_rows > 0)
	{
	   server_list->length(n_rows);

	   for (i=0; i<n_rows; i++)
	   {
	      if ((row = mysql_fetch_row(result)) != NULL)
	      {
	         DEBUG_STREAM << "DataBase::db_get_server_list(): server[ "<< i << "] alias " << row[0] << endl;
	         (*server_list)[i]   = CORBA::string_dup(row[0]);
	      }
	   }
	}
	else
		server_list->length(0);
	mysql_free_result(result);

	return(server_list);
}
//+------------------------------------------------------------------
/**
 *	method:	DataBase::db_get_server_name_list
 *
 *	description:	method to execute "DbGetServerNameList"
 *	Returns the list of server names found for the wildcard specified.
 *	It returns only the server executable name without instance name as DbGetServerList.
 *
 * @param	argin	wildcard for server names.
 * @return	server names found.
 *
 */
//+------------------------------------------------------------------
bool nocase_compare(char c1, char c2)
{	
	return toupper(c1) == toupper(c2);
}
//+------------------------------------------------------------------
/**
 *	method:	DataBase::db_get_server_name_list
 *
 *	description:	method to execute "DbGetServerNameList"
 *	Returns the list of server names found for the wildcard specified.
 *	It returns only the server executable name without instance name as DbGetServerList.
 *
 * @param	argin	wildcard for server names.
 * @return	server names found.
 *
 */
//+------------------------------------------------------------------
Tango::DevVarStringArray *DataBase::db_get_server_name_list(Tango::DevString wildcard)
{
	DEBUG_STREAM << "DataBase::db_get_server_name_list(): entering... !" << endl;
	Tango::DevVarStringArray *server_list = db_get_server_list(wildcard);
  
  int i;
	vector<string>	server_names;

	for (i = 0 ; i<server_list->length() ; i++)
	{
		//	Take only server name
		string	str((*server_list)[i]);
		int	idx;
		if ((idx=str.find("/"))!= string::npos)
		{
			str = str.substr(0, idx);
			//	Search if already in vector
			bool	found = false;
			for(int j=0 ; j<server_names.size() && !found ; j++)
			{
				string	name(server_names[j]);
				//	compare without case sensitive
				found = (name.size() ==  str.size()  &&   //	ensure same size
						equal (name.begin(), name.end(),  //	server name from vector
								str.begin(),              //	server name from db
								nocase_compare));         //	case insensitive
			}
			//	if not already in vector, put in
			if (!found)
				server_names.push_back(str);
		}
	}

	//	copy result vector to arg out.
	Tango::DevVarStringArray	*argout = new Tango::DevVarStringArray;
	argout->length(server_names.size());
	for (i = 0 ; i<server_names.size() ; i++)
	{
		string	name(server_names[i]);
		(*argout)[i] = CORBA::string_dup(name.c_str());
	}

	delete server_list;
	return argout;
}

//+------------------------------------------------------------------
/**
 *	method:	DataBase::db_get_instance_name_list
 *
 *	description:	method to execute "DbGetInstanceNameList"
 *	Returns the instance names found for specified server.
 *
 * @param	argin	server name
 * @return	The instance names found for specified server.
 *
 */
//+------------------------------------------------------------------
Tango::DevVarStringArray *DataBase::db_get_instance_name_list(Tango::DevString argin)
{
	DEBUG_STREAM << "DataBase::db_get_instance_name_list(): entering... !" << endl;

	//	Build a wildcard wing arg in
	char	*wildcard = new char[strlen(argin) + 3];
	strcpy(wildcard, argin);
	strcat(wildcard, "/*");
	Tango::DevVarStringArray *server_list = db_get_server_list(wildcard);

  int i;
	vector<string>	instance_names;

	for (i = 0 ; i<server_list->length() ; i++)
	{
		//	Take only server name
		string	str((*server_list)[i]);
		int	idx;
		if ((idx=str.find("/"))!= string::npos)
		{
			str = str.substr(idx+1);
			instance_names.push_back(str);
		}
	}
	//	copy result vector to arg out.
	Tango::DevVarStringArray	*argout = new Tango::DevVarStringArray;
	argout->length(instance_names.size());
	for (i = 0 ; i<instance_names.size() ; i++)
	{
		string	name(instance_names[i]);
		(*argout)[i] = CORBA::string_dup(name.c_str());
	}
	delete wildcard;
	return argout;
}

//+------------------------------------------------------------------
/**
 *	method:	DataBase::db_get_device_property_list
 *
 *	description:	method to execute "DbGetDevicePropertyList"
 *
 * @param	argin	
 * @return	
 *
 */
//+------------------------------------------------------------------
Tango::DevVarStringArray *DataBase::db_get_device_property_list(const Tango::DevVarStringArray *device_wildcard)
{
	static char sql_query[256];
	static char error_mess[256];
	MYSQL_RES *result;
	MYSQL_ROW row;
	int n_rows, i;
	const char *device,*wildcard;
	string tmp_wildcard;

	device = (*device_wildcard)[0];
	wildcard = (*device_wildcard)[1];
	INFO_STREAM << "DataBase::db_get_device_property_list(): device " << device ;
	INFO_STREAM << " wildcard " << wildcard << endl;

	if (wildcard == NULL)
	{
		sprintf(sql_query,"SELECT name FROM property_device WHERE device=\"%s\" AND name LIKE \"%\" AND count=1 ORDER BY name",device);
	}
	else
	{
		tmp_wildcard = replace_wildcard(wildcard);
		sprintf(sql_query,"SELECT name FROM property_device WHERE device=\"%s\" AND name LIKE \"%s\" AND count=1 ORDER BY name",
	        device,tmp_wildcard.c_str());
	}
	DEBUG_STREAM << "DataBase::db_get_device_property_list(): sql_query " << sql_query << endl;

	if (mysql_real_query(&mysql, sql_query, strlen(sql_query)) != 0)
	{
	   WARN_STREAM << "DataBase::db_get_device_property_list(): failed to query TANGO database for list of properties";
	   WARN_STREAM << device << " (error=" << mysql_error(&mysql) << ")" << endl;
	   sprintf(error_mess,"failed to query TANGO database for list of properties (error=%s)",mysql_error(&mysql));
	   Tango::Except::throw_exception((const char *)DB_SQLError,
	   				  (const char *)error_mess,
					  (const char *)"DataBase::db_get_device_property_list()");
	}

	if ((result = mysql_store_result(&mysql)) == NULL)
	{
	   WARN_STREAM << "DataBase::db_get_device_property_list(): mysql_store_result() failed  (error=" << mysql_error(&mysql) << ")" << endl;
	   sprintf(error_mess,"mysql_store_result() failed (error=%s)",mysql_error(&mysql));
	   Tango::Except::throw_exception((const char *)DB_SQLError,
	   				  (const char *)error_mess,
					  (const char *)"DataBase::db_get_device_property_list()");
	}

	n_rows = mysql_num_rows(result);
	DEBUG_STREAM << "DataBase::db_get_device_property_list(): mysql_num_rows() " << n_rows << endl;
	Tango::DevVarStringArray *property_list = new Tango::DevVarStringArray;

	if (n_rows > 0)
	{
	   property_list->length(n_rows);

	   for (i=0; i<n_rows; i++)
	   {
	      if ((row = mysql_fetch_row(result)) != NULL)
	      {
	         DEBUG_STREAM << "DataBase::db_get_device_property_list(): property[ "<< i << "] alias " << row[0] << endl;
	         (*property_list)[i]   = CORBA::string_dup(row[0]);
	      }
	   }
	}
	else
		property_list->length(0);
	mysql_free_result(result);

	return(property_list);
}

//+------------------------------------------------------------------
/**
 *	method:	DataBase::db_get_class_property_list
 *
 *	description:	method to execute "DbGetClassPropertyList"
 *
 * @param	argin	
 * @return	
 *
 */
//+------------------------------------------------------------------
Tango::DevVarStringArray *DataBase::db_get_class_property_list(Tango::DevString class_name)
{
	static char sql_query[256];
	static char error_mess[256];
	MYSQL_RES *result;
	MYSQL_ROW row;
	int n_rows, i;

	INFO_STREAM << "DataBase::db_get_class_property_list(): class " << class_name << endl;

	if (class_name == NULL)
	{
		sprintf(sql_query,"SELECT DISTINCT name FROM property_class WHERE class LIKE \"%\" ORDER BY name");
	}
	else
	{
		sprintf(sql_query,"SELECT DISTINCT name FROM property_class WHERE class LIKE \"%s\" ORDER BY name",
	        class_name);
	}
	DEBUG_STREAM << "DataBase::db_get_class_property_list(): sql_query " << sql_query << endl;

	if (mysql_real_query(&mysql, sql_query, strlen(sql_query)) != 0)
	{
	   WARN_STREAM << "DataBase::db_get_class_property_list(): failed to query TANGO database for list of properties";
	   WARN_STREAM << class_name << " (error=" << mysql_error(&mysql) << ")" << endl;
	   sprintf(error_mess,"failed to query TANGO database for list of properties (error=%s)",mysql_error(&mysql));
	   Tango::Except::throw_exception((const char *)DB_SQLError,
	   				  (const char *)error_mess,
					  (const char *)"DataBase::db_get_class_property_list()");
	}

	if ((result = mysql_store_result(&mysql)) == NULL)
	{
	   WARN_STREAM << "DataBase::db_get_class_property_list(): mysql_store_result() failed  (error=" << mysql_error(&mysql) << ")" << endl;
	   sprintf(error_mess,"mysql_store_result() failed (error=%s)",mysql_error(&mysql));
	   Tango::Except::throw_exception((const char *)DB_SQLError,
	   				  (const char *)error_mess,
					  (const char *)"DataBase::db_get_class_property_list()");
	}

	n_rows = mysql_num_rows(result);
	DEBUG_STREAM << "DataBase::db_get_class_property_list(): mysql_num_rows() " << n_rows << endl;
	Tango::DevVarStringArray *property_list = new Tango::DevVarStringArray;

	if (n_rows > 0)
	{
	   property_list->length(n_rows);

	   for (i=0; i<n_rows; i++)
	   {
	      if ((row = mysql_fetch_row(result)) != NULL)
	      {
	         DEBUG_STREAM << "DataBase::db_get_class_property_list(): property[ "<< i << "] alias " << row[0] << endl;
	         (*property_list)[i]   = CORBA::string_dup(row[0]);
	      }
	   }
	}
	else
		property_list->length(0);
	mysql_free_result(result);

	return(property_list);
}

//+------------------------------------------------------------------
/**
 *	method:	DataBase::db_get_device_member_list
 *
 *	description:	method to execute "DbGetDeviceMemberList"
 *
 * @param	argin	
 * @return	
 *
 */
//+------------------------------------------------------------------
Tango::DevVarStringArray *DataBase::db_get_device_member_list(Tango::DevString wildcard)
{
	static char sql_query[256];
	static char error_mess[256];
	string tmp_wildcard;
	MYSQL_RES *result;
	MYSQL_ROW row;
	int n_rows, i;

	INFO_STREAM << "DataBase::db_get_device_member_list(): wild card " << wildcard << endl;

	if (wildcard == NULL)
	{
		sprintf(sql_query,"SELECT DISTINCT member FROM device WHERE name LIKE \"%\" ORDER BY member");
	}
	else
	{
		tmp_wildcard = replace_wildcard(wildcard);
		sprintf(sql_query,"SELECT DISTINCT member FROM device WHERE name LIKE \"%s\" OR alias LIKE \"%s\" ORDER BY member",
	        tmp_wildcard.c_str(),tmp_wildcard.c_str());
	}
	DEBUG_STREAM << "DataBase::db_get_device_member_list(): sql_query " << sql_query << endl;

	if (mysql_real_query(&mysql, sql_query, strlen(sql_query)) != 0)
	{
	   WARN_STREAM << "DataBase::db_get_device_member_list(): failed to query TANGO database for list of member names";
	   WARN_STREAM << wildcard << " (error=" << mysql_error(&mysql) << ")" << endl;
	   sprintf(error_mess,"failed to query TANGO database for list of member names (error=%s)",mysql_error(&mysql));
	   Tango::Except::throw_exception((const char *)DB_SQLError,
	   				  (const char *)error_mess,
					  (const char *)"DataBase::db_get_device_member_list()");
	}

	if ((result = mysql_store_result(&mysql)) == NULL)
	{
	   WARN_STREAM << "DataBase::db_get_device_member_list(): mysql_store_result() failed  (error=" << mysql_error(&mysql) << ")" << endl;
	   sprintf(error_mess,"mysql_store_result() failed (error=%s)",mysql_error(&mysql));
	   Tango::Except::throw_exception((const char *)DB_SQLError,
	   				  (const char *)error_mess,
					  (const char *)"DataBase::db_get_device_member_list()");
	}

	n_rows = mysql_num_rows(result);
	DEBUG_STREAM << "DataBase::db_get_device_member_list(): mysql_num_rows() " << n_rows << endl;
	Tango::DevVarStringArray *member_list = new Tango::DevVarStringArray;

	if (n_rows > 0)
	{
	   member_list->length(n_rows);
	   for (i=0; i<n_rows; i++)
	   {
	      if ((row = mysql_fetch_row(result)) != NULL)
	      {
	         DEBUG_STREAM << "DataBase::db_get_device_member_list(): member[ "<< i << "] " << row[0] << endl;
	         (*member_list)[i] = CORBA::string_dup(row[0]);
	      }
	   }
	}
	else
		member_list->length(0);
	mysql_free_result(result);

	return(member_list);
}

//+------------------------------------------------------------------
/**
 *	method:	DataBase::db_get_device_family_list
 *
 *	description:	method to execute "DbGetDeviceFamilyList"
 *
 * @param	argin	
 * @return	
 *
 */
//+------------------------------------------------------------------
Tango::DevVarStringArray *DataBase::db_get_device_family_list(Tango::DevString wildcard)
{
	static char sql_query[256];
	static char error_mess[256];
	string tmp_wildcard;
	MYSQL_RES *result;
	MYSQL_ROW row;
	int n_rows, i;

	INFO_STREAM << "DataBase::db_get_device_family_list(): wild card " << wildcard << endl;

	if (wildcard == NULL)
	{
		sprintf(sql_query,"SELECT DISTINCT family FROM device WHERE name LIKE \"%\" ORDER BY family");
	}
	else
	{
		tmp_wildcard = replace_wildcard(wildcard);
		sprintf(sql_query,"SELECT DISTINCT family FROM device WHERE name LIKE \"%s\" OR alias LIKE \"%s\" ORDER BY family",
	        tmp_wildcard.c_str(),tmp_wildcard.c_str());
	}
	DEBUG_STREAM << "DataBase::db_get_device_family_list(): sql_query " << sql_query << endl;

	if (mysql_real_query(&mysql, sql_query, strlen(sql_query)) != 0)
	{
	   WARN_STREAM << "DataBase::db_get_device_family_list(): failed to query TANGO database for list of member names";
	   WARN_STREAM << wildcard << " (error=" << mysql_error(&mysql) << ")" << endl;
	   sprintf(error_mess,"failed to query TANGO database for list of family names (error=%s)",mysql_error(&mysql));
	   Tango::Except::throw_exception((const char *)DB_SQLError,
	   				  (const char *)error_mess,
					  (const char *)"DataBase::db_get_device_family_list()");
	}

	if ((result = mysql_store_result(&mysql)) == NULL)
	{
	   WARN_STREAM << "DataBase::db_get_device_family_list(): mysql_store_result() failed  (error=" << mysql_error(&mysql) << ")" << endl;
	   sprintf(error_mess,"mysql_store_result() failed (error=%s)",mysql_error(&mysql));
	   Tango::Except::throw_exception((const char *)DB_SQLError,
	   				  (const char *)error_mess,
					  (const char *)"DataBase::db_get_device_family_list()");
	}

	n_rows = mysql_num_rows(result);
	DEBUG_STREAM << "DataBase::db_get_device_family_list(): mysql_num_rows() " << n_rows << endl;
	Tango::DevVarStringArray *family_list = new Tango::DevVarStringArray;

	if (n_rows > 0)
	{
	   family_list->length(n_rows);
	   for (i=0; i<n_rows; i++)
	   {
	      if ((row = mysql_fetch_row(result)) != NULL)
	      {
	         DEBUG_STREAM << "DataBase::db_get_device_family_list(): family[ "<< i << "] " << row[0] << endl;
	         (*family_list)[i] = CORBA::string_dup(row[0]);
	      }
	   }
	}
	else
		family_list->length(0);
	mysql_free_result(result);

	return(family_list);
}

//+------------------------------------------------------------------
/**
 *	method:	DataBase::db_get_device_domain_list
 *
 *	description:	method to execute "DbGetDeviceDomainList"
 *
 * @param	argin	
 * @return	
 *
 */
//+------------------------------------------------------------------
Tango::DevVarStringArray *DataBase::db_get_device_domain_list(Tango::DevString wildcard)
{
	static char sql_query[256];
	static char error_mess[256];
	string tmp_wildcard;
	MYSQL_RES *result;
	MYSQL_ROW row;
	int n_rows, i;

	INFO_STREAM << "DataBase::db_get_device_domain_list(): wild card " << wildcard << endl;

	if (wildcard == NULL)
	{
		sprintf(sql_query,"SELECT DISTINCT domain FROM device WHERE name LIKE \"%\" ORDER BY domain");
	}
	else
	{
		tmp_wildcard = replace_wildcard(wildcard);
		sprintf(sql_query,"SELECT DISTINCT domain FROM device WHERE name LIKE \"%s\" OR alias LIKE \"%s\" ORDER BY domain",
	        tmp_wildcard.c_str(),tmp_wildcard.c_str());
	}
	DEBUG_STREAM << "DataBase::db_get_device_domain_list(): sql_query " << sql_query << endl;

	if (mysql_real_query(&mysql, sql_query, strlen(sql_query)) != 0)
	{
	   WARN_STREAM << "DataBase::db_get_device_domain_list(): failed to query TANGO database for list of domain names";
	   WARN_STREAM << wildcard << " (error=" << mysql_error(&mysql) << ")" << endl;
	   sprintf(error_mess,"failed to query TANGO database for list of domain names (error=%s)",mysql_error(&mysql));
	   Tango::Except::throw_exception((const char *)DB_SQLError,
	   				  (const char *)error_mess,
					  (const char *)"DataBase::db_get_device_domain_list()");
	}

	if ((result = mysql_store_result(&mysql)) == NULL)
	{
	   WARN_STREAM << "DataBase::db_get_device_domain_list(): mysql_store_result() failed  (error=" << mysql_error(&mysql) << ")" << endl;
	   sprintf(error_mess,"mysql_store_result() failed (error=%s)",mysql_error(&mysql));
	   Tango::Except::throw_exception((const char *)DB_SQLError,
	   				  (const char *)error_mess,
					  (const char *)"DataBase::db_get_device_domain_list()");
	}

	n_rows = mysql_num_rows(result);
	DEBUG_STREAM << "DataBase::db_get_device_domain_list(): mysql_num_rows() " << n_rows << endl;
	Tango::DevVarStringArray *domain_list = new Tango::DevVarStringArray;

	if (n_rows > 0)
	{
	   domain_list->length(n_rows);
	   for (i=0; i<n_rows; i++)
	   {
	      if ((row = mysql_fetch_row(result)) != NULL)
	      {
	         DEBUG_STREAM << "DataBase::db_get_device_domain_list(): domain[ "<< i << "] " << row[0] << endl;
	         (*domain_list)[i] = CORBA::string_dup(row[0]);
	      }
	   }
	}
	else
		domain_list->length(0);
	mysql_free_result(result);

	return(domain_list);
}

//+------------------------------------------------------------------
/**
 *	method:	DataBase::db_info
 *
 *	description:	method to execute "DbInfo"
 *
 * @return	
 *
 */
//+------------------------------------------------------------------
Tango::DevVarStringArray *DataBase::db_info()
{

	static char sql_query[256];
	static char error_mess[256];
	static char info_str[256];
	MYSQL_RES *result;
	MYSQL_ROW row;
	int n_rows=0, n_infos=0;
	Tango::DevVarStringArray *info_list = new Tango::DevVarStringArray;

	INFO_STREAM << "DataBase::db_info(): get general database infos" << endl;

	sprintf(info_str,"TANGO Database %s",DataBase::db_name.c_str());
	n_infos = 1;
	info_list->length(n_infos);
	(*info_list)[n_infos-1] = CORBA::string_dup(info_str);

// newline

	n_infos++;
	info_list->length(n_infos);
	(*info_list)[n_infos-1] = CORBA::string_dup(" ");

// get start time of database

	sprintf(sql_query,"SELECT started FROM device WHERE name = \"%s\" ",DataBase::db_name.c_str());
//	DEBUG_STREAM << "DataBase::db_info(): sql_query " << sql_query << endl;

	if (mysql_real_query(&mysql, sql_query, strlen(sql_query)) != 0)
	{
	   WARN_STREAM << "DataBase::db_info(): failed to query TANGO database for start time " ;
	   WARN_STREAM << " (error=" << mysql_error(&mysql) << ")" << endl;
	   sprintf(error_mess,"failed to query TANGO database start time (error=%s)",mysql_error(&mysql));
	   Tango::Except::throw_exception((const char *)DB_SQLError,
	   				  (const char*)error_mess,
					  (const char *)"DataBase::db_info()");
	}

	if ((result = mysql_store_result(&mysql)) == NULL)
	{
	   WARN_STREAM << "DataBase::db_info(): mysql_store_result() failed  (error=" << mysql_error(&mysql) << ")" << endl;
	   sprintf(error_mess,"mysql_store_result() failed (error=%s)",mysql_error(&mysql));
	   Tango::Except::throw_exception((const char *)DB_SQLError,
	   				  (const char *)error_mess,
					  (const char *)"DataBase::db_info()");
	}

	sprintf(info_str,"Running since ...");
	n_rows = mysql_num_rows(result);
	if (n_rows > 0)
	{
	   if ((row = mysql_fetch_row(result)) != NULL)
	   {
	      DEBUG_STREAM << "DataBase::db_info(): database started " << row[0] << endl;
	      sprintf(info_str,"Running since %s",row[0]);
	   }
	}
	mysql_free_result(result);
	n_infos++;
	info_list->length(n_infos);
	(*info_list)[n_infos-1] = CORBA::string_dup(info_str);

// newline

	n_infos++;
	info_list->length(n_infos);
	(*info_list)[n_infos-1] = CORBA::string_dup(" ");

// get number of devices defined

	sprintf(sql_query,"SELECT COUNT(*) FROM device ");
//	DEBUG_STREAM << "DataBase::db_info(): sql_query " << sql_query << endl;

	if (mysql_real_query(&mysql, sql_query, strlen(sql_query)) != 0)
	{
	   WARN_STREAM << "DataBase::db_info(): failed to query TANGO database for no. of devices " ;
	   WARN_STREAM << " (error=" << mysql_error(&mysql) << ")" << endl;
	   sprintf(error_mess,"failed to query TANGO database for no. of devices (error=%s)",mysql_error(&mysql));
	   Tango::Except::throw_exception((const char *)DB_SQLError,
	   				  (const char*)error_mess,
					  (const char *)"DataBase::db_info()");
	}

	if ((result = mysql_store_result(&mysql)) == NULL)
	{
	   WARN_STREAM << "DataBase::db_info(): mysql_store_result() failed  (error=" << mysql_error(&mysql) << ")" << endl;
	   sprintf(error_mess,"mysql_store_result() failed (error=%s)",mysql_error(&mysql));
	   Tango::Except::throw_exception((const char *)DB_SQLError,
	   				  (const char *)error_mess,
					  (const char *)"DataBase::db_info()");
	}

	sprintf(info_str,"Devices defined ...");
	n_rows = mysql_num_rows(result);
	if (n_rows > 0)
	{
	   if ((row = mysql_fetch_row(result)) != NULL)
	   {
	      DEBUG_STREAM << "DataBase::db_info(): no. of devices " << row[0] << endl;
	      sprintf(info_str,"Devices defined  = %s",row[0]);
	   }
	}
	mysql_free_result(result);
	n_infos++;
	info_list->length(n_infos);
	(*info_list)[n_infos-1] = CORBA::string_dup(info_str);

// get number of devices exported

	sprintf(sql_query,"SELECT COUNT(*) FROM device WHERE exported = 1 ");
//	DEBUG_STREAM << "DataBase::db_info(): sql_query " << sql_query << endl;

	if (mysql_real_query(&mysql, sql_query, strlen(sql_query)) != 0)
	{
	   WARN_STREAM << "DataBase::db_info(): failed to query TANGO database for no. of devices exported" ;
	   WARN_STREAM << " (error=" << mysql_error(&mysql) << ")" << endl;
	   sprintf(error_mess,"failed to query TANGO database for no. of devices exported (error=%s)",mysql_error(&mysql));
	   Tango::Except::throw_exception((const char *)DB_SQLError,
	   				  (const char*)error_mess,
					  (const char *)"DataBase::db_info()");
	}

	if ((result = mysql_store_result(&mysql)) == NULL)
	{
	   WARN_STREAM << "DataBase::db_info(): mysql_store_result() failed  (error=" << mysql_error(&mysql) << ")" << endl;
	   sprintf(error_mess,"mysql_store_result() failed (error=%s)",mysql_error(&mysql));
	   Tango::Except::throw_exception((const char *)DB_SQLError,
	   				  (const char *)error_mess,
					  (const char *)"DataBase::db_info()");
	}

	sprintf(info_str,"Devices exported ...");
	n_rows = mysql_num_rows(result);
	if (n_rows > 0)
	{
	   if ((row = mysql_fetch_row(result)) != NULL)
	   {
	      DEBUG_STREAM << "DataBase::db_info(): no. of devices exported " << row[0] << endl;
	      sprintf(info_str,"Devices exported  = %s",row[0]);
	   }
	}
	mysql_free_result(result);
	n_infos++;
	info_list->length(n_infos);
	(*info_list)[n_infos-1] = CORBA::string_dup(info_str);

// get number of devices servers defined

	sprintf(sql_query,"SELECT COUNT(*) FROM device WHERE class = \"DServer\" ");
//	DEBUG_STREAM << "DataBase::db_info(): sql_query " << sql_query << endl;

	if (mysql_real_query(&mysql, sql_query, strlen(sql_query)) != 0)
	{
	   WARN_STREAM << "DataBase::db_info(): failed to query TANGO database for no. of device servers " ;
	   WARN_STREAM << " (error=" << mysql_error(&mysql) << ")" << endl;
	   sprintf(error_mess,"failed to query TANGO database for no. of device servers (error=%s)",mysql_error(&mysql));
	   Tango::Except::throw_exception((const char *)DB_SQLError,
	   				  (const char*)error_mess,
					  (const char *)"DataBase::db_info()");
	}

	if ((result = mysql_store_result(&mysql)) == NULL)
	{
	   WARN_STREAM << "DataBase::db_info(): mysql_store_result() failed  (error=" << mysql_error(&mysql) << ")" << endl;
	   sprintf(error_mess,"mysql_store_result() failed (error=%s)",mysql_error(&mysql));
	   Tango::Except::throw_exception((const char *)DB_SQLError,
	   				  (const char *)error_mess,
					  (const char *)"DataBase::db_info()");
	}

	sprintf(info_str,"Device servers defined ...");
	n_rows = mysql_num_rows(result);
	if (n_rows > 0)
	{
	   if ((row = mysql_fetch_row(result)) != NULL)
	   {
	      DEBUG_STREAM << "DataBase::db_info(): no. of device servers defined " << row[0] << endl;
	      sprintf(info_str,"Device servers defined  = %s",row[0]);
	   }
	}
	mysql_free_result(result);
	n_infos++;
	info_list->length(n_infos);
	(*info_list)[n_infos-1] = CORBA::string_dup(info_str);

// get number of devices servers exported

	sprintf(sql_query,"SELECT COUNT(*) FROM device WHERE class = \"DServer\" AND exported = 1 ");
//	DEBUG_STREAM << "DataBase::db_info(): sql_query " << sql_query << endl;

	if (mysql_real_query(&mysql, sql_query, strlen(sql_query)) != 0)
	{
	   WARN_STREAM << "DataBase::db_info(): failed to query TANGO database for no. of device servers exported " ;
	   WARN_STREAM << " (error=" << mysql_error(&mysql) << ")" << endl;
	   sprintf(error_mess,"failed to query TANGO database for no. of device servers exported (error=%s)",mysql_error(&mysql));
	   Tango::Except::throw_exception((const char *)DB_SQLError,
	   				  (const char*)error_mess,
					  (const char *)"DataBase::db_info()");
	}

	if ((result = mysql_store_result(&mysql)) == NULL)
	{
	   WARN_STREAM << "DataBase::db_info(): mysql_store_result() failed  (error=" << mysql_error(&mysql) << ")" << endl;
	   sprintf(error_mess,"mysql_store_result() failed (error=%s)",mysql_error(&mysql));
	   Tango::Except::throw_exception((const char *)DB_SQLError,
	   			  	  (const char *)error_mess,
					  (const char *)"DataBase::db_info()");
	}

	sprintf(info_str,"Device servers exported ...");
	n_rows = mysql_num_rows(result);
	if (n_rows > 0)
	{
	   if ((row = mysql_fetch_row(result)) != NULL)
	   {
	      DEBUG_STREAM << "DataBase::db_info(): no. of device servers exported " << row[0] << endl;
	      sprintf(info_str,"Device servers exported  = %s",row[0]);
	   }
	}
	mysql_free_result(result);
	n_infos++;
	info_list->length(n_infos);
	(*info_list)[n_infos-1] = CORBA::string_dup(info_str);

// newline

	n_infos++;
	info_list->length(n_infos);
	(*info_list)[n_infos-1] = CORBA::string_dup(" ");

// get number of class properties 

	sprintf(sql_query,"SELECT COUNT(*) FROM property_class ");
//	DEBUG_STREAM << "DataBase::db_info(): sql_query " << sql_query << endl;

	if (mysql_real_query(&mysql, sql_query, strlen(sql_query)) != 0)
	{
	   WARN_STREAM << "DataBase::db_info(): failed to query TANGO database for no. of class properties defined " ;
	   WARN_STREAM << " (error=" << mysql_error(&mysql) << ")" << endl;
	   sprintf(error_mess,"failed to query TANGO database for no. of class properties defined (error=%s)",mysql_error(&mysql));
	   Tango::Except::throw_exception((const char *)DB_SQLError,
	   				  (const char*)error_mess,
					  (const char *)"DataBase::db_info()");
	}

	if ((result = mysql_store_result(&mysql)) == NULL)
	{
	   WARN_STREAM << "DataBase::db_info(): mysql_store_result() failed  (error=" << mysql_error(&mysql) << ")" << endl;
	   sprintf(error_mess,"mysql_store_result() failed (error=%s)",mysql_error(&mysql));
	   Tango::Except::throw_exception((const char *)DB_SQLError,
	   				  (const char *)error_mess,
					  (const char *)"DataBase::db_info()");
	}

	sprintf(info_str,"Class properties defined ...");
	n_rows = mysql_num_rows(result);
	if (n_rows > 0)
	{
	   if ((row = mysql_fetch_row(result)) != NULL)
	   {
	      DEBUG_STREAM << "DataBase::db_info(): no. of class properties defined " << row[0] << endl;
	      sprintf(info_str,"Class properties defined  = %s",row[0]);
	   }
	}
	mysql_free_result(result);
	n_infos++;
	info_list->length(n_infos);
	(*info_list)[n_infos-1] = CORBA::string_dup(info_str);
// get number of devices properties 

	sprintf(sql_query,"SELECT COUNT(*) FROM property_device ");
//	DEBUG_STREAM << "DataBase::db_info(): sql_query " << sql_query << endl;

	if (mysql_real_query(&mysql, sql_query, strlen(sql_query)) != 0)
	{
	   WARN_STREAM << "DataBase::db_info(): failed to query TANGO database for no. of device properties " ;
	   WARN_STREAM << " (error=" << mysql_error(&mysql) << ")" << endl;
	   sprintf(error_mess,"failed to query TANGO database for no. of device properties (error=%s)",mysql_error(&mysql));
	   Tango::Except::throw_exception((const char *)DB_SQLError,
	   				  (const char*)error_mess,
					  (const char *)"DataBase::db_info()");
	}

	if ((result = mysql_store_result(&mysql)) == NULL)
	{
	   WARN_STREAM << "DataBase::db_info(): mysql_store_result() failed  (error=" << mysql_error(&mysql) << ")" << endl;
	   sprintf(error_mess,"mysql_store_result() failed (error=%s)",mysql_error(&mysql));
	   Tango::Except::throw_exception((const char *)DB_SQLError,
	   				  (const char *)error_mess,
					  (const char *)"DataBase::db_info()");
	}

	sprintf(info_str,"Device Properties defined ...");
	n_rows = mysql_num_rows(result);
	if (n_rows > 0)
	{
	   if ((row = mysql_fetch_row(result)) != NULL)
	   {
	      DEBUG_STREAM << "DataBase::db_info(): no. of device properties defined " << row[0] << endl;
	      sprintf(info_str,"Device properties defined  = %s",row[0]);
	   }
	}
	mysql_free_result(result);
	n_infos++;
	info_list->length(n_infos);
	(*info_list)[n_infos-1] = CORBA::string_dup(info_str);

// get number of class attribute properties 

	sprintf(sql_query,"SELECT COUNT(*) FROM property_attribute_class ");
//	DEBUG_STREAM << "DataBase::db_info(): sql_query " << sql_query << endl;

	if (mysql_real_query(&mysql, sql_query, strlen(sql_query)) != 0)
	{
	   WARN_STREAM << "DataBase::db_info(): failed to query TANGO database for no. of class attribute properties defined " ;
	   WARN_STREAM << " (error=" << mysql_error(&mysql) << ")" << endl;
	   sprintf(error_mess,"failed to query TANGO database for no. of class attribute properties defined (error=%s)",mysql_error(&mysql));
	   Tango::Except::throw_exception((const char *)DB_SQLError,
	   				  (const char*)error_mess,
					  (const char *)"DataBase::db_info()");
	}

	if ((result = mysql_store_result(&mysql)) == NULL)
	{
	   WARN_STREAM << "DataBase::db_info(): mysql_store_result() failed  (error=" << mysql_error(&mysql) << ")" << endl;
	   sprintf(error_mess,"mysql_store_result() failed (error=%s)",mysql_error(&mysql));
	   Tango::Except::throw_exception((const char *)DB_SQLError,
	   				  (const char *)error_mess,
					  (const char *)"DataBase::db_info()");
	}

	sprintf(info_str,"Class Attribute properties defined ...");
	n_rows = mysql_num_rows(result);
	if (n_rows > 0)
	{
	   if ((row = mysql_fetch_row(result)) != NULL)
	   {
	      DEBUG_STREAM << "DataBase::db_info(): no. of class attribute properties defined " << row[0] << endl;
	      sprintf(info_str,"Class attribute properties defined  = %s",row[0]);
	   }
	}
	mysql_free_result(result);
	n_infos++;
	info_list->length(n_infos);
	(*info_list)[n_infos-1] = CORBA::string_dup(info_str);

// get number of device attribute properties 

	sprintf(sql_query,"SELECT COUNT(*) FROM property_attribute_device ");
//	DEBUG_STREAM << "DataBase::db_info(): sql_query " << sql_query << endl;

	if (mysql_real_query(&mysql, sql_query, strlen(sql_query)) != 0)
	{
	   WARN_STREAM << "DataBase::db_info(): failed to query TANGO database for no. of device attribute properties defined " ;
	   WARN_STREAM << " (error=" << mysql_error(&mysql) << ")" << endl;
	   sprintf(error_mess,"failed to query TANGO database for no. of device attribute properties defined (error=%s)",mysql_error(&mysql));
	   Tango::Except::throw_exception((const char *)DB_SQLError,
	   				  (const char*)error_mess,
					  (const char *)"DataBase::db_info()");
	}

	if ((result = mysql_store_result(&mysql)) == NULL)
	{
	   WARN_STREAM << "DataBase::db_info(): mysql_store_result() failed  (error=" << mysql_error(&mysql) << ")" << endl;
	   sprintf(error_mess,"mysql_store_result() failed (error=%s)",mysql_error(&mysql));
	   Tango::Except::throw_exception((const char *)DB_SQLError,
	   				  (const char *)error_mess,
					  (const char *)"DataBase::db_info()");
	}

	sprintf(info_str,"Device Attribute properties defined ...");
	n_rows = mysql_num_rows(result);
	if (n_rows > 0)
	{
	   if ((row = mysql_fetch_row(result)) != NULL)
	   {
	      DEBUG_STREAM << "DataBase::db_info(): no. of device attribute properties defined " << row[0] << endl;
	      sprintf(info_str,"Device attribute properties defined  = %s",row[0]);
	   }
	}
	mysql_free_result(result);
	n_infos++;
	info_list->length(n_infos);
	(*info_list)[n_infos-1] = CORBA::string_dup(info_str);

	DEBUG_STREAM << "DataBase::db_info(): info_list->length() "<< info_list->length() << endl;

	return(info_list);
}

//+------------------------------------------------------------------
/**
 *	method:	DataBase::db_get_device_list
 *
 *	description:	method to execute "DbGetDeviceList"
 *
 * @param	argin	
 * @return	
 *
 */
//+------------------------------------------------------------------
Tango::DevVarStringArray *DataBase::db_get_device_list(const Tango::DevVarStringArray *server_class)
{
	static char sql_query[256];
	static char error_mess[256];
	MYSQL_RES *result;
	MYSQL_ROW row;
	int n_rows, i;
	const char *tmp_server;
	string tmp_class;

	if (server_class->length() != 2)
	{
	   WARN_STREAM << "DataBase::db_get_device_list(): incorrect number of input arguments " << endl;
	   sprintf(error_mess,"incorrect no. of input arguments, needs 2 (server,class)");
	   Tango::Except::throw_exception((const char *)DB_IncorrectArguments,
	   				  (const char *)error_mess,
					  (const char *)"DataBase::GetDeviceList()");
	}
		
	tmp_server = (*server_class)[0];
	tmp_class = replace_wildcard((*server_class)[1]);

	INFO_STREAM << "DataBase::GetClassList(): server " << tmp_server << endl;

	sprintf(sql_query,"SELECT DISTINCT name FROM device WHERE server LIKE \"%s\" AND class LIKE \"%s\" ORDER BY name",
	        tmp_server, tmp_class.c_str());
	DEBUG_STREAM << "DataBase::GetDeviceList(): sql_query " << sql_query << endl;

	if (mysql_real_query(&mysql, sql_query, strlen(sql_query)) != 0)
	{
	   WARN_STREAM << "DataBase::GetDeviceList(): failed to query TANGO database for list of classes for server ";
	   WARN_STREAM << server_class << " (error=" << mysql_error(&mysql) << ")" << endl;
	   sprintf(error_mess,"failed to query TANGO database for list of classes for server (error=%s)",mysql_error(&mysql));
	   Tango::Except::throw_exception((const char *)DB_SQLError,
	   				  (const char *)error_mess,
					  (const char *)"DataBase::GetDeviceList()");
	}

	if ((result = mysql_store_result(&mysql)) == NULL)
	{
	   WARN_STREAM << "DataBase::GetDeviceList(): mysql_store_result() failed  (error=" << mysql_error(&mysql) << ")" << endl;
	   sprintf(error_mess,"mysql_store_result() failed (error=%s)",mysql_error(&mysql));
	   Tango::Except::throw_exception((const char *)DB_SQLError,
	   				  (const char *)error_mess,
					  (const char *)"DataBase::GetDeviceList()");
	}

	n_rows = mysql_num_rows(result);
	DEBUG_STREAM << "DataBase::GetDeviceList(): mysql_num_rows() " << n_rows << endl;
	Tango::DevVarStringArray *device_list = new Tango::DevVarStringArray;

	if (n_rows > 0)
	{
	   device_list->length(n_rows);

	   for (i=0; i<n_rows; i++)
	   {
	      if ((row = mysql_fetch_row(result)) != NULL)
	      {
	         DEBUG_STREAM << "DataBase::GetdeviceList(): row[ "<< i << "] name " << row[0] << endl;
	         (*device_list)[i]   = CORBA::string_dup(row[0]);
	      }
	   }
	}
	else
		device_list->length(0);
	mysql_free_result(result);

	return(device_list);
}

//+------------------------------------------------------------------
/**
 *	method:	DataBase::db_get_class_list
 *
 *	description:	method to execute "DbGetClassList"
 *
 * @param	argin	
 * @return	
 *
 */
//+------------------------------------------------------------------
Tango::DevVarStringArray *DataBase::db_get_class_list(Tango::DevString server)
{
	static char sql_query[256];
	static char error_mess[256];
	MYSQL_RES *result;
	MYSQL_ROW row;
	int n_rows, i;
	string tmp_server;

	INFO_STREAM << "DataBase::db_get_class_list(): server " << server << endl;

	tmp_server = replace_wildcard(server);
	sprintf(sql_query,"SELECT DISTINCT class FROM device WHERE class LIKE \"%s\" ORDER BY class",
	        tmp_server.c_str());
	DEBUG_STREAM << "DataBase::db_get_class_list(): sql_query " << sql_query << endl;

	if (mysql_real_query(&mysql, sql_query, strlen(sql_query)) != 0)
	{
	   WARN_STREAM << "DataBase::db_get_class_list(): failed to query TANGO database for list of classes for server ";
	   WARN_STREAM << server << " (error=" << mysql_error(&mysql) << ")" << endl;
	   sprintf(error_mess,"failed to query TANGO database for list of classes for server (error=%s)",mysql_error(&mysql));
	   Tango::Except::throw_exception((const char *)DB_SQLError,
	   				  (const char *)error_mess,
					  (const char *)"DataBase::db_get_class_list()");
	}

	if ((result = mysql_store_result(&mysql)) == NULL)
	{
	   WARN_STREAM << "DataBase::db_get_class_list(): mysql_store_result() failed  (error=" << mysql_error(&mysql) << ")" << endl;
	   sprintf(error_mess,"mysql_store_result() failed (error=%s)",mysql_error(&mysql));
	   Tango::Except::throw_exception((const char *)DB_SQLError,
	   				  (const char *)error_mess,
					  (const char *)"DataBase::db_get_class_list()");
	}

	n_rows = mysql_num_rows(result);
	DEBUG_STREAM << "DataBase::db_get_class_list(): mysql_num_rows() " << n_rows << endl;
	Tango::DevVarStringArray *class_list = new Tango::DevVarStringArray;

	if (n_rows > 0)
	{
	   class_list->length(n_rows);

	   for (i=0; i<n_rows; i++)
	   {
	      if ((row = mysql_fetch_row(result)) != NULL)
	      {
	         DEBUG_STREAM << "DataBase::db_get_class_list(): row[ "<< i << "] class " << row[0] << endl;
	         (*class_list)[i]   = CORBA::string_dup(row[0]);
	      }
	   }
	}
	else
		class_list->length(0);
	mysql_free_result(result);

	return(class_list);
}

//+------------------------------------------------------------------
/**
 *	method:	DataBase::db_get_device_class_list
 *
 *	description:	method to execute "DbGetDeviceClassList"
 *
 * @param	argin	
 * @return	
 *
 */
//+------------------------------------------------------------------
Tango::DevVarStringArray *DataBase::db_get_device_class_list(Tango::DevString server)
{
	static char sql_query[256];
	static char error_mess[256];
	MYSQL_RES *result;
	MYSQL_ROW row;
	int n_rows, i;

	INFO_STREAM << "DataBase::GetDeviceClassList(): server " << server << endl;

	sprintf(sql_query,"SELECT name,class FROM device WHERE server = \"%s\" ORDER BY name",
	        server);
	DEBUG_STREAM << "DataBase::GetDeviceClassList(): sql_query " << sql_query << endl;

	if (mysql_real_query(&mysql, sql_query, strlen(sql_query)) != 0)
	{
	   WARN_STREAM << "DataBase::GetDeviceClassList(): failed to query TANGO database for list of devices for server ";
	   WARN_STREAM << server << " (error=" << mysql_error(&mysql) << ")" << endl;
	   sprintf(error_mess,"failed to query TANGO database for list of devices for server (error=%s)",mysql_error(&mysql));
	   Tango::Except::throw_exception((const char *)DB_SQLError,
	   				  (const char *)error_mess,
					  (const char *)"DataBase::GetDeviceClassList()");
	}

	if ((result = mysql_store_result(&mysql)) == NULL)
	{
	   WARN_STREAM << "DataBase::GetDeviceClassList(): mysql_store_result() failed  (error=" << mysql_error(&mysql) << ")" << endl;
	   sprintf(error_mess,"mysql_store_result() failed (error=%s)",mysql_error(&mysql));
	   Tango::Except::throw_exception((const char *)DB_SQLError,
	   				  (const char *)error_mess,
					  (const char *)"DataBase::GetDeviceClassList()");
	}

	n_rows = mysql_num_rows(result);
	INFO_STREAM << "DataBase::GetDeviceClassList(): mysql_num_rows() " << n_rows << endl;
	Tango::DevVarStringArray *device_class_list = new Tango::DevVarStringArray;

	if (n_rows > 0)
	{
	   device_class_list->length(n_rows*2);

	   for (i=0; i<n_rows; i++)
	   {
	      if ((row = mysql_fetch_row(result)) != NULL)
	      {
	         DEBUG_STREAM << "DataBase::GetDeviceClassList(): row[ "<< i << "] name " << row[0] << " class " << row[1] << endl;
	         (*device_class_list)[i*2]   = CORBA::string_dup(row[0]);
	         (*device_class_list)[i*2+1] = CORBA::string_dup(row[1]);
	      }
	   }
	}
	else
		device_class_list->length(0);
	mysql_free_result(result);

	return(device_class_list);
}

//+------------------------------------------------------------------
/**
 *	method:	DataBase::db_get_device_property
 *
 *	description:	method to execute "DbGetDeviceProperty"
 *
 * @param	argin	
 * @return	
 *
 */
//+------------------------------------------------------------------
Tango::DevVarStringArray *DataBase::db_get_device_property(const Tango::DevVarStringArray *property_names)
{
	static char sql_query[256];
	static char error_mess[256];
	static char n_properties_str[256];
	static char n_rows_str[256];
	MYSQL_RES *result;
	MYSQL_ROW row;
	int n_properties=0, n_rows=0, n_props=0, i, j;
	Tango::DevVarStringArray *property_list = new Tango::DevVarStringArray;
	const char *tmp_device;
	string	tmp_name;
	string	prop_name;

	if (property_names->length() < 2)
	{
	   WARN_STREAM << "DataBase::GetDeviceProperty(): incorrect number of input arguments " << endl;
	   sprintf(error_mess,"incorrect no. of input arguments, needs at least 2 (device,property)");
	   Tango::Except::throw_exception((const char *)DB_IncorrectArguments,
	   				  (const char *)error_mess,
					  (const char *)"DataBase::GetDeviceProperty()");
	}

	INFO_STREAM << "DataBase::GetDeviceProperty(): get " << property_names->length()-1 << " properties for device " << (*property_names)[0] << endl;

	tmp_device = (*property_names)[0];
	sprintf(n_properties_str, "%d", property_names->length()-1);
	n_props = 2;
	property_list->length(n_props);
	(*property_list)[0] = CORBA::string_dup(tmp_device);
	(*property_list)[1] = CORBA::string_dup(n_properties_str);

	for (i=1; i<property_names->length(); i++)
	{
		prop_name = (*property_names)[i];
	   tmp_name = replace_wildcard((*property_names)[i]);
	   sprintf(sql_query,"SELECT count,value,name FROM property_device WHERE device = \"%s\" AND name LIKE \"%s\" ORDER BY count",
	           tmp_device, tmp_name.c_str());
	   DEBUG_STREAM << "DataBase::GetDeviceProperty(): sql_query " << sql_query << endl;

	   if (mysql_real_query(&mysql, sql_query, strlen(sql_query)) != 0)
	   {
	      WARN_STREAM << "DataBase::GetDeviceProperty(): failed to query TANGO database for list of properties ";
	      WARN_STREAM << "for " << (*property_names)[0] << "/" << (*property_names)[i] << " (error=" << mysql_error(&mysql) << ")" << endl;
	      sprintf(error_mess,"Failed to query TANGO database for list of properties for %s/%s (error=%s)",tmp_device,tmp_name.c_str(),mysql_error(&mysql));
	      Tango::Except::throw_exception((const char *)DB_SQLError,
	      				     (const char *)error_mess,
					     (const char *)"DataBase::GetDeviceList()");
	   }

	   if ((result = mysql_store_result(&mysql)) == NULL)
	   {
	      WARN_STREAM << "DataBase::GetDeviceProperty(): mysql_store_result() failed  (error=" << mysql_error(&mysql) << ")" << endl;
	      sprintf(error_mess,"mysql_store_result() failed (error=%s)",mysql_error(&mysql));
	      Tango::Except::throw_exception((const char *)DB_SQLError,
	      				     (const char *)error_mess,
					     (const char *)"DataBase::GetDeviceProperty()");
	   }

	   n_rows = mysql_num_rows(result);
	   DEBUG_STREAM << "DataBase::GetDeviceProperty(): mysql_num_rows() " << n_rows << endl;
	   sprintf(n_rows_str,"%d",n_rows);
	   n_props = n_props+2;
	   property_list->length(n_props);
	   (*property_list)[n_props-2] = CORBA::string_dup(prop_name.c_str());
       (*property_list)[n_props-1] = CORBA::string_dup(n_rows_str);
	   if (n_rows > 0)
	   {

	      for (j=0; j<n_rows; j++)
	      {
	         if ((row = mysql_fetch_row(result)) != NULL)
	         {
	            DEBUG_STREAM << "DataBase::GetDeviceProperty(): property[ "<< i << "] count " << row[0] << " value " << row[1] << endl;
		    n_props++;
		    property_list->length(n_props);
	            (*property_list)[n_props-1] = CORBA::string_dup(row[1]);
	         }
	      }
	      n_properties += n_rows;
	   }
	   else
	   {
	      n_props++;
	      property_list->length(n_props);
	      (*property_list)[n_props-1] = CORBA::string_dup(" ");
	   }
	   mysql_free_result(result);
	}

	DEBUG_STREAM << "DataBase::GetDeviceProperty(): property_list->length() "<< property_list->length() << endl;

	return(property_list);
}

//+------------------------------------------------------------------
/**
 *	method:	DataBase::db_put_device_property
 *
 *	description:	method to execute "DbPutDeviceProperty"
 *
 * @param	argin	
 *
 */
//+------------------------------------------------------------------
void DataBase::db_put_device_property(const Tango::DevVarStringArray *property_list)
{
	static char sql_query[512];
	static char error_mess[256];
	static char tmp_count_str[256];
	int n_properties=0, n_rows=0, i, j, k;
	const char *tmp_device, *tmp_count;
	string tmp_name;

	sscanf((*property_list)[1],"%d",&n_properties);
	INFO_STREAM << "DataBase::PutDeviceProperty(): put " << n_properties << " properties for device " << (*property_list)[0] << endl;

	k = 2;
	for (i=0; i<n_properties; i++)
	{
	   tmp_count = 0;
	   tmp_device = (*property_list)[0];
	   tmp_name = (*property_list)[k];

// first delete all tuples (device,name) from the property table

	      sprintf(sql_query,"DELETE FROM property_device WHERE device LIKE \"%s\" AND name LIKE \"%s\"",
	              tmp_device, tmp_name.c_str());
	      DEBUG_STREAM << "DataBase::PutDeviceProperty(): sql_query " << sql_query << endl;
	      if (mysql_real_query(&mysql, sql_query, strlen(sql_query)) != 0)
	      {
	         WARN_STREAM << "DataBase::PutDeviceProperty(): failed to delete property from TANGO database ";
	         WARN_STREAM << "for " << tmp_device << "/" << tmp_name << " (error=" << mysql_error(&mysql) << ")" << endl;
	         sprintf(error_mess,"failed to delete property from TANGO database  for %s/%s (error=%s)",tmp_device,tmp_name.c_str(),mysql_error(&mysql));
	         Tango::Except::throw_exception((const char *)DB_SQLError,
		 				(const char *)error_mess,
						(const char *)"DataBase::PutDeviceProperty()");
	      }
	   sscanf((*property_list)[k+1], "%d", &n_rows);
	   for (j=k+2; j<k+n_rows+2; j++)
	   {
          string tmp_escaped_string = escape_string((*property_list)[j]);
	      tmp_count++; sprintf(tmp_count_str, "%d", tmp_count);


// then insert the new value for this tuple

	      sprintf(sql_query,"INSERT INTO property_device SET device=\'%s\',name=\'%s\',count=\'%s\',value=\'%s\',updated=NULL,accessed=NULL",
	              tmp_device, tmp_name.c_str(), tmp_count_str, tmp_escaped_string.c_str());
	      DEBUG_STREAM << "DataBase::PutDeviceProperty(): sql_query " << sql_query << endl;

	      if (mysql_real_query(&mysql, sql_query, strlen(sql_query)) != 0)
	      {
	         WARN_STREAM << "DataBase::PutDeviceProperty(): failed to insert property into TANGO database ";
	         WARN_STREAM << "for " << tmp_device << "/" << tmp_name << " (error=" << mysql_error(&mysql) << ")" << endl;
	         sprintf(error_mess,"failed to insert property into TANGO database  for %s/%s (error=%s)",tmp_device,tmp_name.c_str(),mysql_error(&mysql));
	         Tango::Except::throw_exception((const char *)DB_SQLError,
		 				(const char *)error_mess,
						(const char *)"DataBase::PutDeviceProperty()");
	      }
	   }
	   k = k+n_rows+2;
	}

	return;
}

//+------------------------------------------------------------------
/**
 *	method:	DataBase::db_delete_device_property
 *
 *	description:	method to execute "DbDeleteDeviceProperty"
 *
 * @param	argin	
 *
 */
//+------------------------------------------------------------------
void DataBase::db_delete_device_property(const Tango::DevVarStringArray *property_list)
{
	static char sql_query[512];
	static char error_mess[256];
	static char tmp_count_str[256];
	int n_properties=0, i;
	const char *tmp_device;
	string tmp_name;

	n_properties = property_list->length() - 1;
	INFO_STREAM << "DataBase::PutDeviceProperty(): put " << n_properties << " properties for device " << (*property_list)[0] << endl;

	for (i=0; i<n_properties; i++)
	{
	      tmp_device = (*property_list)[0];
	      tmp_name = replace_wildcard((*property_list)[i+1]);

// delete the tuple (device,name,count) from the property table

	      sprintf(sql_query,"DELETE FROM property_device WHERE device=\"%s\" AND name LIKE \"%s\"",
	              tmp_device, tmp_name.c_str());
	      DEBUG_STREAM << "DataBase::DeleteDeviceProperty(): sql_query " << sql_query << endl;
	      if (mysql_real_query(&mysql, sql_query, strlen(sql_query)) != 0)
	      {
	         WARN_STREAM << "DataBase::DeleteDeviceProperty(): failed to delete property from TANGO database ";
	         WARN_STREAM << "for " << tmp_device << "/" << tmp_name << " (error=" << mysql_error(&mysql) << ")" << endl;
	         sprintf(error_mess,"failed to delete property from TANGO database  for %s/%s (error=%s)",tmp_device,tmp_name.c_str(),mysql_error(&mysql));
	         Tango::Except::throw_exception((const char *)DB_SQLError,
		 				(const char *)error_mess,
						(const char *)"DataBase::DeleteDeviceProperty()");
	      }

	}

	return;
}

//+------------------------------------------------------------------
/**
 *	method:	DataBase::db_get_device_attribute_property
 *
 *	description:	method to execute "DbGetDeviceAttributeProperty"
 *
 * @param	argin	
 * @return	
 *
 */
//+------------------------------------------------------------------
Tango::DevVarStringArray *DataBase::db_get_device_attribute_property(const Tango::DevVarStringArray *property_names)
{
	static char sql_query[256];
	static char error_mess[256];
	static char n_attributes_str[256];
	static char n_rows_str[256];
	MYSQL_RES *result;
	MYSQL_ROW row;
	int n_attributes=0, n_rows=0, n_props=0, i, j;
	Tango::DevVarStringArray *property_list = new Tango::DevVarStringArray;
	const char *tmp_device, *tmp_attribute;

	INFO_STREAM << "DataBase::GetAttributeProperty(): get " << property_names->length()-1 << " properties for device " << (*property_names)[0] << endl;

	tmp_device = (*property_names)[0];
	sprintf(n_attributes_str, "%d", property_names->length()-1);
	n_props = 2;
	property_list->length(n_props);
	(*property_list)[n_props-2] = CORBA::string_dup(tmp_device);
	(*property_list)[n_props-1] = CORBA::string_dup(n_attributes_str);

	for (i=1; i<property_names->length(); i++)
	{
	   tmp_attribute = (*property_names)[i];
	   sprintf(sql_query,"SELECT name,value FROM property_attribute_device WHERE device = \"%s\" AND attribute LIKE \"%s\" ",
	           tmp_device, tmp_attribute);
	   DEBUG_STREAM << "DataBase::GetAttributeProperty(): sql_query " << sql_query << endl;

	   if (mysql_real_query(&mysql, sql_query, strlen(sql_query)) != 0)
	   {
	      WARN_STREAM << "DataBase::GetAttributeProperty(): failed to query TANGO database for list of properties ";
	      WARN_STREAM << "for " << (*property_names)[0] << "/" << (*property_names)[i] << " (error=" << mysql_error(&mysql) << ")" << endl;
	      sprintf(error_mess,"failed to query TANGO database for list of properties for %s/%s (error=%s)",tmp_device,tmp_attribute,mysql_error(&mysql));
	      Tango::Except::throw_exception((const char *)DB_SQLError,
	      				     (const char *)error_mess,
					     (const char *)"DataBase::GetAttributeProperty()");
	   }

	   if ((result = mysql_store_result(&mysql)) == NULL)
	   {
	      WARN_STREAM << "DataBase::GetAttributeProperty(): mysql_store_result() failed  (error=" << mysql_error(&mysql) << ")" << endl;
	      sprintf(error_mess,"mysql_store_result() failed (error=%s)",mysql_error(&mysql));
	      Tango::Except::throw_exception((const char *)DB_SQLError,
	      				     (const char *)error_mess,
					     (const char *)"DataBase::GetAttributeProperty()");
	   }

	   n_rows = mysql_num_rows(result);
	   DEBUG_STREAM << "DataBase::GetAttributeProperty(): mysql_num_rows() " << n_rows << endl;
	   sprintf(n_rows_str,"%d",n_rows);
	   n_props = n_props+2;
	   property_list->length(n_props);
	   (*property_list)[n_props-2] = CORBA::string_dup(tmp_attribute);
           (*property_list)[n_props-1] = CORBA::string_dup(n_rows_str);
	   if (n_rows > 0)
	   {

	      for (j=0; j<n_rows; j++)
	      {
	         if ((row = mysql_fetch_row(result)) != NULL)
	         {
	            DEBUG_STREAM << "DataBase::GetAttributeProperty(): property[ "<< i << "] count " << row[0] << " value " << row[1] << endl;
	   	    n_props = n_props+2;
	   	    property_list->length(n_props);
	            (*property_list)[n_props-2] = CORBA::string_dup(row[0]);
	            (*property_list)[n_props-1] = CORBA::string_dup(row[1]);
	         }
	      }
	      n_attributes += n_rows;
	   }
	   mysql_free_result(result);
	}

	DEBUG_STREAM << "DataBase::GetDeviceProperty(): property_list->length() "<< property_list->length() << endl;

	return(property_list);
}

//+------------------------------------------------------------------
/**
 *	method:	DataBase::db_put_device_attribute_property
 *
 *	description:	method to execute "DbPutDeviceAttributeProperty"
 *
 * @param	argin	
 *
 */
//+------------------------------------------------------------------
void DataBase::db_put_device_attribute_property(const Tango::DevVarStringArray *property_list)
{
	static char sql_query[512];
	static char error_mess[256];
	static char tmp_count_str[256];
	int n_attributes, n_properties=0, n_rows=0, i, j, k;
	const char *tmp_device, *tmp_attribute, *tmp_name;

	sscanf((*property_list)[1],"%d",&n_attributes);
	INFO_STREAM << "DataBase::PutAttributeProperty(): put " << n_attributes << " attributes for device " << (*property_list)[0] << endl;

	k = 2;
	for (i=0; i<n_attributes; i++)
	{
	   tmp_device = (*property_list)[0];
	   tmp_attribute = (*property_list)[k];
	   sscanf((*property_list)[k+1], "%d", &n_properties);
	   for (j=k+2; j<k+n_properties*2+2; j=j+2)
	   {
	      tmp_name = (*property_list)[j];
          string tmp_escaped_string = escape_string((*property_list)[j+1]);

// first delete the tuple (device,name,count) from the property table

	      sprintf(sql_query,"DELETE FROM property_attribute_device WHERE device LIKE \"%s\" AND attribute LIKE \"%s\" AND name LIKE \"%s\" ",
	              tmp_device, tmp_attribute, tmp_name);
	      DEBUG_STREAM << "DataBase::PutAttributeProperty(): sql_query " << sql_query << endl;
	      if (mysql_real_query(&mysql, sql_query, strlen(sql_query)) != 0)
	      {
	         WARN_STREAM << "DataBase::PutAttributeProperty(): failed to delete property from TANGO database ";
	         WARN_STREAM << "for " << tmp_device << "/" << tmp_attribute << "." << tmp_name << " (error=" << mysql_error(&mysql) << ")" << endl;
	         sprintf(error_mess,"failed to delete property from TANGO database  for %s/%s.%s (error=%s)",tmp_device,tmp_attribute,tmp_name,mysql_error(&mysql));
	         Tango::Except::throw_exception((const char *)DB_SQLError,
		 				(const char *)error_mess,
						(const char *)"DataBase::PutAttributeProperty()");
	      }

// then insert the new value for this tuple

	      sprintf(sql_query,"INSERT INTO property_attribute_device SET device=\'%s\',attribute=\'%s\',name=\'%s\',count=\'1\',value=\'%s\',updated=NULL,accessed=NULL",
	              tmp_device, tmp_attribute, tmp_name, tmp_escaped_string.c_str());
	      DEBUG_STREAM << "DataBase::PutAttributeProperty(): sql_query " << sql_query << endl;

	      if (mysql_real_query(&mysql, sql_query, strlen(sql_query)) != 0)
	      {
	         WARN_STREAM << "DataBase::PutAttributeProperty(): failed to insert property into TANGO database ";
	         WARN_STREAM << "for " << tmp_device << "/" << tmp_attribute << "." << tmp_name << " (error=" << mysql_error(&mysql) << ")" << endl;
	         sprintf(error_mess,"failed to insert property into TANGO database  for %s/%s.%s (error=%s)",tmp_device,tmp_attribute,tmp_name,mysql_error(&mysql));
	         Tango::Except::throw_exception((const char *)DB_SQLError,
		 				(const char *)error_mess,
						(const char *)"DataBase::PutAttributeProperty()");
	      }
	   }
	   k = k+n_properties*2+2;
	}

	return;
}

//+------------------------------------------------------------------
/**
 *	method:	DataBase::db_get_class_attribute_property
 *
 *	description:	method to execute "DbGetClassAttributeProperty"
 *
 * @param	argin	
 * @return	
 *
 */
//+------------------------------------------------------------------
Tango::DevVarStringArray *DataBase::db_get_class_attribute_property(const Tango::DevVarStringArray *property_names)
{
	static char sql_query[256];
	static char error_mess[256];
	static char n_attributes_str[256];
	static char n_rows_str[256];
	MYSQL_RES *result;
	MYSQL_ROW row;
	int n_attributes=0, n_rows=0, n_props=0, i, j;
	Tango::DevVarStringArray *property_list = new Tango::DevVarStringArray;
	const char *tmp_class, *tmp_attribute;

	INFO_STREAM << "DataBase::GetAttributeProperty(): get " << property_names->length()-1 << " attributes for class " << (*property_names)[0] << endl;

	tmp_class = (*property_names)[0];
	sprintf(n_attributes_str, "%d", property_names->length()-1);
	n_props = 2;
	property_list->length(n_props);
	(*property_list)[n_props-2] = CORBA::string_dup(tmp_class);
	(*property_list)[n_props-1] = CORBA::string_dup(n_attributes_str);

	for (i=1; i<property_names->length(); i++)
	{
	   tmp_attribute = (*property_names)[i];
	   sprintf(sql_query,"SELECT name,value FROM property_attribute_class WHERE class = \"%s\" AND attribute LIKE \"%s\" ",
	           tmp_class, tmp_attribute);
	   DEBUG_STREAM << "DataBase::GetAttributeProperty(): sql_query " << sql_query << endl;

	   if (mysql_real_query(&mysql, sql_query, strlen(sql_query)) != 0)
	   {
	      WARN_STREAM << "DataBase::GetAttributeProperty(): failed to query TANGO database for list of properties ";
	      WARN_STREAM << "for " << (*property_names)[0] << "/" << (*property_names)[i] << " (error=" << mysql_error(&mysql) << ")" << endl;
	      sprintf(error_mess,"failed to query TANGO database for list of properties for %s/%s (error=%s)",tmp_class,tmp_attribute,mysql_error(&mysql));
	      Tango::Except::throw_exception((const char *)DB_SQLError,
	      				     (const char *)error_mess,
					     (const char *)"DataBase::GetAttributeProperty()");
	   }

	   if ((result = mysql_store_result(&mysql)) == NULL)
	   {
	      WARN_STREAM << "DataBase::GetAttributeProperty(): mysql_store_result() failed  (error=" << mysql_error(&mysql) << ")" << endl;
	      sprintf(error_mess,"mysql_store_result() failed (error=%s)",mysql_error(&mysql));
	      Tango::Except::throw_exception((const char *)DB_SQLError,
	      				     (const char *)error_mess,
					     (const char *)"DataBase::GetAttributeProperty()");
	   }

	   n_rows = mysql_num_rows(result);
	   DEBUG_STREAM << "DataBase::GetAttributeProperty(): mysql_num_rows() " << n_rows << endl;
	   sprintf(n_rows_str,"%d",n_rows);
	   n_props = n_props+2;
	   property_list->length(n_props);
	   (*property_list)[n_props-2] = CORBA::string_dup(tmp_attribute);
           (*property_list)[n_props-1] = CORBA::string_dup(n_rows_str);
	   if (n_rows > 0)
	   {

	      for (j=0; j<n_rows; j++)
	      {
	         if ((row = mysql_fetch_row(result)) != NULL)
	         {
	            DEBUG_STREAM << "DataBase::GetAttributeProperty(): property[ "<< i << "] count " << row[0] << " value " << row[1] << endl;
	            n_props = n_props+2;
	            property_list->length(n_props);
	            (*property_list)[n_props-2] = CORBA::string_dup(row[0]);
	            (*property_list)[n_props-1] = CORBA::string_dup(row[1]);
	         }
	      }
	      n_attributes += n_rows;
	   }
	   mysql_free_result(result);
	}

	DEBUG_STREAM << "DataBase::GetClassProperty(): property_list->length() "<< property_list->length() << endl;

	return(property_list);
}

//+------------------------------------------------------------------
/**
 *	method:	DataBase::db_put_class_attribute_property
 *
 *	description:	method to execute "DbPutClassAttributeProperty"
 *
 * @param	argin	
 *
 */
//+------------------------------------------------------------------
void DataBase::db_put_class_attribute_property(const Tango::DevVarStringArray *property_list)
{
	static char sql_query[512];
	static char error_mess[256];
	static char tmp_count_str[256];
	int n_attributes, n_properties=0, n_rows=0, i, j, k;
	const char *tmp_class, *tmp_attribute, *tmp_name;

	sscanf((*property_list)[1],"%d",&n_attributes);
	INFO_STREAM << "DataBase::PutAttributeProperty(): put " << n_attributes << " attributes for device " << (*property_list)[0] << endl;

	k = 2;
	for (i=0; i<n_attributes; i++)
	{
	   tmp_class = (*property_list)[0];
	   tmp_attribute = (*property_list)[k];
	   sscanf((*property_list)[k+1], "%d", &n_properties);
	   for (j=k+2; j<k+n_properties*2+2; j=j+2)
	   {
	      tmp_name = (*property_list)[j];
          string tmp_escaped_string = escape_string((*property_list)[j+1]);

// first delete the tuple (device,name,count) from the property table

	      sprintf(sql_query,"DELETE FROM property_attribute_class WHERE class LIKE \"%s\" AND attribute LIKE \"%s\" AND name LIKE \"%s\" ",
	              tmp_class, tmp_attribute, tmp_name);
	      DEBUG_STREAM << "DataBase::PutAttributeProperty(): sql_query " << sql_query << endl;
	      if (mysql_real_query(&mysql, sql_query, strlen(sql_query)) != 0)
	      {
	         WARN_STREAM << "DataBase::PutAttributeProperty(): failed to delete property from TANGO database ";
	         WARN_STREAM << "for " << tmp_class << "/" << tmp_attribute << "." << tmp_name << " (error=" << mysql_error(&mysql) << ")" << endl;
	         sprintf(error_mess,"failed to delete property from TANGO database  for %s/%s.%s (error=%s)",tmp_class,tmp_attribute,tmp_name,mysql_error(&mysql));
	         Tango::Except::throw_exception((const char *)DB_SQLError,
		 				(const char *)error_mess,
						(const char *)"DataBase::PutAttributeProperty()");
	      }

// then insert the new value for this tuple

	      sprintf(sql_query,"INSERT INTO property_attribute_class SET class=\'%s\',attribute=\'%s\',name=\'%s\',count=\'1\',value=\'%s\',updated=NULL,accessed=NULL",
	              tmp_class, tmp_attribute, tmp_name, tmp_escaped_string.c_str());
	      DEBUG_STREAM << "DataBase::PutAttributeProperty(): sql_query " << sql_query << endl;

	      if (mysql_real_query(&mysql, sql_query, strlen(sql_query)) != 0)
	      {
	         WARN_STREAM << "DataBase::PutAttributeProperty(): failed to insert property into TANGO database ";
	         WARN_STREAM << "for " << tmp_class << "/" << tmp_attribute << "." << tmp_name << " (error=" << mysql_error(&mysql) << ")" << endl;
	         sprintf(error_mess,"failed to insert property into TANGO database  for %s/%s.%s (error=%s)",tmp_class,tmp_attribute,tmp_name,mysql_error(&mysql));
	         Tango::Except::throw_exception((const char *)DB_SQLError,
		 				(const char *)error_mess,
						(const char *)"DataBase::PutAttributeProperty()");
	      }
	   }
	   k = k+n_properties*2+2;
	}

	return;
}

//+------------------------------------------------------------------
/**
 *	method:	DataBase::db_get_class_attribute_list
 *
 *	description:	method to execute "DbGetClassAttributeList"
 *
 * @param	argin	
 * @return	
 *
 */
//+------------------------------------------------------------------
Tango::DevVarStringArray *DataBase::db_get_class_attribute_list(const Tango::DevVarStringArray *class_wildcard)
{

	static char sql_query[256];
	static char error_mess[256];
	MYSQL_RES *result;
	MYSQL_ROW row;
	int n_attributes=0, n_rows=0, n_attrs=0, j;
	Tango::DevVarStringArray *attribute_list = new Tango::DevVarStringArray;
	const char *class_name, *wildcard;
	string tmp_wildcard;

	class_name = (*class_wildcard)[0];	
	INFO_STREAM << "DataBase::db_get_class_attribute(): get attributes for class " << class_name << endl;

	wildcard = (*class_wildcard)[1];	
	if (wildcard == NULL)
	{
#ifdef WIN32
		sprintf(sql_query,"SELECT DISTINCT attribute FROM property_attribute_class WHERE class = \"%s\"  AND name like \"\\%\"",
#else
		sprintf(sql_query,"SELECT DISTINCT attribute FROM property_attribute_class WHERE class = \"%s\"  AND name like \"\%\"",
#endif /* WIN32 */

	           class_name);
	}
	else
	{
		tmp_wildcard = replace_wildcard(wildcard);
		sprintf(sql_query,"SELECT DISTINCT attribute FROM property_attribute_class WHERE class = \"%s\"  AND name like \"%s\"",
	           class_name, tmp_wildcard.c_str());
	}
	DEBUG_STREAM << "DataBase::GetClassAttributeList(): sql_query " << sql_query << endl;

	if (mysql_real_query(&mysql, sql_query, strlen(sql_query)) != 0)
	{
	      WARN_STREAM << "DataBase::GetClassAttributeList(): failed to query TANGO database for list of attribute ";
	      WARN_STREAM << "for " << class_name << "(error=" << mysql_error(&mysql) << ")" << endl;
	      sprintf(error_mess,"failed to query TANGO database for list of attributes for %s (error=%s)",class_name,mysql_error(&mysql));
	      Tango::Except::throw_exception((const char *)DB_SQLError,
	      				     (const char *)error_mess,
					     (const char *)"DataBase::GetClassAttribute()");
	}

	if ((result = mysql_store_result(&mysql)) == NULL)
	{
	      WARN_STREAM << "DataBase::GetClassAttributeList(): mysql_store_result() failed  (error=" << mysql_error(&mysql) << ")" << endl;
	      sprintf(error_mess,"mysql_store_result() failed (error=%s)",mysql_error(&mysql));
	      Tango::Except::throw_exception((const char *)DB_SQLError,
	      				     (const char *)error_mess,
					     (const char *)"DataBase::GetClassAttributeList()");
	}

	n_rows = mysql_num_rows(result);
	DEBUG_STREAM << "DataBase::GetClassAttributeList(): num_rows() " << n_rows << endl;
	if (n_rows > 0)
	{

	      for (j=0; j<n_rows; j++)
	      {
	         if ((row = mysql_fetch_row(result)) != NULL)
	         {
	            DEBUG_STREAM << "DataBase::GetClassAttributeList(): attribute[ "<< j << "] " << row[0] << endl;
		    n_attrs++;
		    attribute_list->length(n_attrs);
	            (*attribute_list)[n_attrs-1] = CORBA::string_dup(row[0]);
	         }
	      }
	}
	mysql_free_result(result);

	DEBUG_STREAM << "DataBase::GetClassAttributeList(): attribute_list->length() "<< attribute_list->length() << endl;

	return(attribute_list);
}
//+------------------------------------------------------------------
/**
 *	method:	DataBase::db_get_class_property
 *
 *	description:	method to execute "DbGetClassProperty"
 *
 * @param	argin	
 * @return	
 *
 */
//+------------------------------------------------------------------
Tango::DevVarStringArray *DataBase::db_get_class_property(const Tango::DevVarStringArray *property_names)
{
	static char sql_query[256];
	static char error_mess[256];
	static char n_properties_str[256];
	static char n_rows_str[256];
	MYSQL_RES *result;
	MYSQL_ROW row;
	int n_properties=0, n_rows=0, n_props=0, i, j;
	Tango::DevVarStringArray *property_list = new Tango::DevVarStringArray;
	const char *tmp_class, *tmp_name;

	INFO_STREAM << "DataBase::GetClassProperty(): get " << property_names->length()-1 << " properties for device " << (*property_names)[0] << endl;

	tmp_class = (*property_names)[0];
	sprintf(n_properties_str, "%d", property_names->length()-1);
	n_props = 2;
	property_list->length(n_props);
	(*property_list)[0] = CORBA::string_dup(tmp_class);
	(*property_list)[1] = CORBA::string_dup(n_properties_str);

	for (i=1; i<property_names->length(); i++)
	{
	   tmp_name = (*property_names)[i];
	   sprintf(sql_query,"SELECT count,value FROM property_class WHERE class = \"%s\" AND name LIKE \"%s\" ORDER BY count",
	           tmp_class, tmp_name);
	   DEBUG_STREAM << "DataBase::GetClassProperty(): sql_query " << sql_query << endl;

	   if (mysql_real_query(&mysql, sql_query, strlen(sql_query)) != 0)
	   {
	      WARN_STREAM << "DataBase::GetClassProperty(): failed to query TANGO database for list of properties ";
	      WARN_STREAM << "for " << (*property_names)[0] << "/" << (*property_names)[i] << " (error=" << mysql_error(&mysql) << ")" << endl;
	      sprintf(error_mess,"failed to query TANGO database for list of properties for %s/%s (error=%s)",tmp_class,tmp_name,mysql_error(&mysql));
	      Tango::Except::throw_exception((const char *)DB_SQLError,
	      				     (const char *)error_mess,
					     (const char *)"DataBase::GetClassProperty()");
	   }

	   if ((result = mysql_store_result(&mysql)) == NULL)
	   {
	      WARN_STREAM << "DataBase::GetClassProperty(): mysql_store_result() failed  (error=" << mysql_error(&mysql) << ")" << endl;
	      sprintf(error_mess,"mysql_store_result() failed (error=%s)",mysql_error(&mysql));
	      Tango::Except::throw_exception((const char *)DB_SQLError,
	      				     (const char *)error_mess,
					     (const char *)"DataBase::GetClassProperty()");
	   }

	   n_rows = mysql_num_rows(result);
	   DEBUG_STREAM << "DataBase::GetClassProperty(): mysql_num_rows() " << n_rows << endl;
	   sprintf(n_rows_str,"%d",n_rows);
	   n_props = n_props+2;
	   property_list->length(n_props);
	   (*property_list)[n_props-2] = CORBA::string_dup(tmp_name);
           (*property_list)[n_props-1] = CORBA::string_dup(n_rows_str);
	   if (n_rows > 0)
	   {

	      for (j=0; j<n_rows; j++)
	      {
	         if ((row = mysql_fetch_row(result)) != NULL)
	         {
	            DEBUG_STREAM << "DataBase::GetClassProperty(): property[ "<< i << "] count " << row[0] << " value " << row[1] << endl;
		    n_props++;
		    property_list->length(n_props);
	            (*property_list)[n_props-1] = CORBA::string_dup(row[1]);
	         }
	      }
	      n_properties += n_rows;
	   }
	   mysql_free_result(result);
	}

	DEBUG_STREAM << "DataBase::GetClassProperty(): property_list->length() "<< property_list->length() << endl;

	return(property_list);
}

//+------------------------------------------------------------------
/**
 *	method:	DataBase::db_put_class_property
 *
 *	description:	method to execute "DbPutClassProperty"
 *
 * @param	argin	
 *
 */
//+------------------------------------------------------------------
void DataBase::db_put_class_property(const Tango::DevVarStringArray *property_list)
{
	static char sql_query[512];
	static char error_mess[256];
	static char tmp_count_str[256];
	int n_properties=0, n_rows=0, i, j, k;
	const char *tmp_class, *tmp_name, *tmp_count;

	sscanf((*property_list)[1],"%d",&n_properties);
	INFO_STREAM << "DataBase::PutClassProperty(): put " << n_properties << " properties for device " << (*property_list)[0] << endl;

	k = 2;
	tmp_class = (*property_list)[0];
	for (i=0; i<n_properties; i++)
	{
	   tmp_count = 0;
	   tmp_name = (*property_list)[k];

// first delete all tuples (device,name) from the property table

	   sprintf(sql_query,"DELETE FROM property_class WHERE class LIKE \"%s\" AND name LIKE \"%s\"",
	              tmp_class, tmp_name);
	   DEBUG_STREAM << "DataBase::PutClassProperty(): sql_query " << sql_query << endl;
	   if (mysql_real_query(&mysql, sql_query, strlen(sql_query)) != 0)
	   {
	      WARN_STREAM << "DataBase::PutClassProperty(): failed to delete property from TANGO database ";
	      WARN_STREAM << "for " << tmp_class << "/" << tmp_name << " (error=" << mysql_error(&mysql) << ")" << endl;
	      sprintf(error_mess,"failed to delete property from TANGO database  for %s/%s (error=%s)",tmp_class,tmp_name,mysql_error(&mysql));
	      Tango::Except::throw_exception((const char *)DB_SQLError,
				(const char *)error_mess,
				(const char *)"DataBase::PutClassProperty()");
	   }
	   sscanf((*property_list)[k+1], "%d", &n_rows);
	   for (j=k+2; j<k+n_rows+2; j++)
	   {
          string tmp_escaped_string = escape_string((*property_list)[j]);
	      tmp_count++; sprintf(tmp_count_str, "%d", tmp_count);

// then insert the new value for this tuple

	      sprintf(sql_query,"INSERT INTO property_class SET class=\'%s\',name=\'%s\',count=\'%s\',value=\'%s\',updated=NULL,accessed=NULL",
	              tmp_class, tmp_name, tmp_count_str, tmp_escaped_string.c_str());
	      DEBUG_STREAM << "DataBase::PutClassProperty(): sql_query " << sql_query << endl;

	      if (mysql_real_query(&mysql, sql_query, strlen(sql_query)) != 0)
	      {
	         WARN_STREAM << "DataBase::PutClassProperty(): failed to insert property into TANGO database ";
	         WARN_STREAM << "for " << tmp_class << "/" << tmp_name << " (error=" << mysql_error(&mysql) << ")" << endl;
	         sprintf(error_mess,"failed to insert property into TANGO database  for %s/%s (error=%s)",tmp_class,tmp_name,mysql_error(&mysql));
	         Tango::Except::throw_exception((const char *)DB_SQLError,
		 				(const char *)error_mess,
						(const char *)"DataBase::PutClassProperty()");
	      }
	   }
	   k = k+n_rows+2;
	}

	return;
}

//+------------------------------------------------------------------
/**
 *	method:	DataBase::db_delete_class_property
 *
 *	description:	method to execute "DbDeleteClassProperty"
 *
 * @param	argin	
 *
 */
//+------------------------------------------------------------------
void DataBase::db_delete_class_property(const Tango::DevVarStringArray *property_list)
{
	static char sql_query[512];
	static char error_mess[256];
	static char tmp_count_str[256];
	int n_properties=0, i;
	const char *tmp_class;
	string tmp_name;

	n_properties = property_list->length() - 1;
	INFO_STREAM << "DataBase::DeleteClassProperty(): delete " << n_properties << " properties for class " << (*property_list)[0] << endl;

	for (i=0; i<n_properties; i++)
	{
	      tmp_class = (*property_list)[0];
	      tmp_name = replace_wildcard((*property_list)[i+1]);

// delete the tuple (device,name,count) from the property table

	      sprintf(sql_query,"DELETE FROM property_class WHERE class=\"%s\" AND name LIKE \"%s\"",
	              tmp_class, tmp_name.c_str());
	      DEBUG_STREAM << "DataBase::DeleteClassProperty(): sql_query " << sql_query << endl;
	      if (mysql_real_query(&mysql, sql_query, strlen(sql_query)) != 0)
	      {
	         WARN_STREAM << "DataBase::DeleteClassProperty(): failed to delete property from TANGO database ";
	         WARN_STREAM << "for " << tmp_class << "/" << tmp_name << " (error=" << mysql_error(&mysql) << ")" << endl;
	         sprintf(error_mess,"failed to delete property from TANGO database  for %s/%s (error=%s)",tmp_class,tmp_name.c_str(),mysql_error(&mysql));
	         Tango::Except::throw_exception((const char *)DB_SQLError,
		 				(const char *)error_mess,
						(const char *)"DataBase::DeleteClassProperty()");
	      }

	}

	return;
}

//+------------------------------------------------------------------
/**
 *	method:	DataBase::db_export_device
 *
 *	description:	method to execute "DbExportDevice"
 *
 * @param	argin	
 *
 */
//+------------------------------------------------------------------
void DataBase::db_export_device(const Tango::DevVarStringArray *export_info)
{
	static char sql_query[4096];
	static char error_mess[256];
	MYSQL_RES *result;
	MYSQL_ROW row;
	long n_rows=0, i;
	const char *tmp_ior, *tmp_host, *tmp_pid, *tmp_version;
	string tmp_device, tmp_server;


	if (export_info->length() < 5) {
   		WARN_STREAM << "DataBase::DbExportDevice(): insufficient export info for device ";
   		WARN_STREAM << tmp_device << endl;
   		sprintf(error_mess,"insufficient export info for device %s",tmp_device.c_str());
   		Tango::Except::throw_exception((const char *)DB_IncorrectArguments,
					       (const char *)error_mess,
					       (const char *)"DataBase::DbExportDevice()");
	}
	
	INFO_STREAM << "DataBase::ExportDevice(): put " << export_info->length()-1 << " export info for device " << (*export_info)[0] << endl;

	tmp_device = (*export_info)[0];
	for (i=0; i<tmp_device.length(); i++) { 
		tmp_device[i] = tolower(tmp_device[i]);
	}
	tmp_ior = (*export_info)[1];
	tmp_host = (*export_info)[2];
	tmp_pid = (*export_info)[3];
	tmp_version = (*export_info)[4];

//
//	If it is a server stating -> get previous host where running
//
	bool	do_fire = false;
	string	previous_host;
	if (fireToStarter==true)
	{
		if (tmp_device.substr(0,8) == "dserver/")
		{
			//	Get database server name
			//--------------------------------------
			Tango::Util *tg = Tango::Util::instance();
			string	db_serv = tg->get_ds_name();
			transform(db_serv.begin(), db_serv.end(), db_serv.begin(), ::tolower);
			string	adm_dev("dserver/");
			adm_dev += db_serv;

			//	Check if not database or starter servers
			if (tmp_device !=  adm_dev &&
				tmp_device.substr(0,16) != "dserver/starter/" )
			{
				do_fire = true;
				previous_host = db_get_device_host((Tango::DevString)tmp_device.c_str());
				DEBUG_STREAM << tmp_device << " was running on " << previous_host << endl;
			}
		}
	}
//
// check if device is defined and if so get server name in order to
// update server table 
//
	sprintf(sql_query,"SELECT server FROM device WHERE name LIKE \"%s\" ",
	        tmp_device.c_str());
	DEBUG_STREAM << "DataBase::ExportDevice(): sql_query " << sql_query << endl;

	if (mysql_real_query(&mysql, sql_query, strlen(sql_query)) != 0)
	{
	   WARN_STREAM << "DataBase::ExportDevice(): failed to query TANGO database for server name ";
	   WARN_STREAM << "for device " << tmp_device << " (error=" << mysql_error(&mysql) << ")" << endl;
	   sprintf(error_mess,"failed to query TANGO database for server name of device for %s (error=%s)",tmp_device.c_str(),mysql_error(&mysql));
	   Tango::Except::throw_exception((const char *)DB_SQLError,
	   				  (const char *)error_mess,
					  (const char *)"DataBase::ExportDevice()");
	}

	if ((result = mysql_store_result(&mysql)) == NULL)
	{
	   WARN_STREAM << "DataBase::ExportDevice(): mysql_store_result() failed  (error=" << mysql_error(&mysql) << ")" << endl;
	   sprintf(error_mess,"mysql_store_result() failed (error=%s)",mysql_error(&mysql));
	   Tango::Except::throw_exception((const char *)DB_SQLError,
	   				  (const char *)error_mess,
					  (const char *)"DataBase::ExportDevice()");
	}

	n_rows = mysql_num_rows(result);
	DEBUG_STREAM << "DataBase::ExportDevice(): mysql_num_rows() " << n_rows << endl;

	if (n_rows > 0)
	{

	   if ((row = mysql_fetch_row(result)) != NULL)
	   {
	      DEBUG_STREAM << "DataBase::ExportDevice(): device "<< tmp_device << " server name " << row[0] << endl;
	      tmp_server = row[0];
	   }
	}
	else {
	     INFO_STREAM << "DataBase::ExportDevice(): device not defined !" << endl;
	     sprintf(error_mess,"device %s not defined in the database !",tmp_device.c_str());
	     mysql_free_result(result);
	     Tango::Except::throw_exception((const char *)DB_DeviceNotDefined,
	     				    (const char *)error_mess,
					    (const char *)"DataBase::ExportDevice()");
	}
	mysql_free_result(result);

// update the new value for this tuple

	sprintf(sql_query,"UPDATE device set exported=1,ior=\'%s\',host=\'%s\',pid=\'%s\',version=\'%s\',started=NOW() where name LIKE \'%s\'",
        	tmp_ior, tmp_host, tmp_pid, tmp_version, tmp_device.c_str());
	DEBUG_STREAM << "DataBase::ExportDevice(): sql_query " << sql_query << endl;

	if (mysql_real_query(&mysql, sql_query, strlen(sql_query)) != 0)
	{
   		WARN_STREAM << "DataBase::DbExportDevice(): failed to export device to TANGO database ";
   		WARN_STREAM << "for " << tmp_device << " (error=" << mysql_error(&mysql) << ")" << endl;
   		sprintf(error_mess,"failed to insert export info into TANGO database for %s",tmp_device.c_str());
   		Tango::Except::throw_exception((const char *)DB_SQLError,
					       (const char *)error_mess,
					       (const char *)"DataBase::DbExportDevice()");
	}

// update host name in server table

        sprintf(sql_query,"UPDATE server set host=\'%s\' where name LIKE \'%s\'",
                tmp_host, tmp_server.c_str());
        DEBUG_STREAM << "DataBase::ExportDevice(): sql_query " << sql_query << endl;
 
        if (mysql_real_query(&mysql, sql_query, strlen(sql_query)) != 0)
        {
                WARN_STREAM << "DataBase::DbExportDevice(): failed to update host name in server table";
                WARN_STREAM << "for " << tmp_device << " (error=" << mysql_error(&mysql) << ")" << endl;
                sprintf(error_mess,"failed to update host name in server table for %s",tmp_device.c_str());
                Tango::Except::throw_exception((const char *)DB_SQLError,
                                               (const char *)error_mess,
                                               (const char *)"DataBase::DbExportDevice()");
        }              

	//	Check if a server has been started.
	if (do_fire)
	{
		//	Update host's starter to update controlled servers list
		vector<string>	hosts;
		hosts.push_back(tmp_host);
		DEBUG_STREAM << "New Host is " << tmp_host << endl;

		if (previous_host!=""      &&
			previous_host!="nada"  && 
			previous_host!=tmp_host)
				hosts.push_back(previous_host);

		starter_shared->send_starter_cmd(hosts);
	}
	return;
}
//+------------------------------------------------------------------
/**
 *	method:	DataBase::db_un_export_device
 *
 *	description:	method to execute "DbUnExportDevice"
 *
 * @param	argin	
 *
 */
//+------------------------------------------------------------------
void DataBase::db_un_export_device(Tango::DevString devname)
{
	static char sql_query[4096];
	static char error_mess[256];
	long n_rows=0, i;
	char *tmp_device;

	INFO_STREAM << "DataBase::UnExportDevice(): un-export " << devname << " device " << endl;

	tmp_device = (char*)malloc(strlen(devname)+1);
	sprintf(tmp_device,"%s",devname);
	for (i=0; i<strlen(tmp_device); i++) { 
		tmp_device[i] = tolower(tmp_device[i]);
	}

// un-export device from database by seting ior="not exported"

	sprintf(sql_query,"UPDATE device SET exported=0,stopped=NOW() WHERE name like \"%s\"",
	        tmp_device);
	DEBUG_STREAM << "DataBase::UnExportDevice(): sql_query " << sql_query << endl;
	if (mysql_real_query(&mysql, sql_query, strlen(sql_query)) != 0)
	{
   		WARN_STREAM << "DataBase::UnExportDevice(): failed to unexport device from TANGO database ";
   		WARN_STREAM << "for " << tmp_device << " (error=" << mysql_error(&mysql) << ")" << endl;
   		sprintf(error_mess,"failed to un-export device %s from TANGO database  (error=%s)",tmp_device,mysql_error(&mysql));
   		Tango::Except::throw_exception((const char *)DB_SQLError,
					       (const char *)error_mess,
					       (const char *)"DataBase::UnExportDevice()");
	}

	free(tmp_device);

	return;
}

//+------------------------------------------------------------------
/**
 *	method:	DataBase::db_un_export_server
 *
 *	description:	method to execute "DbUnExportServer"
 *
 * @param	argin	
 *
 */
//+------------------------------------------------------------------
void DataBase::db_un_export_server(Tango::DevString server_name)
{
	static char sql_query[4096];
	static char error_mess[256];
	long n_rows=0, i;
	char *tmp_server;

	INFO_STREAM << "DataBase::UnExportServer(): un-export all device(s) from server " << server_name << " device " << endl;

	tmp_server = (char*)malloc(strlen(server_name)+1);
	sprintf(tmp_server,"%s",server_name);
	for (i=0; i<strlen(tmp_server); i++) { 
		tmp_server[i] = tolower(tmp_server[i]);
	}

// un-export all devices belonging to this server from database by setting exported="false"

	sprintf(sql_query,"UPDATE device SET exported=0,stopped=NOW() WHERE server like \"%s\"",
	        tmp_server);
	DEBUG_STREAM << "DataBase::UnExportServer(): sql_query " << sql_query << endl;
	if (mysql_real_query(&mysql, sql_query, strlen(sql_query)) != 0)
	{
   		WARN_STREAM << "DataBase::UnExportServer(): failed to unexport devices from TANGO database ";
   		WARN_STREAM << "for " << tmp_server << " (mysql_error=" << mysql_error(&mysql) << ")" << endl;
   		sprintf(error_mess,"failed to un-export devices %s from TANGO database  (error=%s)",tmp_server,mysql_error(&mysql));
   		Tango::Except::throw_exception((const char *)DB_SQLError,
					       (const char *)error_mess,
					       (const char *)"DataBase::UnExportServer()");
	}

	free(tmp_server);

	return;
}
//+------------------------------------------------------------------
/**
 *	method:	DataBase::db_import_device
 *
 *	description:	method to execute "DbImportDevice"
 *
 * @param	argin	
 * @return	
 *
 */
//+------------------------------------------------------------------
Tango::DevVarLongStringArray *DataBase::db_import_device(Tango::DevString devname)
{
	static char sql_query[256];
	static char error_mess[256];
	MYSQL_RES *result;
	MYSQL_ROW row;
	int n_devices=0, n_rows=0, n_svalues=0, n_lvalues=0; 
	int i, exported, pid;
	string tmp_device;

	INFO_STREAM << "DataBase::ImportDevice(): get import info for " << devname << " device " << endl;

	tmp_device = devname;
	for (i=0; i<tmp_device.size(); i++) { 
		tmp_device[i] = tolower(tmp_device[i]);
	}
//	tmp_device = replace_wildcard(tmp_device.c_str());

	sprintf(sql_query,"SELECT exported,ior,version,pid,server,host FROM device WHERE name = '%s' or alias = '%s';",
//	sprintf(sql_query,"SELECT exported,ior,version,pid,server,host FROM device WHERE name = '%s';",
	        tmp_device.c_str(),tmp_device.c_str());
	DEBUG_STREAM << "DataBase::ImportDevice(): sql_query " << sql_query << endl;

	if (mysql_real_query(&mysql, sql_query, strlen(sql_query)) != 0)
	{
	   WARN_STREAM << "DataBase::ImportDeviceList(): failed to query TANGO database for list of properties ";
	   WARN_STREAM << "for import info for device " << tmp_device << " (error=" << mysql_error(&mysql) << ")" << endl;
	   sprintf(error_mess,"failed to query TANGO database for import info of device for %s (error=%s)",tmp_device.c_str(),mysql_error(&mysql));
	   Tango::Except::throw_exception((const char *)DB_SQLError,
	   				  (const char *)error_mess,
					  (const char *)"DataBase::ImportDevice()");
	}

	if ((result = mysql_store_result(&mysql)) == NULL)
	{
	   WARN_STREAM << "DataBase::ImportDeviceList(): mysql_store_result() failed  (error=" << mysql_error(&mysql) << ")" << endl;
	   sprintf(error_mess,"mysql_store_result() failed (error=%s)",mysql_error(&mysql));
	   Tango::Except::throw_exception((const char *)DB_SQLError,
	   				  (const char *)error_mess,
					  (const char *)"DataBase::ImportDevice()");
	}

	n_rows = mysql_num_rows(result);
	DEBUG_STREAM << "DataBase::ImportDeviceList(): mysql_num_rows() " << n_rows << endl;

	Tango::DevVarLongStringArray *import_info = new Tango::DevVarLongStringArray;
	if (n_rows > 0)
	{

	   if ((row = mysql_fetch_row(result)) != NULL)
	   {
	      DEBUG_STREAM << "DataBase::ImportDeviceList(): device[ "<< i << "] exported " << row[0] << " version " << row[2] << " server " << row[4] << " host " << row[5] << endl;
	      n_svalues = n_svalues+5;
	      (import_info->svalue).length(n_svalues);
	      (import_info->svalue)[n_svalues-5] = CORBA::string_dup(tmp_device.c_str());
	      (import_info->svalue)[n_svalues-4] = CORBA::string_dup(row[1]);
	      (import_info->svalue)[n_svalues-3] = CORBA::string_dup(row[2]);
	      (import_info->svalue)[n_svalues-2] = CORBA::string_dup(row[4]);
	      (import_info->svalue)[n_svalues-1] = CORBA::string_dup(row[5]);
	      exported = -1;
	      if (row[0] != NULL) sscanf(row[0],"%d",&exported);
	      n_lvalues++;
	      (import_info->lvalue).length(n_lvalues);
	      (import_info->lvalue)[n_lvalues-1] = exported;
	      pid = -1;
	      if (row[3] != NULL) sscanf(row[3],"%d",&pid);
	      n_lvalues++;
	      (import_info->lvalue).length(n_lvalues);
	      (import_info->lvalue)[n_lvalues-1] = pid;
	   }
		else {
	    	 INFO_STREAM << "DataBase::ImportDevice(): info not defined !" << endl;
	    	 sprintf(error_mess,"device %s import info not found in the database !",tmp_device.c_str());
	    	 mysql_free_result(result);
			 delete import_info;
	    	 Tango::Except::throw_exception((const char *)DB_DeviceNotDefined,
								(const char *)error_mess,
								(const char *)"DataBase::ImportDevice()");
		}
	}
	else {
	     INFO_STREAM << "DataBase::ImportDevice(): device not defined !" << endl;
	     sprintf(error_mess,"device %s not defined in the database !",tmp_device.c_str());
	     mysql_free_result(result);
		 delete import_info;
	     Tango::Except::throw_exception((const char *)DB_DeviceNotDefined,
							(const char *)error_mess,
							(const char *)"DataBase::ImportDevice()");
	}
	mysql_free_result(result);
	/*
	      cout << "DataBase::ImportDeviceList() "<< devname << " : " << endl;
		  for (i=0 ; i<(import_info->lvalue).length() ; i++)
			  cout << "\t lvalue[" << i << "] : " << (import_info->lvalue)[i] << endl;
		  for (i=0 ; i<(import_info->svalue).length() ; i++)
			  cout << "\t svalue[" << i << "] : " << (import_info->svalue)[i] << endl;
		 cout <<  endl;
	*/
	return(import_info);
}

//+------------------------------------------------------------------
/**
 *	method:	DataBase::db_add_server
 *
 *	description:	method to execute "DbAddServer"
 *
 * @param	argin	
 *
 */
//+------------------------------------------------------------------
void DataBase::db_add_server(const Tango::DevVarStringArray *server_device_list)
{
	static char sql_query[512];
	static char error_mess[256];
	static char tmp_count_str[256];
	char domain[256], family[256], member[256];
	int n_rows=0;
	const char *tmp_server, *tmp_class;

	if (server_device_list->length() < 3)
	{
	   WARN_STREAM << "DataBase::AddServer(): incorrect number of input arguments " << endl;
	   sprintf(error_mess,"incorrect no. of input arguments, needs at least 3 (server,device,class)");
	   Tango::Except::throw_exception((const char *)DB_IncorrectArguments,
	   				  (const char *)error_mess,
					  (const char *)"DataBase::AddServer()");
	}
		
	INFO_STREAM << "DataBase::AddServer(): insert " << (*server_device_list)[0] << " server with device " << (*server_device_list)[1] << endl;
	tmp_server = (*server_device_list)[0];
	for (int i=0; i<(server_device_list->length()-1)/2; i++)
	{
		string tmp_device((*server_device_list)[i*2+1].in());
		tmp_class = (*server_device_list)[i*2+2];
		if (!check_device_name(tmp_device))
		{
	   		sprintf(error_mess,"device name (%s) syntax error (should be [tango:][//instance/]domain/family/member)",tmp_device.c_str());
	   		Tango::Except::throw_exception((const char *)DB_IncorrectDeviceName,
						       (const char *)error_mess,
						       (const char *)"DataBase::AddServer()");
		}
		device_name_to_dfm(tmp_device, domain, family, member);

// first delete the tuple (device,name,count) from the property table

		sprintf(sql_query,"DELETE FROM device WHERE server=\"%s\" AND name=\"%s\" ",
	              	tmp_server,tmp_device.c_str());
		DEBUG_STREAM << "DataBase::AddServer(): sql_query " << sql_query << endl;
		if (mysql_real_query(&mysql, sql_query, strlen(sql_query)) != 0)
		{
	   		WARN_STREAM << "DataBase::AddServer(): failed to delete server from TANGO database ";
	   		WARN_STREAM << "for " << tmp_server << " " << tmp_device << " (error=" << mysql_error(&mysql) << ")" << endl;
	   		sprintf(error_mess,"failed to delete server from TANGO database  for %s %s (error=%s)",tmp_server,tmp_device.c_str(),mysql_error(&mysql));
	   		Tango::Except::throw_exception((const char *)DB_SQLError,
						       (const char *)error_mess,
						       (const char *)"DataBase::AddServer()");
		}

// then insert the new value for this tuple

		sprintf(sql_query,"INSERT INTO device SET name=\"%s\",domain=\"%s\",family=\"%s\",member=\"%s\",exported=0,ior=\"nada\",host=\"nada\",server=\"%s\",pid=0,class=\"nada\",version=0,started=NULL,stopped=NULL",
	         	tmp_device.c_str(), domain, family, member, tmp_server);
		DEBUG_STREAM << "DataBase::AddServer(): sql_query " << sql_query << endl;

		if (mysql_real_query(&mysql, sql_query, strlen(sql_query)) != 0)
		{
	      		WARN_STREAM << "DataBase::AddServer(): failed to insert server into TANGO database ";
	      		WARN_STREAM << "for " << tmp_server << " " << tmp_device << " (error=" << mysql_error(&mysql) << ")" << endl;
	      		sprintf(error_mess,"failed to insert server into TANGO database  for %s/%s (error=%s)",tmp_server,tmp_device.c_str(),mysql_error(&mysql));
	      		Tango::Except::throw_exception((const char *)DB_SQLError,
						       (const char *)error_mess,
						       (const char *)"DataBase::AddServer()");
		}
	}

	return;
}

//+------------------------------------------------------------------
/**
 *	method:	DataBase::db_add_device
 *
 *	description:	method to execute "DbAddDevice"
 *
 * @param	argin	
 *
 */
//+------------------------------------------------------------------
void DataBase::db_add_device(const Tango::DevVarStringArray *server_device)
{
	static char sql_query[512];
	static char error_mess[256];
	static char tmp_count_str[256];
 	static char domain[256], family[256], member[256];
	int n_rows=0;
	const char *tmp_server, *tmp_class, *tmp_alias;
	string tmp_device;
	string dserver_name;
	MYSQL_RES *result;


	if (server_device->length() < 3)
	{
	   WARN_STREAM << "DataBase::AddServer(): incorrect number of input arguments " << endl;
	   sprintf(error_mess,"incorrect no. of input arguments, needs at least 3 (server,device,class)");
	   Tango::Except::throw_exception((const char *)DB_IncorrectArguments,
	   				  (const char *)error_mess,
					  (const char *)"DataBase::AddServer()");
	}
		
	INFO_STREAM << "DataBase::AddDevice(): insert " << (*server_device)[0] << " server with device " << (*server_device)[1] << endl;
	tmp_server = (*server_device)[0];
	tmp_device = (*server_device)[1];
	tmp_class = (*server_device)[2];
	if (server_device->length() > 3)
	{
		tmp_alias = (*server_device)[3];
	}
	if (!check_device_name(tmp_device))
	{
	   sprintf(error_mess,"device name (%s) syntax error (should be [tango:][//instance/]domain/family/member)",tmp_device.c_str());
	   Tango::Except::throw_exception((const char *)DB_IncorrectDeviceName,
	   				  (const char *)error_mess,
					  (const char *)"DataBase::AddDevice()");
	}
	device_name_to_dfm(tmp_device, domain, family, member);

// first delete the tuple (device,name) from the device table

	sprintf(sql_query,"DELETE FROM device WHERE name LIKE \"%s\"",
	              tmp_device.c_str());
	DEBUG_STREAM << "DataBase::AddDevice(): sql_query " << sql_query << endl;
	if (mysql_real_query(&mysql, sql_query, strlen(sql_query)) != 0)
	{
	   WARN_STREAM << "DataBase::AddDevice(): failed to delete device from TANGO database ";
	   WARN_STREAM << "for " << tmp_server << " " << tmp_device << " (error=" << mysql_error(&mysql) << ")" << endl;
	   sprintf(error_mess,"failed to delete device from TANGO database  for %s %s (error=%s)",tmp_server,tmp_device.c_str(),mysql_error(&mysql));
	   Tango::Except::throw_exception((const char *)DB_SQLError,
	   				  (const char *)error_mess,
					  (const char *)"DataBase::AddDevice()");
	}

// then insert the new value for this tuple

	if (server_device->length() < 4)
	{
		sprintf(sql_query,"INSERT INTO device SET name=\"%s\",domain=\"%s\",family=\"%s\",member=\"%s\",exported=0,ior=\"nada\",host=\"nada\",server=\"%s\",pid=0,class=\"%s\",version=\"0\",started=NULL,stopped=NULL",
		tmp_device.c_str(), domain, family, member, tmp_server,tmp_class);
	}
	else
	{
		sprintf(sql_query,"INSERT INTO device SET name=\"%s\",domain=\"%s\",family=\"%s\",member=\"%s\",exported=0,ior=\"nada\",host=\"nada\",server=\"%s\",pid=0,class=\"%s\",alias=\"%s\",version=\"0\",started=NULL,stopped=NULL",
		tmp_device.c_str(), domain, family, member, tmp_server, tmp_class, tmp_alias);
	}
	DEBUG_STREAM << "DataBase::AddServer(): sql_query " << sql_query << endl;

	if (mysql_real_query(&mysql, sql_query, strlen(sql_query)) != 0)
	{
	      WARN_STREAM << "DataBase::AddDevice(): failed to insert device into TANGO database ";
	      WARN_STREAM << "for " << tmp_server << " " << tmp_device << " (error=" << mysql_error(&mysql) << ")" << endl;
	      sprintf(error_mess,"failed to insert device into TANGO database  for %s/%s (error=%s)",tmp_server,tmp_device.c_str(),mysql_error(&mysql));
	      Tango::Except::throw_exception((const char *)DB_SQLError,
	      				     (const char *)error_mess,
					     (const char *)"DataBase::AddDevice()");
	}

//
// Check if a DServer device entry for the process already exists
//

	sprintf(sql_query,"SELECT name FROM device WHERE server LIKE \"%s\" AND class LIKE \"DServer\"",
	              tmp_server);
	DEBUG_STREAM << "DataBase::AddDevice(): sql_query " << sql_query << endl;
	if (mysql_real_query(&mysql, sql_query, strlen(sql_query)) != 0)
	{
	   WARN_STREAM << "DataBase::AddDevice(): failed to search DServer from TANGO database ";
	   WARN_STREAM << "for " << tmp_server << " (error=" << mysql_error(&mysql) << ")" << endl;
	   sprintf(error_mess,"failed to search DServer from TANGO database  for %s (error=%s)",tmp_server,mysql_error(&mysql));
	   Tango::Except::throw_exception((const char *)DB_SQLError,
	   				  (const char *)error_mess,
					  (const char *)"DataBase::AddDevice()");
	}
	
	if ((result = mysql_store_result(&mysql)) == NULL)
	{
	   WARN_STREAM << "DataBase::AddDevice(): mysql_store_result() failed  (error=" << mysql_error(&mysql) << ")" << endl;
	   sprintf(error_mess,"mysql_store_result() failed (error=%s)",mysql_error(&mysql));
	   Tango::Except::throw_exception((const char *)DB_SQLError,
	   				  (const char *)error_mess,
					  (const char *)"DataBase::db_get_device_attribute_list()");
	}

	n_rows = mysql_num_rows(result);
	DEBUG_STREAM << "DataBase::AddDevice(): mysql_num_rows() " << n_rows << endl;

//
// If there is no admin device for the device's server, create one
//
	
	if (n_rows == 0)
	{		
		dserver_name = "dserver/";
		dserver_name = dserver_name + string(tmp_server);
		device_name_to_dfm(dserver_name, domain, family, member);
		sprintf(sql_query,"INSERT INTO device SET name=\"dserver/%s\",domain=\"%s\",family=\"%s\",member=\"%s\",exported=0,ior=\"nada\",host=\"nada\",server=\"%s\",pid=0,class=\"DServer\",version=\"0\",started=NULL,stopped=NULL",
		tmp_server, domain, family, member, tmp_server,tmp_class);
		DEBUG_STREAM << "DataBase::AddServer(): sql_query " << sql_query << endl;

		if (mysql_real_query(&mysql, sql_query, strlen(sql_query)) != 0)
		{
	      		WARN_STREAM << "DataBase::AddDevice(): failed to insert DServer into TANGO database ";
	      		WARN_STREAM << "for " << tmp_server << " (error=" << mysql_error(&mysql) << ")" << endl;
	      		sprintf(error_mess,"failed to insert DServer into TANGO database  for %s (error=%s)",tmp_server,mysql_error(&mysql));
	      		Tango::Except::throw_exception((const char *)DB_SQLError,
	      				     (const char *)error_mess,
					     (const char *)"DataBase::AddDevice()");
		}
	}

	mysql_free_result(result);	
	return;
}

//+----------------------------------------------------------------------------
//
// method : 		DataBase::check_device_name(string *device_name)
// 
// description : 	utility function to check whether device name conforms
//			to the TANGO naming convention of 
//
//			[tango!taco:][//instance/]domain/family/member
//
// in :			string *device_name - device name
//
// out :		bool - true or false
//
//-----------------------------------------------------------------------------

bool DataBase::check_device_name(string &device_name_str)
{
	string devname(device_name_str);
	string::size_type index, index2;

	DEBUG_STREAM << "DataBase::check_device_name(): device_name in " << devname << endl;

// check there are no special characters which could be interpreted
// as wildcards or which are otherwise excluded

	if (devname.find('*') != string::npos) return false;

// check protocol - "tango:" | "taco:"
	
	if (devname.substr(0,6) == "tango:")
	{
		devname.erase(0,6);
	}
	else
	{
		if (devname.substr(0,5) == "taco:")
		{
			devname.erase(0,5);
		}
	}

// check instance - "//instance/"

	if (devname.substr(0,2) == "//")
	{
		index = devname.find('/',(string::size_type)2);
		if (index == 0 || index == string::npos)		
		{
			return false;
			
		}
		devname.erase(0,index+1);
	}

// check name conforms to "D/F/M"

	index = devname.find('/');
	index2 = devname.find('/',index+1);

	if (index == 0 || index == string::npos || index2 == string::npos ||
	    index2-index <= 0 || devname.length() - index2 <= 0)
	{
		return false;
	}

	device_name_str = devname;

	DEBUG_STREAM << "DataBase::check_device_name(): device_name out " << device_name_str << endl;

	return true;
}

//+----------------------------------------------------------------------------
//
// method : 		DataBase::device_name_to_dfm(string &device_name,
//			          char **domain, char **family, char **member)
// 
// description : 	utility function to return domain, family and member
//			from device name. Assumes device name has (optional)
//			protocol and instance stripped off i.e. conforms
//			to domain/family/member
//
// in :			char *devname - device name
//
// out :		bool - true or false
//
//-----------------------------------------------------------------------------

bool DataBase::device_name_to_dfm(string &devname, char domain[], char family[], char member[])
{
	string::size_type index, index2;

	DEBUG_STREAM << "DataBase::device_name_to_dfm() device name in " << devname << endl;

	index = devname.find('/');
	index2 = devname.find('/', index+1);

	(devname.substr(0,index)).copy(domain,index);
	domain[index] = '\0';

	(devname.substr(index+1,index2-index)).copy(family,index2-index);
	family[index2-index-1] = '\0';

	(devname.substr(index2+1)).copy(member,devname.length()-index2);
	member[devname.length()-index2-1] = '\0';

	DEBUG_STREAM << "DataBase::device_name_to_dfm() domain/family/member out " << domain << "/" << family << "/" << member << endl;

	return true;
}

//+----------------------------------------------------------------------------
//
// method : 		DataBase::replace_wildcard(char *wildcard_c_str)
// 
// description : 	utility method to replace all occurrences of
//			wildcards (*) with SQL wildcards % and to escape
//			all occurrences	of % and _ with \
//
// in :			string **wildcard_c_str - wildcard C string
//
// out :		void - nothing
//
//-----------------------------------------------------------------------------

string DataBase::replace_wildcard(const char *wildcard_c_str)
{
	string wildcard(wildcard_c_str);
	string::size_type index;

	DEBUG_STREAM << "DataBase::replace_wildcard() wildcard in " << wildcard << endl;
// escape %

	index = 0;
	while ((index = wildcard.find('%',index)) != string::npos)
	{
		wildcard.insert(index, 1, '\\');
		index = index+2;
	}

// escape _

	index = 0;
	while ((index = wildcard.find('_',index)) != string::npos)
	{
		wildcard.insert(index, 1, '\\');
		index = index+2;
	}


// escape "

	index = 0;
	while ((index = wildcard.find('"',index)) != string::npos)
	{
		wildcard.insert(index, 1, '\\');
		index = index+2;
	}


// escape '

	index = 0;
	while ((index = wildcard.find('\'',index)) != string::npos)
	{
		wildcard.insert(index, 1, '\\');
		index = index+2;
	}

// replace wildcard * with %

	while ((index = wildcard.find('*')) != string::npos)
	{
		wildcard.replace(index, 1, 1, '%');
	}

	DEBUG_STREAM << "DataBase::replace_wildcard() wildcard out " << wildcard << endl;
	return wildcard;
}

//+------------------------------------------------------------------
/**
 *	method:	DataBase::db_get_device_attribute_list
 *
 *	description:	method to execute "DbGetDeviceAttributeList"
 *	return list of attributes for device which match the
 *	wildcard
 *
 * @param	argin	device name
 * @return	attribute wildcard
 *
 */
//+------------------------------------------------------------------
Tango::DevVarStringArray *DataBase::db_get_device_attribute_list(const Tango::DevVarStringArray *device_wildcard)
{
	static char sql_query[256];
	static char error_mess[256];
	string tmp_wildcard;
	MYSQL_RES *result;
	MYSQL_ROW row;
	int n_rows, i;
	const char *device, *wildcard;

	device = (*device_wildcard)[0];
	wildcard = (*device_wildcard)[1];
	INFO_STREAM << "DataBase::db_get_device_attribute_list(): device " << device;
	WARN_STREAM << " wildcard " << wildcard << endl;

	if (wildcard == NULL)
	{
		sprintf(sql_query,"SELECT DISTINCT attribute FROM property_attribute_device WHERE device=\"%s\" AND attribute LIKE \"%\" ORDER BY attribute",device);
	}
	else
	{
		tmp_wildcard = replace_wildcard (wildcard);
		sprintf(sql_query,"SELECT DISTINCT attribute FROM property_attribute_device WHERE device=\"%s\" AND attribute LIKE \"%s\" ORDER BY attribute",
	        device,tmp_wildcard.c_str());
	}
	DEBUG_STREAM << "DataBase::db_get_device_attrribute_list(): sql_query " << sql_query << endl;

	if (mysql_real_query(&mysql, sql_query, strlen(sql_query)) != 0)
	{
	   WARN_STREAM << "DataBase::db_get_device_attribute_list(): failed to query TANGO database for list of properties";
	   WARN_STREAM << device << " (error=" << mysql_error(&mysql) << ")" << endl;
	   sprintf(error_mess,"failed to query TANGO database for list of properties (error=%s)",mysql_error(&mysql));
	   Tango::Except::throw_exception((const char *)DB_SQLError,
	   				  (const char *)error_mess,
					  (const char *)"DataBase::db_get_device_attribute_list()");
	}

	if ((result = mysql_store_result(&mysql)) == NULL)
	{
	   WARN_STREAM << "DataBase::db_get_device_attribute_list(): mysql_store_result() failed  (error=" << mysql_error(&mysql) << ")" << endl;
	   sprintf(error_mess,"mysql_store_result() failed (error=%s)",mysql_error(&mysql));
	   Tango::Except::throw_exception((const char *)DB_SQLError,
	   				  (const char *)error_mess,
					  (const char *)"DataBase::db_get_device_attribute_list()");
	}

	n_rows = mysql_num_rows(result);
	DEBUG_STREAM << "DataBase::db_get_device_attribute_list(): mysql_num_rows() " << n_rows << endl;
	Tango::DevVarStringArray *attribute_list = new Tango::DevVarStringArray;

	if (n_rows > 0)
	{
	   attribute_list->length(n_rows);

	   for (i=0; i<n_rows; i++)
	   {
	      if ((row = mysql_fetch_row(result)) != NULL)
	      {
	         DEBUG_STREAM << "DataBase::db_get_device_attribute_list(): attribute[ "<< i << "] attribute " << row[0] << endl;
	         (*attribute_list)[i]   = CORBA::string_dup(row[0]);
	      }
	   }
	}
	else
		attribute_list->length(0);
	mysql_free_result(result);

	return(attribute_list);
}

//+------------------------------------------------------------------
/**
 *	method:	DataBase::db_delete_server
 *
 *	description:	method to execute "DbDeleteServer"
 *	delete server from the database, do not delete device properties
 *
 * @param	argin	server name
 *
 */
//+------------------------------------------------------------------
void DataBase::db_delete_server(Tango::DevString server)
{
	static char sql_query[512];
	static char error_mess[256];

	INFO_STREAM << "DataBase::db_delete_server(): delete server " << server << " from database" << endl;

// first check the server name that it has no wildcard '*' and
// that it has at least one slash in it

	string tmp_server = string(server);

	if ((tmp_server.find('*') != string::npos) || 
	    (tmp_server.find('%') != string::npos) ||
	    (tmp_server.find('/') == string::npos))
	{
         	WARN_STREAM << "DataBase::db_delete_server(): server name  " << server << "incorrect ";
         	WARN_STREAM << endl;
         	sprintf(error_mess,"failed to delete server, server name incorrect %s",server);
         	Tango::Except::throw_exception((const char *)DB_IncorrectServerName,
					       (const char *)error_mess,
					       (const char *)"DataBase::db_delete_server()");
	}

	tmp_server = replace_wildcard(server);

// then delete the device from the device table

      sprintf(sql_query,"DELETE FROM device WHERE server LIKE \"%s\"",
              tmp_server.c_str());
      DEBUG_STREAM << "DataBase::db_delete_server(): sql_query " << sql_query << endl;
      if (mysql_real_query(&mysql, sql_query, strlen(sql_query)) != 0)
      {
         WARN_STREAM << "DataBase::db_delete_server(): failed to delete server from TANGO database ";
         WARN_STREAM << "for " << server << " (error=" << mysql_error(&mysql) << ")" << endl;
         sprintf(error_mess,"failed to delete server from TANGO database  for %s (error=%s)",server,mysql_error(&mysql));
         Tango::Except::throw_exception((const char *)DB_SQLError,
	 				(const char *)error_mess,
					(const char *)"DataBase::db_delete_server()");
      }

}

//+------------------------------------------------------------------
/**
 *	method:	DataBase::db_delete_device
 *
 *	description:	method to execute "DbDeleteDevice"
 *
 * @param	argin	device name
 *
 */
//+------------------------------------------------------------------
void DataBase::db_delete_device(Tango::DevString device)
{
	static char sql_query[512];
	static char error_mess[256];
	string tmp_device;

	INFO_STREAM << "DataBase::db_delete_device(): delete " << device << " from database" << endl;

// first check the device name

	tmp_device = device;
	if (!check_device_name(tmp_device))
	{
         	WARN_STREAM << "DataBase::db_delete_device(): device name  " << device << " incorrect ";
         	WARN_STREAM << endl;
         	sprintf(error_mess,"failed to delete device, device name incorrect %s",tmp_device.c_str());
         	Tango::Except::throw_exception((const char *)DB_IncorrectDeviceName,
					       (const char *)error_mess,
					       (const char *)"DataBase::db_delete_device()");
	}

// replace database wildcards (% and _)

	string tmp_wildcard = replace_wildcard(tmp_device.c_str());

// then delete the device from the device table

      sprintf(sql_query,"DELETE FROM device WHERE name LIKE \"%s\"",
              tmp_wildcard.c_str());
      DEBUG_STREAM << "DataBase::db_delete_device(): sql_query " << sql_query << endl;
      if (mysql_real_query(&mysql, sql_query, strlen(sql_query)) != 0)
      {
         WARN_STREAM << "DataBase::db_delete_device(): failed to delete property from TANGO database ";
         WARN_STREAM << "for " << device << " (error=" << mysql_error(&mysql) << ")" << endl;
         sprintf(error_mess,"failed to delete property from TANGO database  for %s (error=%s)",device,mysql_error(&mysql));
         Tango::Except::throw_exception((const char *)DB_SQLError,
	 				(const char *)error_mess,
					(const char *)"DataBase::db_delete_device()");
      }

// then delete device from the property_device table

      sprintf(sql_query,"DELETE FROM property_device WHERE device LIKE \"%s\"",
              tmp_wildcard.c_str());
      DEBUG_STREAM << "DataBase::db_delete_device(): sql_query " << sql_query << endl;
      if (mysql_real_query(&mysql, sql_query, strlen(sql_query)) != 0)
      {
         WARN_STREAM << "DataBase::db_delete_device(): failed to delete property from TANGO database ";
         WARN_STREAM << "for " << device << " (error=" << mysql_error(&mysql) << ")" << endl;
         sprintf(error_mess,"failed to delete property from TANGO database  for %s (error=%s)",device,mysql_error(&mysql));
         Tango::Except::throw_exception((const char *)DB_SQLError,
	 				(const char *)error_mess,
					(const char *)"DataBase::db_delete_device()");
      }

// then delete device from the property_attribute_device table

      sprintf(sql_query,"DELETE FROM property_attribute_device WHERE device LIKE \"%s\"",
              tmp_wildcard.c_str());
      DEBUG_STREAM << "DataBase::db_delete_device(): sql_query " << sql_query << endl;
      if (mysql_real_query(&mysql, sql_query, strlen(sql_query)) != 0)
      {
         WARN_STREAM << "DataBase::db_delete_device(): failed to delete property from TANGO database ";
         WARN_STREAM << "for " << device << " (error=" << mysql_error(&mysql) << ")" << endl;
         sprintf(error_mess,"failed to delete property from TANGO database  for %s (error=%s)",device,mysql_error(&mysql));
         Tango::Except::throw_exception((const char *)DB_SQLError,
	 				(const char *)error_mess,
					(const char *)"DataBase::db_delete_device()");
      }

      return;
}

//+------------------------------------------------------------------
/**
 *	method:	DataBase::db_delete_device_attribute
 *
 *	description:	method to execute "DbDeleteDeviceAttribute"
 *	delete a device attribute and all its properties from
 *	the database
 *
 * @param	argin	device
 *
 */
//+------------------------------------------------------------------
void DataBase::db_delete_device_attribute(const Tango::DevVarStringArray *argin)
{
	const char *attribute;
	string tmp_device;
	static char sql_query[512];
	static char error_mess[256];

	if (argin->length() < 2) {
   		WARN_STREAM << "DataBase::db_delete_device_attribute(): insufficient number of arguments ";
   		WARN_STREAM << endl;
   		sprintf(error_mess,"insufficient number of arguments to delete device attribute");
   		Tango::Except::throw_exception((const char *)DB_IncorrectArguments,
					       (const char *)error_mess,
					       (const char *)"DataBase::db_delete_device_attribute()");
	}

	tmp_device = (*argin)[0].in();
	attribute = (*argin)[1];

	INFO_STREAM << "DataBase::db_delete_device(): delete " << tmp_device << " from database" << endl;

// first check the device name

	if (!check_device_name(tmp_device))
	{
         	WARN_STREAM << "DataBase::db_delete_device_attribute(): device name  " << tmp_device << " incorrect ";
         	WARN_STREAM << endl;
         	sprintf(error_mess,"failed to delete device attribute, device name incorrect %s",tmp_device.c_str());
         	Tango::Except::throw_exception((const char *)DB_IncorrectDeviceName,
					       (const char *)error_mess,
					       (const char *)"DataBase::db_delete_device_attribute()");
	}

// replace database wildcards (% and _)

	string tmp_wildcard = replace_wildcard(tmp_device.c_str());

// then delete device from the property_attribute_device table

      sprintf(sql_query,"DELETE FROM property_attribute_device WHERE device LIKE \"%s\" AND attribute LIKE \"%s\" ",
              tmp_wildcard.c_str(),attribute);
      DEBUG_STREAM << "DataBase::db_delete_device_attribute(): sql_query " << sql_query << endl;
      if (mysql_real_query(&mysql, sql_query, strlen(sql_query)) != 0)
      {
         WARN_STREAM << "DataBase::db_delete_device_attribute(): failed to delete attribute from TANGO database ";
         WARN_STREAM << "for " << tmp_device << " (error=" << mysql_error(&mysql) << ")" << endl;
         sprintf(error_mess,"failed to delete attribute from TANGO database  for %s (error=%s)",tmp_device.c_str(),mysql_error(&mysql));
         Tango::Except::throw_exception((const char *)DB_SQLError,
	 				(const char *)error_mess,
					(const char *)"DataBase::db_delete_device_attribute()");
      }

      return;

}

//+------------------------------------------------------------------
/**
 *	method:	DataBase::db_delete_class_attribute
 *
 *	description:	method to execute "DbDeleteClassAttribute"
 *	delete a class attribute and all its properties from
 *	the database
 *
 * @param	argin	device
 *
 */
//+------------------------------------------------------------------
void DataBase::db_delete_class_attribute(const Tango::DevVarStringArray *argin)
{
	const char *attribute;
	string tmp_class;
	static char sql_query[512];
	static char error_mess[256];

	if (argin->length() < 2) {
   		WARN_STREAM << "DataBase::db_delete_class_attribute(): insufficient number of arguments ";
   		WARN_STREAM << endl;
   		sprintf(error_mess,"insufficient number of arguments to delete class attribute");
   		Tango::Except::throw_exception((const char *)DB_IncorrectArguments,
					       (const char *)error_mess,
					       (const char *)"DataBase::db_delete_class_attribute()");
	}

	tmp_class = (*argin)[0];
	attribute = (*argin)[1];

	INFO_STREAM << "DataBase::db_delete_class_attribute(): delete " << tmp_class << " from database" << endl;

// then delete class from the property_attribute_class table

      sprintf(sql_query,"DELETE FROM property_attribute_class WHERE class LIKE \"%s\" AND attribute LIKE \"%s\" ",
              tmp_class.c_str(),attribute);
      DEBUG_STREAM << "DataBase::db_delete_class_attribute(): sql_query " << sql_query << endl;
      if (mysql_real_query(&mysql, sql_query, strlen(sql_query)) != 0)
      {
         WARN_STREAM << "DataBase::db_delete_class_attribute(): failed to delete attribute from TANGO database ";
         WARN_STREAM << "for " << tmp_class << " (error=" << mysql_error(&mysql) << ")" << endl;
         sprintf(error_mess,"failed to delete attribute from TANGO database  for %s (error=%s)",tmp_class.c_str(),mysql_error(&mysql));
         Tango::Except::throw_exception((const char *)DB_SQLError,
	 				(const char *)error_mess,
					(const char *)"DataBase::db_delete_class_attribute()");
      }

      return;

}

//+------------------------------------------------------------------
/**
 *	method:	DataBase::db_delete_device_attribute_property
 *
 *	description:	method to execute "DbDeleteDeviceAttributeProperty"
 *	delete a device attribute property from the database
 *
 * @param	argin	device
 *
 */
//+------------------------------------------------------------------
void DataBase::db_delete_device_attribute_property(const Tango::DevVarStringArray *argin)
{
	const char *attribute, *property;
	string tmp_device;
	static char sql_query[512];
	static char error_mess[256];
	int i;

	if (argin->length() < 3) {
   		WARN_STREAM << "DataBase::db_delete_device_attribute_property(): insufficient number of arguments ";
   		WARN_STREAM << endl;
   		sprintf(error_mess,"insufficient number of arguments to delete device attribute property");
   		Tango::Except::throw_exception((const char *)DB_IncorrectArguments,
					       (const char *)error_mess,
					       (const char *)"DataBase::db_delete_device_attribute_property()");
	}

	tmp_device = (*argin)[0];
	if (!check_device_name(tmp_device))
	{
        	WARN_STREAM << "DataBase::db_delete_device_attribute(): device name  " << tmp_device << " incorrect ";
         	WARN_STREAM << endl;
         	sprintf(error_mess,"failed to delete device attribute, device name incorrect %s",tmp_device.c_str());
         	Tango::Except::throw_exception((const char *)DB_IncorrectDeviceName,
				       	(const char *)error_mess,
				       	(const char *)"DataBase::db_delete_device_attribute()");
	}
	string tmp_wildcard = replace_wildcard(tmp_device.c_str());
	attribute = (*argin)[1];
	for (i=0; i<argin->length()-2; i++)
	{
		property = (*argin)[i+2];

		INFO_STREAM << "DataBase::db_delete_device_attribute_property(): delete device " << tmp_device ;
		INFO_STREAM << " attribute " << attribute << " property[" << i <<"] " << property << " from database" << endl;


// then delete property from the property_attribute_device table

      		sprintf(sql_query,"DELETE FROM property_attribute_device WHERE device = \"%s\" AND attribute = \"%s\" AND name = \"%s\" ",
              		tmp_wildcard.c_str(),attribute,property);
      		DEBUG_STREAM << "DataBase::db_delete_device_attribute_property(): sql_query " << sql_query << endl;
      		if (mysql_real_query(&mysql, sql_query, strlen(sql_query)) != 0)
      		{
         	WARN_STREAM << "DataBase::db_delete_device_attribute_property(): failed to delete attribute property from TANGO database ";
         	WARN_STREAM << "for " << tmp_device << " (error=" << mysql_error(&mysql) << ")" << endl;
         	sprintf(error_mess,"failed to delete attribute property from TANGO database  for %s (error=%s)",tmp_device.c_str(),mysql_error(&mysql));
         	Tango::Except::throw_exception((const char *)DB_SQLError,
	 					(const char *)error_mess,
						(const char *)"DataBase::db_delete_device_attribute_property()");
      		}
	}

      	return;
}

//+------------------------------------------------------------------
/**
 *	method:	DataBase::db_delete_class_attribute_property
 *
 *	description:	method to execute "DbDeleteClassAttributeProperty"
 *	delete a class attribute property from the database
 *
 * @param	argin	device
 *
 */
//+------------------------------------------------------------------
void DataBase::db_delete_class_attribute_property(const Tango::DevVarStringArray *argin)
{
	const char *attribute, *property;
	string tmp_class;
	static char sql_query[512];
	static char error_mess[256];
	int i;

	if (argin->length() < 3) {
   		WARN_STREAM << "DataBase::db_delete_class_attribute_property(): insufficient number of arguments ";
   		WARN_STREAM << endl;
   		sprintf(error_mess,"insufficient number of arguments to delete class attribute property");
   		Tango::Except::throw_exception((const char *)DB_IncorrectArguments,
					       (const char *)error_mess,
					       (const char *)"DataBase::db_delete_class_attribute_property()");
	}

	tmp_class = (*argin)[0];
	attribute = (*argin)[1];
	for (i=0; i<argin->length()-2; i++)
	{
		property = (*argin)[i+2];

		INFO_STREAM << "DataBase::db_delete_class_attribute_property(): delete class " << tmp_class ;
		INFO_STREAM << " attribute " << attribute << " property[" << i <<"] " << property << " from database" << endl;


// then delete property from the property_attribute_class table

      		sprintf(sql_query,"DELETE FROM property_attribute_class WHERE class = \"%s\" AND attribute = \"%s\" AND name = \"%s\" ",
              		tmp_class.c_str(),attribute,property);
      		DEBUG_STREAM << "DataBase::db_delete_class_attribute_property(): sql_query " << sql_query << endl;
      		if (mysql_real_query(&mysql, sql_query, strlen(sql_query)) != 0)
      		{
         	WARN_STREAM << "DataBase::db_delete_class_attribute_property(): failed to delete attribute property from TANGO database ";
         	WARN_STREAM << "for " << tmp_class << " (error=" << mysql_error(&mysql) << ")" << endl;
         	sprintf(error_mess,"failed to delete attribute property from TANGO database  for %s (error=%s)",tmp_class.c_str(),mysql_error(&mysql));
         	Tango::Except::throw_exception((const char *)DB_SQLError,
	 					(const char *)error_mess,
						(const char *)"DataBase::db_delete_class_attribute_property()");
      		}
	}

      	return;
}
//+------------------------------------------------------------------
/**
 *	method:	DataBase::db_get_host_servers_info
 *
 *	description:	method to execute "DbGetHostServersInfo"
 *	return info about all servers running on specified host, name, mode and level
 *
 * @param	argin	host name
 * @return	server info for all servers running on specified host
 *
 */
//+------------------------------------------------------------------
Tango::DevVarStringArray *DataBase::db_get_host_servers_info(Tango::DevString argin)
{
	//- struct timeval	t0, t;
	//- gettimeofday(&t0, NULL);
	INFO_STREAM << "DataBase::db_get_host_servers_info(): entering... !" << endl;
	//	Get server list
	Tango::DevVarStringArray	*servers = db_get_host_server_list(argin);
	Tango::DevVarStringArray	*argout  = new Tango::DevVarStringArray();
	argout->length(servers->length()*3);
	int		idx = 0;
	for (int i=0 ; i<servers->length() ; i++)
	{
		//	Get info for each server
		Tango::DevVarStringArray	*info = dev_get_server_info((*servers)[i]);
		(*argout)[idx++] = CORBA::string_dup((*servers)[i]);//	Server name
		(*argout)[idx++] = CORBA::string_dup((*info)[2]);	//	Controlled ?
		(*argout)[idx++] = CORBA::string_dup((*info)[3]);	//	Startup level
		delete info;
	}
	delete servers;

	//	Check execution duration
	//- gettimeofday(&t, NULL);
	//- WARN_STREAM << argin << "; " << 1000.0*(t.tv_sec - t0.tv_sec) + 
	//-	((double)t.tv_usec - t0.tv_usec) / 1000 << " ms" << endl;
	return argout;
}
//+------------------------------------------------------------------
/**
 *	method:	DataBase::dev_get_server_info
 *
 *	description:	method to execute "DbGetServerInfo"
 *	return info about host, mode and level for specified server
 *
 * @param	argin	server name
 * @return	server info
 *
 */
//+------------------------------------------------------------------
Tango::DevVarStringArray *DataBase::dev_get_server_info(Tango::DevString server_name)
{
	static char sql_query[256];
	static char error_mess[256];
	MYSQL_RES *result;
	MYSQL_ROW row;
	int n_rows=0;
	Tango::DevVarStringArray *server_info = new Tango::DevVarStringArray;
	string tmp_name;

	INFO_STREAM << "DataBase::db_get_server_info(): server " << server_name << endl;

	server_info->length(4);
	(*server_info)[0] = CORBA::string_dup(server_name);

//	tmp_name = replace_wildcard(server_name);
	sprintf(sql_query,"SELECT host,mode,level FROM server WHERE name = '%s';",
	        server_name);
	DEBUG_STREAM << "DataBase::db_get_server_info(): sql_query " << sql_query << endl;

	if (mysql_real_query(&mysql, sql_query, strlen(sql_query)) != 0)
	{
	   delete server_info;
	   WARN_STREAM << "DataBase::db_get_server_info(): failed to query TANGO database for server info";
	   WARN_STREAM << "for " << server_name << "/" << server_name << " (error=" << mysql_error(&mysql) << ")" << endl;
	   sprintf(error_mess,"failed to query TANGO database for server info for %s (error=%s)",tmp_name.c_str(),mysql_error(&mysql));
	   Tango::Except::throw_exception((const char *)DB_SQLError,
	   				  (const char *)error_mess,
					  (const char *)"DataBase::db_get_server_info()");
	}

	if ((result = mysql_store_result(&mysql)) == NULL)
	{
	   delete server_info;
	   WARN_STREAM << "DataBase::db_get_server_info(): mysql_store_result() failed  (error=" << mysql_error(&mysql) << ")" << endl;
	   sprintf(error_mess,"mysql_store_result() failed (error=%s)",mysql_error(&mysql));
	   Tango::Except::throw_exception((const char *)DB_SQLError,
	   				  (const char *)error_mess,
					  (const char *)"DataBase::db_get_server_info()");
	}

	n_rows = mysql_num_rows(result);
	DEBUG_STREAM << "DataBase::db_get_server_info(): mysql_num_rows() " << n_rows << endl;
	if (n_rows > 0)
	{
	   if ((row = mysql_fetch_row(result)) != NULL)
	   {
	      DEBUG_STREAM << "DataBase::db_get_server_info(): host "<< row[0] << " mode " << row[1] << " level " << row[2] << endl;
	      (*server_info)[1] = CORBA::string_dup(row[0]);
	      (*server_info)[2] = CORBA::string_dup(row[1]);
	      (*server_info)[3] = CORBA::string_dup(row[2]);
	   }
	}
	else
	{
	   (*server_info)[1] = CORBA::string_dup(" ");
	   (*server_info)[2] = CORBA::string_dup(" ");
	   (*server_info)[3] = CORBA::string_dup(" ");
	}
	mysql_free_result(result);

	return server_info;
}

//+------------------------------------------------------------------
/**
 *	method:	DataBase::db_put_server_info
 *
 *	description:	method to execute "DbPutServerInfo"
 *	update server info including host, mode and level
 *
 * @param	argin	server info
 *
 */
//+------------------------------------------------------------------
void DataBase::db_put_server_info(const Tango::DevVarStringArray *server_info)
{
	static char sql_query[4096];
	static char error_mess[256];
	long n_rows=0, i;
	const char *tmp_host, *tmp_mode, *tmp_level;
	string tmp_server;

	if (server_info->length() < 4) {
   		WARN_STREAM << "DataBase::db_put_server_info(): insufficient info for server ";
   		WARN_STREAM << endl;
   		sprintf(error_mess,"insufficient server info");
   		Tango::Except::throw_exception((const char *)DB_IncorrectArguments,
					       (const char *)error_mess,
					       (const char *)"DataBase::db_put_server_info()");
	}
	
	INFO_STREAM << "DataBase::db_put_server_info(): put " << server_info->length()-1 << " export info for device " << (*server_info)[0] << endl;

	tmp_server = (*server_info)[0];
// replace database wildcards (% and _)
//	string tmp_wildcard = replace_wildcard(tmp_server.c_str());
// replace uppercase by lowercase
	for (i=0; i<tmp_server.length(); i++) { 
		tmp_server[i] = tolower(tmp_server[i]);
	}
	tmp_host = (*server_info)[1];
	tmp_mode = (*server_info)[2];
	tmp_level = (*server_info)[3];

// first delete the server from the server table

      sprintf(sql_query,"DELETE FROM server WHERE name = \"%s\"",
              tmp_server.c_str());
      DEBUG_STREAM << "DataBase::db_put_server_info(): sql_query " << sql_query << endl;
      if (mysql_real_query(&mysql, sql_query, strlen(sql_query)) != 0)
      {
         WARN_STREAM << "DataBase::db_put_server_info(): failed to delete server from TANGO database ";
         WARN_STREAM << "for " << tmp_server << " (error=" << mysql_error(&mysql) << ")" << endl;
         sprintf(error_mess,"failed to delete server from TANGO database  for %s (error=%s)",tmp_server.c_str(),mysql_error(&mysql));
         Tango::Except::throw_exception((const char *)DB_SQLError,
	 				(const char *)error_mess,
					(const char *)"DataBase::db_put_server_info()");
      }

// insert the new info for this server

	sprintf(sql_query,"INSERT INTO server SET name=\'%s\',host=\'%s\',mode=\'%s\',level=\'%s\'",
        	tmp_server.c_str(),tmp_host, tmp_mode, tmp_level);
	DEBUG_STREAM << "DataBase::db_put_server_info(): sql_query " << sql_query << endl;

	if (mysql_real_query(&mysql, sql_query, strlen(sql_query)) != 0)
	{
   		WARN_STREAM << "DataBase::db_put_server_info(): failed to insert server info into TANGO database ";
   		WARN_STREAM << "for " << tmp_server << " (error=" << mysql_error(&mysql) << ")" << endl;
   		sprintf(error_mess,"failed to insert server info into TANGO database for %s",tmp_server.c_str());
   		Tango::Except::throw_exception((const char *)DB_SQLError,
					       (const char *)error_mess,
					       (const char *)"DataBase::db_put_server_info()");
	}

	//	Update host's starter to update controlled servers list
	if (fireToStarter==true)
	{
		vector<string>	hosts;
		hosts.push_back(tmp_host);
		starter_shared->send_starter_cmd(hosts);
	}
	return;

}

//+------------------------------------------------------------------
/**
 *	method:	DataBase::db_delete_server_info
 *
 *	description:	method to execute "DbDeleteServerInfo"
 *	delete info related to a server
 *
 * @param	argin	server name
 *
 */
//+------------------------------------------------------------------
void DataBase::db_delete_server_info(Tango::DevString server_name)
{
	static char sql_query[512];
	static char error_mess[256];

	INFO_STREAM << "DataBase::db_delete_server_info(): delete " << server_name << " from database" << endl;

// replace database wildcards (% and _)

//	string tmp_wildcard = replace_wildcard(server_name);

// then delete the device from the device table

      sprintf(sql_query,"DELETE FROM server WHERE name = \"%s\"",
              server_name);
      DEBUG_STREAM << "DataBase::db_delete_server_info(): sql_query " << sql_query << endl;
      if (mysql_real_query(&mysql, sql_query, strlen(sql_query)) != 0)
      {
         WARN_STREAM << "DataBase::db_delete_server_info(): failed to delete property from TANGO database ";
         WARN_STREAM << "for " << server_name << " (error=" << mysql_error(&mysql) << ")" << endl;
         sprintf(error_mess,"failed to delete property from TANGO database  for %s (error=%s)",server_name,mysql_error(&mysql));
         Tango::Except::throw_exception((const char *)DB_SQLError,
	 				(const char *)error_mess,
					(const char *)"DataBase::db_delete_server_info()");
      }

      return;

}

//+------------------------------------------------------------------
/**
 *	method:	DataBase::db_get_device_exported_list
 *
 *	description:	method to execute "DbGetDeviceExportedList"
 *	Get a list of exported devices whose names satisfy the filter (wildcard is
 *
 * @param	argin	filter
 * @return	list of exported devices
 *
 */
//+------------------------------------------------------------------
Tango::DevVarStringArray *DataBase::db_get_device_exported_list(Tango::DevString filter)
{
	static char sql_query[256];
	static char error_mess[256];
	string tmp_filter;
	MYSQL_RES *result;
	MYSQL_ROW row;
	int n_rows, i;

	INFO_STREAM << "DataBase::db_get_device_exported_list(): filter " << filter << endl;

	if (filter == NULL)
	{
		sprintf(sql_query,"SELECT DISTINCT name FROM device WHERE name LIKE \"%\" AND exported=1 ORDER BY name");
	}
	else
	{
		tmp_filter = replace_wildcard(filter);
		sprintf(sql_query,"SELECT DISTINCT name FROM device WHERE (name LIKE \"%s\" OR alias LIKE \"%s\") AND exported=1 ORDER BY name",
	        tmp_filter.c_str(),tmp_filter.c_str());
	}
	DEBUG_STREAM << "DataBase::db_get_device_exported_list(): sql_query " << sql_query << endl;

	if (mysql_real_query(&mysql, sql_query, strlen(sql_query)) != 0)
	{
	   WARN_STREAM << "DataBase::db_get_device_exported_list(): failed to query TANGO database for list of exported devices";
	   WARN_STREAM << filter << " (error=" << mysql_error(&mysql) << ")" << endl;
	   sprintf(error_mess,"failed to query TANGO database for list of exported devices (error=%s)",mysql_error(&mysql));
	   Tango::Except::throw_exception((const char *)DB_SQLError,
	   				  (const char *)error_mess,
					  (const char *)"DataBase::db_get_device_exported_list()");
	}

	if ((result = mysql_store_result(&mysql)) == NULL)
	{
	   WARN_STREAM << "DataBase::db_get_device_exported_list(): mysql_store_result() failed  (error=" << mysql_error(&mysql) << ")" << endl;
	   sprintf(error_mess,"mysql_store_result() failed (error=%s)",mysql_error(&mysql));
	   Tango::Except::throw_exception((const char *)DB_SQLError,
	   				  (const char *)error_mess,
					  (const char *)"DataBase::db_get_device_exported_list()");
	}

	n_rows = mysql_num_rows(result);
	DEBUG_STREAM << "DataBase::db_get_device_exported_list(): mysql_num_rows() " << n_rows << endl;
	Tango::DevVarStringArray *device_list = new Tango::DevVarStringArray;

	if (n_rows > 0)
	{
	   device_list->length(n_rows);

	   for (i=0; i<n_rows; i++)
	   {
	      if ((row = mysql_fetch_row(result)) != NULL)
	      {
	         DEBUG_STREAM << "DataBase::db_get_device_exported_list(): device_list[ "<< i << "] alias " << row[0] << endl;
	         (*device_list)[i]   = CORBA::string_dup(row[0]);
	      }
	   }
	}
	else
		device_list->length(0);
	mysql_free_result(result);

	return(device_list);
}

//+------------------------------------------------------------------
/**
 *	method:	DataBase::db_get_exportd_device_list_for_class
 *
 *	description:	method to execute "DbGetExportdDeviceListForClass"
 *	Query the database for device exported for the specified class.
 *
 * @param	argin	Class name
 * @return	Device exported list
 *
 */
//+------------------------------------------------------------------
Tango::DevVarStringArray *DataBase::db_get_exportd_device_list_for_class(Tango::DevString classname)
{
	static char sql_query[256];
	static char error_mess[256];
	string tmp_classname;
	MYSQL_RES *result;
	MYSQL_ROW row;
	int n_rows, i;

	INFO_STREAM << "DataBase::db_get_device_exported_list(): classname " << classname << endl;

	if (classname == NULL)
	{
		sprintf(sql_query,"SELECT DISTINCT name FROM device WHERE class LIKE \"%\" AND exported=1 ORDER BY name");
	}
	else
	{
		tmp_classname = replace_wildcard(classname);
		sprintf(sql_query,"SELECT DISTINCT name FROM device WHERE class LIKE \"%s\" AND exported=1 ORDER BY name",
	        tmp_classname.c_str(),tmp_classname.c_str());
	}
	DEBUG_STREAM << "DataBase::db_get_device_exported_list(): sql_query " << sql_query << endl;

	if (mysql_real_query(&mysql, sql_query, strlen(sql_query)) != 0)
	{
	   WARN_STREAM << "DataBase::db_get_device_exported_list(): failed to query TANGO database for list of exported devices";
	   WARN_STREAM << classname << " (error=" << mysql_error(&mysql) << ")" << endl;
	   sprintf(error_mess,"failed to query TANGO database for list of exported devices (error=%s)",mysql_error(&mysql));
	   Tango::Except::throw_exception((const char *)DB_SQLError,
	   				  (const char *)error_mess,
					  (const char *)"DataBase::db_get_device_exported_list()");
	}

	if ((result = mysql_store_result(&mysql)) == NULL)
	{
	   WARN_STREAM << "DataBase::db_get_device_exported_list(): mysql_store_result() failed  (error=" << mysql_error(&mysql) << ")" << endl;
	   sprintf(error_mess,"mysql_store_result() failed (error=%s)",mysql_error(&mysql));
	   Tango::Except::throw_exception((const char *)DB_SQLError,
	   				  (const char *)error_mess,
					  (const char *)"DataBase::db_get_device_exported_list()");
	}

	n_rows = mysql_num_rows(result);
	DEBUG_STREAM << "DataBase::db_get_device_exported_list(): mysql_num_rows() " << n_rows << endl;
	Tango::DevVarStringArray *device_list = new Tango::DevVarStringArray;

	if (n_rows > 0)
	{
	   device_list->length(n_rows);

	   for (i=0; i<n_rows; i++)
	   {
	      if ((row = mysql_fetch_row(result)) != NULL)
	      {
	         DEBUG_STREAM << "DataBase::db_get_device_exported_list(): device_list[ "<< i << "] alias " << row[0] << endl;
	         (*device_list)[i]   = CORBA::string_dup(row[0]);
	      }
	   }
	}
	else
		device_list->length(0);
	mysql_free_result(result);

	return(device_list);
}

//+------------------------------------------------------------------
/**
 *	method:	DataBase::db_get_device_server_class_list
 *
 *	description:	method to execute "DbGetDeviceServerClassList"
 *	return list of device classes for a device server
 *
 * @param	argin	device server instance name
 * @return	list of classes for this device server
 *
 */
//+------------------------------------------------------------------
Tango::DevVarStringArray *DataBase::db_get_device_server_class_list(Tango::DevString server)
{
	static char sql_query[256];
	static char error_mess[256];
	MYSQL_RES *result;
	MYSQL_ROW row;
	int n_rows, i;
	string tmp_server;

	INFO_STREAM << "DataBase::db_get_device_server_class_list(): server " << server << endl;

	tmp_server = replace_wildcard(server);
	sprintf(sql_query,"SELECT DISTINCT class FROM device WHERE server LIKE \"%s\" ORDER BY class",
	        tmp_server.c_str());
	DEBUG_STREAM << "DataBase::db_get_device_server_class_list(): sql_query " << sql_query << endl;

	if (mysql_real_query(&mysql, sql_query, strlen(sql_query)) != 0)
	{
	   WARN_STREAM << "DataBase::db_get_device_server_class_list(): failed to query TANGO database for list of classes for server ";
	   WARN_STREAM << server << " (error=" << mysql_error(&mysql) << ")" << endl;
	   sprintf(error_mess,"failed to query TANGO database for list of classes for server (error=%s)",mysql_error(&mysql));
	   Tango::Except::throw_exception((const char *)DB_SQLError,
	   				  (const char *)error_mess,
					  (const char *)"DataBase::db_get_device_server_class_list()");
	}

	if ((result = mysql_store_result(&mysql)) == NULL)
	{
	   WARN_STREAM << "DataBase::db_get_device_server_class_list(): mysql_store_result() failed  (error=" << mysql_error(&mysql) << ")" << endl;
	   sprintf(error_mess,"mysql_store_result() failed (error=%s)",mysql_error(&mysql));
	   Tango::Except::throw_exception((const char *)DB_SQLError,
	   				  (const char *)error_mess,
					  (const char *)"DataBase::db_get_device_server_class_list()");
	}

	n_rows = mysql_num_rows(result);
	DEBUG_STREAM << "DataBase::db_get_device_server_class_list(): mysql_num_rows() " << n_rows << endl;
	Tango::DevVarStringArray *class_list = new Tango::DevVarStringArray;

	if (n_rows > 0)
	{
	   class_list->length(n_rows);

	   for (i=0; i<n_rows; i++)
	   {
	      if ((row = mysql_fetch_row(result)) != NULL)
	      {
	         DEBUG_STREAM << "DataBase::db_get_device_server_class_list(): row[ "<< i << "] class " << row[0] << endl;
	         (*class_list)[i]   = CORBA::string_dup(row[0]);
	      }
	   }
	}
	else
		class_list->length(0);
	mysql_free_result(result);

	return(class_list);
}

//+------------------------------------------------------------------
/**
 *	method:	DataBase::db_export_event
 *
 *	description:	method to execute "DbExportEvent"
 *
 * @param	argin	event channel or factory
 *
 */
//+------------------------------------------------------------------
void DataBase::db_export_event(const Tango::DevVarStringArray *export_info)
{
	static char sql_query[4096];
	static char error_mess[256];
	long n_rows=0, i;
	const char *tmp_ior, *tmp_host, *tmp_pid, *tmp_version;
	string tmp_event, tmp_server;

	if (export_info->length() < 5) {
   		WARN_STREAM << "DataBase::db_export_event(): insufficient export info for event ";
   		sprintf(error_mess,"insufficient export info for event");
   		Tango::Except::throw_exception((const char *)DB_IncorrectArguments,
					       (const char *)error_mess,
					       (const char *)"DataBase::db_export_event()");
	}
	
	INFO_STREAM << "DataBase::db_export_event(): put " << export_info->length()-1 << " export info for event " << (*export_info)[0] << endl;

	tmp_event = (*export_info)[0];
	for (i=0; i<tmp_event.length(); i++) { 
		tmp_event[i] = tolower(tmp_event[i]);
	}
	tmp_ior = (*export_info)[1];
	tmp_host = (*export_info)[2];
	tmp_pid = (*export_info)[3];
	tmp_version = (*export_info)[4];

// first delete existing information from database

	sprintf(sql_query,"DELETE FROM event WHERE name=\"%s\"",
              tmp_event.c_str());
      	DEBUG_STREAM << "DataBase::db_export_event(): sql_query " << sql_query << endl;
      	if (mysql_real_query(&mysql, sql_query, strlen(sql_query)) != 0)
      	{
         	WARN_STREAM << "DataBase::db_export_event(): failed to delete event from TANGO database ";
         	WARN_STREAM << "for " << tmp_event << " (error=" << mysql_error(&mysql) << ")" << endl;
         	sprintf(error_mess,"failed to delete event %s from TANGO database (error=%s)",tmp_event.c_str(),mysql_error(&mysql));
         	Tango::Except::throw_exception((const char *)DB_SQLError,
	 					(const char *)error_mess,
						(const char *)"DataBase::db_export_event()");
      	}

// update the new value for this tuple

	sprintf(sql_query,"INSERT event set name=\'%s\',exported=1,ior=\'%s\',host=\'%s\',server=\'%s\',pid=\'%s\',version=\'%s\',started=NOW();",
        	tmp_event.c_str(), tmp_ior, tmp_host, tmp_event.c_str(), tmp_pid, tmp_version);
	DEBUG_STREAM << "DataBase::export_event(): sql_query " << sql_query << endl;

	if (mysql_real_query(&mysql, sql_query, strlen(sql_query)) != 0)
	{
   		WARN_STREAM << "DataBase::export_event(): failed to export event to TANGO database ";
   		WARN_STREAM << "for " << tmp_event << " (error=" << mysql_error(&mysql) << ")" << endl;
   		sprintf(error_mess,"failed to insert export info into TANGO database for event %s",tmp_event.c_str());
   		Tango::Except::throw_exception((const char *)DB_SQLError,
					       (const char *)error_mess,
					       (const char *)"DataBase::export_event()");
	}

	return;

}

//+------------------------------------------------------------------
/**
 *	method:	DataBase::db_import_event
 *
 *	description:	method to execute "DbImportEvent"
 *
 * @param	argin	name of event channel or factory
 * @return	export information e.g. IOR
 *
 */
//+------------------------------------------------------------------
Tango::DevVarLongStringArray *DataBase::db_import_event(Tango::DevString event_name)
{
	static char sql_query[256];
	static char error_mess[256];
	MYSQL_RES *result;
	MYSQL_ROW row;
	int n_devices=0, n_rows=0, n_svalues=0, n_lvalues=0; 
	int i, exported, pid;
	string tmp_event;

	INFO_STREAM << "DataBase::db_import_event(): get import info for " << event_name << endl;

	tmp_event = event_name;
	for (i=0; i<tmp_event.size(); i++) { 
		tmp_event[i] = tolower(tmp_event[i]);
	}
	tmp_event = replace_wildcard(tmp_event.c_str());

	sprintf(sql_query,"SELECT exported,ior,version,pid,host FROM event WHERE name = '%s';",
	        tmp_event.c_str());
	DEBUG_STREAM << "DataBase::db_import_event(): sql_query " << sql_query << endl;

	if (mysql_real_query(&mysql, sql_query, strlen(sql_query)) != 0)
	{
	   WARN_STREAM << "DataBase::db_import_event(): failed to query TANGO database for import info ";
	   WARN_STREAM << "for event " << tmp_event << " (error=" << mysql_error(&mysql) << ")" << endl;
	   sprintf(error_mess,"failed to query TANGO database for import info of event for %s (error=%s)",tmp_event.c_str(),mysql_error(&mysql));
	   Tango::Except::throw_exception((const char *)DB_SQLError,
	   				  (const char *)error_mess,
					  (const char *)"DataBase::db_import_event()");
	}

	if ((result = mysql_store_result(&mysql)) == NULL)
	{
	   WARN_STREAM << "DataBase::db_import_event(): mysql_store_result() failed  (error=" << mysql_error(&mysql) << ")" << endl;
	   sprintf(error_mess,"mysql_store_result() failed (error=%s)",mysql_error(&mysql));
	   Tango::Except::throw_exception((const char *)DB_SQLError,
	   				  (const char *)error_mess,
					  (const char *)"DataBase::db_import_event()");
	}

	n_rows = mysql_num_rows(result);
	DEBUG_STREAM << "DataBase::db_import_event(): mysql_num_rows() " << n_rows << endl;

	Tango::DevVarLongStringArray *import_info = new Tango::DevVarLongStringArray;
	if (n_rows > 0)
	{

	   if ((row = mysql_fetch_row(result)) != NULL)
	   {
	      DEBUG_STREAM << "DataBase::db_import_event(): device[ "<< i << "] exported " << row[0] << " IOR  " << row[1] << " version " << row[2] << endl;
	      n_svalues = n_svalues+4;
	      (import_info->svalue).length(n_svalues);
	      (import_info->svalue)[n_svalues-4] = CORBA::string_dup(tmp_event.c_str());
	      (import_info->svalue)[n_svalues-3] = CORBA::string_dup(row[1]);
	      (import_info->svalue)[n_svalues-2] = CORBA::string_dup(row[2]);
	      (import_info->svalue)[n_svalues-1] = CORBA::string_dup(row[4]);
	      exported = -1;
	      if (row[0] != NULL) sscanf(row[0],"%d",&exported);
	      n_lvalues++;
	      (import_info->lvalue).length(n_lvalues);
	      (import_info->lvalue)[n_lvalues-1] = exported;
	      pid = -1;
	      if (row[3] != NULL) sscanf(row[3],"%d",&pid);
	      n_lvalues++;
	      (import_info->lvalue).length(n_lvalues);
	      (import_info->lvalue)[n_lvalues-1] = pid;
	   }
	}
	else {
	     INFO_STREAM << "DataBase::db_import_event(): event not defined !" << endl;
	     sprintf(error_mess,"event %s not defined in the database !",tmp_event.c_str());
	     mysql_free_result(result);
		 delete import_info;
	     Tango::Except::throw_exception((const char *)DB_DeviceNotDefined,
	     				    (const char *)error_mess,
					    (const char *)"DataBase::db_import_event()");
	}
	mysql_free_result(result);

	return(import_info);

}

//+------------------------------------------------------------------
/**
 *	method:	DataBase::db_un_export_event
 *
 *	description:	method to execute "DbUnExportEvent"
 *
 * @param	argin	name of event channel or factory to unexport
 *
 */
//+------------------------------------------------------------------
void DataBase::db_un_export_event(Tango::DevString event_name)
{
	static char sql_query[4096];
	static char error_mess[256];
	long n_rows=0, i;
	char *tmp_event;

	INFO_STREAM << "DataBase::un_export_event(): un-export " << event_name << " device " << endl;

	tmp_event = (char*)malloc(strlen(event_name)+1);
	sprintf(tmp_event,"%s",event_name);
	for (i=0; i<strlen(tmp_event); i++) { 
		tmp_event[i] = tolower(tmp_event[i]);
	}

// un-export event from database by seting ior="not exported"

	sprintf(sql_query,"UPDATE event SET exported=0,stopped=NOW() WHERE name like \"%s\"",
	        tmp_event);
	DEBUG_STREAM << "DataBase::un_export_event(): sql_query " << sql_query << endl;
	if (mysql_real_query(&mysql, sql_query, strlen(sql_query)) != 0)
	{
   		WARN_STREAM << "DataBase::un_export_event(): failed to unexport event from TANGO database ";
   		WARN_STREAM << "for " << tmp_event << " (error=" << mysql_error(&mysql) << ")" << endl;
   		sprintf(error_mess,"failed to un-export event %s from TANGO database  (error=%s)",tmp_event,mysql_error(&mysql));
   		Tango::Except::throw_exception((const char *)DB_SQLError,
					       (const char *)error_mess,
					       (const char *)"DataBase::un_export_event()");
	}

	free(tmp_event);
}

//============================================================================
//============================================================================
Tango::DevString DataBase::db_get_device_host(Tango::DevString argin)
{
	static char sql_query[256];
	static char error_mess[256];
	string tmp_argin;
	MYSQL_RES *result;
	MYSQL_ROW row;
	int n_rows;
	
	tmp_argin = replace_wildcard(argin);
	sprintf(sql_query,"SELECT host FROM device WHERE name LIKE \"%s\"", tmp_argin.c_str());

	if (mysql_real_query(&mysql, sql_query, strlen(sql_query)) != 0)
	{
	   WARN_STREAM << "DataBase::db_get_server_host(): failed to query TANGO database";
	   WARN_STREAM << argin << " (error=" << mysql_error(&mysql) << ")" << endl;
	   sprintf(error_mess,"failed to query TANGO database (error=%s)",mysql_error(&mysql));
	   Tango::Except::throw_exception((const char *)DB_SQLError,
	   				  (const char *)error_mess,
					  (const char *)"DataBase::db_get_server_host()");
	}

	if ((result = mysql_store_result(&mysql)) == NULL)
	{
	   WARN_STREAM << "DataBase::db_get_server_host(): mysql_store_result() failed  (error=" << mysql_error(&mysql) << ")" << endl;
	   sprintf(error_mess,"mysql_store_result() failed (error=%s)",mysql_error(&mysql));
	   Tango::Except::throw_exception((const char *)DB_SQLError,
	   				  (const char *)error_mess,
					  (const char *)"DataBase::db_get_server_host()");
	}
	n_rows = mysql_num_rows(result);
	DEBUG_STREAM << "DataBase::db_get_server_host(): mysql_num_rows() " << n_rows << endl;
	Tango::DevString	argout;
	if (n_rows > 0)
	{
		if ((row = mysql_fetch_row(result)) != NULL)
		{
			argout = CORBA::string_dup(row[0]);
		}
	}
	else
	{
		sprintf(error_mess,"No device found for host \'%s\'", argin);
		WARN_STREAM << error_mess << endl;
		Tango::Except::throw_exception((const char *)DB_DeviceNotDefined,
	   				  (const char *)error_mess,
					  (const char *)"DataBase::db_get_server_host()");
	}
	mysql_free_result(result); //C.S. 05-10-2004

	return argout;
}

//+----------------------------------------------------------------------------
//
// method : 		DataBase::escape_string(char *string_c_str)
// 
// description : 	utility method to escape all occurrences	
//			of ' and " with \
//
// in :			const char *string_c_str -  C string to be modified.
//
// out :		string - The result string .
//
//-----------------------------------------------------------------------------

string DataBase::escape_string(const char *string_c_str)
{
	string escaped_string(string_c_str);
	string::size_type index;

	DEBUG_STREAM << "DataBase::escape_string() string in : " << escaped_string << endl;

	//	escape bakckslash
	index = 0;
	while ((index = escaped_string.find('\\',index)) != string::npos)
	{
		//	Check if double backslash already treated by client
		string	s = escaped_string.substr(index+1);
		if (s.find('\\')==0)
			index++;
		else
		{
			//	Check if another escape sequence treated by client
			if (s.find('\"')==0 || s.find('\'')==0)
				index++;
			else
			{
				escaped_string.insert(index, 1, '\\');
				index += 2;
			}
		}
	}

	//	escape "
	index = 0;
	while ((index = escaped_string.find('"',index)) != string::npos)
	{
		//	Check if double quotes already treated by client
		string	s = escaped_string.substr(index-1);
		if (s.find('\\')==0)
			index++;
		else
		{
			escaped_string.insert(index, 1, '\\');
			index += 2;
		}
	}


	//	escape '
	index = 0;
	while ((index = escaped_string.find('\'',index)) != string::npos)
	{
		//	Check if simple quotes already treated by client
		string	s = escaped_string.substr(index-1);
		if (s.find('\\')==0)
			index++;
		else
		{
			escaped_string.insert(index, 1, '\\');
			index += 2;
		}
	}

	DEBUG_STREAM << "DataBase::escaped_string() wildcard out : " << escaped_string << endl;
	return escaped_string;
}



//+------------------------------------------------------------------
/**
 *	method:	DataBase::db_get_device_alias_list
 *
 *	description:	method to execute "DbGetDeviceAliasList"
 *
 * @param	argin	
 * @return	
 *
 */
//+------------------------------------------------------------------
Tango::DevVarStringArray *DataBase::db_get_device_alias_list(Tango::DevString wildcard)
{
	static char sql_query[256];
	static char error_mess[256];
	string tmp_wildcard;
	MYSQL_RES *result;
	MYSQL_ROW row;
	int n_rows, i;

	INFO_STREAM << "DataBase::db_get_device_alias_list(): wild card " << wildcard << endl;

	if (wildcard == NULL)
	{
		sprintf(sql_query,"SELECT DISTINCT alias FROM device WHERE alias LIKE \"%\" ORDER BY alias");
	}
	else
	{
		tmp_wildcard = replace_wildcard(wildcard);
		sprintf(sql_query,"SELECT DISTINCT alias FROM device WHERE alias LIKE \"%s\" ORDER BY alias",
	        tmp_wildcard.c_str());
	}
	DEBUG_STREAM << "DataBase::db_get_device_alias_list(): sql_query " << sql_query << endl;

	if (mysql_real_query(&mysql, sql_query, strlen(sql_query)) != 0)
	{
	   WARN_STREAM << "DataBase::db_get_device_alias_list(): failed to query TANGO database for list of aliases";
	   WARN_STREAM << wildcard << " (error=" << mysql_error(&mysql) << ")" << endl;
	   sprintf(error_mess,"failed to query TANGO database for list of aliases (error=%s)",mysql_error(&mysql));
	   Tango::Except::throw_exception((const char *)DB_SQLError,
	   				  (const char *)error_mess,
					  (const char *)"DataBase::db_get_device_alias_list()");
	}

	if ((result = mysql_store_result(&mysql)) == NULL)
	{
	   WARN_STREAM << "DataBase::db_get_device_alias_list(): mysql_store_result() failed  (error=" << mysql_error(&mysql) << ")" << endl;
	   sprintf(error_mess,"mysql_store_result() failed (error=%s)",mysql_error(&mysql));
	   Tango::Except::throw_exception((const char *)DB_SQLError,
	   				  (const char *)error_mess,
					  (const char *)"DataBase::db_get_device_alias_list()");
	}

	n_rows = mysql_num_rows(result);
	DEBUG_STREAM << "DataBase::db_get_device_alias_list(): mysql_num_rows() " << n_rows << endl;
	Tango::DevVarStringArray *alias_list = new Tango::DevVarStringArray;

	if (n_rows > 0)
	{
	   alias_list->length(n_rows);

	   for (i=0; i<n_rows; i++)
	   {
	      if ((row = mysql_fetch_row(result)) != NULL)
	      {
	         DEBUG_STREAM << "DataBase::db_get_device_alias_list(): row[ "<< i << "] alias " << row[0] << endl;
	         (*alias_list)[i]   = CORBA::string_dup(row[0]);
	      }
	   }
	}
	else
		alias_list->length(0);
	mysql_free_result(result);

	return(alias_list);
}
//+------------------------------------------------------------------
/**
 *	method:	DataBase::db_get_device_alias
 *
 *	description:	method to execute "DbGetDeviceAlias"
 *	Return alias for device name if found.
 *
 * @param	argin	The device name
 * @return	The alias found
 *
 */
//+------------------------------------------------------------------
Tango::DevString DataBase::db_get_device_alias(Tango::DevString argin)
{
	static char	sql_query[256];
	static char	error_mess[256];
	string		devname(argin);
	string		tmp_devname;
	MYSQL_RES	*result;
	MYSQL_ROW	row;
	int			n_rows;

	INFO_STREAM << "DataBase::db_get_device_alias(): devname " << devname << endl;
	if (!check_device_name(devname))
	{
         	WARN_STREAM << "DataBase::db_get_device_alias(): device name  " << devname << " incorrect ";
         	WARN_STREAM << endl;
         	sprintf(error_mess,"failed to delete device, device name incorrect %s",devname.c_str());
         	Tango::Except::throw_exception((const char *)DB_IncorrectDeviceName,
					       (const char *)error_mess,
					       (const char *)"DataBase::db_get_device_alias()");
	}

	tmp_devname = replace_wildcard(devname.c_str());
	sprintf(sql_query,"SELECT DISTINCT alias FROM device WHERE name LIKE \"%s\" ORDER BY alias",
	        tmp_devname.c_str());
	DEBUG_STREAM << "DataBase::db_get_device_alias(): sql_query " << sql_query << endl;

	if (mysql_real_query(&mysql, sql_query, strlen(sql_query)) != 0)
	{
	   WARN_STREAM << "DataBase::db_get_device_alias(): failed to query TANGO database for list of device ";
	   WARN_STREAM << devname << " (error=" << mysql_error(&mysql) << ")" << endl;
	   sprintf(error_mess,"failed to query TANGO database for a device alias (error=%s)",mysql_error(&mysql));
	   Tango::Except::throw_exception((const char *)DB_SQLError,
	   				  (const char *)error_mess,
					  (const char *)"DataBase::db_get_device_alias()");
	}

	if ((result = mysql_store_result(&mysql)) == NULL)
	{
	   WARN_STREAM << "DataBase::db_get_device_alias(): mysql_store_result() failed  (error=" << mysql_error(&mysql) << ")" << endl;
	   sprintf(error_mess,"mysql_store_result() failed (error=%s)",mysql_error(&mysql));
	   Tango::Except::throw_exception((const char *)DB_SQLError,
	   				  (const char *)error_mess,
					  (const char *)"DataBase::db_get_device_alias()");
	}

	n_rows = mysql_num_rows(result);
	DEBUG_STREAM << "DataBase::db_get_device_alias_(): mysql_num_rows() " << n_rows << endl;

	Tango::DevString	argout;
	if (n_rows > 0)
	{
		if ((row = mysql_fetch_row(result)) != NULL)
		{
			if (row[0]==NULL)
			{
				sprintf(error_mess,"No alias found for device \'%s\'", devname.c_str());
				WARN_STREAM << error_mess << endl;
				Tango::Except::throw_exception((const char *)DB_AliasNotDefined,
	   						  (const char *)error_mess,
							  (const char *)"DataBase::db_get_device_alias()");
			}
			else
				argout = CORBA::string_dup(row[0]);
		}
	}
	else
	{
		sprintf(error_mess,"No alias found for device \'%s\'", devname.c_str());
		WARN_STREAM << error_mess << endl;
		Tango::Except::throw_exception((const char *)DB_AliasNotDefined,
	   				  (const char *)error_mess,
					  (const char *)"DataBase::db_get_device_alias()");
	}
	return argout;
}

//+------------------------------------------------------------------
/**
 *	method:	DataBase::db_put_device_alias
 *
 *	description:	method to execute "DbPutDeviceAlias"
 *
 * @param	argin	device name
 *
 */
//+------------------------------------------------------------------
void DataBase::db_put_device_alias(const Tango::DevVarStringArray *device_alias)
{
	static char sql_query[4096];
	static char error_mess[256];
	MYSQL_RES *result;
	long n_rows=0, i;
	const char *tmp_alias;
	string tmp_device;


	if (device_alias->length() < 2) {
   		WARN_STREAM << "DataBase::db_put_device_alias(): insufficient alias info for device ";
   		sprintf(error_mess,"insufficient alias info for device");
   		Tango::Except::throw_exception((const char *)DB_IncorrectArguments,
					       (const char *)error_mess,
					       (const char *)"DataBase::db_put_device_alias()");
	}

	tmp_device = (*device_alias)[0];
	tmp_alias = (*device_alias)[1];
	for (i=0; i<tmp_device.length(); i++) { 
		tmp_device[i] = tolower(tmp_device[i]);
	}
	INFO_STREAM << "DataBase::db_put_device_alias(): put " << tmp_alias << " for device " << tmp_device << endl;

// first check to see if this alias exists

	sprintf(sql_query,"SELECT alias from device WHERE alias=\'%s\' AND name <> \'%s\'",
        	tmp_alias,tmp_device.c_str());
	DEBUG_STREAM << "DataBase::db_put_device_alias(): sql_query " << sql_query << endl;
	if (mysql_real_query(&mysql, sql_query, strlen(sql_query)) != 0)
	{
   		WARN_STREAM << "DataBase::db_put_device_alias(): failed to query for device alias in TANGO database ";
   		WARN_STREAM << "for " << tmp_device << " (error=" << mysql_error(&mysql) << ")" << endl;
   		sprintf(error_mess,"failed to query for device alias in TANGO database for %s",tmp_device.c_str());
   		Tango::Except::throw_exception((const char *)DB_SQLError,
					       (const char *)error_mess,
					       (const char *)"DataBase::db_put_device_alias()");
	}
	if ((result = mysql_store_result(&mysql)) == NULL)
	{
	   WARN_STREAM << "DataBase::db_put_device_alias(): mysql_store_result() failed  (error=" << mysql_error(&mysql) << ")" << endl;
	   sprintf(error_mess,"mysql_store_result() failed (error=%s)",mysql_error(&mysql));
	   Tango::Except::throw_exception((const char *)DB_SQLError,
	   				  (const char *)error_mess,
					  (const char *)"DataBase::db_put_device_alias()");
	}

	n_rows = mysql_num_rows(result);
	DEBUG_STREAM << "DataBase::db_put_device_alias(): mysql_num_rows() " << n_rows << endl;

	mysql_free_result(result);
	if (n_rows > 0)
	{
	   WARN_STREAM << "DataBase::db_put_device_alias(): this alias exists already !" << endl;
	   sprintf(error_mess,"alias %s already exists !",tmp_alias);
	   Tango::Except::throw_exception((const char *)DB_SQLError,
	   				  (const char *)error_mess,
					  (const char *)"DataBase::db_put_device_alias()");
	}

// update the new value for this tuple

	sprintf(sql_query,"UPDATE device set alias=\'%s\',started=NOW() where name LIKE \'%s\'",
        	tmp_alias, tmp_device.c_str());
	DEBUG_STREAM << "DataBase::db_put_device_alias(): sql_query " << sql_query << endl;

	if (mysql_real_query(&mysql, sql_query, strlen(sql_query)) != 0)
	{
   		WARN_STREAM << "DataBase::db_put_device_alias(): failed to update device alias in TANGO database ";
   		WARN_STREAM << "for " << tmp_device << " (error=" << mysql_error(&mysql) << ")" << endl;
   		sprintf(error_mess,"failed to update device alias in TANGO database for %s",tmp_device.c_str());
   		Tango::Except::throw_exception((const char *)DB_SQLError,
					       (const char *)error_mess,
					       (const char *)"DataBase::db_put_device_alias()");
	}

	return;
}

//+------------------------------------------------------------------
/**
 *	method:	DataBase::db_get_alias_device
 *
 *	description:	method to execute "DbGetAliasDevice"
 *	Return the device name for specified alias.
 *
 * @param	argin	specified alias.
 * @return	Device name found.
 *
 */
//+------------------------------------------------------------------
Tango::DevString DataBase::db_get_alias_device(Tango::DevString argin)
{
	DEBUG_STREAM << "DataBase::db_get_alias_device(): entering... !" << endl;
	static char sql_query[256];
	static char error_mess[256];
	string tmp_argin;
	MYSQL_RES *result;
	MYSQL_ROW row;
	int n_rows;

	if (argin == NULL)
	{
		sprintf(sql_query,"SELECT name FROM device WHERE alias LIKE \"%\"");
	}
	else
	{
		tmp_argin = replace_wildcard(argin);
		sprintf(sql_query,"SELECT name FROM device WHERE alias LIKE \"%s\"",
	        tmp_argin.c_str());
	}
	DEBUG_STREAM << "DataBase::db_get_alias_device(): sql_query " << sql_query << endl;


	if (mysql_real_query(&mysql, sql_query, strlen(sql_query)) != 0)
	{
	   WARN_STREAM << "DataBase::db_get_alias_device(): failed to query TANGO database";
	   WARN_STREAM << argin << " (error=" << mysql_error(&mysql) << ")" << endl;
	   sprintf(error_mess,"failed to query TANGO database (error=%s)",mysql_error(&mysql));
	   Tango::Except::throw_exception((const char *)DB_SQLError,
	   				  (const char *)error_mess,
					  (const char *)"DataBase::db_get_alias_device()");
	}

	if ((result = mysql_store_result(&mysql)) == NULL)
	{
	   WARN_STREAM << "DataBase::db_get_alias_device(): mysql_store_result() failed  (error=" << mysql_error(&mysql) << ")" << endl;
	   sprintf(error_mess,"mysql_store_result() failed (error=%s)",mysql_error(&mysql));
	   Tango::Except::throw_exception((const char *)DB_SQLError,
	   				  (const char *)error_mess,
					  (const char *)"DataBase::db_get_alias_device()");
	}
	n_rows = mysql_num_rows(result);
	DEBUG_STREAM << "DataBase::db_get_alias_device(): mysql_num_rows() " << n_rows << endl;
	Tango::DevString	argout;
	if (n_rows > 0)
	{
		if ((row = mysql_fetch_row(result)) != NULL)
		{
			argout = CORBA::string_dup(row[0]);
		}
	}
	else
	{
		sprintf(error_mess,"No device found for alias \'%s\'", argin);
		WARN_STREAM << error_mess << endl;
		Tango::Except::throw_exception((const char *)DB_DeviceNotDefined,
	   				  (const char *)error_mess,
					  (const char *)"DataBase::db_get_alias_device()");
	}

	return argout;
}

//+------------------------------------------------------------------
/**
 *	method:	DataBase::db_delete_device_alias
 *
 *	description:	method to execute "DbDeleteDeviceAlias"
 *	Delete a device alias.
 *
 * @param	argin	alias name
 *
 */
//+------------------------------------------------------------------
void DataBase::db_delete_device_alias(Tango::DevString argin)
{
	DEBUG_STREAM << "DataBase::db_delete_device_alias(" << argin << ")" << endl;

	static char sql_query[4096];
	static char error_mess[256];

	// first check to see if this alias exists
	sprintf(sql_query,"UPDATE device SET alias=null WHERE alias=\'%s\' ", argin);
	DEBUG_STREAM  << "DataBase::db_delete_device_alias(): sql_query " << sql_query << endl;
	if (mysql_real_query(&mysql, sql_query, strlen(sql_query)) != 0)
	{
		//TangoSys_MemStream out_stream;
		//out_stream <<  "DataBase::db_delete_device_alias(): failed to delete for device alias in TANGO database\n";
   		//out_stream <<  "for alias " << argin << " (error=" << mysql_error(&mysql) << ")" << endl;
		//WARN_STREAM << out_stream.str().c_str() << endl;
   		sprintf(error_mess, "DataBase::db_delete_device_alias(): failed to delete for device alias in TANGO database\nfor alias %s (error=%s)",
					 argin, mysql_error(&mysql));
		WARN_STREAM << error_mess << endl;
		Tango::Except::throw_exception((const char *)DB_SQLError,
					       (const char *)error_mess, 	//out_stream.str().c_str(),
					       (const char *)"DataBase::db_delete_device_alias()");
	}
}





//+------------------------------------------------------------------
/**
 *	method:	DataBase::db_put_attribute_alias
 *
 *	description:	method to execute "DbPutAttributeAlias"
 *	Define an alias for an attribute
 *
 * @param	argin	attribute name, alias
 *
 */
//+------------------------------------------------------------------
void DataBase::db_put_attribute_alias(const Tango::DevVarStringArray *argin)
{
	DEBUG_STREAM << "DataBase::db_put_attribute_alias(): entering... !" << endl;

	static char sql_query[4096];
	static char error_mess[256];
	MYSQL_RES *result;
	long n_rows=0, i;
	const char *tmp_alias;
	string tmp_name, tmp_attribute, tmp_device;


	if (argin->length() < 2) {
   		WARN_STREAM << "DataBase::db_put_attribute_alias(): insufficient alias info for attribute ";
   		sprintf(error_mess,"insufficient alias info for attribute");
   		Tango::Except::throw_exception((const char *)DB_IncorrectArguments,
					       (const char *)error_mess,
					       (const char *)"DataBase::db_put_attribute_alias()");
	}

	tmp_name = (*argin)[0];
	tmp_alias = (*argin)[1];
	for (i=0; i<tmp_name.length(); i++) { 
		tmp_name[i] = tolower(tmp_name[i]);
	}
	INFO_STREAM << "DataBase::db_put_attribute_alias(): put " << tmp_alias << " for attribute " << tmp_name << endl;

// first check to see if this alias exists

	sprintf(sql_query,"SELECT alias from attribute_alias WHERE alias=\'%s\' AND name <> \'%s\'",
        	tmp_alias,tmp_name.c_str());
	DEBUG_STREAM << "DataBase::db_put_attribute_alias(): sql_query " << sql_query << endl;
	if (mysql_real_query(&mysql, sql_query, strlen(sql_query)) != 0)
	{
   		WARN_STREAM << "DataBase::db_put_device_alias(): failed to query for attribute alias in TANGO database ";
   		WARN_STREAM << "for " << tmp_device << " (error=" << mysql_error(&mysql) << ")" << endl;
   		sprintf(error_mess,"failed to query for attribute alias in TANGO database for %s",tmp_name.c_str());
   		Tango::Except::throw_exception((const char *)DB_SQLError,
					       (const char *)error_mess,
					       (const char *)"DataBase::db_put_attribute_alias()");
	}
	if ((result = mysql_store_result(&mysql)) == NULL)
	{
	   WARN_STREAM << "DataBase::db_put_attribute_alias(): mysql_store_result() failed  (error=" << mysql_error(&mysql) << ")" << endl;
	   sprintf(error_mess,"mysql_store_result() failed (error=%s)",mysql_error(&mysql));
	   Tango::Except::throw_exception((const char *)DB_SQLError,
	   				  (const char *)error_mess,
					  (const char *)"DataBase::db_put_attribute_alias()");
	}

	n_rows = mysql_num_rows(result);
	DEBUG_STREAM << "DataBase::db_put_attribute_alias(): mysql_num_rows() " << n_rows << endl;

	mysql_free_result(result);
	if (n_rows > 0)
	{
	   WARN_STREAM << "DataBase::db_put_attribute_alias(): this alias exists already !" << endl;
	   sprintf(error_mess,"alias %s already exists !",tmp_alias);
	   Tango::Except::throw_exception((const char *)DB_SQLError,
	   				  (const char *)error_mess,
					  (const char *)"DataBase::db_put_attribute_alias()");
	}

	string::size_type pos=0;
	int nsep=0;
	do 
	{
		if (pos != 0) pos++;
		pos = tmp_name.find("/",pos);
		if (pos != string::npos) nsep++;
		WARN_STREAM << "DataBase::db_put_attribute_alias(): found " << nsep << " separators , remaining string " << tmp_name.substr(pos+1) << endl;
	}
	while (pos != string::npos);
	if (nsep != 3)
	{
	   WARN_STREAM << "DataBase::db_put_attribute_alias(): attribute name has bad syntax, must have 3 / in it" << endl;
	   sprintf(error_mess,"attribute name %s has bad syntax, must have 3 / in it",tmp_name.c_str());
	   Tango::Except::throw_exception((const char *)DB_SQLError,
	   				  (const char *)error_mess,
					  (const char *)"DataBase::db_put_attribute_alias()");
	}
	tmp_device = tmp_name.substr(0,tmp_name.rfind("/"));
	tmp_attribute = tmp_name.substr(tmp_name.rfind("/")+1);
// first delete the current entry (if any)

	sprintf(sql_query,"DELETE FROM attribute_alias WHERE name=\'%s\'", tmp_name.c_str());
	DEBUG_STREAM << "DataBase::db_put_attribute_alias(): sql_query " << sql_query << endl;

	if (mysql_real_query(&mysql, sql_query, strlen(sql_query)) != 0)
	{
   		WARN_STREAM << "DataBase::db_put_attribute_alias(): failed to delete attribute alias from TANGO database ";
   		WARN_STREAM << "for " << tmp_device << " (error=" << mysql_error(&mysql) << ")" << endl;
   		sprintf(error_mess,"failed to delete attribute alias from TANGO database for %s",tmp_name.c_str());
   		Tango::Except::throw_exception((const char *)DB_SQLError,
					       (const char *)error_mess,
					       (const char *)"DataBase::db_put_attribute_alias()");
	}
// update the new value for this tuple

	sprintf(sql_query,"INSERT attribute_alias SET alias=\'%s\',name=\'%s\',device=\'%s\',attribute=\'%s\',updated=NOW()",
        	tmp_alias, tmp_name.c_str(), tmp_device.c_str(), tmp_attribute.c_str());
	DEBUG_STREAM << "DataBase::db_put_attribute_alias(): sql_query " << sql_query << endl;

	if (mysql_real_query(&mysql, sql_query, strlen(sql_query)) != 0)
	{
   		WARN_STREAM << "DataBase::db_put_attribute_alias(): failed to update attribute alias in TANGO database ";
   		WARN_STREAM << "for " << tmp_device << " (error=" << mysql_error(&mysql) << ")" << endl;
   		sprintf(error_mess,"failed to update device alias in TANGO database for %s",tmp_device.c_str());
   		Tango::Except::throw_exception((const char *)DB_SQLError,
					       (const char *)error_mess,
					       (const char *)"DataBase::db_put_device_alias()");
	}

	return;
}

//+------------------------------------------------------------------
/**
 *	method:	DataBase::db_get_attribute_alias
 *
 *	description:	method to execute "DbGetAttributeAlias"
 *	Get the device attribute name for the given alias. If no alias corresponds then return an empty string.
 *
 * @param	argin	alias
 * @return	attribute name
 *
 */
//+------------------------------------------------------------------
Tango::DevString DataBase::db_get_attribute_alias(Tango::DevString argin)
{
	DEBUG_STREAM << "DataBase::db_get_attribute_alias(): entering... !" << endl;

	static char sql_query[4096];
	static char error_mess[256];
	MYSQL_RES *result;
	MYSQL_ROW row;
	long n_rows=0;
	Tango::DevString	argout  = new char[256];

	INFO_STREAM << "DataBase::db_get_attribute_alias(): put " << argin << endl;

// first check to see if this alias exists

	sprintf(sql_query,"SELECT name from attribute_alias WHERE alias LIKE \'%s\' ",
        	argin);
	DEBUG_STREAM << "DataBase::db_get_attribute_alias(): sql_query " << sql_query << endl;
	if (mysql_real_query(&mysql, sql_query, strlen(sql_query)) != 0)
	{
   		WARN_STREAM << "DataBase::db_put_device_alias(): failed to query for attribute alias in TANGO database ";
   		WARN_STREAM << "for alias " << argin << " (error=" << mysql_error(&mysql) << ")" << endl;
   		sprintf(error_mess,"failed to query for attribute alias in TANGO database for %s",argin);
   		Tango::Except::throw_exception((const char *)DB_SQLError,
					       (const char *)error_mess,
					       (const char *)"DataBase::db_get_attribute_alias()");
	}
	if ((result = mysql_store_result(&mysql)) == NULL)
	{
	   WARN_STREAM << "DataBase::db_get_attribute_alias(): mysql_store_result() failed  (error=" << mysql_error(&mysql) << ")" << endl;
	   sprintf(error_mess,"mysql_store_result() failed (error=%s)",mysql_error(&mysql));
	   Tango::Except::throw_exception((const char *)DB_SQLError,
	   				  (const char *)error_mess,
					  (const char *)"DataBase::db_get_attribute_alias()");
	}

	n_rows = mysql_num_rows(result);
	DEBUG_STREAM << "DataBase::db_get_attribute_alias(): mysql_num_rows() " << n_rows << endl;

	if (n_rows > 0)
	{
        if ((row = mysql_fetch_row(result)) != NULL)
        {
            DEBUG_STREAM << "DataBase::db_get_attribute_alias(): attribute name "<< row[0] << endl;
            strcpy(argout,row[0]);
		}
		else
		{
	   		WARN_STREAM << "DataBase::db_get_attribute_alias(): mysql_fetch_row() failed  (error=" << mysql_error(&mysql) << ")" << endl;
	   		sprintf(error_mess,"mysql_fetch_row() failed (error=%s)",mysql_error(&mysql));
	   		Tango::Except::throw_exception((const char *)DB_SQLError,
	   				  		(const char *)error_mess,
					  		(const char *)"DataBase::db_get_attribute_alias()");
		}
	}
	else
	{
		//strcpy(argout,"");
	   	sprintf(error_mess,"No attribute found for alias  \'%s\'", argin);
	   	Tango::Except::throw_exception((const char *)DB_SQLError,
	   				  	(const char *)error_mess,
					  	(const char *)"DataBase::db_get_attribute_alias()");
	}
	//	Add your own code to control device here

	mysql_free_result(result);

	return argout;
}

//+------------------------------------------------------------------
/**
 *	method:	DataBase::db_get_attribute_alias_list
 *
 *	description:	method to execute "DbGetAttributeAliasList"
 *
 * @param	argin	attribute alias
 * @return	attribute name
 *
 */
//+------------------------------------------------------------------
Tango::DevVarStringArray *DataBase::db_get_attribute_alias_list(Tango::DevString argin)
{
	static char sql_query[256];
	static char error_mess[256];
	string tmp_wildcard;
	MYSQL_RES *result;
	MYSQL_ROW row;
	int n_rows, i;

	INFO_STREAM << "DataBase::db_get_attribute_alias_list(): alias " << argin;
	WARN_STREAM << " wildcard " << argin << endl;

	if (argin == NULL)
	{
		sprintf(sql_query,"SELECT DISTINCT alias FROM attribute_alias WHERE alias LIKE \"%\" ORDER BY attribute");
	}
	else
	{
		tmp_wildcard = replace_wildcard (argin);
		sprintf(sql_query,"SELECT DISTINCT alias FROM attribute_alias WHERE alias LIKE \"%s\" ORDER BY attribute",
	        tmp_wildcard.c_str());
	}
	DEBUG_STREAM << "DataBase::db_get_attribute_alias_list(): sql_query " << sql_query << endl;

	if (mysql_real_query(&mysql, sql_query, strlen(sql_query)) != 0)
	{
	   WARN_STREAM << "DataBase::db_get_attribute_alias_list(): failed to query TANGO database for list of attribute aliases";
	   WARN_STREAM << " (error=" << mysql_error(&mysql) << ")" << endl;
	   sprintf(error_mess,"failed to query TANGO database for list of attributes (error=%s)",mysql_error(&mysql));
	   Tango::Except::throw_exception((const char *)DB_SQLError,
	   				  (const char *)error_mess,
					  (const char *)"DataBase::db_get_attribute_alias_list()");
	}

	if ((result = mysql_store_result(&mysql)) == NULL)
	{
	   WARN_STREAM << "DataBase::db_get_attribute_alias_list(): mysql_store_result() failed  (error=" << mysql_error(&mysql) << ")" << endl;
	   sprintf(error_mess,"mysql_store_result() failed (error=%s)",mysql_error(&mysql));
	   Tango::Except::throw_exception((const char *)DB_SQLError,
	   				  (const char *)error_mess,
					  (const char *)"DataBase::db_get_attribute_alias_list()");
	}

	n_rows = mysql_num_rows(result);
	DEBUG_STREAM << "DataBase::db_get_attribute_alias_list(): mysql_num_rows() " << n_rows << endl;
	Tango::DevVarStringArray *attribute_list = new Tango::DevVarStringArray;

	if (n_rows > 0)
	{
	   attribute_list->length(n_rows);

	   for (i=0; i<n_rows; i++)
	   {
	      if ((row = mysql_fetch_row(result)) != NULL)
	      {
	         DEBUG_STREAM << "DataBase::db_get_attribute_alias_list(): attribute[ "<< i << "] attribute " << row[0] << endl;
	         (*attribute_list)[i]   = CORBA::string_dup(row[0]);
	      }
	   }
	}
	else
		attribute_list->length(0);
	mysql_free_result(result);

	return(attribute_list);
}

//+------------------------------------------------------------------
/**
 *	method:	DataBase::db_delete_attribute_alias
 *
 *	description:	method to execute "DbDeleteAttributeAlias"
 *	Delete an attribute alias.
 *
 * @param	argin	alias name.
 *
 */
//+------------------------------------------------------------------
void DataBase::db_delete_attribute_alias(Tango::DevString argin)
{
	DEBUG_STREAM << "DataBase::db_delete_attribute_alias(" << argin << ")" << endl;

	static char sql_query[4096];
	static char error_mess[256];

	// first check to see if this alias exists
	sprintf(sql_query,"DELETE  FROM attribute_alias WHERE alias=\'%s\' ", argin);
	DEBUG_STREAM << "DataBase::db_delete_attribute_alias(): sql_query " << sql_query << endl;
	if (mysql_real_query(&mysql, sql_query, strlen(sql_query)) != 0)
	{
/*
		TangoSys_MemStream out_stream;
		out_stream <<  "DataBase::db_delete_attribute_alias(): failed to delete for attribute alias in TANGO database\n";
   		out_stream <<  "for alias " << argin << " (error=" << mysql_error(&mysql) << ")" << endl;
		WARN_STREAM << out_stream.str().c_str() << endl;
*/
   		sprintf(error_mess, "DataBase::db_delete_attribute_alias(): failed to delete for attribute alias in TANGO database\nfor alias %s (error=%s)",
					 argin, mysql_error(&mysql));
		WARN_STREAM<< error_mess << endl;
   		Tango::Except::throw_exception((const char *)DB_SQLError,
					       (const char *)error_mess,	//out_stream.str().c_str(),
					       (const char *)"DataBase::db_delete_attribute_alias()");
	}
}

//+------------------------------------------------------------------
/**
 *	method:	DataBase::db_put_device_attribute_property2
 *
 *	description:	method to execute "DbPutDeviceAttributeProperty2"
 *	Put device attribute property. This command adds the possibility to have attribute property
 *	which are arrays. Not possible with the old DbPutDeviceAttributeProperty command.
 *	This old command is not deleted for compatibility reasons.
 *
 * @param	argin	
 *
 */
//+------------------------------------------------------------------
void DataBase::db_put_device_attribute_property2(const Tango::DevVarStringArray *argin)
{
	DEBUG_STREAM << "DataBase::db_put_device_attribute_property2(): entering... !" << endl;

	//	Add your own code to control device here
	static char sql_query[512];
	static char error_mess[256];
	static char tmp_count_str[256];
	int n_attributes, n_properties=0, n_rows=0, tmp_count, i, j, k, l, jj;
	const char *tmp_device, *tmp_attribute, *tmp_name;

	sscanf((*argin)[1],"%d",&n_attributes);
	INFO_STREAM << "DataBase::PutAttributeProperty2(): put " << n_attributes << " attributes for device " << (*argin)[0] << endl;

	k = 2;
	for (i=0; i<n_attributes; i++)
	{
	   	tmp_device = (*argin)[0];
	   	tmp_attribute = (*argin)[k];
	   	sscanf((*argin)[k+1], "%d", &n_properties);
	   	for (jj=0; jj<n_properties; jj++)
	   	{
			j = k + 2;
	      		tmp_name = (*argin)[j];
			
// first delete the tuple (device,name,count) from the property table

	      		sprintf(sql_query,"DELETE FROM property_attribute_device WHERE device LIKE \"%s\" AND attribute LIKE \"%s\" AND name LIKE \"%s\" ",
	              		tmp_device, tmp_attribute, tmp_name);
	      		DEBUG_STREAM << "DataBase::PutAttributeProperty2(): sql_query " << sql_query << endl;
	      		if (mysql_real_query(&mysql, sql_query, strlen(sql_query)) != 0)
	      		{
	         		WARN_STREAM << "DataBase::PutAttributeProperty2(): failed to delete property from TANGO database ";
	        	 	WARN_STREAM << "for " << tmp_device << "/" << tmp_attribute << "." << tmp_name << " (error=" << mysql_error(&mysql) << ")" << endl;
	         		sprintf(error_mess,"Failed to delete property from TANGO database  for %s/%s.%s (error=%s)",tmp_device,tmp_attribute,tmp_name,mysql_error(&mysql));
	         		Tango::Except::throw_exception((const char *)DB_SQLError,
		 					       (const char *)error_mess,
							       (const char *)"DataBase::PutAttributeProperty2()");
	      		}	   	
			
			sscanf((*argin)[j+1], "%d", &n_rows);
			tmp_count = 0;
	   		for (l=j+1; l<j+n_rows+1; l++)
	   		{
          			string tmp_escaped_string = escape_string((*argin)[l+1]);
	      			tmp_count++; sprintf(tmp_count_str, "%d", tmp_count);

// then insert the new value for this tuple

	      			sprintf(sql_query,"INSERT INTO property_attribute_device SET device=\'%s\',attribute=\'%s\',name=\'%s\',count=\'%s\',value=\'%s\',updated=NULL,accessed=NULL",
	              			tmp_device, tmp_attribute, tmp_name, tmp_count_str, tmp_escaped_string.c_str());
	      			DEBUG_STREAM << "DataBase::PutAttributeProperty(): sql_query " << sql_query << endl;

	      			if (mysql_real_query(&mysql, sql_query, strlen(sql_query)) != 0)
	      			{		
	        			WARN_STREAM << "DataBase::PutAttributeProperty2(): failed to insert property into TANGO database ";
	         			WARN_STREAM << "for " << tmp_device << "/" << tmp_attribute << "." << tmp_name << " (error=" << mysql_error(&mysql) << ")" << endl;
	         			sprintf(error_mess,"Failed to insert property into TANGO database  for %s/%s.%s (error=%s)",tmp_device,tmp_attribute,tmp_name,mysql_error(&mysql));
	         			Tango::Except::throw_exception((const char *)DB_SQLError,
		 					(const char *)error_mess,
							(const char *)"DataBase::PutAttributeProperty2()");
	      			}
			}
			k = k + n_rows + 2;
	   	}
	   	k = k+2;
	}

	return;
}

//+------------------------------------------------------------------
/**
 *	method:	DataBase::db_get_device_attribute_property2
 *
 *	description:	method to execute "DbGetDeviceAttributeProperty2"
 *	Retrieve device attribute properties. This command has the possibility to retrieve
 *	device attribute properties which are arrays. It is not possible with the old
 *	DbGetDeviceAttributeProperty command. Nevertheless, the old command has not been
 *	deleted for compatibility reason
 *
 * @param	argin	
 * @return	
 *
 */
//+------------------------------------------------------------------
Tango::DevVarStringArray *DataBase::db_get_device_attribute_property2(const Tango::DevVarStringArray *property_names)
{
	DEBUG_STREAM << "DataBase::db_get_device_attribute_property2(): entering... !" << endl;

	//	Add your own code to control device here

	static char sql_query[256];
	static char error_mess[256];
	static char n_attributes_str[256];
	static char n_rows_str[256];
	static char prop_size_str[256];
	MYSQL_RES *result;
	MYSQL_ROW row;
	int n_attributes=0, n_rows=0, n_props=0, i, j;
	Tango::DevVarStringArray *property_list = new Tango::DevVarStringArray;
	const char *tmp_device, *tmp_attribute;

	INFO_STREAM << "DataBase::GetDeviceAttributeProperty2(): get " << property_names->length()-1 << " properties for device " << (*property_names)[0] << endl;

	tmp_device = (*property_names)[0];
	sprintf(n_attributes_str, "%d", property_names->length()-1);
	n_props = 2;
	property_list->length(n_props);
	(*property_list)[n_props-2] = CORBA::string_dup(tmp_device);
	(*property_list)[n_props-1] = CORBA::string_dup(n_attributes_str);

	for (i=1; i<property_names->length(); i++)
	{
	   	tmp_attribute = (*property_names)[i];
	   	sprintf(sql_query,"SELECT name,value FROM property_attribute_device WHERE device = \"%s\" AND attribute LIKE \"%s\" ORDER BY name,count",
	           	tmp_device, tmp_attribute);
	   	DEBUG_STREAM << "DataBase::GetDeviceAttributeProperty2(): sql_query " << sql_query << endl;

	   	if (mysql_real_query(&mysql, sql_query, strlen(sql_query)) != 0)
	   	{
	     		WARN_STREAM << "DataBase::GetDeviceAttributeProperty2(): failed to query TANGO database for list of properties ";
	      		WARN_STREAM << "for " << (*property_names)[0] << "/" << (*property_names)[i] << " (error=" << mysql_error(&mysql) << ")" << endl;
	      		sprintf(error_mess,"Failed to query TANGO database for list of properties for %s/%s (error=%s)",tmp_device,tmp_attribute,mysql_error(&mysql));
	      		Tango::Except::throw_exception((const char *)DB_SQLError,
	      				     (const char *)error_mess,
					     (const char *)"DataBase::GetDeviceAttributeProperty2()");
	   	}

	   	if ((result = mysql_store_result(&mysql)) == NULL)
	   	{
	      		WARN_STREAM << "DataBase::GetDeviceAttributeProperty2(): mysql_store_result() failed  (error=" << mysql_error(&mysql) << ")" << endl;
	      		sprintf(error_mess,"mysql_store_result() failed (error=%s)",mysql_error(&mysql));
	      		Tango::Except::throw_exception((const char *)DB_SQLError,
	      				     (const char *)error_mess,
					     (const char *)"DataBase::GetDeviceAttributeProperty2()");
	   	}

	   	n_rows = mysql_num_rows(result);
	   	DEBUG_STREAM << "DataBase::GetDeviceAttributeProperty2(): mysql_num_rows() " << n_rows << endl;
	   	n_props = n_props+2;
	   	property_list->length(n_props);
	   	(*property_list)[n_props-2] = CORBA::string_dup(tmp_attribute);
		int prop_number_idx = n_props-1;
		int prop_number = 0;
	   	if (n_rows > 0)
	   	{
			string name, old_name;
			bool new_prop = true;
			int prop_size_idx;
			int prop_size = 0;
	      		for (j=0; j<n_rows; j++)
	      		{
	        		if ((row = mysql_fetch_row(result)) != NULL)
	         		{
					name = row[0];
					if (j == 0)
						old_name = name;
					else
					{
						name = row[0];
						if (name != old_name)
						{
							new_prop = true;
							old_name = name;
						}
						else
							new_prop = false;
					}
//	            			DEBUG_STREAM << "DataBase::GetDeviceAttributeProperty2(): property[ "<< i << "] count " << row[0] << " value " << row[1] << endl;
					if (new_prop == true)
					{
						n_props = n_props + 3;
						property_list->length(n_props);
	            				(*property_list)[n_props-3] = CORBA::string_dup(row[0]);
	            				(*property_list)[n_props-1] = CORBA::string_dup(row[1]);
						if (prop_size != 0)
						{
							sprintf(prop_size_str,"%d",prop_size);
							(*property_list)[prop_size_idx] = CORBA::string_dup(prop_size_str);
							prop_number++;
						}						
						prop_size_idx = n_props - 2;
						prop_size = 1;
					}
					else
					{
						n_props = n_props + 1;
						property_list->length(n_props);
						(*property_list)[n_props-1] = CORBA::string_dup(row[1]);
						prop_size++;
					}
	         		}
	      		}
			if (prop_size != 0)
			{
				sprintf(prop_size_str,"%d",prop_size);
				(*property_list)[prop_size_idx] = CORBA::string_dup(prop_size_str);
				prop_number++;
			}
	   	}
	   	sprintf(n_rows_str,"%d",prop_number);
		(*property_list)[prop_number_idx] = CORBA::string_dup(n_rows_str);
	   	mysql_free_result(result);
	}

	DEBUG_STREAM << "DataBase::GetDeviceAttributeProperty2(): property_list->length() "<< property_list->length() << endl;
	
	return(property_list);
}

//+------------------------------------------------------------------
/**
 *	method:	DataBase::db_put_class_attribute_property2
 *
 *	description:	method to execute "DbPutClassAttributeProperty2"
 *	This command adds support for array properties compared to the previous one
 *	called DbPutClassAttributeProperty. The old comman is still there for compatibility reason
 *
 * @param	argin	
 *
 */
//+------------------------------------------------------------------
void DataBase::db_put_class_attribute_property2(const Tango::DevVarStringArray *argin)
{
	DEBUG_STREAM << "DataBase::db_put_class_attribute_property2(): entering... !" << endl;

	//	Add your own code to control device here
	static char sql_query[512];
	static char error_mess[256];
	static char tmp_count_str[256];
	int n_attributes, n_properties=0, n_rows=0, tmp_count, i, j, k, l, jj;
	const char *tmp_class, *tmp_attribute, *tmp_name;

	sscanf((*argin)[1],"%d",&n_attributes);
	INFO_STREAM << "DataBase::PutClassAttributeProperty2(): put " << n_attributes << " attributes for device " << (*argin)[0] << endl;

	k = 2;
	for (i=0; i<n_attributes; i++)
	{
	   	tmp_class = (*argin)[0];
	   	tmp_attribute = (*argin)[k];
	   	sscanf((*argin)[k+1], "%d", &n_properties);
	   	for (jj=0; jj<n_properties; jj++)
	   	{
			j = k + 2;
	      		tmp_name = (*argin)[j];
			
// first delete the tuple (device,name,count) from the property table

	      		sprintf(sql_query,"DELETE FROM property_attribute_class WHERE class LIKE \"%s\" AND attribute LIKE \"%s\" AND name LIKE \"%s\" ",
	              		tmp_class, tmp_attribute, tmp_name);
	      		DEBUG_STREAM << "DataBase::PutClassAttributeProperty2(): sql_query " << sql_query << endl;
	      		if (mysql_real_query(&mysql, sql_query, strlen(sql_query)) != 0)
	      		{
	         		WARN_STREAM << "DataBase::PutClassAttributeProperty2(): failed to delete property from TANGO database ";
	        	 	WARN_STREAM << "for " << tmp_class << "/" << tmp_attribute << "." << tmp_name << " (error=" << mysql_error(&mysql) << ")" << endl;
	         		sprintf(error_mess,"Failed to delete property from TANGO database  for %s/%s.%s (error=%s)",tmp_class,tmp_attribute,tmp_name,mysql_error(&mysql));
	         		Tango::Except::throw_exception((const char *)DB_SQLError,
		 					       (const char *)error_mess,
							       (const char *)"DataBase::PutClassAttributeProperty2()");
	      		}	   	
			
			sscanf((*argin)[j+1], "%d", &n_rows);
			tmp_count = 0;
	   		for (l=j+1; l<j+n_rows+1; l++)
	   		{
          			string tmp_escaped_string = escape_string((*argin)[l+1]);
	      			tmp_count++; sprintf(tmp_count_str, "%d", tmp_count);

// then insert the new value for this tuple

	      			sprintf(sql_query,"INSERT INTO property_attribute_class SET class=\'%s\',attribute=\'%s\',name=\'%s\',count=\'%s\',value=\'%s\',updated=NULL,accessed=NULL",
	              			tmp_class, tmp_attribute, tmp_name, tmp_count_str, tmp_escaped_string.c_str());
	      			DEBUG_STREAM << "DataBase::PutClassAttributeProperty2(): sql_query " << sql_query << endl;

	      			if (mysql_real_query(&mysql, sql_query, strlen(sql_query)) != 0)
	      			{		
	        			WARN_STREAM << "DataBase::PutClassAttributeProperty2(): failed to insert property into TANGO database ";
	         			WARN_STREAM << "for " << tmp_class << "/" << tmp_attribute << "." << tmp_name << " (error=" << mysql_error(&mysql) << ")" << endl;
	         			sprintf(error_mess,"Failed to insert property into TANGO database  for %s/%s.%s (error=%s)",tmp_class,tmp_attribute,tmp_name,mysql_error(&mysql));
	         			Tango::Except::throw_exception((const char *)DB_SQLError,
		 					(const char *)error_mess,
							(const char *)"DataBase::PutClassAttributeProperty2()");
	      			}
			}
			k = k + n_rows + 2;
	   	}
	   	k = k+2;
	}

	return;
}

//+------------------------------------------------------------------
/**
 *	method:	DataBase::db_get_class_attribute_property2
 *
 *	description:	method to execute "DbGetClassAttributeProperty2"
 *	This command supports array property compared to the old command called
 *	DbGetClassAttributeProperty. The old command has not been deleted from the
 *	server for compatibility reasons.
 *
 * @param	argin	
 * @return	
 *
 */
//+------------------------------------------------------------------
Tango::DevVarStringArray *DataBase::db_get_class_attribute_property2(const Tango::DevVarStringArray *property_names)
{
	//	POGO has generated a method core with argout allocation.
	//	If you would like to use a static reference without copying,
	//	See "TANGO Device Server Programmer's Manual"
	//		(chapter : Writing a TANGO DS / Exchanging data)
	//------------------------------------------------------------
	DEBUG_STREAM << "DataBase::db_get_class_attribute_property2(): entering... !" << endl;

	//	Add your own code to control device here

	static char sql_query[256];
	static char error_mess[256];
	static char n_attributes_str[256];
	static char n_rows_str[256];
	static char prop_size_str[256];
	MYSQL_RES *result;
	MYSQL_ROW row;
	int n_attributes=0, n_rows=0, n_props=0, i, j;
	Tango::DevVarStringArray *property_list = new Tango::DevVarStringArray;
	const char *tmp_class, *tmp_attribute;

	INFO_STREAM << "DataBase::GetClassAttributeProperty2(): get " << property_names->length()-1 << " properties for device " << (*property_names)[0] << endl;

	tmp_class = (*property_names)[0];
	sprintf(n_attributes_str, "%d", property_names->length()-1);
	n_props = 2;
	property_list->length(n_props);
	(*property_list)[n_props-2] = CORBA::string_dup(tmp_class);
	(*property_list)[n_props-1] = CORBA::string_dup(n_attributes_str);

	for (i=1; i<property_names->length(); i++)
	{
	   	tmp_attribute = (*property_names)[i];
	   	sprintf(sql_query,"SELECT name,value FROM property_attribute_class WHERE class = \"%s\" AND attribute LIKE \"%s\" ORDER BY name,count",
	           	tmp_class, tmp_attribute);
	   	DEBUG_STREAM << "DataBase::GetClassAttributeProperty2(): sql_query " << sql_query << endl;

	   	if (mysql_real_query(&mysql, sql_query, strlen(sql_query)) != 0)
	   	{
	     		WARN_STREAM << "DataBase::GetClassAttributeProperty2(): failed to query TANGO database for list of properties ";
	      		WARN_STREAM << "for " << (*property_names)[0] << "/" << (*property_names)[i] << " (error=" << mysql_error(&mysql) << ")" << endl;
	      		sprintf(error_mess,"Failed to query TANGO database for list of properties for %s/%s (error=%s)",tmp_class,tmp_attribute,mysql_error(&mysql));
	      		Tango::Except::throw_exception((const char *)DB_SQLError,
	      				     (const char *)error_mess,
					     (const char *)"DataBase::GetClassAttributeProperty2()");
	   	}

	   	if ((result = mysql_store_result(&mysql)) == NULL)
	   	{
	      		WARN_STREAM << "DataBase::GetClassAttributeProperty2(): mysql_store_result() failed  (error=" << mysql_error(&mysql) << ")" << endl;
	      		sprintf(error_mess,"mysql_store_result() failed (error=%s)",mysql_error(&mysql));
	      		Tango::Except::throw_exception((const char *)DB_SQLError,
	      				     (const char *)error_mess,
					     (const char *)"DataBase::GetClassAttributeProperty2()");
	   	}

	   	n_rows = mysql_num_rows(result);
	   	DEBUG_STREAM << "DataBase::GetClassAttributeProperty2(): mysql_num_rows() " << n_rows << endl;
	   	n_props = n_props+2;
	   	property_list->length(n_props);
	   	(*property_list)[n_props-2] = CORBA::string_dup(tmp_attribute);
		int prop_number_idx = n_props-1;
		int prop_number = 0;
	   	if (n_rows > 0)
	   	{
			string name, old_name;
			bool new_prop = true;
			int prop_size_idx;
			int prop_size = 0;
	      		for (j=0; j<n_rows; j++)
	      		{
	        		if ((row = mysql_fetch_row(result)) != NULL)
	         		{
					name = row[0];
					if (j == 0)
						old_name = name;
					else
					{
						name = row[0];
						if (name != old_name)
						{
							new_prop = true;
							old_name = name;
						}
						else
							new_prop = false;
					}
//	            			DEBUG_STREAM << "DataBase::GetClassAttributeProperty2(): property[ "<< i << "] count " << row[0] << " value " << row[1] << endl;
					if (new_prop == true)
					{
						n_props = n_props + 3;
						property_list->length(n_props);
	            				(*property_list)[n_props-3] = CORBA::string_dup(row[0]);
	            				(*property_list)[n_props-1] = CORBA::string_dup(row[1]);
						if (prop_size != 0)
						{
							sprintf(prop_size_str,"%d",prop_size);
							(*property_list)[prop_size_idx] = CORBA::string_dup(prop_size_str);
							prop_number++;
						}						
						prop_size_idx = n_props - 2;
						prop_size = 1;
					}
					else
					{
						n_props = n_props + 1;
						property_list->length(n_props);
						(*property_list)[n_props-1] = CORBA::string_dup(row[1]);
						prop_size++;
					}
	         		}
	      		}
			if (prop_size != 0)
			{
				sprintf(prop_size_str,"%d",prop_size);
				(*property_list)[prop_size_idx] = CORBA::string_dup(prop_size_str);
				prop_number++;
			}
	   	}
	   	sprintf(n_rows_str,"%d",prop_number);
		(*property_list)[prop_number_idx] = CORBA::string_dup(n_rows_str);
	   	mysql_free_result(result);
	}

	DEBUG_STREAM << "DataBase::GetClassAttributeProperty2(): property_list->length() "<< property_list->length() << endl;
	
	return(property_list);
}

}	//	namespace
