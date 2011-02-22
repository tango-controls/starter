static const char *RcsId = "$Header$";
//+=============================================================================
//
// file :         Starter.cpp
//
// description :  C++ source for the Starter if used as windows service.
//
// project :      TANGO Device Server
//
// $Author$
//
// This file is part of Tango.
//
// Tango is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// Tango is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with Tango.  If not, see <http://www.gnu.org/licenses/>.
//
// $Revision$
//
// $Log$
// Revision 3.9  2010/02/09 15:09:49  pascal_verdier
// Define  _TG_WINDOWS_  replace WIN32.
// LogFileHome property added.
//
// Revision 3.8  2008/04/07 08:54:55  pascal_verdier
// Check if this starter instance is able to ru on this host.
//
// Revision 3.7  2007/09/27 15:12:01  pascal_verdier
// Get TANGO_HOST from registry and add it to environement.
//
// Revision 3.6  2004/09/28 07:13:19  pascal_verdier
// bug on state with notify daemon fixed.
//
// Revision 3.5  2004/07/05 11:01:20  pascal_verdier
// PB on service fixed.
//
// Revision 3.4  2004/06/29 04:24:26  pascal_verdier
// First revision using events.
//
//-=============================================================================


#ifdef _TG_WINDOWS_

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


		//	Set TANGO_HOST  gotten from registry 
		//	to environement for sterted servers
		//----------------------------------------
		char	*tango_host;
		Tango::Database	*db = tg->get_database();
		db->get_tango_host_from_reg(&tango_host, tg->get_ds_exec_name(), tg->get_ds_inst_name());
		string	s("TANGO_HOST=");
		s += (tango_host==NULL)? "NULL" : tango_host;
		log->info(s.c_str());

		_putenv(s.c_str());
		delete tango_host;

		// Run the endless loop
		//----------------------------------------
		tg->server_run();
	}
	catch(bad_alloc) {
		logger_->error("Cannot allocate memory to store device object");
	}
	catch(Tango::DevFailed &e) {
		logger_->error(e.errors[0].desc.in());
	}
	catch(CORBA::Exception &) {
		logger_->error("CORBA Exception");
	}
}

#endif
