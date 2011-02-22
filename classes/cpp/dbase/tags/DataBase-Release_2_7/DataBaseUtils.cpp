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
#include <mysql.h>
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
//			cout << "init_timing_stats(): index[" << i << "] = " << timing_stats_index[i] << endl;
		i++;
	}
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
//
//-----------------------------------------------------------------------------
unsigned int DataBase::get_id(const char *name) {

	TangoSys_MemStream sql_query;
	TangoSys_OMemStream o;
  	MYSQL_RES *result;
	MYSQL_ROW row;

    // Get id
    sql_query.str("");
    sql_query << "SELECT id FROM history_ids WHERE name='" << name << "'";
	result = query(sql_query.str(),"get_id()");
	if(mysql_num_rows(result)==0) {
       mysql_free_result(result);
	   o << "Failed to get history id : " << name;
	   Tango::Except::throw_exception((const char *)DB_SQLError,o.str(),"DataBase::get_id()");	
	}
	row = mysql_fetch_row(result);
    unsigned int ret = atoi(row[0]);
    mysql_free_result(result);

    // Increment id
	sql_query.str("");
    sql_query << "UPDATE history_ids SET id = id + 1 WHERE name='" << name << "'";
	simple_query(sql_query.str(),"get_id()");

	return ret;			
}

//+----------------------------------------------------------------------------
//
// method : 		DataBase::simple_query()
// 
// description : 	Execute a SQL query , ignore the result.
//
//-----------------------------------------------------------------------------
void DataBase::simple_query(string sql_query,const char *method) {
	
	TangoSys_OMemStream o;
	TangoSys_OMemStream o2;
	
	if (mysql_real_query(&mysql, sql_query.c_str(),sql_query.length()) != 0)
	{
	   WARN_STREAM << "DataBase::" << method << " failed to query TANGO database:" << endl;
	   WARN_STREAM << "  query = " << sql_query << endl;
	   WARN_STREAM << " (SQL error=" << mysql_error(&mysql) << ")" << endl;
	   o << "Failed to query TANGO database (error=" << mysql_error(&mysql) << ")";
	   o2 << "DataBase::" << method;
	   Tango::Except::throw_exception((const char *)DB_SQLError,o.str(),o2.str());
	}

}

//+----------------------------------------------------------------------------
//
// method : 		DataBase::query()
// 
// description : 	Execute a SQL query and return the result.
//
//-----------------------------------------------------------------------------
MYSQL_RES *DataBase::query(string sql_query,const char *method) {

	TangoSys_OMemStream o;
	TangoSys_OMemStream o2;
	MYSQL_RES *result;
	
	if (mysql_real_query(&mysql, sql_query.c_str(),sql_query.length()) != 0)
	{
	   WARN_STREAM << "DataBase::" << method << " failed to query TANGO database:" << endl;
	   WARN_STREAM << "  query = " << sql_query << endl;
	   WARN_STREAM << " (SQL error=" << mysql_error(&mysql) << ")" << endl;
	   o << "Failed to query TANGO database (error=" << mysql_error(&mysql) << ")";
	   o2 << "DataBase::" << method;
	   Tango::Except::throw_exception((const char *)DB_SQLError,o.str(),o2.str());
	}

	if ((result = mysql_store_result(&mysql)) == NULL)
	{
	   WARN_STREAM << "DataBase:: " << method << " : mysql_store_result() failed  (error=" << mysql_error(&mysql) << ")" << endl;
	   o << "mysql_store_result() failed (error=" << mysql_error(&mysql) << ")";
	   o2 << "DataBase::" << method;	   
	   Tango::Except::throw_exception((const char *)DB_SQLError,o.str(),o2.str());
	}
	
	return result;

}

}
