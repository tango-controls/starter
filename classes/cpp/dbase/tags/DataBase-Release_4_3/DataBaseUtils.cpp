static const char *RcsId = "$Header$";
//+=============================================================================
//
// file :         DataBaseUtils.cpp
//
// description :  Various utility for the DataBase.
//
// project :      TANGO Device Servers
//
// author(s) :    A.Gotz, P.Verdier, JL Pons
//
//
// copyleft :     European Synchrotron Radiation Facility
//                BP 220, Grenoble 38043
//                FRANCE
//

#include <tango.h>
#include <DataBase.h>
#include <DataBaseClass.h>

#include <stdio.h>

using namespace std;

namespace DataBase_ns {

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
		if (index==0)	//	Cannot have '\' at -1 !
		{
			escaped_string.insert(index, 1, '\\');
			index += 2;
		}
		else
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
	}


	//	escape '
	index = 0;
	while ((index = escaped_string.find('\'',index)) != string::npos)
	{
		if (index==0)	//	Cannot have '\' at -1 !
		{
			escaped_string.insert(index, 1, '\\');
			index += 2;
		}
		else
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
	}

	DEBUG_STREAM << "DataBase::escaped_string() wildcard out : " << escaped_string << endl;
	return escaped_string;
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

//+------------------------------------------------------------------
/**
 *	method:	DataBase::init_timing_stats
 *
 *	description:	method to initialise the timing statistics variables
 *	Returns void
 *
 * @param	none
 * @return	void
 *
 */
//+------------------------------------------------------------------
void DataBase::init_timing_stats()
{
	TimingStatsStruct new_timing_stats = {0.0, 0.0, 0.0, 0.0, 0.0};

	timing_stats_mutex.lock();
	
	timing_stats_map["DbImportDevice"] = new TimingStatsStruct;
	timing_stats_map["DbExportDevice"] = new TimingStatsStruct;
	timing_stats_map["DbGetHostServerList"] = new TimingStatsStruct;
	timing_stats_map["DbGetHostList"] = new TimingStatsStruct;
	timing_stats_map["DbGetServerList"] = new TimingStatsStruct;
	timing_stats_map["DbGetDevicePropertyList"] = new TimingStatsStruct;
	timing_stats_map["DbGetClassPropertyList"] = new TimingStatsStruct;
	timing_stats_map["DbGetDeviceMemberList"] = new TimingStatsStruct;
	timing_stats_map["DbGetDeviceFamilyList"] = new TimingStatsStruct;
	timing_stats_map["DbGetDeviceDomainList"] = new TimingStatsStruct;
	timing_stats_map["DbGetDeviceProperty"] = new TimingStatsStruct;
	timing_stats_map["DbPutDeviceProperty"] = new TimingStatsStruct;
	timing_stats_map["DbDeleteDeviceProperty"] = new TimingStatsStruct;
	timing_stats_map["DbInfo"] = new TimingStatsStruct;
	timing_stats_map["DbGetDeviceClassList"] = new TimingStatsStruct;
	timing_stats_map["DbGetDeviceAttributeProperty"] = new TimingStatsStruct;
	timing_stats_map["DbPutDeviceAttributeProperty"] = new TimingStatsStruct;
	timing_stats_map["DbGetDeviceAttributeProperty2"] = new TimingStatsStruct;
	timing_stats_map["DbPutDeviceAttributeProperty2"] = new TimingStatsStruct;
	timing_stats_map["DbUnExportServer"] = new TimingStatsStruct;
	timing_stats_map["DbGetDeviceExportedList"] = new TimingStatsStruct;
	timing_stats_map["DbExportEvent"] = new TimingStatsStruct;
	timing_stats_map["DbImportEvent"] = new TimingStatsStruct;
	timing_stats_map["DbGetDataForServerCache"] = new TimingStatsStruct;
	timing_stats_map["DbPutClassProperty"] = new TimingStatsStruct;

	timing_stats_size = timing_stats_map.size();
	timing_stats_average = new double[timing_stats_size];
	timing_stats_minimum = new double[timing_stats_size];
	timing_stats_maximum = new double[timing_stats_size];
	timing_stats_calls = new double[timing_stats_size];
	timing_stats_index = new Tango::DevString[timing_stats_size];

	//	now loop over map and initialise remaining variables
	std::map<std::string,TimingStatsStruct*>::iterator iter;
	int i=0;

	for (iter = timing_stats_map.begin(); iter != timing_stats_map.end(); iter++)
	{
		*(iter->second) = new_timing_stats;
		timing_stats_index[i] = (char*)malloc(strlen(iter->first.c_str())+1);
		strcpy(timing_stats_index[i],iter->first.c_str());
		i++;
	}
	
	timing_stats_mutex.unlock();
}

//+----------------------------------------------------------------------------
//
// method : 		DataBase::get_id()
// 
// description : 	Return history id
//
//-----------------------------------------------------------------------------
void DataBase::check_history_tables()
{
	TangoSys_MemStream	sql_query_stream;
	MYSQL_RES *result;

	INFO_STREAM << "DataBase::check_history_tables(): entering" << endl;

	sql_query_stream.str("");
	sql_query_stream << "SELECT count(*) FROM property_device_hist";
	DEBUG_STREAM << "DataBase::check_history_tables(): sql_query " << sql_query_stream.str() << endl;
	result = query(sql_query_stream.str(),"check_history_tables()");
	mysql_free_result(result);
}

//+----------------------------------------------------------------------------
//
// method : 		DataBase::get_id()
// 
// description : 	Return history id
//					In this method, we don't use the classical query()
//					method in order to be sure that the UPDATE and the following
//					mysql_insert_id() are done using the same MySQL connection
//
//-----------------------------------------------------------------------------

unsigned int DataBase::get_id(const char *name,int con_nb) 
{
	TangoSys_MemStream sql_query;

//
// If no MySQL connection passed to this method,
// get one
//

	bool need_release = false;
	
	if (con_nb == -1)
	{
		con_nb = get_connection();
		need_release = true;
	}

    sql_query.str("");
    sql_query << "UPDATE " << name << "_history_id SET id=LAST_INSERT_ID(id+1)";
	string tmp_str = sql_query.str();

	if (mysql_real_query(conn_pool[con_nb].db, tmp_str.c_str(),tmp_str.length()) != 0)
	{
		TangoSys_OMemStream o;
		  
		WARN_STREAM << "DataBase::get_id() failed to query TANGO database:" << endl;
		WARN_STREAM << "  query = " << tmp_str << endl;
		WARN_STREAM << " (SQL error=" << mysql_error(conn_pool[con_nb].db) << ")" << endl;
		
		o << "Failed to query TANGO database (error=" << mysql_error(conn_pool[con_nb].db) << ")" << ends;

		if (need_release == true)
			release_connection(con_nb);
			
		Tango::Except::throw_exception((const char *)DB_SQLError,o.str(),(const char *)"DataBase::get_id()");
	}

	my_ulonglong val = mysql_insert_id(conn_pool[con_nb].db);
	if (val == 0)
	{
		TangoSys_OMemStream o;

		o << "Failed to get history id : " << name;
		
		if (need_release == true)
			release_connection(con_nb);
			
		Tango::Except::throw_exception((const char *)DB_SQLError,o.str(),(const char *)"DataBase::get_id()");	
	}

	if (need_release == true)
		release_connection(con_nb);

	return val;
}

//+----------------------------------------------------------------------------
//
// method : 		DataBase::simple_query()
// 
// description : 	Execute a SQL query , ignore the result.
//
//-----------------------------------------------------------------------------
void DataBase::simple_query(string sql_query,const char *method,int con_nb)
{
	
//
// If no MySQL connection passed to this method,
// get one
//

	bool need_release = false;
	
	if (con_nb == -1)
	{
		con_nb = get_connection();
		need_release = true;
	}

	DEBUG_STREAM << "Using MySQL connection with semaphore " << con_nb << endl;	

//
// Call MySQL
//

	if (mysql_real_query(conn_pool[con_nb].db, sql_query.c_str(),sql_query.length()) != 0)
	{
		TangoSys_OMemStream o;
		TangoSys_OMemStream o2;	 
		  
		WARN_STREAM << "DataBase::" << method << " failed to query TANGO database:" << endl;
		WARN_STREAM << "  query = " << sql_query << endl;
		WARN_STREAM << " (SQL error=" << mysql_error(conn_pool[con_nb].db) << ")" << endl;
		
		o << "Failed to query TANGO database (error=" << mysql_error(conn_pool[con_nb].db) << ")";
		o << "\n.The query was: " << sql_query << ends;
		o2 << "DataBase::" << method << ends;

		if (need_release == true)
			release_connection(con_nb);
				
		Tango::Except::throw_exception((const char *)DB_SQLError,o.str(),o2.str());
	}

	if (need_release)
		release_connection(con_nb);

}

//+----------------------------------------------------------------------------
//
// method : 		DataBase::query()
// 
// description : 	Execute a SQL query and return the result.
//
//-----------------------------------------------------------------------------
MYSQL_RES *DataBase::query(string sql_query,const char *method,int con_nb)
{
	MYSQL_RES *result;

//
// If no MySQL connection passed to this method,
// get one
//

	bool need_release = false;
	
	if (con_nb == -1)
	{
		con_nb = get_connection();
		need_release = true;
	}

	DEBUG_STREAM << "Using MySQL connection with semaphore " << con_nb << endl;
		
//
// Call MySQL
//
		
	if (mysql_real_query(conn_pool[con_nb].db, sql_query.c_str(),sql_query.length()) != 0)
	{
		TangoSys_OMemStream o;
		TangoSys_OMemStream o2;
	
		WARN_STREAM << "DataBase::" << method << " failed to query TANGO database:" << endl;
		WARN_STREAM << "  query = " << sql_query << endl;
		WARN_STREAM << " (SQL error=" << mysql_error(conn_pool[con_nb].db) << ")" << endl;
		
		o << "Failed to query TANGO database (error=" << mysql_error(conn_pool[con_nb].db) << ")";
		o << "\nThe query was: " << sql_query << ends;
		o2 << "DataBase::" << method << ends;

		if (need_release == true)
			release_connection(con_nb);
		
		Tango::Except::throw_exception((const char *)DB_SQLError,o.str(),o2.str());
	}

	if ((result = mysql_store_result(conn_pool[con_nb].db)) == NULL)
	{
		TangoSys_OMemStream o;
		TangoSys_OMemStream o2;
		
		WARN_STREAM << "DataBase:: " << method << " : mysql_store_result() failed  (error=" << mysql_error(conn_pool[con_nb].db) << ")" << endl;
	   
		o << "mysql_store_result() failed (error=" << mysql_error(conn_pool[con_nb].db) << ")";
		o2 << "DataBase::" << method;	   

		if (need_release == true)
			release_connection(con_nb);
		
		Tango::Except::throw_exception((const char *)DB_SQLError,o.str(),o2.str());
	}

	if (need_release)
		release_connection(con_nb);
		
	return result;

}

//+----------------------------------------------------------------------------
//
// method : 		DataBase::get_connection()
// 
// description : 	Get a MySQl connection from the connection pool
//
//-----------------------------------------------------------------------------
int DataBase::get_connection()
{
	
//
// Get a MySQL connection and lock it
// If none available, wait for one
//

	int loop = 0;
	while (conn_pool[loop].the_sema.trywait() == 0)
	{
		loop++;
		if (loop == conn_pool_size)
		{
			int sem_to_wait;
			{
				omni_mutex_lock oml(sem_wait_mutex);
				sem_to_wait = last_sem_wait++;
				if (last_sem_wait == conn_pool_size)
					last_sem_wait = 0;
			}
			loop = sem_to_wait;
			WARN_STREAM << "Waiting for one free MySQL connection on semaphore " << loop << endl;
			conn_pool[loop].the_sema.wait();
			break;
		}
	}
	
	return loop;
}


//+------------------------------------------------------------------
/**
 *	method:	purge_property()
 *
 *	description:	purge a property history table.
 *
 */
//+------------------------------------------------------------------
void DataBase::purge_property(const char *table,const char *field,const char *object,const char *name,int con_nb) {

  TangoSys_MemStream sql_query;
  MYSQL_RES *result;
  MYSQL_ROW row2;
  int j;
  
  TimeVal before, after;
  GetTime(before);
	
  sql_query.str("");
  sql_query << "SELECT DISTINCT id FROM " << table
            << " WHERE " << field << "=\"" << object << "\" AND name=\"" << name 
            << "\" ORDER by date";
	    
  result = query(sql_query.str(),"purge_property()",con_nb);
  int nb_item = mysql_num_rows(result);

  GetTime(after);
  double time_elapsed = Elapsed(before, after);
  //cout << "purge_property(select DISTINCT) : " << time_elapsed << endl;

  GetTime(before);

  if(nb_item>historyDepth) {
    // Purge 
    int toDelete = nb_item-historyDepth;
    for(j=0;j<toDelete;j++) {
        row2 = mysql_fetch_row(result);
        sql_query.str("");
        sql_query << "DELETE FROM " << table << " WHERE id='" << row2[0] << "'";
		simple_query(sql_query.str(),"purge_property()",con_nb);
    }
  }
  
  mysql_free_result(result);
  
  GetTime(after);
  time_elapsed = Elapsed(before, after);
  //cout << "purge_property(DELETE) : " << time_elapsed << endl;

}

//+------------------------------------------------------------------
/**
 *	method:	purge_attribute_property()
 *
 *	description:	purge an attribute property history table.
 *
 */
//+------------------------------------------------------------------
void DataBase::purge_att_property(const char *table,const char *field,const char *object,const char *attribute,const char *name,int con_nb) {

  TangoSys_MemStream sql_query;
  MYSQL_RES *result;
  MYSQL_ROW row2;
  int j;
  
  //cout << "purge_att_property(" << object << "," << attribute << "," << name << ")" << endl;

  TimeVal before, after;
  GetTime(before);
		
  sql_query.str("");
  sql_query << "SELECT DISTINCT id FROM " << table
            << " WHERE " << field << "=\"" << object << "\" AND name=\"" << name 
            << "\" AND attribute=\"" << attribute << "\" ORDER by date";

  result = query(sql_query.str(),"purge_att_property()",con_nb);
  int nb_item = mysql_num_rows(result);

  GetTime(after);
  double time_elapsed = Elapsed(before, after);
  //cout << "  select DISTINCT : " << time_elapsed << endl;

  GetTime(before);
  
  if(nb_item>historyDepth) {
    // Purge 
    int toDelete = nb_item-historyDepth;
    for(j=0;j<toDelete;j++) {
		row2 = mysql_fetch_row(result);
        sql_query.str("");
        sql_query << "DELETE FROM " << table << " WHERE id='" << row2[0] << "'";
		simple_query(sql_query.str(),"purge_att_property()",con_nb);
    }
  }
  mysql_free_result(result);

  GetTime(after);
  time_elapsed = Elapsed(before, after);
  //cout << "  DELETE : " << time_elapsed << endl;
      
}

//+------------------------------------------------------------------
/**
 *	method:	create_connection_pool()
 *
 *	description:	Create the MySQL connections pool
 *
 */
//+------------------------------------------------------------------

void DataBase::create_connection_pool(const char *mysql_user,const char *mysql_password)
{
	char *database = (char *)"tango";

	if (mysql_user != NULL && mysql_password != NULL)
	{	
		WARN_STREAM << "DataBase::create_connection_pool(): mysql database user =  " << mysql_user 
	           	 << " , password = " << mysql_password << endl;
	}
					
	for (int loop = 0;loop < conn_pool_size;loop++)
	{
	
// Initialise mysql database structure and connect to TANGO database

		conn_pool[loop].db = mysql_init(conn_pool[loop].db);
		mysql_options(conn_pool[loop].db,MYSQL_READ_DEFAULT_GROUP,"client");


#if   (MYSQL_VERSION_ID > 50000)

	//C. Scafuri: auto reconnection has been off since 5.0.3. From 5.0.13 it is possible to set it as an option
	// with reconnection enabled DataBase keeps working after timeouts and mysql shutdown/restart
		if(mysql_get_client_version() >= 50013)
		{
			my_bool my_auto_reconnect=1;
			if (mysql_options(conn_pool[loop].db,MYSQL_OPT_RECONNECT,&my_auto_reconnect) !=0)
				ERROR_STREAM << "DataBase: error setting mysql auto reconnection: " << mysql_error(conn_pool[loop].db) << endl;
			else
				WARN_STREAM << "DataBase: set mysql auto reconnect to true" << endl;
		}
#endif


		if (!mysql_real_connect(conn_pool[loop].db, NULL, mysql_user, mysql_password, database, 0, NULL, CLIENT_MULTI_STATEMENTS))
		{
			TangoSys_MemStream out_stream;
			out_stream << "Failed to connect to TANGO database (error = " << mysql_error(conn_pool[loop].db) << ")" << ends;
			
			Tango::Except::throw_exception((const char *)"CANNOT_CONNECT_MYSQL",
											out_stream.str(),
											(const char *)"DataBase::init_device()");
		}
	}

	mysql_svr_version = mysql_get_server_version(conn_pool[0].db);
	last_sem_wait = 0;
	
}

//+------------------------------------------------------------------
/**
 *	method:	AutoLock class ctor and dtor
 *
 *	description:	AutoLock is a small helper class which get a
 *					MySQL connection from the pool and which lock
 *					table(s). The exact lock statemen is passed to the
 *					ctor as a parameter
 *					The dtor release the table(s) lock
 *
 */
//+------------------------------------------------------------------

AutoLock::AutoLock(const char *lock_cmd,DataBase *db):the_db(db)
{
	con_nb = the_db->get_connection();
	TangoSys_MemStream sql_query_stream;
	
	sql_query_stream << lock_cmd;
	try
	{
		the_db->simple_query(sql_query_stream.str(),"AutoLock",con_nb);
	}
	catch (...)
	{
		the_db->release_connection(con_nb);
		throw;
	}
}

AutoLock::~AutoLock()
{
	TangoSys_MemStream sql_query_stream;
	sql_query_stream << "UNLOCK TABLES";
	the_db->simple_query(sql_query_stream.str(),"~AutoLock",con_nb);
	the_db->release_connection(con_nb);
}

}
