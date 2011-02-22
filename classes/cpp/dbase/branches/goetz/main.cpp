#include <tango_ds.h>
#include <dserverclass.h>
#include <DataBase.h>


int main(int argc,char *argv[])
{

	TangoUtil *tango_util;
	TangoUtil::_UseDb = false; // suppress database use
	DServerClass *dsclass;

	try
	{
// construct database name	
		DataBase::db_name = "sys/database/";
		DataBase::db_name.append(argv[1]);
		cout << "main(): create DataBase " << DataBase::db_name << endl;
//
// Initialise the device server
//
		cout1 << "main(): calling  TangoUtil::Init(argc,argv)" << endl;
		tango_util = TangoUtil::init(argc,argv);

//
// Create the device server singleton which will create everyhing
//

		cout1 << "main(): calling  tango_util->server_init()" << endl;
		tango_util->server_init();

//
// Export devices to the outside world as CORBA named servant and to TANGO
// database
//
		DeviceImpl *dbase, *dserver;
        	Tango_DevVarStringArray *export_parms = new Tango_DevVarStringArray(5);

		dserver = tango_util->get_dserver_device();
		dbase = tango_util->get_device_by_name(DataBase::db_name);
//
// export database as named servant 
//
// NOTE : this uses an Orbacus extension to the CORBA standard
//        if this is the first database (argv[]1="1") then use the database
//        name "database" for backwards compatibility
	
		if (DataBase::db_name == "sys/database/1")
		{
			cout << "main(): export DataBase as named servant (name=database)" << endl;
			(TangoUtil::instance()->get_orb())->connect(dbase,"database"); 
		}
		else
		{
			cout << "main(): export DataBase as named servant (name=" << DataBase::db_name <<")" << endl;
			(TangoUtil::instance()->get_orb())->connect(dbase,DataBase::db_name.c_str()); 
		}

		export_parms->length(5);

// export dserver object to TANGO database

        	(*export_parms)[0] = CORBA_string_dup(dserver->get_name().c_str());
        	(*export_parms)[1] = CORBA_string_dup((TangoUtil::instance()->get_orb()->object_to_string(dserver)));
        	(*export_parms)[2] = CORBA_string_dup(tango_util->get_host_name().c_str());
        	(*export_parms)[3] = CORBA_string_dup(tango_util->get_pid_str().c_str());
        	(*export_parms)[4] = CORBA_string_dup(tango_util->get_version_str().c_str());
		((DataBase*)dbase)->ExportDevice(export_parms);

// export database object to TANGO database

        	(*export_parms)[0] = CORBA_string_dup(dbase->get_name().c_str());
        	(*export_parms)[1] = CORBA_string_dup((TangoUtil::instance()->get_orb()->object_to_string(dbase)));
		((DataBase*)dbase)->ExportDevice(export_parms);

//
// Run the endless loop
//

		cout << "Ready to accept request" << endl;		
		(tango_util->get_boa())->impl_is_ready(CORBA_ImplementationDef::_nil());
	}
	catch (bad_alloc)
	{
		cout << "Can't allocate memory to store device object !!!" << endl;
		cout << "Exiting" << endl;
	}
	catch (CORBA_Exception &e)
	{
		cout << "Received a CORBA_Exception" << endl;
		TangoUtil::print_exception(e);
		cout << "Exiting" << endl;
	}

}
