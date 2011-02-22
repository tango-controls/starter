//=============================================================================
//
// file :        db_init_history.cpp
//
// description : Initialise history tables with the current database content.
//
// project :	TANGO
//
// Author(s): JL Pons
//
// $Revision$
//
// $Log$
//
// copyleft :    European Synchrotron Radiation Facility
//               BP 220, Grenoble 38043
//               FRANCE
//
//=============================================================================
#ifdef WIN32
#include <winsock.h>
#endif
#include <mysql.h>

#if ((defined WIN32) || (defined __SUNPRO_CC) || (defined GCC_STD) || (defined __HP_aCC))
	#include <iostream>
	#include <sstream>
	#include <fstream>
        #define  MemStream      stringstream
#else
	#include <iostream.h>
	#include <strstream.h>
	#include <fstream.h>
	#include <sstream>
        #define  MemStream      strstream
#endif

using namespace std;


MYSQL mysql;  

// ----------------------------------------------------------------------------

MYSQL_RES *query(string sql_query) {

  MYSQL_RES *result;
	
  if (mysql_real_query(&mysql, sql_query.c_str(),sql_query.length()) != 0)
  {
    cout << "Failed to query TANGO database (error=" << mysql_error(&mysql) << ")" << endl;
    exit(0);
  }

  if ((result = mysql_store_result(&mysql)) == NULL)
  {
    cout << "mysql_store_result() failed (error=" << mysql_error(&mysql) << ")" << endl;
    exit(0);
  }
	
  return result;

}

void simple_query(string sql_query) {

  if (mysql_real_query(&mysql, sql_query.c_str(),sql_query.length()) != 0)
  {
    cout << "Failed to query TANGO database (error=" << mysql_error(&mysql) << ")" << endl;
    exit(0);
  }

}

string escape_string(const char *string_c_str)
{
	string escaped_string(string_c_str);
	string::size_type index;

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

	return escaped_string;
}

// ----------------------------------------------------------------------------

int main(int argc,char *argv[]) {

  // ---------------------------------------------------------------  
  // Initialise MySQL
  // ---------------------------------------------------------------  

  char *mysql_user = (char *)"root";
  char *mysql_password = (char *)"";

  if(argc==2) {
    mysql_user = argv[1];
  }

  if(argc==3) {
    mysql_user     = argv[1];
    mysql_password = argv[2];
  }

  mysql_init(&mysql);
  
  if (!mysql_real_connect(&mysql, "localhost", mysql_user, mysql_password, "tango", 0, NULL, 0))
  {
    cout << "Failed to connect to MySQL: " << mysql_error(&mysql) << endl;
    cout << "Usage: db_init_history  [user] [password]." << endl;
    return 0;
  }
  
  // ---------------------------------------------------------------  
  // Create and initialise history tables
  // ---------------------------------------------------------------  

  MemStream sql_query;
  int i,j,nb_prop;
  MYSQL_RES *list;
  MYSQL_ROW row,row2;

  // Id table ----------------------------------------------

  sql_query.str("");
  sql_query << "drop table if exists history_ids";  
  simple_query(sql_query.str());

  sql_query.str("");
  sql_query << "CREATE TABLE history_ids (";
  sql_query << "  name   varchar(255) NOT NULL default '',";
  sql_query << "  id     int unsigned NOT NULL";
  sql_query << ") TYPE=MyISAM;";
  simple_query(sql_query.str());
  
  // Device properties ----------------------------------------------
  
  // Create the table
  sql_query.str("");
  sql_query << "drop table if exists property_device_hist";  
  simple_query(sql_query.str());

  sql_query.str("");
  sql_query << "CREATE TABLE property_device_hist (";
  sql_query << "  id int unsigned NOT NULL,";
  sql_query << "  date timestamp NOT NULL,";
  sql_query << "  device varchar(255) NOT NULL default '',";
  sql_query << "  name varchar(255) NOT NULL default '',";
  sql_query << "  count int NOT NULL default '0',";
  sql_query << "  value text default NULL,";
  sql_query << "  KEY index_resource (id)";
  sql_query << ") TYPE=MyISAM;";
  simple_query(sql_query.str());
  
  // Copy all properties within the history table    
  sql_query.str("");
  sql_query << "SELECT device,name FROM property_device WHERE count=1";
  list = query(sql_query.str());
  nb_prop = mysql_num_rows(list);
  for(i=0;i<nb_prop;i++) {
  
     row = mysql_fetch_row(list);
     char *device = row[0];
     char *name   = row[1];
     sql_query.str("");
     sql_query << "SELECT value FROM property_device WHERE device='" << device 
               << "' AND name='" << name << "' ORDER by count";
     
     MYSQL_RES *result = query(sql_query.str());
     int nb_item = mysql_num_rows(result);
     for(j=0;j<nb_item;j++) {

        row2 = mysql_fetch_row(result);
	string tmp_escaped_string = escape_string(row2[0]);
	sql_query.str("");
	sql_query << "INSERT INTO property_device_hist SET " \
	          << "device='"  << device \
		  << "',id='"    << i \
		  << "',name='"  << name \
		  << "',count='" << (j+1) \
		  << "',value='" << tmp_escaped_string << "'";
	simple_query(sql_query.str());
	
     }
     mysql_free_result(result);
    
  }
  mysql_free_result(list);
  
  //Update id
  sql_query.str("");
  sql_query << "INSERT INTO history_ids SET name='device'" << ",id='" << i << "'"; 
  simple_query(sql_query.str());
    
  
  cout << "Table property_device_hist succesfully created [" << nb_prop << " items]." << endl;

  // Device attribute properties -------------------------------------------
  
  // Create the table
  sql_query.str("");
  sql_query << "drop table if exists property_attribute_device_hist";  
  simple_query(sql_query.str());

  sql_query.str("");
  sql_query << "CREATE TABLE property_attribute_device_hist (";
  sql_query << "  id int unsigned NOT NULL,";
  sql_query << "  date timestamp NOT NULL,";
  sql_query << "  device varchar(255) NOT NULL default '',";
  sql_query << "  attribute varchar(255) NOT NULL default '',";
  sql_query << "  name varchar(255) NOT NULL default '',";
  sql_query << "  count int NOT NULL default '0',";
  sql_query << "  value text default NULL,";
  sql_query << "  KEY index_resource (id)";
  sql_query << ") TYPE=MyISAM;";
  simple_query(sql_query.str());
  
  // Copy all properties within the history table    
  sql_query.str("");
  sql_query << "SELECT device,attribute,name FROM property_attribute_device WHERE count=1";
  list = query(sql_query.str());
  nb_prop = mysql_num_rows(list);
  for(i=0;i<nb_prop;i++) {
  
     row = mysql_fetch_row(list);
     char *device    = row[0];
     char *attribute = row[1];
     char *name      = row[2];
     sql_query.str("");
     sql_query << "SELECT value FROM property_attribute_device WHERE device='" << device 
               << "' AND attribute='" << attribute << "' AND name='" << name << "' ORDER by count";
     
     MYSQL_RES *result = query(sql_query.str());
     int nb_item = mysql_num_rows(result);
     for(j=0;j<nb_item;j++) {

        row2 = mysql_fetch_row(result);
	string tmp_escaped_string = escape_string(row2[0]);
	sql_query.str("");
	sql_query << "INSERT INTO property_attribute_device_hist SET " \
	          << "device='"  << device \
	          << "',attribute='"  << attribute \
		  << "',id='"    << i \
		  << "',name='"  << name \
		  << "',count='" << (j+1) \
		  << "',value='" << tmp_escaped_string << "'";
	simple_query(sql_query.str());
	
     }
     mysql_free_result(result);
    
  }
  mysql_free_result(list);

  //Update id
  sql_query.str("");
  sql_query << "INSERT INTO history_ids SET name='device_attribute'" << ",id='" << i << "'"; 
  simple_query(sql_query.str());
  
  cout << "Table property_attribute_device_hist succesfully created [" << nb_prop << " items]." << endl;

  // Class properties ----------------------------------------------
  
  // Create the table
  sql_query.str("");
  sql_query << "drop table if exists property_class_hist";  
  simple_query(sql_query.str());

  sql_query.str("");
  sql_query << "CREATE TABLE property_class_hist (";
  sql_query << "  id int unsigned NOT NULL,";
  sql_query << "  date timestamp NOT NULL,";
  sql_query << "  class varchar(255) NOT NULL default '',";
  sql_query << "  name varchar(255) NOT NULL default '',";
  sql_query << "  count int NOT NULL default '0',";
  sql_query << "  value text default NULL,";
  sql_query << "  KEY index_resource (id)";
  sql_query << ") TYPE=MyISAM;";
  simple_query(sql_query.str());
  
  // Copy all properties within the history table    
  sql_query.str("");
  sql_query << "SELECT class,name FROM property_class WHERE count=1";
  list = query(sql_query.str());
  nb_prop = mysql_num_rows(list);
  for(i=0;i<nb_prop;i++) {
  
     row = mysql_fetch_row(list);
     char *_class = row[0];
     char *name   = row[1];
     sql_query.str("");
     sql_query << "SELECT value FROM property_class WHERE class='" << _class 
               << "' AND name='" << name << "' ORDER by count";
     
     MYSQL_RES *result = query(sql_query.str());
     int nb_item = mysql_num_rows(result);
     for(j=0;j<nb_item;j++) {

        row2 = mysql_fetch_row(result);
	string tmp_escaped_string = escape_string(row2[0]);
	sql_query.str("");
	sql_query << "INSERT INTO property_class_hist SET " \
	          << "class='"  << _class \
		  << "',id='"    << i \
		  << "',name='"  << name \
		  << "',count='" << (j+1) \
		  << "',value='" << tmp_escaped_string << "'";
	simple_query(sql_query.str());
	
     }
     mysql_free_result(result);
    
  }
  mysql_free_result(list);

  //Update id
  sql_query.str("");
  sql_query << "INSERT INTO history_ids SET name='class'"  << ",id='" << i << "'"; 
  simple_query(sql_query.str());
  
  cout << "Table property_class_hist succesfully created [" << nb_prop << " items]." << endl;

  // Class attribute properties -------------------------------------------
  
  // Create the table
  sql_query.str("");
  sql_query << "drop table if exists property_attribute_class_hist";  
  simple_query(sql_query.str());

  sql_query.str("");
  sql_query << "CREATE TABLE property_attribute_class_hist (";
  sql_query << "  id int unsigned NOT NULL,";
  sql_query << "  date timestamp NOT NULL,";
  sql_query << "  class varchar(255) NOT NULL default '',";
  sql_query << "  attribute varchar(255) NOT NULL default '',";
  sql_query << "  name varchar(255) NOT NULL default '',";
  sql_query << "  count int NOT NULL default '0',";
  sql_query << "  value text default NULL,";
  sql_query << "  KEY index_resource (id)";
  sql_query << ") TYPE=MyISAM;";
  simple_query(sql_query.str());
  
  // Copy all properties within the history table    
  sql_query.str("");
  sql_query << "SELECT class,attribute,name FROM property_attribute_class WHERE count=1";
  list = query(sql_query.str());
  nb_prop = mysql_num_rows(list);
  for(i=0;i<nb_prop;i++) {
  
     row = mysql_fetch_row(list);
     char *_class    = row[0];
     char *attribute = row[1];
     char *name      = row[2];
     sql_query.str("");
     sql_query << "SELECT value FROM property_attribute_class WHERE class='" << _class 
               << "' AND attribute='" << attribute << "' AND name='" << name << "' ORDER by count";
     
     MYSQL_RES *result = query(sql_query.str());
     int nb_item = mysql_num_rows(result);
     for(j=0;j<nb_item;j++) {

        row2 = mysql_fetch_row(result);
	string tmp_escaped_string = escape_string(row2[0]);
	sql_query.str("");
	sql_query << "INSERT INTO property_attribute_class_hist SET " \
	          << "class='"  << _class \
	          << "',attribute='"  << attribute \
		  << "',id='"    << i \
		  << "',name='"  << name \
		  << "',count='" << (j+1) \
		  << "',value='" << tmp_escaped_string << "'";
	simple_query(sql_query.str());
	
     }
     mysql_free_result(result);
    
  }
  mysql_free_result(list);

  //Update id
  sql_query.str("");
  sql_query << "INSERT INTO history_ids SET name='class_attribute'" << ",id='" << i << "'"; 
  simple_query(sql_query.str());
  
  cout << "Table property_attribute_class_hist succesfully created [" << nb_prop << " items]." << endl;

  // Object properties ----------------------------------------------
  
  // Create the table
  sql_query.str("");
  sql_query << "drop table if exists property_hist";  
  simple_query(sql_query.str());

  sql_query.str("");
  sql_query << "CREATE TABLE property_hist (";
  sql_query << "  id int unsigned NOT NULL,";
  sql_query << "  date timestamp NOT NULL,";
  sql_query << "  object varchar(255) NOT NULL default '',";
  sql_query << "  name varchar(255) NOT NULL default '',";
  sql_query << "  count int NOT NULL default '0',";
  sql_query << "  value text default NULL,";
  sql_query << "  KEY index_resource (id)";
  sql_query << ") TYPE=MyISAM;";
  simple_query(sql_query.str());
  
  // Copy all properties within the history table    
  sql_query.str("");
  sql_query << "SELECT object,name FROM property WHERE count=1";
  list = query(sql_query.str());
  nb_prop = mysql_num_rows(list);
  for(i=0;i<nb_prop;i++) {
  
     row = mysql_fetch_row(list);
     char *object = row[0];
     char *name   = row[1];
     sql_query.str("");
     sql_query << "SELECT value FROM property WHERE object='" << object 
               << "' AND name='" << name << "' ORDER by count";
     
     MYSQL_RES *result = query(sql_query.str());
     int nb_item = mysql_num_rows(result);
     for(j=0;j<nb_item;j++) {

        row2 = mysql_fetch_row(result);
	string tmp_escaped_string = escape_string(row2[0]);
	sql_query.str("");
	sql_query << "INSERT INTO property_hist SET " \
	          << "object='"  << object \
		  << "',id='"    << i \
		  << "',name='"  << name \
		  << "',count='" << (j+1) \
		  << "',value='" << tmp_escaped_string << "'";
	simple_query(sql_query.str());
	
     }
     mysql_free_result(result);
    
  }
  mysql_free_result(list);

  //Update id
  sql_query.str("");
  sql_query << "INSERT INTO history_ids SET name='object'" << ",id='" << i << "'"; 
  simple_query(sql_query.str());
  
  cout << "Table property_hist succesfully created [" << nb_prop << " items]." << endl;
  

  return 0;

}
