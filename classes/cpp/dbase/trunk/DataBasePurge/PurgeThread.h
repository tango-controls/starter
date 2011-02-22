#ifndef PURGETHREAD_H_
#define PURGETHREAD_H_

#include <DataBasePurge.h>

namespace DataBasePurge_ns
{
        
class PurgeThread : public omni_thread
{

public :

  PurgeThread (DataBasePurge *_obj);

private :

  DataBasePurge		*obj;
                     
  void *run_undetached(void*);
  void purge_property(const char *table,const char *field);
  void purge_att_property(const char *table,const char *field);
  
};

}

#endif /*PURGETHREAD_H_*/
