


#ifdef WIN32

#include <tango.h>
#include <StarterService.h>
#include <Starter.h>



/**
 *	The NT service class
 */

using namespace std;



//+------------------------------------------------------------------
/**
 *	The service constructor
 */
//+------------------------------------------------------------------
StarterService::StarterService(char *exe_name):NTService(exe_name)
{
}
//+------------------------------------------------------------------
/**
 *	Start the NT Service
 */
//+------------------------------------------------------------------
void StarterService::start(int argc, char **argv, Tango::NTEventLogger *log)
{
	log->info("Starting start method");

	Tango::Util	*tg;
	Tango::Util::_daemon = true;
	Tango::Util::_sleep_between_connect = 5;

	log->info("Wait database done");

	try
	{
		// Initialise logger
		//----------------------------------------
		Tango::Util::_service = true;
		log->info("Tango::Util::_service = true;");

		// Initialise the device server
		//----------------------------------------
		tg = Tango::Util::init(argc, argv);
		log->info("tg = Tango::Util::init(argc, argv);");

		// Create the device server singleton 
		//	which will create everything
		//----------------------------------------
		tg->server_init();
		log->info("tg->server_init();");

		//
		//	It is now done in always_executed_hook()
		//
/*
		//	This thread must be known by omni_thread
		omni_thread::create_dummy();

		//	Special initializing when device is exported
		//---------------------------------------------------
		stringstream	devname;
		devname << "tango/admin/" << argv[1];
		Tango::DeviceImpl *dev = tg->get_device_by_name(devname.str());

		((static_cast<Starter::Starter *>(dev))->manage_PollingState_startup());
*/

		// Run the endless loop
		//----------------------------------------
		tg->server_run();
	}
	catch(bad_alloc) {
		logger_->error("Cannot allocate memory to store device objecet");
	}
	catch(Tango::DevFailed &e) {
		logger_->error(e.errors[0].desc.in());
	}
	catch(CORBA::Exception &) {
		logger_->error("CORBA Exception");
	}
}

#endif
