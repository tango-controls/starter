#include <PurgeThread.h>

namespace DataBasePurge_ns 
{

PurgeThread::PurgeThread (DataBasePurge *_obj) : omni_thread()
{
	obj = _obj;
	start_undetached();
}
	
//+------------------------------------------------------------------
/**
 *	method:	run_undetached()
 *
 *	description:	thread to purge tango db history table.
 *
 */
//+------------------------------------------------------------------
void *PurgeThread::run_undetached(void*)
{
  int toSleep;
  time_t now;

  // Wait a while before starting
  Tango_sleep(5);
  
  while(obj->thread_alive) {

    // Wait next purge -----------------------------------
    
    toSleep=0;
    while(toSleep<obj->purge_Period && obj->thread_alive) {
      Tango_sleep(1);
      toSleep++;
    }
  
    now = time(NULL);
    cout << "Purge started at " << ctime(&now);
    obj->purge_in_progress = true;
    obj->purge_progress = 0.0;
	
    purge_property("property_device_hist","device");
    purge_property("property_class_hist","class");
    purge_property("property_hist","object");
    purge_att_property("property_attribute_device_hist","device");
    purge_att_property("property_attribute_class_hist","class");
    
    obj->purge_in_progress = false;
    now = time(NULL);
    cout << "Purge finished at " << ctime(&now);
    
  }

  return NULL;  
}

//+------------------------------------------------------------------
/**
 *	method:	purge_property()
 *
 *	description:	purge a property history table.
 *
 */
//+------------------------------------------------------------------
void PurgeThread::purge_property(const char *table,const char *field) {

  TangoSys_MemStream sql_query;
  MYSQL_RES *list,*result;
  MYSQL_ROW row,row2;
  int i,j,nb_row;

  if(!obj->thread_alive) return;  

  sql_query.str("");
  sql_query << "SELECT DISTINCT " << field << ",name FROM " << table;
  list = obj->query(sql_query.str());
  nb_row = mysql_num_rows(list);
  for(i=0;i<nb_row;i++) {
	
    row = mysql_fetch_row(list);
    char *object = row[0];
    char *name   = row[1];
	
    sql_query.str("");
    sql_query << "SELECT DISTINCT id FROM " << table
              << " WHERE " << field << "=\"" << object << "\" AND name=\"" << name 
	          << "\" ORDER by date";
    result = obj->query(sql_query.str());
    int nb_item = mysql_num_rows(result);
    if(nb_item>obj->history_depth) {
      // Purge 
      int toDelete = nb_item-obj->history_depth;
      for(j=0;j<toDelete;j++) {
	    row2 = mysql_fetch_row(result);
        sql_query.str("");
        sql_query << "DELETE FROM " << table << " WHERE id='" << row2[0] << "'";
	    obj->simple_query(sql_query.str());
	  }
    }
    mysql_free_result(result);
      
    if(!obj->thread_alive) {
      // We got a stop request
      mysql_free_result(list);
	  return;
    }
	
	obj->purge_progress += 20.0/(double)nb_row;
    

  }
  mysql_free_result(list);

}

//+------------------------------------------------------------------
/**
 *	method:	purge_attribute_property()
 *
 *	description:	purge an attribute property history table.
 *
 */
//+------------------------------------------------------------------
void PurgeThread::purge_att_property(const char *table,const char *field) {

  TangoSys_MemStream sql_query;
  MYSQL_RES *list,*result;
  MYSQL_ROW row,row2;
  int i,j,nb_row;

  if(!obj->thread_alive) return;

  sql_query.str("");
  sql_query << "SELECT DISTINCT " << field << ",attribute,name FROM " << table;					 
  list = obj->query(sql_query.str());
  nb_row = mysql_num_rows(list);
  for(i=0;i<nb_row;i++) {
	
    row = mysql_fetch_row(list);
    char *object    = row[0];
    char *attribute = row[1];
    char *name      = row[2];
	
    sql_query.str("");
    sql_query << "SELECT DISTINCT id FROM " << table
              << " WHERE " << field << "=\"" << object << "\" AND name=\"" << name 
  	          << "\" AND attribute=\"" << attribute << "\" ORDER by date";
    result = obj->query(sql_query.str());
    int nb_item = mysql_num_rows(result);
    if(nb_item>obj->history_depth) {
      // Purge 
	  int toDelete = nb_item-obj->history_depth;
	  for(j=0;j<toDelete;j++) {
	    row2 = mysql_fetch_row(result);
        sql_query.str("");
        sql_query << "DELETE FROM " << table << " WHERE id='" << row2[0] << "'";
	    obj->simple_query(sql_query.str());
	  }
    }
    mysql_free_result(result);
      
    if(!obj->thread_alive) {
      // We got a stop request
      mysql_free_result(list);
	  return;
    }

	obj->purge_progress += 20.0/(double)nb_row;
    
  }
  mysql_free_result(list);

}

} // namespace
