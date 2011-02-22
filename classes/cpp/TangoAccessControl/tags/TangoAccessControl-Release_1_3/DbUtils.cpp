static const char *RcsId = "$Header$";
//+=============================================================================
//
// file :         DbUtils.cpp
//
// description :  C++ source for the DbUtils.
//
// project :      TANGO Device Server
//
// $Author$
//
// $Revision$
//
// $Log$
// Revision 1.11  2009/05/11 07:29:10  taurel
// - Now, the my.cnf file is correctly taken into account
//
// Revision 1.10  2009/04/28 16:34:32  taurel
// - Add a mysql_option() call to take the my.cnf file into account
//
// Revision 1.9  2009/04/04 18:46:28  taurel
// Fixed warnings when compiled with gcc 4.3
//
// Revision 1.8  2009/04/04 17:42:36  taurel
// Device now inherits from Device_4Impl.
// Environment variable got from Tango library (To manage tangorc files)
//
// Revision 1.7  2009/02/06 08:07:19  pascal_verdier
// Running on same host than mysql test removed.
//
// Revision 1.6  2008/10/08 09:28:22  pascal_verdier
// GetAllowedCommandClassList command added.
//
// Revision 1.5  2008/09/25 07:38:01  taurel
// - Fix some bugs detected when porting the stuff to C++
//
// Revision 1.4  2006/09/28 08:59:58  pascal_verdier
// Check of host where running added.
//
// Revision 1.3  2006/09/20 07:22:49  pascal_verdier
// Another bug fixed in GetAccess command.
//
// Revision 1.2  2006/09/19 12:38:48  pascal_verdier
// Bug in GetAccess fixed.
//
// Revision 1.1.1.1  2006/09/15 11:55:43  pascal_verdier
// Initial Revision
//
//
// copyleft :     European Synchrotron Radiation Facility
//                BP 220, Grenoble 38043
//                FRANCE
//
//-=============================================================================

#include <tango.h>
#include <TangoAccessControl.h>
#include <TangoAccessControlClass.h>

namespace TangoAccessControl_ns
{


//+------------------------------------------------------------------
/**
 *	Remove the Fully Qualify Domain Name for tango less than 5.2 compatibility
 */
//+------------------------------------------------------------------
string TangoAccessControl::removeFQDN(string s)
{
	string::size_type	pos = s.find('.');
	if (pos == string::npos)
		return s;
	else
		return 	s.substr(0, pos);
}
//+----------------------------------------------------------------------------
//
// method : 		TangoAccessControl::mysql_connection()
// 
// description : 	Execute a SQL query , ignore the result.
//
//-----------------------------------------------------------------------------
void TangoAccessControl::mysql_connection()
{
	// Initialise variables to default values
	//--------------------------------------------
	char *database = (char *)"tango";
	const char *mysql_user = NULL;
	const char *mysql_password = NULL;

	WARN_STREAM << "AccessControl::init_device() create database device " << device_name << endl;

// Initialise mysql database structure and connect to TANGO database

	mysql_init(&mysql);

	DummyDev d;
	string my_user,my_password;
	
	if (d.get_env_var("MYSQL_USER",my_user) != -1)
	{
		mysql_user = my_user.c_str();
	}
	if (d.get_env_var("MYSQL_PASSWORD",my_password) != -1)
	{
		mysql_password = my_password.c_str();
	}
	
	WARN_STREAM << "AccessControl::init_device() mysql database user =  " << mysql_user 
	            << " , password = " << mysql_password << endl;

	mysql_options(&mysql,MYSQL_READ_DEFAULT_GROUP,"client");
	
	if(mysql_get_client_version() >= 50013)
	{
		my_bool my_auto_reconnect=1;
		if (mysql_options(&mysql,MYSQL_OPT_RECONNECT,&my_auto_reconnect) !=0)
			ERROR_STREAM << "AccessControl: error setting mysql auto reconnection: " << mysql_error(&mysql) << endl;
		else
			WARN_STREAM << "AccessControl: set mysql auto reconnect to true" << endl;
	}
		
	if (!mysql_real_connect(&mysql, NULL, mysql_user, mysql_password, database, 0, NULL, 0))
	{
		TangoSys_MemStream out_stream;
		out_stream << "Failed to connect to TANGO database (error = "
         		   << mysql_error(&mysql) << ")" << ends;
		Tango::Except::throw_exception(
					(const char *)"CANNOT_CONNECT_MYSQL",
					out_stream.str(),
					(const char *)"AccessControl::init_device()");
	}
}
//+----------------------------------------------------------------------------
//
// method : 		TangoAccessControl::simple_query()
// 
// description : 	Execute a SQL query , ignore the result.
//
//-----------------------------------------------------------------------------
void TangoAccessControl::simple_query(string sql_query,const char *method)
{
	
	TangoSys_OMemStream o;
	TangoSys_OMemStream o2;
	
	if (mysql_real_query(&mysql, sql_query.c_str(),sql_query.length()) != 0)
	{
	   WARN_STREAM << "TangoAccessControl::" << method << " failed to query TANGO database:" << endl;
	   WARN_STREAM << "  query = " << sql_query << endl;
	   WARN_STREAM << " (SQL error=" << mysql_error(&mysql) << ")" << endl;
	   o << "Failed to query TANGO database (error=" << mysql_error(&mysql) << ")";
	   o2 << "TangoAccessControl::" << method;
	   Tango::Except::throw_exception((const char *)AC_SQLError,o.str(),o2.str());
	}

}

//+----------------------------------------------------------------------------
//
// method : 		TangoAccessControl::query()
// 
// description : 	Execute a SQL query and return the result.
//
//-----------------------------------------------------------------------------
MYSQL_RES *TangoAccessControl::query(string sql_query,const char *method) {

	TangoSys_OMemStream o;
	TangoSys_OMemStream o2;
	MYSQL_RES *result;
	
	if (mysql_real_query(&mysql, sql_query.c_str(),sql_query.length()) != 0)
	{
	   WARN_STREAM << "TangoAccessControl::" << method << " failed to query TANGO database:" << endl;
	   WARN_STREAM << "  query = " << sql_query << endl;
	   WARN_STREAM << " (SQL error=" << mysql_error(&mysql) << ")" << endl;
	   o << "Failed to query TANGO database (error=" << mysql_error(&mysql) << ")";
	   o2 << "TangoAccessControl::" << method;
	   Tango::Except::throw_exception((const char *)AC_SQLError,o.str(),o2.str());
	}

	if ((result = mysql_store_result(&mysql)) == NULL)
	{
	   WARN_STREAM << "TangoAccessControl:: " << method << " : mysql_store_result() failed  (error=" << mysql_error(&mysql) << ")" << endl;
	   o << "mysql_store_result() failed (error=" << mysql_error(&mysql) << ")";
	   o2 << "TangoAccessControl::" << method;	   
	   Tango::Except::throw_exception((const char *)AC_SQLError,o.str(),o2.str());
	}
	
	return result;

}

//============================================================
/**
 *	split device name in domain faily member in a vector.
 */
//============================================================
vector<string> TangoAccessControl::get_dev_members(string &devname)
{
	vector<string>	v;
	string::size_type	pos = devname.find('/');
	string::size_type	pos2 = devname.find('/', pos+1);
	//	domain 
	v.push_back(devname.substr(0, pos));
	pos++;
	//	family 
	v.push_back(devname.substr(pos, pos2-pos));
	pos2++;
	//	member
	v.push_back(devname.substr(pos2, devname.length()-pos2));
	return v;
}
//============================================================
/**
 *	split IP address in members in a vector.
 */
//============================================================
vector<string> TangoAccessControl::get_ip_add_members(string &devname)
{
	vector<string>	v;
	string::size_type	pos = devname.find('.');
	string::size_type	pos1 = devname.find('.', pos+1);
	string::size_type	pos2 = devname.find('.', pos1+1);

	v.push_back(devname.substr(0, pos));
	pos++;
	v.push_back(devname.substr(pos, pos1-pos));
	pos1++;
	v.push_back(devname.substr(pos1, pos2-pos1));
	pos2++;
	v.push_back(devname.substr(pos2, devname.length()-pos2));
	return v;
}

//============================================================
//============================================================
vector<AccessStruct>
	TangoAccessControl::get_access_for_user_address(string &user, string &ip_add)
{
	vector<string>			v_add = get_ip_add_members(ip_add);
	vector<AccessStruct>	as_read;

	TangoSys_MemStream	sql_query_stream;
	
	//	First, check if something defined for user.
	sql_query_stream << "SELECT count(*) FROM access_address WHERE user=\"" << user << "\"";
	MYSQL_RES *res = query(sql_query_stream.str(), "ac_get_device_by_user()");
	MYSQL_ROW ro = mysql_fetch_row(res);

	sql_query_stream.str("");
	sql_query_stream << 
			"SELECT DISTINCT address FROM access_address WHERE ";
	if ((ro[0])[0] != '0')
	{
		//	Something found.
		//	User definition
		sql_query_stream << 
				"(user=\"" << user << "\")  AND  ";
	}
	else
	{
		//	User definition
		sql_query_stream << 
				"(user=\"" << user << "\" OR user=\"*\")  AND  ";
	}	
	//	IP address definition
	sql_query_stream << 
		"(address=\"*.*.*.*\"  OR  "                            <<
		"address=\"" << v_add[0] << ".*.*.*\" OR "              <<
		"address=\"" << v_add[0] << "." << v_add[1] << ".*.*\" OR " <<
		"address=\"" << v_add[0] << "." << v_add[1] << 
				 "." << v_add[2] <<".*\" OR " <<
		"address=\"" << ip_add << "\" )  ORDER BY address DESC";

	//cout << "ac_get_access(): sql_query " << sql_query_stream.str() << endl;
	MYSQL_RES *result = query(sql_query_stream.str(), "ac_get_device_by_user()");
	int	n_rows = mysql_num_rows(result);
	if (n_rows > 0)
	{
		for (int i=0; i<n_rows; i++)
		{
			MYSQL_ROW	row = mysql_fetch_row(result);
			if (row != NULL)
			{
				AccessStruct	acs;
				acs.user    = user;
				acs.address = row[0];
				as_read.push_back(acs);
			}
		}
	}
	mysql_free_result(result);
	mysql_free_result(res);
	
	return as_read;
}
//============================================================
//============================================================
string TangoAccessControl::get_access_for_user_device(string &user, string &device)
{
	vector<string>			v_dev = get_dev_members(device);
	string	retval("read");

	TangoSys_MemStream	sql_query_stream;
	sql_query_stream << 
		"SELECT DISTINCT user,device,rights FROM access_device WHERE "  <<
			//	User definition
				"(user=\"" << user << "\" OR user=\"*\")  AND  "    <<
			//	Device definition
				"(device=\"*/*/*\" OR "                                 <<
				"device=\"" << v_dev[0] << "/*/*\" OR "                 <<
				"device=\"" << v_dev[0] << "/" << v_dev[1] << "/*\" OR "<<
				"device=\"" << device << "\")  ORDER BY device";
	
	//cout << "ac_get_access(): sql_query " << sql_query_stream.str() << endl;
	MYSQL_RES *result = query(sql_query_stream.str(), "ac_get_device_by_user()");
	int	n_rows = mysql_num_rows(result);

	vector<AccessStruct>	as_user;
	vector<AccessStruct>	as_all;
	if (n_rows > 0)
	{
		for (int i=0; i<n_rows; i++)
		{
			MYSQL_ROW	row = mysql_fetch_row(result);
			if (row != NULL)
			{
				AccessStruct	acs;
				acs.user   = row[0];
				acs.device = row[1];
				acs.rights = row[2];
				/*
				cout << "		object[" << i << "} : " << acs.user
					<< "|" << acs.device
					<< "|" << acs.rights << endl;
				*/

				string	tmp(row[0]);
				if (tmp==user)
					as_user.push_back(acs);
				else
					as_all.push_back(acs);
			}
		}
	}
	mysql_free_result(result);

	if (as_user.size()==0 && as_all.size()==0)
		return retval;
	if (as_user.size()==1)
		return as_user[0].rights;
	//	check the minimum number of stars
	int	idx = 0;
	if (as_user.size()>0)
	{
		idx = mini_nb_stars(as_user);
		return as_user[idx].rights;
	}
	else
	{
		idx = mini_nb_stars(as_all);
		return as_all[idx].rights;
	}
}
//============================================================
/**
 *	Count how many specified char is found in specified string
 */
//============================================================
int TangoAccessControl::nb_chars(string s, char c)
{
	int	nb = 0;
	string::size_type	spos = s.find(c);
	while (spos != string::npos)
	{
		nb++;
		spos++;
		spos = s.find(c, spos);
	}
	return nb;
}
//============================================================
/**
 *	Return the element found in vector where the number of stars
 *		is the minimum one.
 */
//============================================================
int	TangoAccessControl::mini_nb_stars(vector<AccessStruct> v)
{
	int nb  = 3;
	int	idx = 0;
	for(int i=0 ; i<v.size() ; i++)
	{
		int n = nb_chars(v[i].device, '*');
		if (n<nb)
		{
			nb = n;
			idx = i;
		}
	}
	return idx;
}
//============================================================
//============================================================
void TangoAccessControl::register_service(string servicename, string instname, string devname)
{
	//	Get service property
	vector<string>	services;
	Tango::DbData	data;
	data.push_back(Tango::DbDatum(SERVICE_PROP_NAME));
	Tango::Util *tg = Tango::Util::instance();
	tg->get_database()->get_property(CONTROL_SYSTEM, data);
	if (data[0].is_empty()==false)		data[0] >> services;

	//	Build what to be searched
	TangoSys_MemStream	new_line;
	new_line << servicename << "/" << instname;
	string 	target(new_line.str());
	transform(target.begin(), target.end(), target.begin(), ::tolower);

	new_line << ":" << devname;

	//	Search if already exists
	bool	exists = false;
	vector<string>::iterator	pos = services.begin();
	for (int i=0 ; i<services.size() ; i++, pos++)
	{
		string::size_type	spos = services[i].find(':');
		if (spos != string::npos)	// found
		{
			string	s = services[i].substr(0, spos);
			transform(s.begin(), s.end(), s.begin(), ::tolower);
			if (s==target)
			{
				//	If exists --> replace
				services.erase(pos);
				services.insert(pos, new_line.str()); 
				exists = true;
			}
		}
	}
	//	Else add it
	if (!exists)
		services.push_back(new_line.str());
	
	data[0] << services;
	tg->get_database()->put_property(CONTROL_SYSTEM, data);
}
//============================================================
//============================================================
void TangoAccessControl::unregister_service(string servicename, string instname, string devname)
{
	//	Get service property
	vector<string>	services;
	Tango::DbData	data;
	data.push_back(Tango::DbDatum(SERVICE_PROP_NAME));
	Tango::Util *tg = Tango::Util::instance();
	tg->get_database()->get_property(CONTROL_SYSTEM, data);
	if (data[0].is_empty()==false)		data[0] >> services;
	
	//	Build what to be searched
	TangoSys_MemStream	line;
	line << servicename << "/" << instname << ":" << devname;
	string 	target(line.str());
	transform(target.begin(), target.end(), target.begin(), ::tolower);

	//	Search if exists
	vector<string>::iterator	pos = services.begin();
	for (int i=0 ; i<services.size() ; i++, pos++)
	{
		string	s(services[i]);
		transform(s.begin(), s.end(), s.begin(), ::tolower);
		if (s==target)
		{
			//	If exists --> remove
			services.erase(pos);
		}
	}
	data[0] << services;
	tg->get_database()->put_property(CONTROL_SYSTEM, data);
}


}	//	namespace
