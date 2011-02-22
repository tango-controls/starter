
static char *Version="Version=$Version$";
//+=============================================================================
//
// file :               DataBase.cpp
//
// description :        C++ source for the DataBase and its commands. 
//			The class is derived from Device. It represents the
//			CORBA servant objects which will be accessed from the
//			network to search or modify the database. 
//
//			This version of database is implemented in MySQL.
//
// project :            TANGO
//
// author(s) :          A.Gotz + E.Taurel
//
// original :		July 1999
//
// $Version$
//
// $Log$
// Revision 1.4  2000/02/15 17:51:49  goetz
// testing 1-2-3
//
// Revision 1.1.1.1  2000/02/15 11:20:40  goetz
// Imported sources
//
// Revision 1.1.1.1  1999/12/22 14:49:43  goetz
// Imported sources
//
// Revision 1.4  1999/11/26 10:27:08  goetz
// added DeviceProperty, AttributeProperty and ClassProperty
//
// Revision 1.3  1999/11/05 15:34:43  goetz
// ported to latest version of device pattern, resource now property
//
// Revision 1.2  1999/08/03 14:47:00  goetz
// added DbGetResource and DbPutResource commands and DEBUG cpp flag
//
// Revision 1.1  1999/08/02 11:32:14  goetz
// Initial revision
//
//
// copyleft :           European Synchrotron Radiation Facility
//                      BP 220, Grenoble 38043
//                      FRANCE
//
//-=============================================================================
#include <stdio.h>
#include <tango_ds.h>
#include <DataBase.h>
#include <mysql.h>

string DataBase::db_name("sys/database/1");

//+----------------------------------------------------------------------------
//
// method : 		DataBase::DataBase(string &s)
// 
// description : 	constructor for TANGO database device 
//
// in :			string s - database name 
//
//-----------------------------------------------------------------------------

DataBase::DataBase(DeviceClass *cl,const char *s,const char *d)
:DeviceImpl(cl,s,d)
{
	init_device();
}

void DataBase::init_device()
{
	char *database = "tango";

	cout << "DataBase::DataBase() create database device " << dev_name << endl;

// Initialise mysql database structure and connect to TANGO database

	mysql_init(&mysql);

	if (!mysql_real_connect(&mysql,NULL,NULL,NULL,database,0,NULL,0))
	{
	   cout << "DataBase::init_device(): failed to connect to TANGO database (error = " ;
	   cout << mysql_error(&mysql) << endl;
	}

}

//+----------------------------------------------------------------------------
//
// method : 		DataBase::Info(Tango_DevVarStringArray &property_names)
// 
// description : 	command to retrieve general information about database
//
// in :			void - nothing
//
// out :		Tango__DevVarStringArray - list of info
//
//-----------------------------------------------------------------------------

Tango_DevVarStringArray *DataBase::Info()
{
	static char sql_query[256];
	static char error_mess[256];
	static char info_str[256];
	MYSQL_RES *result;
	MYSQL_ROW row;
	int n_rows=0, i, j;
	Tango_DevVarStringArray *info_list = new Tango_DevVarStringArray;

	cout1 << "DataBase::Info(): get general database infos" << endl;

	sprintf(info_str,"TANGO Database %s",DataBase::db_name.c_str());
	info_list->append(CORBA_string_dup(info_str));

// newline

	info_list->append(CORBA_string_dup(" "));

// get start time of database

	sprintf(sql_query,"SELECT started FROM device WHERE name = \"%s\" ",DataBase::db_name.c_str());
//	cout4 << "DataBase::Info(): sql_query " << sql_query << endl;

	if (mysql_real_query(&mysql, sql_query, strlen(sql_query)) != 0)
	{
	   cout << "DataBase::Info(): failed to query TANGO database for start time " ;
	   cout << " (error=" << mysql_error(&mysql) << ")" << endl;
	   sprintf(error_mess,"failed to query TANGO database start time (error=%s)",mysql_error(&mysql));
	   throw_exception((const char*)error_mess, (const char *)"DataBase::Info()",
		           Tango_DevErr_DataBase ,Tango_DATABASE);
	}

	if ((result = mysql_store_result(&mysql)) == NULL)
	{
	   cout << "DataBase::Info(): mysql_store_result() failed  (error=" << mysql_error(&mysql) << ")" << endl;
	   sprintf(error_mess,"mysql_store_result() failed (error=%s)",mysql_error(&mysql));
	   throw_exception((const char *)error_mess, (const char *)"DataBase::Info()",
		           Tango_DevErr_DataBase ,Tango_DATABASE);
	}

	sprintf(info_str,"Running since ...");
	n_rows = mysql_num_rows(result);
	if (n_rows > 0)
	{
	   if ((row = mysql_fetch_row(result)) != NULL)
	   {
	      cout4 << "DataBase::Info(): database started " << row[0] << endl;
	      sprintf(info_str,"Running since %s",row[0]);
	   }
	}
	mysql_free_result(result);
	info_list->append(CORBA_string_dup(info_str));

// newline

	info_list->append(CORBA_string_dup(" "));

// get number of devices defined

	sprintf(sql_query,"SELECT COUNT(*) FROM device ");
//	cout4 << "DataBase::Info(): sql_query " << sql_query << endl;

	if (mysql_real_query(&mysql, sql_query, strlen(sql_query)) != 0)
	{
	   cout << "DataBase::Info(): failed to query TANGO database for no. of devices " ;
	   cout << " (error=" << mysql_error(&mysql) << ")" << endl;
	   sprintf(error_mess,"failed to query TANGO database for no. of devices (error=%s)",mysql_error(&mysql));
	   throw_exception((const char*)error_mess, (const char *)"DataBase::Info()",
		           Tango_DevErr_DataBase ,Tango_DATABASE);
	}

	if ((result = mysql_store_result(&mysql)) == NULL)
	{
	   cout << "DataBase::Info(): mysql_store_result() failed  (error=" << mysql_error(&mysql) << ")" << endl;
	   sprintf(error_mess,"mysql_store_result() failed (error=%s)",mysql_error(&mysql));
	   throw_exception((const char *)error_mess, (const char *)"DataBase::Info()",
		           Tango_DevErr_DataBase ,Tango_DATABASE);
	}

	sprintf(info_str,"Devices defined ...");
	n_rows = mysql_num_rows(result);
	if (n_rows > 0)
	{
	   if ((row = mysql_fetch_row(result)) != NULL)
	   {
	      cout4 << "DataBase::Info(): no. of devices " << row[0] << endl;
	      sprintf(info_str,"Devices defined  = %s",row[0]);
	   }
	}
	mysql_free_result(result);
	info_list->append(CORBA_string_dup(info_str));

// get number of devices exported

	sprintf(sql_query,"SELECT COUNT(*) FROM device WHERE exported = 1 ");
//	cout4 << "DataBase::Info(): sql_query " << sql_query << endl;

	if (mysql_real_query(&mysql, sql_query, strlen(sql_query)) != 0)
	{
	   cout << "DataBase::Info(): failed to query TANGO database for no. of devices exported" ;
	   cout << " (error=" << mysql_error(&mysql) << ")" << endl;
	   sprintf(error_mess,"failed to query TANGO database for no. of devices exported (error=%s)",mysql_error(&mysql));
	   throw_exception((const char*)error_mess, (const char *)"DataBase::Info()",
		           Tango_DevErr_DataBase ,Tango_DATABASE);
	}

	if ((result = mysql_store_result(&mysql)) == NULL)
	{
	   cout << "DataBase::Info(): mysql_store_result() failed  (error=" << mysql_error(&mysql) << ")" << endl;
	   sprintf(error_mess,"mysql_store_result() failed (error=%s)",mysql_error(&mysql));
	   throw_exception((const char *)error_mess, (const char *)"DataBase::Info()",
		           Tango_DevErr_DataBase ,Tango_DATABASE);
	}

	sprintf(info_str,"Devices exported ...");
	n_rows = mysql_num_rows(result);
	if (n_rows > 0)
	{
	   if ((row = mysql_fetch_row(result)) != NULL)
	   {
	      cout4 << "DataBase::Info(): no. of devices exported " << row[0] << endl;
	      sprintf(info_str,"Devices exported  = %s",row[0]);
	   }
	}
	mysql_free_result(result);
	info_list->append(CORBA_string_dup(info_str));

// get number of devices servers defined

	sprintf(sql_query,"SELECT COUNT(*) FROM device WHERE class = \"DServer\" ");
//	cout4 << "DataBase::Info(): sql_query " << sql_query << endl;

	if (mysql_real_query(&mysql, sql_query, strlen(sql_query)) != 0)
	{
	   cout << "DataBase::Info(): failed to query TANGO database for no. of device servers " ;
	   cout << " (error=" << mysql_error(&mysql) << ")" << endl;
	   sprintf(error_mess,"failed to query TANGO database for no. of device servers (error=%s)",mysql_error(&mysql));
	   throw_exception((const char*)error_mess, (const char *)"DataBase::Info()",
		           Tango_DevErr_DataBase ,Tango_DATABASE);
	}

	if ((result = mysql_store_result(&mysql)) == NULL)
	{
	   cout << "DataBase::Info(): mysql_store_result() failed  (error=" << mysql_error(&mysql) << ")" << endl;
	   sprintf(error_mess,"mysql_store_result() failed (error=%s)",mysql_error(&mysql));
	   throw_exception((const char *)error_mess, (const char *)"DataBase::Info()",
		           Tango_DevErr_DataBase ,Tango_DATABASE);
	}

	sprintf(info_str,"Device servers defined ...");
	n_rows = mysql_num_rows(result);
	if (n_rows > 0)
	{
	   if ((row = mysql_fetch_row(result)) != NULL)
	   {
	      cout4 << "DataBase::Info(): no. of device servers defined " << row[0] << endl;
	      sprintf(info_str,"Device servers defined  = %s",row[0]);
	   }
	}
	mysql_free_result(result);
	info_list->append(CORBA_string_dup(info_str));

// get number of devices servers exported

	sprintf(sql_query,"SELECT COUNT(*) FROM device WHERE class = \"DServer\" AND exported = 1 ");
//	cout4 << "DataBase::Info(): sql_query " << sql_query << endl;

	if (mysql_real_query(&mysql, sql_query, strlen(sql_query)) != 0)
	{
	   cout << "DataBase::Info(): failed to query TANGO database for no. of device servers exported " ;
	   cout << " (error=" << mysql_error(&mysql) << ")" << endl;
	   sprintf(error_mess,"failed to query TANGO database for no. of device servers exported (error=%s)",mysql_error(&mysql));
	   throw_exception((const char*)error_mess, (const char *)"DataBase::Info()",
		           Tango_DevErr_DataBase ,Tango_DATABASE);
	}

	if ((result = mysql_store_result(&mysql)) == NULL)
	{
	   cout << "DataBase::Info(): mysql_store_result() failed  (error=" << mysql_error(&mysql) << ")" << endl;
	   sprintf(error_mess,"mysql_store_result() failed (error=%s)",mysql_error(&mysql));
	   throw_exception((const char *)error_mess, (const char *)"DataBase::Info()",
		           Tango_DevErr_DataBase ,Tango_DATABASE);
	}

	sprintf(info_str,"Device servers exported ...");
	n_rows = mysql_num_rows(result);
	if (n_rows > 0)
	{
	   if ((row = mysql_fetch_row(result)) != NULL)
	   {
	      cout4 << "DataBase::Info(): no. of device servers exported " << row[0] << endl;
	      sprintf(info_str,"Device servers exported  = %s",row[0]);
	   }
	}
	mysql_free_result(result);
	info_list->append(CORBA_string_dup(info_str));

// newline

	info_list->append(CORBA_string_dup(" "));

// get number of class properties 

	sprintf(sql_query,"SELECT COUNT(*) FROM property_class ");
//	cout4 << "DataBase::Info(): sql_query " << sql_query << endl;

	if (mysql_real_query(&mysql, sql_query, strlen(sql_query)) != 0)
	{
	   cout << "DataBase::Info(): failed to query TANGO database for no. of class properties defined " ;
	   cout << " (error=" << mysql_error(&mysql) << ")" << endl;
	   sprintf(error_mess,"failed to query TANGO database for no. of class properties defined (error=%s)",mysql_error(&mysql));
	   throw_exception((const char*)error_mess, (const char *)"DataBase::Info()",
		           Tango_DevErr_DataBase ,Tango_DATABASE);
	}

	if ((result = mysql_store_result(&mysql)) == NULL)
	{
	   cout << "DataBase::Info(): mysql_store_result() failed  (error=" << mysql_error(&mysql) << ")" << endl;
	   sprintf(error_mess,"mysql_store_result() failed (error=%s)",mysql_error(&mysql));
	   throw_exception((const char *)error_mess, (const char *)"DataBase::Info()",
		           Tango_DevErr_DataBase ,Tango_DATABASE);
	}

	sprintf(info_str,"Class properties defined ...");
	n_rows = mysql_num_rows(result);
	if (n_rows > 0)
	{
	   if ((row = mysql_fetch_row(result)) != NULL)
	   {
	      cout4 << "DataBase::Info(): no. of class properties defined " << row[0] << endl;
	      sprintf(info_str,"Class properties defined  = %s",row[0]);
	   }
	}
	mysql_free_result(result);
	info_list->append(CORBA_string_dup(info_str));
// get number of devices properties 

	sprintf(sql_query,"SELECT COUNT(*) FROM property_device ");
//	cout4 << "DataBase::Info(): sql_query " << sql_query << endl;

	if (mysql_real_query(&mysql, sql_query, strlen(sql_query)) != 0)
	{
	   cout << "DataBase::Info(): failed to query TANGO database for no. of device properties " ;
	   cout << " (error=" << mysql_error(&mysql) << ")" << endl;
	   sprintf(error_mess,"failed to query TANGO database for no. of device properties (error=%s)",mysql_error(&mysql));
	   throw_exception((const char*)error_mess, (const char *)"DataBase::Info()",
		           Tango_DevErr_DataBase ,Tango_DATABASE);
	}

	if ((result = mysql_store_result(&mysql)) == NULL)
	{
	   cout << "DataBase::Info(): mysql_store_result() failed  (error=" << mysql_error(&mysql) << ")" << endl;
	   sprintf(error_mess,"mysql_store_result() failed (error=%s)",mysql_error(&mysql));
	   throw_exception((const char *)error_mess, (const char *)"DataBase::Info()",
		           Tango_DevErr_DataBase ,Tango_DATABASE);
	}

	sprintf(info_str,"Device Properties defined ...");
	n_rows = mysql_num_rows(result);
	if (n_rows > 0)
	{
	   if ((row = mysql_fetch_row(result)) != NULL)
	   {
	      cout4 << "DataBase::Info(): no. of device properties defined " << row[0] << endl;
	      sprintf(info_str,"Device properties defined  = %s",row[0]);
	   }
	}
	mysql_free_result(result);
	info_list->append(CORBA_string_dup(info_str));

// get number of attribute properties 

	sprintf(sql_query,"SELECT COUNT(*) FROM property_attribute ");
//	cout4 << "DataBase::Info(): sql_query " << sql_query << endl;

	if (mysql_real_query(&mysql, sql_query, strlen(sql_query)) != 0)
	{
	   cout << "DataBase::Info(): failed to query TANGO database for no. of attribute properties defined " ;
	   cout << " (error=" << mysql_error(&mysql) << ")" << endl;
	   sprintf(error_mess,"failed to query TANGO database for no. of attribute properties defined (error=%s)",mysql_error(&mysql));
	   throw_exception((const char*)error_mess, (const char *)"DataBase::Info()",
		           Tango_DevErr_DataBase ,Tango_DATABASE);
	}

	if ((result = mysql_store_result(&mysql)) == NULL)
	{
	   cout << "DataBase::Info(): mysql_store_result() failed  (error=" << mysql_error(&mysql) << ")" << endl;
	   sprintf(error_mess,"mysql_store_result() failed (error=%s)",mysql_error(&mysql));
	   throw_exception((const char *)error_mess, (const char *)"DataBase::Info()",
		           Tango_DevErr_DataBase ,Tango_DATABASE);
	}

	sprintf(info_str,"Attribute properties defined ...");
	n_rows = mysql_num_rows(result);
	if (n_rows > 0)
	{
	   if ((row = mysql_fetch_row(result)) != NULL)
	   {
	      cout4 << "DataBase::Info(): no. of attribute properties defined " << row[0] << endl;
	      sprintf(info_str,"Attribute properties defined  = %s",row[0]);
	   }
	}
	mysql_free_result(result);
	info_list->append(CORBA_string_dup(info_str));

	cout4 << "DataBase::Info(): info_list->length() "<< info_list->length() << endl;

	return(info_list);
}

//+----------------------------------------------------------------------------
//
// method : 		DataBase::GetDeviceClassList(Tango_DevString server)
// 
// description : 	command to get the list of devices and their classes
//			served by a device server
//
// in :			Tango_DevString server - name of device server e.g. "Myds/test"
//
// out :		Tango__DevVarStringArray* - list of device names and classes
//
//-----------------------------------------------------------------------------

Tango_DevVarStringArray *DataBase::GetDeviceClassList(Tango_DevString server)
{
	static char sql_query[256];
	static char error_mess[256];
	MYSQL_RES *result;
	MYSQL_ROW row;
	int n_rows, i;

	cout1 << "DataBase::GetDeviceClassList(): server " << server << endl;

	sprintf(sql_query,"SELECT name,class FROM device WHERE server = \"%s\" ORDER BY name",
	        server);
	cout4 << "DataBase::GetDeviceClassList(): sql_query " << sql_query << endl;

	if (mysql_real_query(&mysql, sql_query, strlen(sql_query)) != 0)
	{
	   cout << "DataBase::GetDeviceClassList(): failed to query TANGO database for list of devices for server ";
	   cout << server << " (error=" << mysql_error(&mysql) << ")" << endl;
	   sprintf(error_mess,"failed to query TANGO database for list of devices for server (error=%s)",mysql_error(&mysql));
	   throw_exception((const char *)error_mess, (const char *)"DataBase::GetDeviceClassList()",
		                Tango_DevErr_DataBase ,Tango_DATABASE);
	}

	if ((result = mysql_store_result(&mysql)) == NULL)
	{
	   cout << "DataBase::GetDeviceClassList(): mysql_store_result() failed  (error=" << mysql_error(&mysql) << ")" << endl;
	   sprintf(error_mess,"mysql_store_result() failed (error=%s)",mysql_error(&mysql));
	   throw_exception((const char *)error_mess, (const char *)"DataBase::GetDeviceClassList()",
		           Tango_DevErr_DataBase ,Tango_DATABASE);
	}

	n_rows = mysql_num_rows(result);
	cout1 << "DataBase::GetDeviceClassList(): mysql_num_rows() " << n_rows << endl;
	Tango_DevVarStringArray *device_class_list = new Tango_DevVarStringArray;

	if (n_rows > 0)
	{
	   device_class_list->length(n_rows*2);

	   for (i=0; i<n_rows; i++)
	   {
	      if ((row = mysql_fetch_row(result)) != NULL)
	      {
	         cout4 << "DataBase::GetDeviceClassList(): row[ "<< i << "] name " << row[0] << " class " << row[1] << endl;
	         (*device_class_list)[i*2]   = CORBA_string_dup(row[0]);
	         (*device_class_list)[i*2+1] = CORBA_string_dup(row[1]);
	      }
	   }
	}
	mysql_free_result(result);

	return(device_class_list);
}

//+----------------------------------------------------------------------------
//
// method : 		DataBase::GetClassList(Tango_DevString server)
// 
// description : 	command to get the list of classes to be
//			served by a device server
//
// in :			Tango_DevString server - name of device server e.g. "Myds/test"
//
// out :		Tango__DevVarStringArray* - list of classes
//
//-----------------------------------------------------------------------------

Tango_DevVarStringArray *DataBase::GetClassList(Tango_DevString server)
{
	static char sql_query[256];
	static char error_mess[256];
	MYSQL_RES *result;
	MYSQL_ROW row;
	int n_rows, i;

	cout1 << "DataBase::GetClassList(): server " << server << endl;

	sprintf(sql_query,"SELECT DISTINCT class FROM device WHERE server = \"%s\" ORDER BY class",
	        server);
	cout4 << "DataBase::GetClassList(): sql_query " << sql_query << endl;

	if (mysql_real_query(&mysql, sql_query, strlen(sql_query)) != 0)
	{
	   cout << "DataBase::GetClassList(): failed to query TANGO database for list of classes for server ";
	   cout << server << " (error=" << mysql_error(&mysql) << ")" << endl;
	   sprintf(error_mess,"failed to query TANGO database for list of classes for server (error=%s)",mysql_error(&mysql));
	   throw_exception((const char *)error_mess, (const char *)"DataBase::GetClassList()",
		                Tango_DevErr_DataBase ,Tango_DATABASE);
	}

	if ((result = mysql_store_result(&mysql)) == NULL)
	{
	   cout << "DataBase::GetClassList(): mysql_store_result() failed  (error=" << mysql_error(&mysql) << ")" << endl;
	   sprintf(error_mess,"mysql_store_result() failed (error=%s)",mysql_error(&mysql));
	   throw_exception((const char *)error_mess, (const char *)"DataBase::GetClassList()",
		           Tango_DevErr_DataBase ,Tango_DATABASE);
	}

	n_rows = mysql_num_rows(result);
	cout4 << "DataBase::GetClassList(): mysql_num_rows() " << n_rows << endl;
	Tango_DevVarStringArray *class_list = new Tango_DevVarStringArray;

	if (n_rows > 0)
	{
	   class_list->length(n_rows);

	   for (i=0; i<n_rows; i++)
	   {
	      if ((row = mysql_fetch_row(result)) != NULL)
	      {
	         cout4 << "DataBase::GetClassList(): row[ "<< i << "] class " << row[0] << endl;
	         (*class_list)[i]   = CORBA_string_dup(row[0]);
	      }
	   }
	}
	mysql_free_result(result);

	return(class_list);
}

//+----------------------------------------------------------------------------
//
// method : 		DataBase::GetDeviceList(string &server)
// 
// description : 	command to get the list of device if the specified
//			class to be served by a device server
//
// in :			Tango__DevVarStringArray* server_class - name of device 
//			server  and class e.g. "Myds/test","StepperMotor"
//
// out :		Tango__DevVarStringArray* - list of device
//
//-----------------------------------------------------------------------------

Tango_DevVarStringArray *DataBase::GetDeviceList(Tango_DevVarStringArray *server_class)
{
	static char sql_query[256];
	static char error_mess[256];
	MYSQL_RES *result;
	MYSQL_ROW row;
	int n_rows, i;
	char *tmp_server, *tmp_class;

	if (server_class->length() != 2)
	{
	   cout << "DataBase::GetDeviceClassList(): incorrect number of input arguments " << endl;
	   sprintf(error_mess,"incorrect no. of input arguments, needs 2 (server,class)");
	   throw_exception((const char *)error_mess, (const char *)"DataBase::GetDeviceList()",
		                Tango_DevErr_DataBase ,Tango_DATABASE);
	}
		
	tmp_server = (*server_class)[0];
	tmp_class = (*server_class)[1];

	cout1 << "DataBase::GetClassList(): server " << tmp_server << endl;

	sprintf(sql_query,"SELECT DISTINCT name FROM device WHERE server = \"%s\" AND class = \"%s\" ORDER BY name",
	        tmp_server, tmp_class);
	cout4 << "DataBase::GetDeviceList(): sql_query " << sql_query << endl;

	if (mysql_real_query(&mysql, sql_query, strlen(sql_query)) != 0)
	{
	   cout << "DataBase::GetDeviceList(): failed to query TANGO database for list of classes for server ";
	   cout << server_class << " (error=" << mysql_error(&mysql) << ")" << endl;
	   sprintf(error_mess,"failed to query TANGO database for list of classes for server (error=%s)",mysql_error(&mysql));
	   throw_exception((const char *)error_mess, (const char *)"DataBase::GetDeviceList()",
		                Tango_DevErr_DataBase ,Tango_DATABASE);
	}

	if ((result = mysql_store_result(&mysql)) == NULL)
	{
	   cout << "DataBase::GetDeviceList(): mysql_store_result() failed  (error=" << mysql_error(&mysql) << ")" << endl;
	   sprintf(error_mess,"mysql_store_result() failed (error=%s)",mysql_error(&mysql));
	   throw_exception((const char *)error_mess, (const char *)"DataBase::GetDeviceList()",
		           Tango_DevErr_DataBase ,Tango_DATABASE);
	}

	n_rows = mysql_num_rows(result);
	cout4 << "DataBase::GetDeviceList(): mysql_num_rows() " << n_rows << endl;
	Tango_DevVarStringArray *device_list = new Tango_DevVarStringArray;

	if (n_rows > 0)
	{
	   device_list->length(n_rows);

	   for (i=0; i<n_rows; i++)
	   {
	      if ((row = mysql_fetch_row(result)) != NULL)
	      {
	         cout4 << "DataBase::GetdeviceList(): row[ "<< i << "] name " << row[0] << endl;
	         (*device_list)[i]   = CORBA_string_dup(row[0]);
	      }
	   }
	}
	mysql_free_result(result);

	return(device_list);
}

//+----------------------------------------------------------------------------
//
// method : 		DataBase::ExportDevice(Tango_DevVarStringArray *export_info)
// 
// description : 	command to export device information into the database so
//			that clients can import the device (IOR, host, version, ...)
//
// in :			char *export_list - list of export info e.g. "n device res1 res2 ..."
//
// out :		void - nothing
//
//-----------------------------------------------------------------------------

void DataBase::ExportDevice(Tango_DevVarStringArray *export_info)
{
	static char sql_query[4096];
	static char error_mess[256];
	MYSQL_RES *result;
	MYSQL_ROW row;
	long n_rows=0, i, j;
	char *tmp_device, *tmp_ior, *tmp_host, *tmp_pid, *tmp_version;


	if (export_info->length() < 5) {
   		cout << "DataBase::DbExportDevice(): insufficient export info for device ";
   		cout << tmp_device << endl;
   		sprintf(error_mess,"insufficient export info for device %s",tmp_device);
   		throw_exception((const char *)error_mess, (const char *)"DataBase::DbExportDevice()",
	           		Tango_DevErr_DataBase ,Tango_DATABASE);
	}
	
	cout1 << "DataBase::ExportDevice(): put " << export_info->length()-1 << " export info for device " << (*export_info)[0] << endl;

	tmp_device = (*export_info)[0];
	for (i=0; i<strlen(tmp_device); i++) { 
		tmp_device[i] = tolower(tmp_device[i]);
	}
	tmp_ior = (*export_info)[1];
	tmp_host = (*export_info)[2];
	tmp_pid = (*export_info)[3];
	tmp_version = (*export_info)[4];

// update the new value for this tuple

	sprintf(sql_query,"UPDATE device SET  exported=1,ior=\'%s\',host=\'%s\',pid=\'%s\',version=\'%s\',started=NOW() WHERE name = \'%s\'",
        	tmp_ior, tmp_host, tmp_pid, tmp_version, tmp_device);
	cout4 << "DataBase::ExportDevice(): sql_query " << sql_query << endl;

	if (mysql_real_query(&mysql, sql_query, strlen(sql_query)) != 0)
	{
   		cout << "DataBase::DbExportDevice(): failed to export device to TANGO database ";
   		cout << "for " << tmp_device << " (error=" << mysql_error(&mysql) << ")" << endl;
   		sprintf(error_mess,"failed to insert export info into TANGO database for %s",tmp_device);
   		throw_exception((const char *)error_mess, (const char *)"DataBase::DbExportDevice()",
	           		Tango_DevErr_DataBase ,Tango_DATABASE);
	}

	return;
}
//+----------------------------------------------------------------------------
//
// method : 		DataBase::UnExportDevice(Tango_DevString device_name)
// 
// description : 	command to unexport a device from the database 
//
// in :			char *device_name - device name to unexport
//
// out :		void - nothing
//
//-----------------------------------------------------------------------------

void DataBase::UnExportDevice(Tango_DevString device_name)
{
	static char sql_query[4096];
	static char error_mess[256];
	MYSQL_RES *result;
	MYSQL_ROW row;
	long n_rows=0, i, j;
	char *tmp_device;

	cout1 << "DataBase::UnExportDevice(): un-export " << device_name << " device " << endl;

	tmp_device = (char*)malloc(strlen(device_name)+1);
	sprintf(tmp_device,"%s",device_name);
	for (i=0; i<strlen(tmp_device); i++) { 
		tmp_device[i] = tolower(tmp_device[i]);
	}

// un-export device from database by seting ior="not exported"

	sprintf(sql_query,"UPDATE device SET exported=0,stopped=NOW() WHERE name=\"%s\"",
	        tmp_device);
	cout4 << "DataBase::UnExportDevice(): sql_query " << sql_query << endl;
	if (mysql_real_query(&mysql, sql_query, strlen(sql_query)) != 0)
	{
   		cout << "DataBase::UnExportDevice(): failed to unexport device from TANGO database ";
   		cout << "for " << tmp_device << " (error=" << mysql_error(&mysql) << ")" << endl;
   		sprintf(error_mess,"failed to un-export device %s from TANGO database  (error=%s)",tmp_device,mysql_error(&mysql));
   		throw_exception((const char *)error_mess, (const char *)"DataBase::UnExportDevice()",
	           		Tango_DevErr_DataBase ,Tango_DATABASE);
	}

	free(tmp_device);

	return;
}

//+----------------------------------------------------------------------------
//
// method : 		DataBase::UnExportServer(Tango_DevString device_list)
// 
// description : 	command to unexport all devices belonging to a server from 
//			the database 
//
// in :			char *server_name - server for which devices are to unexported
//
// out :		void - nothing
//
//-----------------------------------------------------------------------------

void DataBase::UnExportServer(Tango_DevString server_name)
{
	static char sql_query[4096];
	static char error_mess[256];
	MYSQL_RES *result;
	MYSQL_ROW row;
	long n_rows=0, i, j;
	char *tmp_server;

	cout1 << "DataBase::UnExportServer(): un-export all device(s) from server " << server_name << " device " << endl;

	tmp_server = (char*)malloc(strlen(server_name)+1);
	sprintf(tmp_server,"%s",server_name);
	for (i=0; i<strlen(tmp_server); i++) { 
		tmp_server[i] = tolower(tmp_server[i]);
	}

// un-export all devices belonging to this server from database by setting exported="false"

	sprintf(sql_query,"UPDATE device SET exported=0,stopped=NOW() WHERE server=\"%s\"",
	        tmp_server);
	cout4 << "DataBase::UnExportServer(): sql_query " << sql_query << endl;
	if (mysql_real_query(&mysql, sql_query, strlen(sql_query)) != 0)
	{
   		cout << "DataBase::UnExportServer(): failed to unexport devices from TANGO database ";
   		cout << "for " << tmp_server << " (mysql_error=" << mysql_error(&mysql) << ")" << endl;
   		sprintf(error_mess,"failed to un-export devices %s from TANGO database  (error=%s)",tmp_server,mysql_error(&mysql));
   		throw_exception((const char *)error_mess, (const char *)"DataBase::UnExportServer()",
	           		Tango_DevErr_DataBase ,Tango_DATABASE);
	}

	free(tmp_server);

	return;
}
//+----------------------------------------------------------------------------
//
// method : 		DataBase::ImportDevice(Tango_DevString device_name)
// 
// description : 	command to get the device info from the database
//			so that a client can import it.
//
// in :			Tango_DevString device_name - device name to import 
//
// out :		Tango__DevVarStringArray - list of device info
//
//-----------------------------------------------------------------------------

Tango_DevVarLongStringArray *DataBase::ImportDevice(Tango_DevString device_name)
{
	static char sql_query[256];
	static char error_mess[256];
	MYSQL_RES *result;
	MYSQL_ROW row;
	int n_devices=0, n_rows=0, i, j, exported;
	char *tmp_device;
	Tango_DevVarLongStringArray *import_info = new Tango_DevVarLongStringArray;

	cout1 << "DataBase::ImportDevice(): get import info for " << device_name << " device " << endl;

	tmp_device = (char*)malloc(strlen(device_name)+1);
	sprintf(tmp_device,"%s",device_name);
	for (i=0; i<strlen(tmp_device); i++) { 
		tmp_device[i] = tolower(tmp_device[i]);
	}

	sprintf(sql_query,"SELECT exported,ior,version FROM device WHERE name = \"%s\" ",
	        tmp_device);
	cout4 << "DataBase::ImportDevice(): sql_query " << sql_query << endl;

	if (mysql_real_query(&mysql, sql_query, strlen(sql_query)) != 0)
	{
	   cout << "DataBase::ImportDeviceList(): failed to query TANGO database for list of properties ";
	   cout << "for import info for device " << tmp_device << " (error=" << mysql_error(&mysql) << ")" << endl;
	   sprintf(error_mess,"failed to query TANGO database for import info of device for %s (error=%s)",tmp_device,mysql_error(&mysql));
	   free(tmp_device);
	   throw_exception((const char *)error_mess, (const char *)"DataBase::ImportDevice()",
		           Tango_DevErr_DataBase ,Tango_DATABASE);
	}

	if ((result = mysql_store_result(&mysql)) == NULL)
	{
	   cout << "DataBase::ImportDeviceList(): mysql_store_result() failed  (error=" << mysql_error(&mysql) << ")" << endl;
	   sprintf(error_mess,"mysql_store_result() failed (error=%s)",mysql_error(&mysql));
	   free(tmp_device);
	   throw_exception((const char *)error_mess, (const char *)"DataBase::ImportDevice()",
		           Tango_DevErr_DataBase ,Tango_DATABASE);
	}

	n_rows = mysql_num_rows(result);
	cout4 << "DataBase::ImportDeviceList(): mysql_num_rows() " << n_rows << endl;

	if (n_rows > 0)
	{

	   if ((row = mysql_fetch_row(result)) != NULL)
	   {
	      cout4 << "DataBase::ImportDeviceList(): device[ "<< i << "] exported " << row[0] << " IOR  " << row[1] << " version " << row[2] << endl;
	      (import_info->svalue).append(CORBA_string_dup(tmp_device));
	      (import_info->svalue).append(CORBA_string_dup(row[1]));
	      (import_info->svalue).append(CORBA_string_dup(row[2]));
	      exported = -1;
	      if (row[0] != NULL) sscanf(row[0],"%d",&exported);
	      (import_info->lvalue).append(exported);
	   }
	}
	else {
	     cout << "DataBase::ImportDevice(): device not defined !" << endl;
	     sprintf(error_mess,"device %s not defined in the database !",tmp_device);
	     free(tmp_device);
	     throw_exception((const char *)error_mess, (const char *)"DataBase::ImportDevice()",
		           Tango_DevErr_DataBase ,Tango_DATABASE);
	}
	mysql_free_result(result);
	free(tmp_device);

	return(import_info);
}
//+----------------------------------------------------------------------------
//
// method : 		DataBase::ImportDeviceList(Tango_DevVarStringArray &device_list)
// 
// description : 	command to get the import info for a list devices from the database
//			so that a client can import them.
//
// in :			char *device_list - list of device names to import e.g. "device1 device2 device3 ..."
//
// out :		Tango__DevVarStringArray - list of device info
//
//-----------------------------------------------------------------------------

Tango_DevVarLongStringArray *DataBase::ImportDeviceList(Tango_DevVarStringArray *device_list)
{
	static char sql_query[256];
	static char error_mess[256];
	MYSQL_RES *result;
	MYSQL_ROW row;
	int n_devices=0, n_rows=0, i, j, exported;
	Tango_DevVarLongStringArray *import_info = new Tango_DevVarLongStringArray;
	char *tmp_device, *tmp_name;

	cout1 << "DataBase::ImportDeviceList(): get import info for" << device_list->length() << " devices " << endl;

	n_devices = device_list->length();
	for (i=1; i<n_devices; i++)
	{
	   tmp_device = (*device_list)[i];
	   sprintf(sql_query,"SELECT exported,ior,version FROM device WHERE name = \"%s\" ",
	           tmp_device);
	   cout4 << "DataBase::ImportDeviceList(): sql_query " << sql_query << endl;

	   if (mysql_real_query(&mysql, sql_query, strlen(sql_query)) != 0)
	   {
	      cout << "DataBase::ImportDeviceList(): failed to query TANGO database for list of properties ";
	      cout << "for import info for device " << (*device_list)[i] << " (error=" << mysql_error(&mysql) << ")" << endl;
	      sprintf(error_mess,"failed to query TANGO database for import info of device for %s (error=%s)",tmp_device,mysql_error(&mysql));
	      free(tmp_device);
	      throw_exception((const char *)error_mess, (const char *)"DataBase::GetImportDeviceList()",
		              Tango_DevErr_DataBase ,Tango_DATABASE);
	   }

	   if ((result = mysql_store_result(&mysql)) == NULL)
	   {
	      cout << "DataBase::ImportDeviceList(): mysql_store_result() failed  (error=" << mysql_error(&mysql) << ")" << endl;
	      sprintf(error_mess,"mysql_store_result() failed (error=%s)",mysql_error(&mysql));
	      free(tmp_device);
	      throw_exception((const char *)error_mess, (const char *)"DataBase::ImportDeviceList()",
		              Tango_DevErr_DataBase ,Tango_DATABASE);
	   }

	   n_rows = mysql_num_rows(result);
	   cout4 << "DataBase::ImportDeviceList(): mysql_num_rows() " << n_rows << endl;

	   if (n_rows > 0)
	   {

	      if ((row = mysql_fetch_row(result)) != NULL)
	      {
	         cout4 << "DataBase::ImportDeviceList(): device[ "<< i << "] exported " << row[0] << " IOR  " << row[1] << " version " << row[2] << endl;
	         (import_info->svalue).append(CORBA_string_dup(tmp_device));
	         (import_info->svalue).append(CORBA_string_dup(row[1]));
	         (import_info->svalue).append(CORBA_string_dup(row[2]));
	         exported = -1;
	         if (row[0] != NULL) sscanf(row[0],"%d",&exported);
	         (import_info->lvalue).append(exported);
	      }
	   }
	   else {
	         cout << "DataBase::ImportDeviceList(): device not defined !" << endl;
		(import_info->svalue).append(CORBA_string_dup(tmp_device));
		(import_info->svalue).append(CORBA_string_dup("not exported"));
		(import_info->svalue).append(CORBA_string_dup("-1"));
	        (import_info->lvalue).append(-1);
	   }
	   free(tmp_device);
	   mysql_free_result(result);
	}

	return(import_info);
}
//+----------------------------------------------------------------------------
//
// method : 		DataBase::GetDeviceProperty(Tango_DevVarStringArray &property_names)
// 
// description : 	command to get the list of properties from the database
//
// in :			char *property_names - list of properties names to query e.g. "device res1 res2 ..."
//
// out :		Tango__DevVarStringArray - list of properties
//
//-----------------------------------------------------------------------------

Tango_DevVarStringArray *DataBase::GetDeviceProperty(Tango_DevVarStringArray *property_names)
{
	static char sql_query[256];
	static char error_mess[256];
	static char n_properties_str[256];
	static char n_rows_str[256];
	MYSQL_RES *result;
	MYSQL_ROW row;
	int n_properties=0, n_rows=0, i, j;
	Tango_DevVarStringArray *property_list = new Tango_DevVarStringArray;
	char *tmp_device, *tmp_name;

	cout1 << "DataBase::GetDeviceProperty(): get " << property_names->length()-1 << " properties for device " << (*property_names)[0] << endl;

	tmp_device = (*property_names)[0];
	sprintf(n_properties_str, "%d", property_names->length()-1);
	property_list->append(CORBA_string_dup(tmp_device));
	property_list->append(CORBA_string_dup(n_properties_str));

	for (i=1; i<property_names->length(); i++)
	{
	   tmp_name = (*property_names)[i];
	   sprintf(sql_query,"SELECT count,value,name FROM property_device WHERE device = \"%s\" AND name LIKE \"%s\" ORDER BY count",
	           tmp_device, tmp_name);
	   cout4 << "DataBase::GetDeviceList(): sql_query " << sql_query << endl;

	   if (mysql_real_query(&mysql, sql_query, strlen(sql_query)) != 0)
	   {
	      cout << "DataBase::GetDeviceProperty(): failed to query TANGO database for list of properties ";
	      cout << "for " << (*property_names)[0] << "/" << (*property_names)[i] << " (error=" << mysql_error(&mysql) << ")" << endl;
	      sprintf(error_mess,"failed to query TANGO database for list of properties for %s/%s (error=%s)",tmp_device,tmp_name,mysql_error(&mysql));
	      throw_exception((const char *)error_mess, (const char *)"DataBase::GetDeviceList()",
		              Tango_DevErr_DataBase ,Tango_DATABASE);
	   }

	   if ((result = mysql_store_result(&mysql)) == NULL)
	   {
	      cout << "DataBase::GetDeviceProperty(): mysql_store_result() failed  (error=" << mysql_error(&mysql) << ")" << endl;
	      sprintf(error_mess,"mysql_store_result() failed (error=%s)",mysql_error(&mysql));
	      throw_exception((const char *)error_mess, (const char *)"DataBase::GetDeviceProperty()",
		              Tango_DevErr_DataBase ,Tango_DATABASE);
	   }

	   n_rows = mysql_num_rows(result);
	   cout4 << "DataBase::GetDeviceProperty(): mysql_num_rows() " << n_rows << endl;
	   sprintf(n_rows_str,"%d",n_rows);
	   property_list->append(CORBA_string_dup(tmp_name));
           property_list->append(CORBA_string_dup(n_rows_str));
	   if (n_rows > 0)
	   {

	      for (j=0; j<n_rows; j++)
	      {
	         if ((row = mysql_fetch_row(result)) != NULL)
	         {
	            cout4 << "DataBase::GetDeviceProperty(): property[ "<< i << "] count " << row[0] << " value " << row[1] << endl;
	            property_list->append(CORBA_string_dup(row[1]));
	         }
	      }
	      n_properties += n_rows;
	   }
	   else
	   {
	      property_list->append(CORBA_string_dup(" "));
	   }
	   mysql_free_result(result);
	}

	cout4 << "DataBase::GetDeviceProperty(): property_list->length() "<< property_list->length() << endl;

	return(property_list);
}

//+----------------------------------------------------------------------------
//
// method : 		DataBase::PutDeviceProperty(Tango_DevVarStringArray *property_list)
// 
// description : 	command to put a list of properties into the database
//
// in :			char *property_list - list of properties names to update e.g. "device res1 res2 ..."
//
// out :		void - nothing
//
//-----------------------------------------------------------------------------

void DataBase::PutDeviceProperty(Tango_DevVarStringArray *property_list)
{
	static char sql_query[512];
	static char error_mess[256];
	static char tmp_count_str[256];
	MYSQL_RES *result;
	MYSQL_ROW row;
	int n_properties=0, n_rows=0, i, j, k;
	char *tmp_device, *tmp_name, *tmp_value, *tmp_count;

	sscanf((*property_list)[1],"%d",&n_properties);
	cout1 << "DataBase::PutDeviceProperty(): put " << n_properties << " properties for device " << (*property_list)[0] << endl;

	k = 2;
	for (i=0; i<n_properties; i++)
	{
	   tmp_count = 0;
	   sscanf((*property_list)[k+1], "%d", &n_rows);
	   for (j=k+2; j<k+n_rows+2; j++)
	   {
	      tmp_device = (*property_list)[0];
	      tmp_name = (*property_list)[k];
	      tmp_value = (*property_list)[j];
	      tmp_count++; sprintf(tmp_count_str, "%d", tmp_count);

// first delete the tuple (device,name,count) from the property table

	      sprintf(sql_query,"DELETE FROM property_device WHERE device=\"%s\" AND name=\"%s\" AND count=\"%s\"",
	              tmp_device, tmp_name, tmp_count_str);
	      cout4 << "DataBase::PutDeviceProperty(): sql_query " << sql_query << endl;
	      if (mysql_real_query(&mysql, sql_query, strlen(sql_query)) != 0)
	      {
	         cout << "DataBase::PutDeviceProperty(): failed to delete property from TANGO database ";
	         cout << "for " << tmp_device << "/" << tmp_name << " (error=" << mysql_error(&mysql) << ")" << endl;
	         sprintf(error_mess,"failed to delete property from TANGO database  for %s/%s (error=%s)",tmp_device,tmp_name,mysql_error(&mysql));
	         throw_exception((const char *)error_mess, (const char *)"DataBase::PutDeviceProperty()",
		                 Tango_DevErr_DataBase ,Tango_DATABASE);
	      }

// then insert the new value for this tuple

	      sprintf(sql_query,"INSERT INTO property_device VALUES (\'%s\',\'%s\',\'%s\',\'%s\',NULL,NULL)",
	              tmp_device, tmp_name, tmp_count_str, tmp_value);
	      cout4 << "DataBase::PutDeviceProperty(): sql_query " << sql_query << endl;

	      if (mysql_real_query(&mysql, sql_query, strlen(sql_query)) != 0)
	      {
	         cout << "DataBase::PutDeviceProperty(): failed to insert property into TANGO database ";
	         cout << "for " << tmp_device << "/" << tmp_name << " (error=" << mysql_error(&mysql) << ")" << endl;
	         sprintf(error_mess,"failed to insert property into TANGO database  for %s/%s (error=%s)",tmp_device,tmp_name,mysql_error(&mysql));
	         throw_exception((const char *)error_mess, (const char *)"DataBase::PutDeviceProperty()",
		                 Tango_DevErr_DataBase ,Tango_DATABASE);
	      }
	   }
	   k = k+n_rows+2;
	}

	return;
}
//+----------------------------------------------------------------------------
//
// method : 		DataBase::GetAttributeProperty(Tango_DevVarStringArray &property_names)
// 
// description : 	command to get the list of attribute properties from the database
//
// in :			char *property_names - list of attribute properties names to query e.g. "device attribute1 attribute2 ..."
//
// out :		Tango__DevVarStringArray - list of attribute properties
//
//-----------------------------------------------------------------------------

Tango_DevVarStringArray *DataBase::GetAttributeProperty(Tango_DevVarStringArray *property_names)
{
	static char sql_query[256];
	static char error_mess[256];
	static char n_attributes_str[256];
	static char n_rows_str[256];
	MYSQL_RES *result;
	MYSQL_ROW row;
	int n_attributes=0, n_rows=0, i, j;
	Tango_DevVarStringArray *property_list = new Tango_DevVarStringArray;
	char *tmp_device, *tmp_attribute, *tmp_name;

	cout1 << "DataBase::GetAttributeProperty(): get " << property_names->length()-1 << " properties for device " << (*property_names)[0] << endl;

	tmp_device = (*property_names)[0];
	sprintf(n_attributes_str, "%d", property_names->length()-2);
	property_list->append(CORBA_string_dup(tmp_device));
	property_list->append(CORBA_string_dup(n_attributes_str));

	for (i=1; i<property_names->length(); i++)
	{
	   tmp_attribute = (*property_names)[i];
	   sprintf(sql_query,"SELECT name,value FROM property_attribute WHERE device = \"%s\" AND attribute LIKE \"%s\" ",
	           tmp_device, tmp_attribute);
	   cout4 << "DataBase::GetAttributeProperty(): sql_query " << sql_query << endl;

	   if (mysql_real_query(&mysql, sql_query, strlen(sql_query)) != 0)
	   {
	      cout << "DataBase::GetAttributeProperty(): failed to query TANGO database for list of properties ";
	      cout << "for " << (*property_names)[0] << "/" << (*property_names)[i] << " (error=" << mysql_error(&mysql) << ")" << endl;
	      sprintf(error_mess,"failed to query TANGO database for list of properties for %s/%s (error=%s)",tmp_device,tmp_attribute,mysql_error(&mysql));
	      throw_exception((const char *)error_mess, (const char *)"DataBase::GetAttributeProperty()",
		              Tango_DevErr_DataBase ,Tango_DATABASE);
	   }

	   if ((result = mysql_store_result(&mysql)) == NULL)
	   {
	      cout << "DataBase::GetAttributeProperty(): mysql_store_result() failed  (error=" << mysql_error(&mysql) << ")" << endl;
	      sprintf(error_mess,"mysql_store_result() failed (error=%s)",mysql_error(&mysql));
	      throw_exception((const char *)error_mess, (const char *)"DataBase::GetAttributeProperty()",
		              Tango_DevErr_DataBase ,Tango_DATABASE);
	   }

	   n_rows = mysql_num_rows(result);
	   cout4 << "DataBase::GetAttributeProperty(): mysql_num_rows() " << n_rows << endl;
	   sprintf(n_rows_str,"%d",n_rows);
	   property_list->append(CORBA_string_dup(tmp_attribute));
           property_list->append(CORBA_string_dup(n_rows_str));
	   if (n_rows > 0)
	   {

	      for (j=0; j<n_rows; j++)
	      {
	         if ((row = mysql_fetch_row(result)) != NULL)
	         {
	            cout4 << "DataBase::GetAttributeProperty(): property[ "<< i << "] count " << row[0] << " value " << row[1] << endl;
	            property_list->append(CORBA_string_dup(row[0]));
	            property_list->append(CORBA_string_dup(row[1]));
	         }
	      }
	      n_attributes += n_rows;
	   }
	   else
	   {
	      property_list->append(CORBA_string_dup(" "));
	   }
	   mysql_free_result(result);
	}

	cout4 << "DataBase::GetDeviceProperty(): property_list->length() "<< property_list->length() << endl;

	return(property_list);
}

//+----------------------------------------------------------------------------
//
// method : 		DataBase::PutAttributeProperty(Tango_DevVarStringArray *property_list)
// 
// description : 	command to put a list of attribute properties into the database
//
// in :			char *property_list - list of properties names to update e.g. "device attribute res2 ..."
//
// out :		void - nothing
//
//-----------------------------------------------------------------------------

void DataBase::PutAttributeProperty(Tango_DevVarStringArray *property_list)
{
	static char sql_query[512];
	static char error_mess[256];
	static char tmp_count_str[256];
	MYSQL_RES *result;
	MYSQL_ROW row;
	int n_attributes, n_properties=0, n_rows=0, i, j, k;
	char *tmp_device, *tmp_attribute, *tmp_name, *tmp_value, *tmp_count;

	sscanf((*property_list)[1],"%d",&n_attributes);
	cout1 << "DataBase::PutAttributeProperty(): put " << n_properties << " properties for device " << (*property_list)[0] << endl;

	k = 2;
	for (i=0; i<n_attributes; i++)
	{
	   tmp_device = (*property_list)[0];
	   tmp_attribute = (*property_list)[k+1];
	   sscanf((*property_list)[k+1], "%d", &n_properties);
	   for (j=k+2; j<k+n_rows+2; j=j+1)
	   {
	      tmp_name = (*property_list)[j];
	      tmp_value = (*property_list)[j+1];

// first delete the tuple (device,name,count) from the property table

	      sprintf(sql_query,"DELETE FROM property_attribute WHERE device=\"%s\" AND attribute=\"%s\" AND name=\"%s\" ",
	              tmp_device, tmp_attribute, tmp_name);
	      cout4 << "DataBase::PutAttributeProperty(): sql_query " << sql_query << endl;
	      if (mysql_real_query(&mysql, sql_query, strlen(sql_query)) != 0)
	      {
	         cout << "DataBase::PutAttributeProperty(): failed to delete property from TANGO database ";
	         cout << "for " << tmp_device << "/" << tmp_attribute << "." << tmp_name << " (error=" << mysql_error(&mysql) << ")" << endl;
	         sprintf(error_mess,"failed to delete property from TANGO database  for %s/%s.%s (error=%s)",tmp_device,tmp_attribute,tmp_name,mysql_error(&mysql));
	         throw_exception((const char *)error_mess, (const char *)"DataBase::PutAttributeProperty()",
		                 Tango_DevErr_DataBase ,Tango_DATABASE);
	      }

// then insert the new value for this tuple

	      sprintf(sql_query,"INSERT INTO property_attribute VALUES (\'%s\',\'%s\',\'%s\',NULL,\'%s\',NULL,NULL)",
	              tmp_device, tmp_attribute, tmp_name, tmp_value);
	      cout4 << "DataBase::PutAttributeProperty(): sql_query " << sql_query << endl;

	      if (mysql_real_query(&mysql, sql_query, strlen(sql_query)) != 0)
	      {
	         cout << "DataBase::PutAttributeProperty(): failed to insert property into TANGO database ";
	         cout << "for " << tmp_device << "/" << tmp_attribute << "." << tmp_name << " (error=" << mysql_error(&mysql) << ")" << endl;
	         sprintf(error_mess,"failed to insert property into TANGO database  for %s/%s.%s (error=%s)",tmp_device,tmp_attribute,tmp_name,mysql_error(&mysql));
	         throw_exception((const char *)error_mess, (const char *)"DataBase::PutAttributeProperty()",
		                 Tango_DevErr_DataBase ,Tango_DATABASE);
	      }
	   }
	   k = k+n_properties*2+2;
	}

	return;
}
//+----------------------------------------------------------------------------
//
// method : 		DataBase::GetClassProperty(Tango_DevVarStringArray &property_names)
// 
// description : 	command to get the list of class properties from the database
//
// in :			char *property_names - list of class properties names to query e.g. "class res1 res2 ..."
//
// out :		Tango__DevVarStringArray - list of properties
//
//-----------------------------------------------------------------------------

Tango_DevVarStringArray *DataBase::GetClassProperty(Tango_DevVarStringArray *property_names)
{
	static char sql_query[256];
	static char error_mess[256];
	static char n_properties_str[256];
	static char n_rows_str[256];
	MYSQL_RES *result;
	MYSQL_ROW row;
	int n_properties=0, n_rows=0, i, j;
	Tango_DevVarStringArray *property_list = new Tango_DevVarStringArray;
	char *tmp_class, *tmp_name;

	cout1 << "DataBase::GetClassProperty(): get " << property_names->length()-1 << " properties for device " << (*property_names)[0] << endl;

	tmp_class = (*property_names)[0];
	sprintf(n_properties_str, "%d", property_names->length()-1);
	property_list->append(CORBA_string_dup(tmp_class));
	property_list->append(CORBA_string_dup(n_properties_str));

	for (i=1; i<property_names->length(); i++)
	{
	   tmp_name = (*property_names)[i];
	   sprintf(sql_query,"SELECT count,value FROM property_class WHERE class = \"%s\" AND name LIKE \"%s\" ORDER BY count",
	           tmp_class, tmp_name);
	   cout4 << "DataBase::GetClassProperty(): sql_query " << sql_query << endl;

	   if (mysql_real_query(&mysql, sql_query, strlen(sql_query)) != 0)
	   {
	      cout << "DataBase::GetClassProperty(): failed to query TANGO database for list of properties ";
	      cout << "for " << (*property_names)[0] << "/" << (*property_names)[i] << " (error=" << mysql_error(&mysql) << ")" << endl;
	      sprintf(error_mess,"failed to query TANGO database for list of properties for %s/%s (error=%s)",tmp_class,tmp_name,mysql_error(&mysql));
	      throw_exception((const char *)error_mess, (const char *)"DataBase::GetClassProperty()",
		              Tango_DevErr_DataBase ,Tango_DATABASE);
	   }

	   if ((result = mysql_store_result(&mysql)) == NULL)
	   {
	      cout << "DataBase::GetClassProperty(): mysql_store_result() failed  (error=" << mysql_error(&mysql) << ")" << endl;
	      sprintf(error_mess,"mysql_store_result() failed (error=%s)",mysql_error(&mysql));
	      throw_exception((const char *)error_mess, (const char *)"DataBase::GetClassProperty()",
		              Tango_DevErr_DataBase ,Tango_DATABASE);
	   }

	   n_rows = mysql_num_rows(result);
	   cout4 << "DataBase::GetClassProperty(): mysql_num_rows() " << n_rows << endl;
	   sprintf(n_rows_str,"%d",n_rows);
	   property_list->append(CORBA_string_dup(tmp_name));
           property_list->append(CORBA_string_dup(n_rows_str));
	   if (n_rows > 0)
	   {

	      for (j=0; j<n_rows; j++)
	      {
	         if ((row = mysql_fetch_row(result)) != NULL)
	         {
	            cout4 << "DataBase::GetClassProperty(): property[ "<< i << "] count " << row[0] << " value " << row[1] << endl;
	            property_list->append(CORBA_string_dup(row[1]));
	         }
	      }
	      n_properties += n_rows;
	   }
	   else
	   {
	      property_list->append(CORBA_string_dup(" "));
	   }
	   mysql_free_result(result);
	}

	cout4 << "DataBase::GetClassProperty(): property_list->length() "<< property_list->length() << endl;

	return(property_list);
}

//+----------------------------------------------------------------------------
//
// method : 		DataBase::PutClassProperty(Tango_DevVarStringArray *property_list)
// 
// description : 	command to put a list of class properties into the database
//
// in :			char *property_list - list of class properties names to update e.g. "class res1 res2 ..."
//
// out :		void - nothing
//
//-----------------------------------------------------------------------------

void DataBase::PutClassProperty(Tango_DevVarStringArray *property_list)
{
	static char sql_query[512];
	static char error_mess[256];
	static char tmp_count_str[256];
	MYSQL_RES *result;
	MYSQL_ROW row;
	int n_properties=0, n_rows=0, i, j, k;
	char *tmp_class, *tmp_name, *tmp_value, *tmp_count;

	sscanf((*property_list)[1],"%d",&n_properties);
	cout1 << "DataBase::PutClassProperty(): put " << n_properties << " properties for device " << (*property_list)[0] << endl;

	k = 2;
	tmp_class = (*property_list)[0];
	for (i=0; i<n_properties; i++)
	{
	   tmp_count = 0;
	   tmp_name = (*property_list)[k];
	   sscanf((*property_list)[k+1], "%d", &n_rows);
	   for (j=k+2; j<k+n_rows+2; j++)
	   {
	      tmp_value = (*property_list)[j];
	      tmp_count++; sprintf(tmp_count_str, "%d", tmp_count);

// first delete the tuple (device,name,count) from the property table

	      sprintf(sql_query,"DELETE FROM property_class WHERE class=\"%s\" AND name=\"%s\" AND count=\"%s\"",
	              tmp_class, tmp_name, tmp_count_str);
	      cout4 << "DataBase::PutClassProperty(): sql_query " << sql_query << endl;
	      if (mysql_real_query(&mysql, sql_query, strlen(sql_query)) != 0)
	      {
	         cout << "DataBase::PutClassProperty(): failed to delete property from TANGO database ";
	         cout << "for " << tmp_class << "/" << tmp_name << " (error=" << mysql_error(&mysql) << ")" << endl;
	         sprintf(error_mess,"failed to delete property from TANGO database  for %s/%s (error=%s)",tmp_class,tmp_name,mysql_error(&mysql));
	         throw_exception((const char *)error_mess, (const char *)"DataBase::PutClassProperty()",
		                 Tango_DevErr_DataBase ,Tango_DATABASE);
	      }

// then insert the new value for this tuple

	      sprintf(sql_query,"INSERT INTO property_class VALUES (\'%s\',\'%s\',\'%s\',\'%s\',NULL,NULL)",
	              tmp_class, tmp_name, tmp_count_str, tmp_value);
	      cout4 << "DataBase::PutClassProperty(): sql_query " << sql_query << endl;

	      if (mysql_real_query(&mysql, sql_query, strlen(sql_query)) != 0)
	      {
	         cout << "DataBase::PutClassProperty(): failed to insert property into TANGO database ";
	         cout << "for " << tmp_class << "/" << tmp_name << " (error=" << mysql_error(&mysql) << ")" << endl;
	         sprintf(error_mess,"failed to insert property into TANGO database  for %s/%s (error=%s)",tmp_class,tmp_name,mysql_error(&mysql));
	         throw_exception((const char *)error_mess, (const char *)"DataBase::PutClassProperty()",
		                 Tango_DevErr_DataBase ,Tango_DATABASE);
	      }
	   }
	   k = k+n_rows+2;
	}

	return;
}
//+----------------------------------------------------------------------------
//
// method : 		DataBase::AddServer(Tango_DevVarStringArray *server_device_list)
// 
// description : 	command to insert a device server with a list of devices
//			into the TANGO database
//
// in :			char *server_device_list - list of server and device names to insert e.g. "server dev1 dev2 dev3 ..."
//
// out :		void - nothing
//
//-----------------------------------------------------------------------------

void DataBase::AddServer(Tango_DevVarStringArray *server_device_list)
{
	static char sql_query[512];
	static char error_mess[256];
	static char tmp_count_str[256];
	MYSQL_RES *result;
	MYSQL_ROW row;
	int n_rows=0;
	char *tmp_server, *tmp_device;

	if (server_device_list->length() != 2)
	{
	   cout << "DataBase::AddServer(): incorrect number of input arguments " << endl;
	   sprintf(error_mess,"incorrect no. of input arguments, needs 2 (servers,device)");
	   throw_exception((const char *)error_mess, (const char *)"DataBase::AddServer()",
		                Tango_DevErr_DataBase ,Tango_DATABASE);
	}
		
	cout1 << "DataBase::AddServer(): insert " << (*server_device_list)[0] << " server with device " << (*server_device_list)[1] << endl;
	tmp_server = (*server_device_list)[0];
	tmp_device = (*server_device_list)[1];

// first delete the tuple (device,name,count) from the property table

	sprintf(sql_query,"DELETE FROM device WHERE server=\"%s\" AND name=\"%s\" ",
	              tmp_server,tmp_device);
	cout4 << "DataBase::AddServer(): sql_query " << sql_query << endl;
	if (mysql_real_query(&mysql, sql_query, strlen(sql_query)) != 0)
	{
	   cout << "DataBase::AddServer(): failed to delete server from TANGO database ";
	   cout << "for " << tmp_server << " " << tmp_device << " (error=" << mysql_error(&mysql) << ")" << endl;
	   sprintf(error_mess,"failed to delete server from TANGO database  for %s %s (error=%s)",tmp_server,tmp_device,mysql_error(&mysql));
	   throw_exception((const char *)error_mess, (const char *)"DataBase::AddServer()",
	                 Tango_DevErr_DataBase ,Tango_DATABASE);
	}

// then insert the new value for this tuple

	sprintf(sql_query,"INSERT INTO device VALUES (\"%s\",0,\"nada\",\"nada\",\"%s\",0,\"nada\",0,NULL,NULL)",
	         tmp_device, tmp_server);
	cout4 << "DataBase::AddServer(): sql_query " << sql_query << endl;

	if (mysql_real_query(&mysql, sql_query, strlen(sql_query)) != 0)
	{
	      cout << "DataBase::AddServer(): failed to insert server into TANGO database ";
	      cout << "for " << tmp_server << " " << tmp_device << " (error=" << mysql_error(&mysql) << ")" << endl;
	      sprintf(error_mess,"failed to insert server into TANGO database  for %s/%s (error=%s)",tmp_server,tmp_device,mysql_error(&mysql));
	      throw_exception((const char *)error_mess, (const char *)"DataBase::AddServer()",
	                 Tango_DevErr_DataBase ,Tango_DATABASE);
	}

	return;
}

//+----------------------------------------------------------------------------
//
// method : 		DataBase::AddDevice(Tango_DevVarStringArray *server_device)
// 
// description : 	command to insert a device into the TANGO database
//
// in :			char *server_device - server, device name + class to insert e.g. "server device"
//
// out :		void - nothing
//
//-----------------------------------------------------------------------------

void DataBase::AddDevice(Tango_DevVarStringArray *server_device)
{
	static char sql_query[512];
	static char error_mess[256];
	static char tmp_count_str[256];
	MYSQL_RES *result;
	MYSQL_ROW row;
	int n_rows=0;
	char *tmp_server, *tmp_device, *tmp_class;

	if (server_device->length() != 3)
	{
	   cout << "DataBase::AddServer(): incorrect number of input arguments " << endl;
	   sprintf(error_mess,"incorrect no. of input arguments, needs 3 (server,device,class)");
	   throw_exception((const char *)error_mess, (const char *)"DataBase::AddServer()",
		                Tango_DevErr_DataBase ,Tango_DATABASE);
	}
		
	cout1 << "DataBase::AddDevice(): insert " << (*server_device)[0] << " server with device " << (*server_device)[1] << endl;
	tmp_server = (*server_device)[0];
	tmp_device = (*server_device)[1];
	tmp_class = (*server_device)[2];

// first delete the tuple (device,name) from the device table

	sprintf(sql_query,"DELETE FROM device WHERE server=\"%s\" AND name=\"%s\"",
	              tmp_server,tmp_device);
	cout4 << "DataBase::AddDevice(): sql_query " << sql_query << endl;
	if (mysql_real_query(&mysql, sql_query, strlen(sql_query)) != 0)
	{
	   cout << "DataBase::AddDevice(): failed to delete device from TANGO database ";
	   cout << "for " << tmp_server << " " << tmp_device << " (error=" << mysql_error(&mysql) << ")" << endl;
	   sprintf(error_mess,"failed to delete device from TANGO database  for %s %s (error=%s)",tmp_server,tmp_device,mysql_error(&mysql));
	   throw_exception((const char *)error_mess, (const char *)"DataBase::AddDevice()",
	                 Tango_DevErr_DataBase ,Tango_DATABASE);
	}

// then insert the new value for this tuple

	sprintf(sql_query,"INSERT INTO device VALUES (\"%s\",0,\"nada\",\"nada\",\"%s\",0,\"%s\",\"0\",NULL,NULL)",tmp_device, tmp_server,tmp_class);
	cout4 << "DataBase::AddServer(): sql_query " << sql_query << endl;

	if (mysql_real_query(&mysql, sql_query, strlen(sql_query)) != 0)
	{
	      cout << "DataBase::AddDevice(): failed to insert device into TANGO database ";
	      cout << "for " << tmp_server << " " << tmp_device << " (error=" << mysql_error(&mysql) << ")" << endl;
	      sprintf(error_mess,"failed to insert device into TANGO database  for %s/%s (error=%s)",tmp_server,tmp_device,mysql_error(&mysql));
	      throw_exception((const char *)error_mess, (const char *)"DataBase::AddDevice()",
	                 Tango_DevErr_DataBase ,Tango_DATABASE);
	}

// make sure the device server has an DServer device entry

	sprintf(sql_query,"DELETE FROM device WHERE server=\"%s\" AND class=\"DServer\"",
	              tmp_server);
	cout4 << "DataBase::AddDevice(): sql_query " << sql_query << endl;
	if (mysql_real_query(&mysql, sql_query, strlen(sql_query)) != 0)
	{
	   cout << "DataBase::AddDevice(): failed to delete DServer from TANGO database ";
	   cout << "for " << tmp_server << " (error=" << mysql_error(&mysql) << ")" << endl;
	   sprintf(error_mess,"failed to delete DServer from TANGO database  for %s (error=%s)",tmp_server,mysql_error(&mysql));
	   throw_exception((const char *)error_mess, (const char *)"DataBase::AddDevice()",
	                 Tango_DevErr_DataBase ,Tango_DATABASE);
	}
	sprintf(sql_query,"INSERT INTO device VALUES (\"dserver/%s\",0,\"nada\",\"nada\",\"%s\",0,\"DServer\",\"0\",NULL,NULL)",tmp_server, tmp_server,tmp_class);
	cout4 << "DataBase::AddServer(): sql_query " << sql_query << endl;

	if (mysql_real_query(&mysql, sql_query, strlen(sql_query)) != 0)
	{
	      cout << "DataBase::AddDevice(): failed to insert DServer into TANGO database ";
	      cout << "for " << tmp_server << " (error=" << mysql_error(&mysql) << ")" << endl;
	      sprintf(error_mess,"failed to insert DServer into TANGO database  for %s (error=%s)",tmp_server,mysql_error(&mysql));
	      throw_exception((const char *)error_mess, (const char *)"DataBase::AddDevice()",
	                 Tango_DevErr_DataBase ,Tango_DATABASE);
	}

	return;
}
//+----------------------------------------------------------------------------
//
// method : 		DataBase::State()
// 
// description : 	command to read the device state
//
// out :		device state
//
//-----------------------------------------------------------------------------

Tango_DevState DataBase::State()
{

	cout4 << "DataBase state command" << endl;
	
	return dev_state;
}

//+----------------------------------------------------------------------------
//
// method : 		DataBase::Status()
// 
// description : 	command to read the device status
//
// out :		device status
//
//-----------------------------------------------------------------------------

string &DataBase::Status()
{

	cout4 << "DataBase status command" << endl;
	
	return dev_status;
}
