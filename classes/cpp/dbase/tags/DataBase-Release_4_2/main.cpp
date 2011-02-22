#include <tango.h>
#include <dserverclass.h>
#include <DataBase.h>

#ifndef WIN32
#include <sys/resource.h>
#endif

int DataBase_ns::DataBase::conn_pool_size;

int main(int argc,char *argv[])
{

	cout << "main(): arrived " << endl;

	Tango::Util *tango_util;
	Tango::Util::_UseDb = false; // suppress database use
	
#ifndef WIN32

        // Setting maximum number of opened file
	
	// Default limit
	struct rlimit limit;
	limit.rlim_cur = 1024;
	limit.rlim_max = 1024;
	
	// Browse argv and try to find the -maxFile option
	int found = 0;
	int i = 0;
	while(i<argc && !found) {
	  found = (strcasecmp(argv[i],"-maxFile")==0);
	  if(!found) i++;
	}
	if( found ) {
	  if(i>=argc-1) {
	    cout << "Invalid flimit parameter." << endl;
	    return -1;
	  }
	  int flimit = atoi(argv[i+1]);
	  if(flimit==0) {
	    cout << "Invalid flimit parameter." << endl;
	    return -1;
	  }
	  limit.rlim_cur = flimit;
	  limit.rlim_max = flimit;
	}
	
	// Browse argv and try to find the -connPoolSize option
	int j = 0;
	int found_conn = 0;
	int conn_size = DEFAULT_CONN_POOL_SIZE;
	while(j<argc && !found_conn) {
	  found_conn = (strcasecmp(argv[j],"-poolSize")==0);
	  if(!found_conn) j++;
	}
	if( found_conn ) {
	  if(j>=argc-1) {
	    cout << "Invalid poolSize parameter." << endl;
	    return -1;
	  }
	  conn_size = atoi(argv[j+1]);
	  if(conn_size<=0) {
	    cout << "Invalid poolSize parameter." << endl;
	    return -1;
	  }
	}
	DataBase_ns::DataBase::set_conn_pool_size(conn_size);
	
	
	// Apply the max open file limit
	if( setrlimit(RLIMIT_NOFILE,&limit) != 0 ) {	
	  cout << "setrlimit(RLIMIT_NOFILE," << (int)limit.rlim_cur << ") failed." << endl;	  
	  if(errno==EPERM) {
	    cout << "You may need to increase maximum number of opened file system limit." << endl;
	  } else {
	    cout << "setrlimit() failed with error code : " << (int)errno << endl;
	  }
	  return -1;
	}
	
#endif		

	try
	{
//
// Initialise the device server
//
		cout1 << "main(): calling  Tango::Util::Init(argc,argv)" << endl;
		tango_util = Tango::Util::init(argc,argv);
		
// construct database name
	
		DataBase_ns::DataBase::db_name = "sys/database/";
		DataBase_ns::DataBase::db_name.append(argv[1]);
		cout1 << "main(): create DataBase " << DataBase_ns::DataBase::db_name << endl;

//
// Create and install interceptors
//

		DataBase_ns::DbInter *dbi = new DataBase_ns::DbInter();
		tango_util->set_interceptors(dbi);

//
// Set the serialization method
//

		tango_util->set_serial_model(Tango::NO_SYNC);

//
// Create the device server singleton which will create everything
//

		cout1 << "main(): calling  tango_util->server_init()" << endl;
		tango_util->server_init();

//
// Export devices to the outside world as CORBA named servant and to TANGO
// database
//
		Tango::DeviceImpl *dbase, *dserver;
        Tango::DevVarStringArray *export_parms = new Tango::DevVarStringArray();

		dserver = tango_util->get_dserver_device();
		dbase = tango_util->get_device_by_name(DataBase_ns::DataBase::db_name);
//
// export database as named servant 
//
		cout << "main(): export DataBase as named servant (name=database)" << endl;
		export_parms->length(5);

// export dserver object to TANGO database

		Tango::Device_var d = dserver->_this();
		dserver->set_d_var(Tango::Device::_duplicate(d));
		(*export_parms)[0] = CORBA::string_dup(dserver->get_name().c_str());
		(*export_parms)[1] = CORBA::string_dup((Tango::Util::instance()->get_orb()->object_to_string(d)));
		(*export_parms)[2] = CORBA::string_dup(tango_util->get_host_name().c_str());
		(*export_parms)[3] = CORBA::string_dup(tango_util->get_pid_str().c_str());
		(*export_parms)[4] = CORBA::string_dup(tango_util->get_version_str().c_str());

		(static_cast<DataBase_ns::DataBase *>(dbase))->db_export_device(export_parms);

// export database object to TANGO database

		(*export_parms)[0] = CORBA::string_dup(dbase->get_name().c_str());
		(*export_parms)[1] = CORBA::string_dup((Tango::Util::instance()->get_orb()->object_to_string(dbase->get_d_var())));
		((DataBase_ns::DataBase*)dbase)->db_export_device(export_parms);

//
// Run the endless loop
//

		cout << "Ready to accept request" << endl;		
		(tango_util->get_orb())->run();
	}
	catch (bad_alloc)
	{
		cout << "Can't allocate memory to store device object !!!" << endl;
		cout << "Exiting" << endl;
	}
	catch (CORBA::Exception &e)
	{
		cout << "Received a CORBA::Exception" << endl;
		Tango::Except::print_exception(e);
		cout << "Exiting" << endl;
	}

	return(0);
}
