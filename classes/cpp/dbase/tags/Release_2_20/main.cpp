#include <tango.h>
#include <dserverclass.h>
#include <DataBase.h>

int main(int argc,char *argv[])
{

	cout << "main(): arrived " << endl;

	Tango::Util *tango_util;
	Tango::Util::_UseDb = false; // suppress database use

	try
	{
//
// Initialise the device server
//
		cout1 << "main(): calling  Tango::Util::Init(argc,argv)" << endl;
		tango_util = Tango::Util::init(argc,argv);
		
// construct database name
	
		DataBase::DataBase::db_name = "sys/database/";
		DataBase::DataBase::db_name.append(argv[1]);
		cout1 << "main(): create DataBase " << DataBase::DataBase::db_name << endl;

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
        	Tango::DevVarStringArray *export_parms = new Tango::DevVarStringArray(5);

		dserver = tango_util->get_dserver_device();
		dbase = tango_util->get_device_by_name(DataBase::DataBase::db_name);
//
// export database as named servant 
//
// NOTE : this uses an Orbacus extension to the CORBA standard
//        all database servants have the same name ("database")
//        they only differ in their port address.
	
		cout << "main(): export DataBase as named servant (name=database)" << endl;
/*
		(Tango::Util::instance()->get_orb())->connect(dbase,"database"); 
 */

		export_parms->length(5);

// export dserver object to TANGO database

		Tango::Device_var d = dserver->_this();
		dserver->set_d_var(Tango::Device::_duplicate(d));
        	(*export_parms)[0] = CORBA::string_dup(dserver->get_name().c_str());
        	(*export_parms)[1] = CORBA::string_dup((Tango::Util::instance()->get_orb()->object_to_string(d)));
        	(*export_parms)[2] = CORBA::string_dup(tango_util->get_host_name().c_str());
        	(*export_parms)[3] = CORBA::string_dup(tango_util->get_pid_str().c_str());
        	(*export_parms)[4] = CORBA::string_dup(tango_util->get_version_str().c_str());
		((DataBase::DataBase*)dbase)->db_export_device(export_parms);

// export database object to TANGO database

        	(*export_parms)[0] = CORBA::string_dup(dbase->get_name().c_str());
        	(*export_parms)[1] = CORBA::string_dup((Tango::Util::instance()->get_orb()->object_to_string(dbase->get_d_var())));
		((DataBase::DataBase*)dbase)->db_export_device(export_parms);

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
