static const char *RcsId = "$Header$";
//+=============================================================================
//
// file :         Starter.cpp
//
// description :  C++ source for the Starter and its commands. 
//                The class is derived from Device. It represents the
//                CORBA servant object which will be accessed from the
//                network. All commands which can be executed on the
//                Starter are implemented in this file.
//
// project :      TANGO Device Server
//
// $Author$
//
// $Revision$
//
// $Log$
// Revision 3.38  2008/04/24 06:27:24  pascal_verdier
// Check host and device but in same case.
//
// Revision 3.37  2008/04/09 14:46:11  pascal_verdier
// Bug in init of NotifdState attribute fixed.
//
// Revision 3.36  2008/04/07 13:10:27  pascal_verdier
// New pogo property management.
//
// Revision 3.35  2008/04/07 08:54:55  pascal_verdier
// Check if this starter instance is able to run on this host.
//
// Revision 3.34  2008/02/29 15:15:05  pascal_verdier
// Checking running processes by system call added.
//
// Revision 3.33  2007/09/25 12:12:00  pascal_verdier
// little memory leaks fixed.
//
// Revision 3.32  2007/05/25 06:08:00  pascal_verdier
// Iterator management modified (strange in vc8 ?)
//
// Revision 3.31  2007/03/30 14:13:24  pascal_verdier
// Remove 2 warnings on linux.
//
// Revision 3.30  2007/02/12 13:05:57  pascal_verdier
// Spectrum String attribute management changed.
//
// Revision 3.29  2007/02/01 09:17:44  pascal_verdier
// Monitor added on some shared data.
// Wait 3 seconds added at startup for first ping timeout.
//
// Revision 3.28  2006/11/20 06:58:37  pascal_verdier
// Mutex on start process data added.
//
// Revision 3.27  2006/11/10 14:53:28  pascal_verdier
// Remove vc8 warnings.
//
// Revision 3.26  2006/06/13 19:38:15  pascal_verdier
// Minor changes.
//
// Revision 3.25  2006/06/06 12:01:26  pascal_verdier
// Bug in log file fixed.
//
// Revision 3.24  2006/06/05 07:20:36  pascal_verdier
// Server startup is now delayed (with timeout) in a startup level.
// New state MOVING added.
// At startup, starter loop until properties have been read.
//
// Revision 3.23  2006/05/15 10:52:37  pascal_verdier
// Remove a ifndef WIN32 (?)
//
// Revision 3.22  2006/04/24 07:06:27  pascal_verdier
// A thread is started for each level when at servers startup.
//
// Revision 3.21  2006/02/10 13:28:35  pascal_verdier
// *** empty log message ***
//
// Revision 3.20  2006/02/09 11:59:18  pascal_verdier
// A ping thread is now started for each server.
//
// Revision 3.19  2006/02/08 07:13:55  pascal_verdier
// Minor changes.
//
// Revision 3.18  2006/01/13 15:18:53  pascal_verdier
// Bug on notifd startup fixed.
//
// Revision 3.17  2005/10/04 13:02:52  pascal_verdier
// *** empty log message ***
//
// Revision 3.16  2005/09/30 12:01:50  pascal_verdier
// RunningServers and StoppedServers spectrum attribute max_x set to 200.
//
// Revision 3.15  2005/09/19 13:32:07  pascal_verdier
// Delete and re-create DeviceProxy in case of ping failed seems to
// fixe a random bug at startup under windows (??)
//
// Revision 3.14  2005/09/13 14:36:10  pascal_verdier
// Pogo-4.4.0 compatibility.
//
// Revision 3.13  2005/08/26 07:18:16  pascal_verdier
// FQDN management added.
// Time between startup level added.
//
// Revision 3.12  2005/05/27 13:16:39  pascal_verdier
// InterStartupLevelWait property has been added.
//
// Revision 3.11  2005/05/18 12:11:56  pascal_verdier
// Start a batch file under windows.
//
// Revision 3.10  2005/04/21 07:18:48  pascal_verdier
// Add a little timeout for ping.
// Fix a bug for windows (SIGCHLD).
//
// Revision 3.9  2005/03/24 15:35:29  pascal_verdier
// Add sigset for signal problem on solaris.
//
// Revision 3.8  2005/01/17 12:35:35  pascal_verdier
// *** empty log message ***
//
// Revision 3.7  2004/12/10 08:57:19  pascal_verdier
// Tango 5 compatibility (attribute management).
//
// Revision 3.6  2004/12/03 13:56:16  pascal_verdier
// remove exit on method (used by valgrind).
//
// Revision 3.5  2004/10/20 07:52:53  pascal_verdier
// Memory leak in read_attr method fixed.
//
// Revision 3.4  2004/09/28 07:13:19  pascal_verdier
// bug on state with notify daemon fixed.
//
// Revision 3.3  2004/06/29 04:24:26  pascal_verdier
// First revision using events.
//
// Revision 3.2  2004/05/19 08:56:32  pascal_verdier
// Bug fixed on service mode.
// CreateProcess used under Windows.
//
// Revision 3.0.1.4  2004/02/27 09:53:02  pascal_verdier
// - The starter device is now warned by Database server when something change on a server.
// It replace the DbGetHostServersInfo polling.
// - HostState, ControlledRunningServers, ontrolledStoppedServers attributes added.
//
// Revision 3.0.1.3  2003/12/08 08:53:52  pascal_verdier
// Cluster (multi-host) control implemented.
// Control of notify daemon implemented but not tested.
//
// Revision 3.0.1.2  2003/10/15 10:37:08  pascal_verdier
// *** empty log message ***
//
// Revision 3.0.1.1  2003/09/18 12:02:49  pascal_verdier
// Problem on Windows service startup fixed.
//
// Revision 3.0  2003/06/17 12:06:36  pascal_verdier
// TANGO 3.x server.
// polling bugs fixed.
//
// Revision 2.0  2003/01/09 13:35:50  verdier
// TANGO 2.2
//
// Revision 1.22  2002/12/18 08:09:19  verdier
// omniORB compatibility
//
// Revision 1.6  2002/10/15 18:55:21  verdier
// The host state is now calculated during the State polled command.
//
// Revision 1.5  2002/02/27 15:19:32  verdier
// WinNt services implemented.
//
// Revision 1.4  2001/04/03 09:40:50  verdier
// WInNt version is running.
//
// Revision 1.3  2001/02/12 09:34:21  verdier
// SunOS and Linux OK.
//
// Revision 1.2  2000/10/12 08:54:13  verdier
// Compatible with tango 2 done
//  Update from database info added.
//
// Revision 1.1  2000/07/31 13:48:42  verdier
// Initial revision
//
//
// copyleft :     European Synchrotron Radiation Facility
//                BP 220, Grenoble 38043
//                FRANCE
//
//-=============================================================================
//
//  		This file is generated by POGO
//	(Program Obviously used to Generate tango Object)
//
//             (c) - Pascal Verdier - ESRF
//=============================================================================


//===================================================================
//
//	The following table gives the correspondence
//	between commands and method name.
//
//  Command name          |  Method name
//	----------------------------------------
//  State                 |  dev_state()
//  Status                |  dev_status()
//  DevStartAll           |  dev_start_all()
//  DevStopAll            |  dev_stop_all()
//  DevGetRunningServers  |  dev_get_running_servers()
//  DevGetStopServers     |  dev_get_stop_servers()
//  DevStart              |  dev_start()
//  DevStop               |  dev_stop()
//  DevReadLog            |  dev_read_log()
//  HardKillServer        |  hard_kill_server()
//  NotifyDaemonState     |  notify_daemon_state()
//  UpdateServersInfo     |  update_servers_info()
//
//===================================================================


#ifdef WIN32
#	include <process.h>
#	include <direct.h>
#	include <io.h>
#else
#	include <sys/wait.h>
#	include <sys/time.h>
#endif

#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>

#include <tango.h>
#include <StarterUtil.h>
#include <Starter.h>
#include <StarterClass.h>



#ifndef	TIME_VAR
#ifndef WIN32

#	define	TimeVal	struct timeval
#	define	GetTime(t)	gettimeofday(&t, NULL);
#	define	Elapsed(before, after)	\
		1000.0*(after.tv_sec-before.tv_sec) + \
		((double)after.tv_usec-before.tv_usec) / 1000

#else

#	define	TimeVal	struct _timeb
#	define	GetTime(t)	_ftime(&t);
#	define	Elapsed(before, after)	\
		1000*(after.time - before.time) + (after.millitm - before.millitm)

#endif	/*	WIN32		*/
#endif	/*	TIME_VAR	*/

//#define DisplayTime

namespace Starter_ns
{


//+------------------------------------------------------------------
/**
 *	Class Destructor
 */
//+------------------------------------------------------------------
Starter::~Starter()
{
	//	Stop ping threads
	vector<ControledServer>::iterator it;
	for (it=servers.begin() ; it<servers.end() ; it++)
	{
		it->thread_data->set_stop_thread();
	}
	util->proc_util->stop_it();
	ms_sleep(1000);
	delete dbase;
	delete util;
	delete attr_HostState_read;
	delete attr_NotifdState_read;
	delete start_proc_data;
}

//+----------------------------------------------------------------------------
//
// method : 		Starter::Starter(string &s)
// 
// description : 	constructor for simulated Starter
//
// in : - cl : Pointer to the Tango::DeviceClass object
//      - s : Device name 
//
//-----------------------------------------------------------------------------
Starter::Starter(Tango::DeviceClass *cl,string &s):Tango::Device_3Impl(cl,s.c_str())
{
	starting = true;
	init_device();
}

Starter::Starter(Tango::DeviceClass *cl,const char *s):Tango::Device_3Impl(cl,s)
{
	starting = true;
	init_device();
}

Starter::Starter(Tango::DeviceClass *cl,const char *s,const char *d)
:Tango::Device_3Impl(cl,s,d)
{
	starting = true;
	init_device();
}

//+----------------------------------------------------------------------------
//
// method : 		Starter::init_device()
// 
// description : 	initialise the deviuce at startup and at restart.
//
//+----------------------------------------------------------------------------
void Starter::init_device()
{
	cout << "Starter::Starter() init device " << device_name << endl;

	// Initialise variables to default values.
	//--------------------------------------------
	bool	read_properties_done = false;
	while (!read_properties_done)
	{
		try {
			get_device_property();
			read_properties_done = true;
		}
		catch(Tango::DevFailed &e) {
			Tango::Except::print_exception(e);
		}
	}

	if (serverStartupTimeout<SERVER_TIMEOUT)
		serverStartupTimeout = SERVER_TIMEOUT;

	//	First time, check if instance and host name are coherent
	check_host();


	//	Do it only at startup and not at Init command
	//----------------------------------------------------
	if (starting==true)
	{
		//	Get database server name
		//--------------------------------------
		Tango::Util *tg = Tango::Util::instance();
		string	dbname = tg->get_database()->get_dbase()->name();
		//	And connect database as DeviceProxy
		//--------------------------------------
		dbase = new Tango::DeviceProxy(dbname);
//		dbase->set_timeout_millis(500);

		//	Build a shared data for StartProcessShared
		start_proc_data = new StartProcessShared();

		//	Get hostname (In case of cluster host could be multiple)
		//-------------------------------------------------------------
		vector<string>	hosts_list;
		char	*env = (char *)getenv("TANGO_CLUSTER");
		if (env==NULL)
			hosts_list.push_back(tg->get_host_name());
		else
		if (strlen(env)==0)
			hosts_list.push_back(tg->get_host_name());
		else
		{
			//	If MULTI_HOST is defined, parse host names
			//--------------------------------------------------
			string	str_list(env);
			cout << "hosts_list = " << str_list << endl;
			int	start = 0;
			int	end = 0;
			while ((end=str_list.find_first_of(":", start))>0)
			{
				string	s = str_list.substr(start, end-start);
				hosts_list.push_back(s);
				start = end+1;
			}
			string	s = str_list.substr(start, str_list.length()-start);
			hosts_list.push_back(s);
			for (unsigned int i=0 ; i<hosts_list.size() ; i++)
				cout << hosts_list[i] << endl;
		}
		//	Create a StarterUtil instance
		//--------------------------------------
		util = new StarterUtil(dbase, hosts_list);

		//	Initialize Attribute data member
		attr_HostState_read   = new Tango::DevShort[1];
		attr_NotifdState_read = new Tango::DevState[1];
		attr_NotifdState_read[0] = notifyd_state = Tango::UNKNOWN;

		//	Do not want exception during startup
		throwable = false;

		//	Wait a bit if necessary
		if (waitForDriverStartup>0)
		{
			cout << "Waiting " << waitForDriverStartup <<
					" seconds before starting (wait for drivers)." << endl;
			ms_sleep(1000*waitForDriverStartup);
		}

		//	Start notify daemon if not desabled and not already running
		if (useEvents)
		{
			try
			{
				cout << "Checking " << util->notifyd_name << endl;
				if (util->is_notifyd_alive()!=Tango::ON)
				{
					string	name(NOTIFY_DAEMON_SCRIPT);
					name += "/";
					name += tg->get_host_name();
					cout << "Starting " << name << endl;
					dev_start((char*)name.c_str());
				}
			}
			catch (...) {}
		}

		//	query database for controled objects
		//	Wait for Database device is  OK
		bool	done = false;
		while (!done)
		{
			try {
				util->build_server_ctrl_object(&servers);
				do_update_from_db = false;
				done = true;
			}
			catch(Tango::DevFailed &e) {
				Tango::Except::print_exception(e);
			}
#			ifdef WIN32
				_sleep(1000);
#			else
				sleep(1);
#			endif
		}
		
//	A a wait for first ping timeout !!!!
#	ifdef WIN32
		_sleep(3000);
#	else
		sleep(3);
#	endif

		//	And Start servers for all startup levels.
		//	The interStartupLevelWait value will be managed
		//		by the start process thread.
		//---------------------------------------------------
		int nb_levels =
			((static_cast<StarterClass *>(get_device_class()))->nbStartupLevels);

		if (startServersAtStartup==true)
		{
			//	Update state before
			for (unsigned int i=0 ; i<servers.size() ; i++)
			{
				ControledServer	*server = &servers[i];
				server->state = server->thread_data->get_state();
			}
			//	And then start levels
			for (int level=1 ; level<=nb_levels ; level++)
			{
				dev_start_all(level);
				ms_sleep(50);
			}
		}

		//	Want exception during normal run
		throwable = true;

		//	Set the default state
		//-------------------------------
		set_state(Tango::MOVING);
		//set_status("Tango::MOVING");
		*attr_HostState_read = get_state();

		//	Update Loggs
		WARN_STREAM << "Starter Server Started !" << endl;
		cout << "Starter Server Started !" << endl;

		//	Start a thread to start polling
		PollingState	*poller = new PollingState(get_name());
		poller->start();
	}
}


//+----------------------------------------------------------------------------
//
// method : 		Starter::readDeviceProperies()
// 
// description : 	Read the device properties from database.
//
//-----------------------------------------------------------------------------
void Starter::get_device_property()
{
	//	Initialize your default values here.
	//------------------------------------------
	fireFromDbase = true;

	//	Read device properties from database.(Automatic code generation)
	//-------------------------------------------------------------
	Tango::DbData	dev_prop;
	dev_prop.push_back(Tango::DbDatum("StartDsPath"));
	dev_prop.push_back(Tango::DbDatum("WaitForDriverStartup"));
	dev_prop.push_back(Tango::DbDatum("UseEvents"));
	dev_prop.push_back(Tango::DbDatum("StartServersAtStartup"));
	dev_prop.push_back(Tango::DbDatum("InterStartupLevelWait"));
	dev_prop.push_back(Tango::DbDatum("ServerStartupTimeout"));

	//	Call database and extract values
	//--------------------------------------------
	if (Tango::Util::instance()->_UseDb==true)
		get_db_device()->get_property(dev_prop);
	Tango::DbDatum	def_prop, cl_prop;
	StarterClass	*ds_class =
		(static_cast<StarterClass *>(get_device_class()));
	int	i = -1;

	//	Try to initialize StartDsPath from class property
	cl_prop = ds_class->get_class_property(dev_prop[++i].name);
	if (cl_prop.is_empty()==false)	cl_prop  >>  startDsPath;
	else {
		//	Try to initialize StartDsPath from default device value
		def_prop = ds_class->get_default_device_property(dev_prop[i].name);
		if (def_prop.is_empty()==false)	def_prop  >>  startDsPath;
	}
	//	And try to extract StartDsPath value from database
	if (dev_prop[i].is_empty()==false)	dev_prop[i]  >>  startDsPath;

	//	Try to initialize WaitForDriverStartup from class property
	cl_prop = ds_class->get_class_property(dev_prop[++i].name);
	if (cl_prop.is_empty()==false)	cl_prop  >>  waitForDriverStartup;
	else {
		//	Try to initialize WaitForDriverStartup from default device value
		def_prop = ds_class->get_default_device_property(dev_prop[i].name);
		if (def_prop.is_empty()==false)	def_prop  >>  waitForDriverStartup;
	}
	//	And try to extract WaitForDriverStartup value from database
	if (dev_prop[i].is_empty()==false)	dev_prop[i]  >>  waitForDriverStartup;

	//	Try to initialize UseEvents from class property
	cl_prop = ds_class->get_class_property(dev_prop[++i].name);
	if (cl_prop.is_empty()==false)	cl_prop  >>  useEvents;
	else {
		//	Try to initialize UseEvents from default device value
		def_prop = ds_class->get_default_device_property(dev_prop[i].name);
		if (def_prop.is_empty()==false)	def_prop  >>  useEvents;
	}
	//	And try to extract UseEvents value from database
	if (dev_prop[i].is_empty()==false)	dev_prop[i]  >>  useEvents;

	//	Try to initialize StartServersAtStartup from class property
	cl_prop = ds_class->get_class_property(dev_prop[++i].name);
	if (cl_prop.is_empty()==false)	cl_prop  >>  startServersAtStartup;
	else {
		//	Try to initialize StartServersAtStartup from default device value
		def_prop = ds_class->get_default_device_property(dev_prop[i].name);
		if (def_prop.is_empty()==false)	def_prop  >>  startServersAtStartup;
	}
	//	And try to extract StartServersAtStartup value from database
	if (dev_prop[i].is_empty()==false)	dev_prop[i]  >>  startServersAtStartup;

	//	Try to initialize InterStartupLevelWait from class property
	cl_prop = ds_class->get_class_property(dev_prop[++i].name);
	if (cl_prop.is_empty()==false)	cl_prop  >>  interStartupLevelWait;
	else {
		//	Try to initialize InterStartupLevelWait from default device value
		def_prop = ds_class->get_default_device_property(dev_prop[i].name);
		if (def_prop.is_empty()==false)	def_prop  >>  interStartupLevelWait;
	}
	//	And try to extract InterStartupLevelWait value from database
	if (dev_prop[i].is_empty()==false)	dev_prop[i]  >>  interStartupLevelWait;

	//	Try to initialize ServerStartupTimeout from class property
	cl_prop = ds_class->get_class_property(dev_prop[++i].name);
	if (cl_prop.is_empty()==false)	cl_prop  >>  serverStartupTimeout;
	else {
		//	Try to initialize ServerStartupTimeout from default device value
		def_prop = ds_class->get_default_device_property(dev_prop[i].name);
		if (def_prop.is_empty()==false)	def_prop  >>  serverStartupTimeout;
	}
	//	And try to extract ServerStartupTimeout value from database
	if (dev_prop[i].is_empty()==false)	dev_prop[i]  >>  serverStartupTimeout;



	//	End of Automatic code generation
	//-------------------------------------------------------------
	if (dev_prop[0].is_empty())
		cout << "WARNING : startDsPath NOT defined !!!!" << endl;
	if (startDsPath.size()==0)
		startDsPath.push_back(".");
	else
	for (unsigned int i=0 ; i<startDsPath.size() ; i++)
		INFO_STREAM << "startDsPath[" << i << "] = " << startDsPath[i] << endl;
	INFO_STREAM << "WaitForDriverStartup = " << waitForDriverStartup << " seconds" << endl;
	cout << "UseEvents  = " << ((useEvents==false)? "False": "True") << endl;
	cout << "interStartupLevelWait  = " << interStartupLevelWait << endl;
	cout << "serverStartupTimeout   = " << serverStartupTimeout << endl;



	//	Get the fireFromDbase value from Default object
	Tango::DbData	data;
	data.push_back(Tango::DbDatum("FireToStarter"));
	Tango::Util *tg = Tango::Util::instance();
	tg->get_database()->get_property("Default", data);
	string	tmp;
	if (data[0].is_empty()==false)
		data[0]  >>  tmp;
	transform(tmp.begin(), tmp.end(), tmp.begin(), ::tolower);
	if (tmp=="false")
		fireFromDbase = false;
	cout << "fireFromDbase = " << fireFromDbase << endl;
}
//+----------------------------------------------------------------------------
//
// method : 		Starter::always_executed_hook()
// 
// description : 	method always executed before any command is executed
//
//-----------------------------------------------------------------------------
void Starter::always_executed_hook()
{
}
//+----------------------------------------------------------------------------
//
// method : 		Starter::read_attr_hardware()
// 
// description : 	Hardware acquisition for attributes.
//
//-----------------------------------------------------------------------------
void Starter::read_attr_hardware(vector<long> &attr_list)
{
	
	//	Update servers state
	for (unsigned int i=0 ; i < attr_list.size() ; i++)
	{
		Tango::WAttribute &att = dev_attr->get_w_attr_by_ind(attr_list[i]);
		string attr_name = att.get_name();
		if (attr_name == "Servers")
			for (unsigned int j=0 ; j<servers.size() ; j++)
			{
				servers[j].state = servers[j].thread_data->get_state();
				//if (servers[j].name=="")
				//	cout << "read_attr_hardware:[" << servers[j].name << "]	" <<
				//				Tango::DevStateName[servers[j].state]  << endl;
			}
	}
}
//+----------------------------------------------------------------------------
//
// method : 		Starter::read_HostState
// 
// description : 	Extract real attribute values for HostState acquisition result.
//
//-----------------------------------------------------------------------------
void Starter::read_HostState(Tango::Attribute &attr)
{
	DEBUG_STREAM << "Starter::read_HostState(Tango::Attribute &attr) entering... "<< endl;
	*attr_HostState_read = (short) get_state();
	DEBUG_STREAM << "HostState = " << attr_HostState_read[0] << endl;
	attr.set_value(attr_HostState_read);
}

//+----------------------------------------------------------------------------
//
// method : 		Starter::read_NotifdState
// 
// description : 	Extract real attribute values for NotifdState acquisition result.
//
//-----------------------------------------------------------------------------
void Starter::read_NotifdState(Tango::Attribute &attr)
{
	DEBUG_STREAM << "Starter::read_NotifdState(Tango::Attribute &attr) entering... "<< endl;
	attr_NotifdState_read[0] = notifyd_state;
	attr.set_value(attr_NotifdState_read);
}

//+----------------------------------------------------------------------------
//
// method : 		Starter::read_Servers
// 
// description : 	Extract real attribute values for Servers acquisition result.
//
//-----------------------------------------------------------------------------
void Starter::read_Servers(Tango::Attribute &attr)
{
	DEBUG_STREAM << "Starter::read_Servers(Tango::Attribute &attr) entering... "<< endl;

	//	Check starting ones
	vector<string>	vs;
	for (unsigned int i=0 ; i<servers.size() ; i++)
	{
		TangoSys_OMemStream tms;
		tms << servers[i].name << "\t" << 
					Tango::DevStateName[servers[i].state] << "\t" <<
					servers[i].controled  << "\t" << servers[i].startup_level;
		string	s = tms.str();
		vs.push_back(s);
		
	}
	//	And fill attribute
	stringArrayServers << vs;	
	attr.set_value(stringArrayServers.get_buffer(), stringArrayServers.length());
}


//+----------------------------------------------------------------------------
//
// method : 		Starter::read_RunningServers
// 
// description : 	Extract real attribute values for RunningServers acquisition result.
//
//-----------------------------------------------------------------------------
void Starter::read_RunningServers(Tango::Attribute &attr)
{
	DEBUG_STREAM << "Starter::read_RunningServers(Tango::Attribute &attr) entering... "<< endl;

	//	Check running ones
	vector<string>	runnings;
	for (unsigned int i=0 ; i<servers.size() ; i++)
		if (servers[i].state==Tango::ON)
			runnings.push_back(servers[i].name);

	//	And fill attribute
	stringArrayRunning << runnings;	
	attr.set_value(stringArrayRunning.get_buffer(), stringArrayRunning.length());
}

//+----------------------------------------------------------------------------
//
// method : 		Starter::read_StoppedServers
// 
// description : 	Extract real attribute values for StoppedServers acquisition result.
//
//-----------------------------------------------------------------------------
void Starter::read_StoppedServers(Tango::Attribute &attr)
{
	DEBUG_STREAM << "Starter::read_StoppedServers(Tango::Attribute &attr) entering... "<< endl;

	//	Check stopped ones
	vector<string>	stopped;
	for (unsigned int i=0 ; i<servers.size() ; i++)
		if (servers[i].state!=Tango::ON)
			stopped.push_back(servers[i].name);
	//	And fill attribute
	stringArrayStopped << stopped;	
	attr.set_value(stringArrayStopped.get_buffer(), stringArrayStopped.length());
}

//+------------------------------------------------------------------
/**
 *	method:	Starter::dev_state
 *
 *	description:	method to execute "DevState"
 *	Command to read the device state.
 *	The method dev_state() override a DeviceImpl class virtual method.
 *
 * @return	State Code
 *
 */
//+------------------------------------------------------------------
Tango::DevState Starter::dev_state()
{
	INFO_STREAM << "Starter::dev_state(): entering... !" << endl;

	//	Check if last command is more than readInfoDbPeriod class property
	int	period =
		((static_cast<StarterClass *>(get_device_class()))->readInfoDbPeriod);

	//	If not fired -> do it myself by polling
	//---------------------------------------------
	if (fireFromDbase==false)
	{
		static time_t	t0 = 0;
	    	   time_t	t1 = time(NULL);		
		//	If less -> no update
		if (t1-t0 >= period)
		{
			t0 = t1;

			//	Update control obj from database (could have been modified)
			INFO_STREAM << "Updating from data base" << endl;
			util->build_server_ctrl_object(&servers);
		}
	}
	else
	if (do_update_from_db)
	{
		//	Has been fired from Dbase
		util->build_server_ctrl_object(&servers);
		do_update_from_db = false;
	}
	//	Check for notify daemon state if requested
	//---------------------------------------------
	if (useEvents)
		notifyd_state = util->is_notifyd_alive();
	else
		notifyd_state = Tango::ON;

	//	Check if servers object initilized
	//---------------------------------------
	if (servers.size()==0)
	{
		INFO_STREAM << "Exiting dev_state() with servers.size() null" << endl;
		if (notifyd_state==Tango::ON)
			set_state(Tango::ON);
		else
			set_state(Tango::ALARM);
		return DeviceImpl::dev_state();
	}

	//	Check hown many servers are running
	//-----------------------------------------------------------
	ControledServer		*p_serv;
	int		nb_running   = 0;
	int		nb_controled = 0;
	int		nb_starting  = 0;
	for (unsigned int i=0 ; i<servers.size() ; i++)
	{
		p_serv = &servers[i];
		//	Count how many are controlled
		if (p_serv->controled)
		{
			nb_controled++;

			//	Fixe witch one is running and count how many controlled are running
			if ((p_serv->state==Tango::ON))
				nb_running ++;
			else
			if (p_serv->state==Tango::MOVING)
				nb_starting ++;
		}
	}

	//	compare nb running with nb_controlled controled to set state
	if (nb_starting>0 || start_proc_data->get_starting_processes()>0)
		set_state(Tango::MOVING);
	else
	if (nb_running==nb_controled && notifyd_state==Tango::ON)
		set_state(Tango::ON);
	else
		set_state(Tango::ALARM);

	//cout << DeviceImpl::dev_state() << endl;
//time_t 	t2 = time(NULL);
//cout << "------------------------------------> " << (t2-t1) << " seconds" << endl;

	return DeviceImpl::dev_state();
}


//+------------------------------------------------------------------
/**
 *	Check if a process could be started (file exists, is not running, ...)
 */
//+------------------------------------------------------------------
NewProcess *Starter::processCouldStart(char *argin)
{
	INFO_STREAM << "Starter::processCouldStart(\""<< argin << "\"): entering... !" << endl;
	
	//	Make sure that it's not running.
	//---------------------------------------
	if (servers.size()>0)
	{
		string	name(argin);
		ControledServer	*server = util->get_server_by_name(name, servers);
		if (server!=NULL)
			if (server->state!=Tango::FAULT)
			{
				INFO_STREAM << argin << " is already running !" <<endl;
				TangoSys_OMemStream tms;
				tms << argin << " is already running !" << ends;
				if (throwable)
					Tango::Except::throw_exception(
								(const char *)"ALREADY_RUNNING",
								tms.str(),
								(const char *)"Starter::dev_start()");
				return NULL;
			}
	}

	//	Separate server name and instancename.
	//-------------------------------------
	char	*servname     = util->get_server_name(argin) ;
	char	*instancename = util->get_instance_name(argin);
	char	*adminname = new char[strlen(servname)+ strlen(instancename)+10];
	sprintf(adminname, "dserver/%s/%s", servname, instancename);
	char	*filename;
	try {
		filename = util->check_exe_file(servname, startDsPath);
	}
	catch(Tango::DevFailed &e)
	{
		free(servname);
		if (throwable)
			throw e;
		else
		{
			cout << e.errors[0].desc << endl;
			return NULL;
		}
	}
	free(servname);

	//	Check if log dir already exists.
	//-------------------------------------
	string	log_file = util->build_log_file_name(argin);
	
	string	logpath;
	LogPath(logpath);
	INFO_STREAM << "LOG file : " << log_file << endl;
	if (chdir(logpath.c_str())==-1)
	{
		if (errno==ENOENT)
		{
			//	Create directory
			//-------------------------
			cerr << "ENOENT" << endl;
			cerr << errno << "  " << strerror(errno) << endl;
#ifdef WIN32
			mkdir(TmpRoot);
			int r = mkdir(logpath.c_str());
#else
#	ifdef linux
			int r = mkdir(logpath.c_str(), (mode_t)(0775) );
#	else
			int r = mkdir(logpath.c_str(), (mode_t)(O_RDWR | O_CREAT, 0775) );
#	endif
#endif
			if (r<0)
			{
				TangoSys_OMemStream	message;
				message << "Cannot create error log directory:\n";
				message << logpath;
				message << "\n" << strerror(errno) << endl;
				cerr << message.str() << endl;;
				set_status(message.str());
				Tango::Except::throw_exception(
									(const char *)"CANNOT_CREATE_LOG_FILE",
									message.str(),
									(const char *)"Starter::dev_start");
			}
			else
			{
				TangoSys_OMemStream	tms;
				tms << logpath << " Created !" << endl;
				INFO_STREAM << tms.str() << endl;
				set_status(tms.str());
			}
		}
		else
		{
			TangoSys_OMemStream	tms;
			tms << "Cannot change to log directory:\n";
			tms << logpath;
			tms << "\n" << strerror(errno) << endl;
			cerr << tms.str() << endl;;
			set_status(tms.str());
		}
	}
	
	NewProcess	*np  = new NewProcess;
	np->servname     = filename;
	np->instancename = instancename;
	np->adminname    = adminname;
	np->logfile      = new char[log_file.length()+1];
	np->logfile      = strcpy(np->logfile, log_file.c_str());
	
	return np;
}
//+------------------------------------------------------------------
/**
 *	method:	Starter::dev_start
 *
 *	description:	method to execute "DevStart"
 *	Start the specified server.
 *
 * @param	argin	Server to be started.
 *
 */
//+------------------------------------------------------------------
void Starter::dev_start(Tango::DevString argin)
{
	//INFO_STREAM 
	cout << "Starter::dev_start(\""<< argin << "\"): entering... !" << endl;

	NewProcess	*np = processCouldStart(argin);
	if (np==NULL)
		return;

	//	Build a vector to start process
	vector<NewProcess *>	processes;
	processes.push_back(np);
	startProcesses(processes, 0);
}
//+------------------------------------------------------------------
//+------------------------------------------------------------------
void Starter::startProcesses(vector<NewProcess *> v_np, int level)
{
	//	Start process to start processes
	//-------------------------------------
	start_proc_data->push_back_level(level);
	StartProcessThread	*pt =
		new StartProcessThread(v_np, level, this);
	pt->start();
}

//+------------------------------------------------------------------
/**
 *	method:	Starter::dev_stop
 *
 *	description:	method to execute "DevStop"
 *	Stop the specified server.
 *
 * @param	argin	Servero be stopped.
 *
 */
//+------------------------------------------------------------------
void Starter::dev_stop(Tango::DevString argin)
{
	INFO_STREAM << "Starter::dev_stop("<< argin << "): entering... !" << endl;

	//	Check if servers object initilized
	//---------------------------------------
	if (servers.size()==0)
	{
		TangoSys_OMemStream out_stream;
		out_stream << argin << ": Server  not controlled !" << ends;
		Tango::Except::throw_exception(out_stream.str(),
				out_stream.str(),
				(const char *)"Starter::dev_stop()");
		return;
	}

	//	Check Argin as server name
	//----------------------------------
	string	name(argin);
	ControledServer	*server = util->get_server_by_name(name, servers);
	if (server==NULL)
	{
		TangoSys_OMemStream out_stream;
		out_stream << argin << ": Unkown Server !" << ends;
		Tango::Except::throw_exception(out_stream.str(),
				out_stream.str(),
				(const char *)"Starter::dev_stop()");
		return;
	}

	//	Make shure that it's  running.
	//---------------------------------------
	if (server->state==Tango::ON)
	{
		//	And Kill it with kill signal
		if (server->dev==NULL)
			server->dev =  new Tango::DeviceProxy(server->admin_name);
		server->dev->command_inout("Kill");
		WARN_STREAM << argin << " stopped !" << endl;
		cout << argin << " stopped !" << endl;
	}
	else
	if (server->state==Tango::MOVING)
	{
		TangoSys_OMemStream out_stream;
		out_stream << argin << " is running but not responding !" << ends;
		Tango::Except::throw_exception(
				(const char *)"SERVER_NOT_RESPONDING",
				out_stream.str(),
				(const char *)"Starter::dev_stop()");
		return;
	}
	else
	{
		TangoSys_OMemStream out_stream;
		out_stream << argin << " is NOT running !" << ends;
		Tango::Except::throw_exception(
				(const char *)"SERVER_NOT_RUNNING",
				out_stream.str(),
				(const char *)"Starter::dev_stop()");
		return;
	}
	
}

//+------------------------------------------------------------------
/**
 *	method:	Starter::dev_get_running_servers
 *
 *	description:	method to execute "DevGetRunningServers"
 *	Control the running process from property list.
 *	And return the list of the processes which are really running.
 *
 * @param	argin	True for all servers. False for controled servers only.
 * @return	List of the processes which are running.
 *
 */
//+------------------------------------------------------------------
Tango::DevVarStringArray *Starter::dev_get_running_servers(Tango::DevBoolean all_serv)
{
	Tango::DevVarStringArray	*argout = new Tango::DevVarStringArray;
	INFO_STREAM << "Starter::dev_get_running_server(): entering... !" << endl;

	//	Check if servers object initilized
	//---------------------------------------
	if (servers.size()==0)
	{
		return argout;
	}

	//	prepeare the argout for running servers list
	//-----------------------------------------------------------
	int		nb = 0;
	int		x;
	unsigned int	i;
	for (i=0 ; i<servers.size() ; i++)
		if (all_serv || servers[i].controled)
			if (servers[i].state==Tango::ON)
				nb ++;

	//	And fill it
	//-----------------------------------------------------------
	argout->length(nb);
	for (i=0, x=0 ; i<servers.size() && x<nb ; i++)
		if (all_serv || servers[i].controled)
			if (servers[i].state==Tango::ON)
			{
				INFO_STREAM << "RUNNING: " << servers[i].name << endl;
				(*argout)[x++] = CORBA::string_dup(servers[i].name.c_str());
			}
	return argout;
}

//+------------------------------------------------------------------
/**
 *	method:	Starter::dev_get_stop_servers
 *
 *	description:	method to execute "DevGetStopServers"
 *	Control the running process from property list.
 *	And return the list of the processes which are not running.
 *
 * @param	argin	True for all servers. False for controled servers only.
 * @return	List of the processes which are not running.
 *
 */
//+------------------------------------------------------------------
Tango::DevVarStringArray *Starter::dev_get_stop_servers(Tango::DevBoolean all_serv)
{
	Tango::DevVarStringArray	*argout = new Tango::DevVarStringArray();
	INFO_STREAM << "Starter::dev_get_stop_servers(): entering... !" << endl;

	//	Check if servers object initilized
	//---------------------------------------
	if (servers.size()==0)
	{
		argout->length(0);
		return argout;
	}

	//	prepeare the argout for NOT running servers list
	//-----------------------------------------------------------
	int		nb = 0;
	int		x;
	unsigned int	i;
	for (i=0 ; i<servers.size() ; i++)
		if (all_serv || servers[i].controled)
			if (servers[i].state!=Tango::ON)
				nb ++;

	//	And fill it
	//-----------------------------------------------------------
	argout->length(nb);
	for (i=0, x=0  ; i<servers.size() && x<nb; i++)
		if (all_serv || servers[i].controled)
			if (servers[i].state!=Tango::ON)
			{
				INFO_STREAM << "STOPPED: " << servers[i].name << endl;
				(*argout)[x++] = CORBA::string_dup(servers[i].name.c_str());
			}

	return argout;
}
//+------------------------------------------------------------------
/**
 *	Return how many servers to start for specified level.
 */
//+------------------------------------------------------------------
int	Starter::nb_servers_to_start(int level)
{
	int	cnt = 0;
	for (unsigned int i=0 ; i<servers.size() ; i++)
	{
		ControledServer	*server = &servers[i];
		//	server->running could not be initialized
		if (server->controled  &&  server->startup_level==level)
			if (server->state!=Tango::ON)
				cnt++;
	}
	return cnt;
}

//+------------------------------------------------------------------
/**
 *	method:	Starter::dev_start_all
 *
 *	description:	method to execute "DevStartAll"
 *	Start all device servers controled on the host for the argin level.
 *
 * @param	argin	Startup level.
 *
 */
//+------------------------------------------------------------------
void Starter::dev_start_all(Tango::DevShort level)
{
	cout << "Starter::dev_start_all(): entering for level "<< level <<"... !" << endl;

	Tango::DevBoolean	throw_it = false;
	//	Check if servers object initilized
	//---------------------------------------
	if (servers.size()==0)
		if (throwable)
			throw_it = true;
		else
			return;
	if (throw_it)
	{
			TangoSys_OMemStream out_stream;
			out_stream << "NO Server  controlled !" << ends;
			Tango::Except::throw_exception(out_stream.str(),
			out_stream.str(),
				(const char *)"Starter::dev_start_all()");
	}

	//	Do not want exception during startup
	throwable = false;

	//	And start the stopped ones
	//---------------------------------------------------
	vector<NewProcess *>	processes;
	for (unsigned int i=0 ; i<servers.size() ; i++)
	{
		ControledServer	*server = &servers[i];
		//	server->running could not be initialized
		if (server->controled  &&  server->startup_level==level)
		{
			cout << "Check startup for " << server->name << endl;
			if (server->state==Tango::FAULT)
			{
				NewProcess	*np = processCouldStart((char*)server->name.c_str());
				if (np!=NULL)
				{
					processes.push_back(np);
					cout << "Try to start " << np->servname << endl;
				}
				else
					cout << "np is null (?)" << endl;
			}
			else
				cout << "	Alread running...."<< endl;
		}
	}
	if (processes.size()>0)
		startProcesses(processes, level);

	//	Want exception during normal run
	throwable = true;
}

//+------------------------------------------------------------------
/**
 *	method:	Starter::dev_stop_all
 *
 *	description:	method to execute "DevStopAll"
 *	Stop all device servers controled on the host for the argin level.
 *
 * @param	argin	Startup Level.
 *
 */
//+------------------------------------------------------------------
void Starter::dev_stop_all(Tango::DevShort level)
{
	INFO_STREAM << "Starter::dev_stop_all(): entering... !" << endl;

	//	Check if servers object initilized
	//---------------------------------------
	if (servers.size()==0)
	{
		TangoSys_OMemStream out_stream;
		out_stream << "NO Server  controlled !" << ends;
		Tango::Except::throw_exception(out_stream.str(),
				out_stream.str(),
				(const char *)"Starter::dev_stop_all()");
		return;
	}
	//	And stop the running ones
	//---------------------------------------------------
	for (unsigned int i=0 ; i<servers.size() ; i++)
	{
		ControledServer	*server = &servers[i];
		if (server->controled             &&
			server->startup_level==level  &&
			server->state==Tango::ON)
				dev_stop((char*)server->name.c_str());
	}
}

//+------------------------------------------------------------------
/**
 *	method:	Starter::update_servers_info
 *
 *	description:	method to execute "UpdateServersInfo"
 *	Indicate to the device server than the information about servers to be controlled has been modified.
 *	The device server must read the database to update the servers info list.
 *	If the default case, this command is sent by Database server itself.
 *
 *
 */
//+------------------------------------------------------------------
void Starter::update_servers_info()
{
	INFO_STREAM << "Starter::update_servers_info(): entering... !" << endl;

	do_update_from_db = true;
}

//+------------------------------------------------------------------
/**
 *	method:	Starter::dev_read_log
 *
 *	description:	method to execute "DevReadLog"
 *	At server startup, its standard error is redirected to a log file.
 *	This command will read this file and return the read string from the file.
 *
 * @param	argin	server name and domain
 * @return	ig Starter/corvus)
 *
 */
//+------------------------------------------------------------------
Tango::ConstDevString Starter::dev_read_log(Tango::DevString argin)
{
	string filename = util->build_log_file_name(argin);

	//	Try to open log file
	ifstream	ifs((char *)filename.c_str());
	if (!ifs)
	{
		//	Open log file failed -> Throw exception
		//----------------------------------------------
		TangoSys_OMemStream reason;
		TangoSys_OMemStream description;
		reason << "Cannot open " << filename << ends;
		description << strerror(errno);
		Tango::Except::throw_exception(reason.str(),
						description.str(),
						(const char *)"Starter::dev_read_log");
	}

	//	Read and close log file, and return string read from it.
	//-------------------------------------------------------------
	stringstream	strlog;
	strlog << filename << endl;
	strlog << util->get_file_date((char *)filename.c_str()) << endl << endl;
	strlog << ifs.rdbuf() << ends;
	ifs.close();
	returned_str = strlog.str();
	return returned_str.c_str();
}

//+------------------------------------------------------------------
/**
 *	method:	Starter::notify_daemon_state
 *
 *	description:	method to execute "NotifyDaemonState"
 *	Returns the Notify Daemon state.
 *
 * @return	Tango::ON if Notify daemon is running else Tango::FAULT.
 *
 */
//+------------------------------------------------------------------
Tango::DevState Starter::notify_daemon_state()
{
	INFO_STREAM << "Starter::notify_daemon_state(): entering... !" << endl;

	if (useEvents==false)
		Tango::Except::throw_exception(
					(const char *)"NOTIFY_NOT_AVAILABLE",
					(const char *)"Notify Daemon control is disabled",
					(const char *)"Starter::notify_daemon_state()");
	return notifyd_state;
}

//+------------------------------------------------------------------
/**
 *	method:	Starter::hard_kill_server
 *
 *	description:	method to execute "HardKillServer"
 *	Hard kill a server (kill -9)
 *
 * @param	argin	Server name
 *
 */
//+------------------------------------------------------------------
void Starter::hard_kill_server(Tango::DevString argin)
{
	DEBUG_STREAM << "Starter::hard_kill_server(): entering... !" << endl;

	string	servname(argin);
	int	pid = util->proc_util->get_server_pid(servname);
	if (pid<0)
	{
		TangoSys_OMemStream tms;
		tms << "Server " << argin << " is not running !";
		Tango::Except::throw_exception(
					(const char *)"SERVER_NOT_RUNNING",
					tms.str().c_str(),
					(const char *)"Starter::hard_kill_server()");
	}
#ifdef WIN32

	HANDLE	handle = NULL;				//- process addr (in the heap)
	if( (handle=OpenProcess(PROCESS_TERMINATE, false, pid)) == NULL)
	{
		TangoSys_OMemStream tms;
		tms << "Open handle on server " << argin << " failed !";
		Tango::Except::throw_exception(
					(const char *)"KILL_DERVER_FAILED",
					tms.str().c_str(),
					(const char *)"Starter::hard_kill_server()");
	}
	
	TerminateProcess(handle, 0);
	CloseHandle(handle);
	if (GetLastError()!= ERROR_SUCCESS)
	{
		TangoSys_OMemStream tms;
		tms << "Kill server " << argin << " failed !";
		Tango::Except::throw_exception(
					(const char *)"KILL_DERVER_FAILED",
					tms.str().c_str(),
					(const char *)"Starter::hard_kill_server()");
	}

#else

	TangoSys_OMemStream cmd;
	cmd << "kill -9 " << pid;
	system(cmd.str().c_str());

#endif
}
//=================================================================
//=================================================================
void Starter::check_host()
{
	string	hostname = Tango::Util::instance()->get_host_name();
	transform(hostname.begin(), hostname.end(), hostname.begin(), ::tolower);
	//	remove FQDN
	string::size_type	pos = hostname.find('.');
	if (pos!=string::npos)
		hostname = hostname.substr(0, pos);

	string	devname = device_name;
	transform(devname.begin(), devname.end(), devname.begin(), ::tolower);

	//	Get only member
	pos = devname.find('/');
	if (pos!=string::npos)
	{
		pos = devname.find('/', pos+1);
		if (pos!=string::npos)
			devname = devname.substr(pos+1);
	}
	//cout << hostname << " == " << devname << endl;
	
	if (devname != hostname)
	{
		TangoSys_OMemStream	tms;
		tms << "This server must run on " << devname << " and not on "  << hostname;
		string	descr(tms.str());
		
		Tango::Except::throw_exception(
				(const char *)"BAD_PARAM",
				(const char *) descr.c_str(),
				(const char *)"Starter::check_host()");
	}
}


}	//	namespace
