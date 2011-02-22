static const char *RcsId = "$Header$";
//+=============================================================================
//
// file :         StarterUtil.cpp
//
// description :  C++ source for tools used by the Starter device server.
//
// project :      TANGO Device Server
//
// $Author$
//
// $Revision$
//
// $Log$
// Revision 3.15  2007/05/25 06:08:00  pascal_verdier
// Iterator management modified (strange in vc8 ?)
//
// Revision 3.14  2007/05/03 06:54:58  pascal_verdier
// Re-try on DServer exported added before polling startup added.
//
// Revision 3.13  2007/03/30 14:21:49  pascal_verdier
// remove 2 warnings on linux.
//
// Revision 3.12  2006/11/20 06:58:37  pascal_verdier
// Mutex on start process data added.
//
// Revision 3.11  2006/11/10 14:53:28  pascal_verdier
// Remove vc8 warnings.
//
// Revision 3.10  2006/06/05 07:20:36  pascal_verdier
// Server startup is now delayed (with timeout) in a startup level.
// New state MOVING added.
// At startup, starter loop until properties have been read.
//
// Revision 3.9  2006/04/24 07:06:28  pascal_verdier
// A thread is started for each level when at servers startup.
//
// Revision 3.8  2006/02/09 11:59:19  pascal_verdier
// A ping thread is now started for each server.
//
// Revision 3.7  2005/09/19 13:35:22  pascal_verdier
// change on traces.
//
// Revision 3.6  2005/08/26 07:18:16  pascal_verdier
// FQDN management added.
// Time between startup level added.
//
// Revision 3.5  2005/05/18 12:11:56  pascal_verdier
// Start a batch file under windows.
//
// Revision 3.4  2005/04/21 07:18:48  pascal_verdier
// Add a little timeout for ping.
// Fix a bug for windows (SIGCHLD).
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
// Revision 1.2  2001/04/03 09:40:50  verdier
// WInNt version is running.
//
// Revision 1.1  2001/02/12 09:34:21  verdier
// Initial revision
//
//
// copyleft :     European Synchrotron Radiation Facility
//                BP 220, Grenoble 38043
//                FRANCE
//
//-=============================================================================

#include <stdio.h>

#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#ifndef WIN32
#	include <sys/time.h>
#endif
//#include <dirent.h>


#include <tango.h>
#include <StarterUtil.h>
#include <sstream>


namespace Starter_ns
{

int StarterUtil::elapsed;
#ifndef WIN32
struct timeval	StarterUtil::before, StarterUtil::after;
#else
#endif /* WIN32 */

//+------------------------------------------------------------------
/**
 *	Contructor - Initialize data members.
 */
//+------------------------------------------------------------------
StarterUtil::StarterUtil(Tango::DeviceProxy *database, vector<string> host_names)
{
	dbase = database;
	//hostnames     = host_names;
	notifyd_name  = "notifd/factory/";
//	notifyd_name  = "dserver/starter/";
	notifyd_name += host_names[0];
	ch_factory   = NULL;
	
	//	Remove the Fully Qualify Domain Name for tango less than 5.2 compatibility
	for (unsigned int i=0 ; i<host_names.size() ; i++)
		hostnames.push_back(removeFQDN(host_names[i]));
}
//+------------------------------------------------------------------
/**
 *	Remove the Fully Qualify Domain Name for tango less than 5.2 compatibility
 */
//+------------------------------------------------------------------
string StarterUtil::removeFQDN(string s)
{
	string::size_type	pos = s.find('.');
	if (pos == string::npos)
		return s;
	else
		return 	s.substr(0, pos);
}
//+------------------------------------------------------------------
/**
 *	Extract server name from input parameter (servname/instance).
 */
//+------------------------------------------------------------------
char *StarterUtil::get_server_name(char *argin)
{
	char	*servname;
	char	tmp[50];
	char	*p1 = argin;
	char	*p2 = tmp;	
	while (*p1 && *p1!='/')
		*p2++ = *p1++;
	*p2++ = '\0';
	servname = (char *)malloc(strlen(tmp)+1);
	strcpy(servname, tmp);
	return servname;
}
//+------------------------------------------------------------------
/**
 *	Extract instance name from input parameter (servname/instance).
 */
//+------------------------------------------------------------------
char *StarterUtil::get_instance_name(char *argin)
{
	char	*instancename;
	char	tmp[50];
	char	*p1 = argin;
	char	*p2 = tmp;	
	while (*p1 && *p1!='/')
		p1++;
	p1++;
	while (*p1)
		*p2++ = *p1++;
	*p2++ = '\0';
	
	instancename = (char *)malloc(strlen(tmp)+1);
	strcpy(instancename, tmp);
	return instancename;
}
//+----------------------------------------------------------------------------
//
// method : 		StarterUtil::check_file()
// 
// description : 	Check if executable file exists
//					and return its full name with good path.
//
//-----------------------------------------------------------------------------
char *StarterUtil::check_exe_file(string name)
{
	static char	*result = NULL;
	if (result==NULL)
		free(result);
	string	filename(name);
#ifdef	WIN32
	filename += ".exe";
#endif
	//cout << "Checking " << filename << endl;
	ifstream	ifs(filename.c_str());
	if (ifs)
	{
		ifs.close();
		result = (char *)malloc(strlen(filename.c_str())+1);
		strcpy(result, filename.c_str());
		return result;
	}
#ifdef	WIN32

	//	Check for catch file
	filename = name;
	filename += ".bat";
	//cout << "Checking " << filename << endl;
	ifstream	ifs2(filename.c_str());
	if (ifs2)
	{
		ifs2.close();
		result = (char *)malloc(strlen(filename.c_str())+1);
		strcpy(result, filename.c_str());
		return result;
	}
#endif

	return NULL;
}
//+----------------------------------------------------------------------------
//
// method : 		StarterUtil::check_file()
// 
// description : 	Check if executable file exists
//					and return its full name with good path.
//
//-----------------------------------------------------------------------------
char *StarterUtil::check_exe_file(char *servname, vector<string>v_path)
{
	unsigned int	i;	
	for (i=0 ; i<v_path.size() ; i++)
	{
		string	filename(v_path[i]);
		filename += slash;
		filename += servname;
	
		//	Check if exist
		char *p;
		if ((p=check_exe_file(filename))!=NULL)
			return p;
	}

	//	server has not been found in path(es)
	//----------------------------------------------
	TangoSys_MemStream out_stream;
	out_stream << servname << " :  not found in \'StartDsPath\' property:" << endl;
	for (i=0 ; i<v_path.size() ; i++)
		out_stream << " - " << v_path[i] << endl;
	out_stream << ends;
	Tango::Except::throw_exception("CANNOT_RUN_FILE",
				out_stream.str(),
				(const char *)"StarterUtil::check_exe_file()");
	return NULL;
}
//+------------------------------------------------------------------
/**
 *	Format the date and time in the argin value (Ux format) as string.
 */
//+------------------------------------------------------------------
char *StarterUtil::strtime(time_t t)
{
	static char	str[20] ;
	struct tm	*st = localtime(&t) ;

	if (st->tm_year>=100)
		st->tm_year -= 100 ;
	sprintf (str, "%02d/%02d/%02d   %02d:%02d:%02d",
								st->tm_mday, st->tm_mon+1, st->tm_year,
								st->tm_hour, st->tm_min, st->tm_sec ) ;
	return str ;
}
//+------------------------------------------------------------------
/**
 *	Get the last modification on a file and return it in a string.
 *	@param	filename	file's name to get the date.
 */
//+------------------------------------------------------------------
char *StarterUtil::get_file_date(char *filename)
{
	struct stat	info;
	stat(filename, &info);
	return strtime(info.st_mtime);
}

//+------------------------------------------------------------------
/**
 *	method:	StarterUtil::build_log_file_name
 *
 *	description:	Build the error log file name from server name and intancename.
 *
 *      @param  path     path to write file
 *      @param  server   name of the server
 */
//+------------------------------------------------------------------
string StarterUtil::build_log_file_name(char *server)
{
	//	Separate server name and intance name.
	//-----------------------------------------
	char	servname[50];
	char	intancename[50];
	char	*p1 = server;
	char	*p2 = servname;	
	while (*p1 && *p1!='/')
		*p2++ = *p1++;
	*p2++ = '\0';
	p1++;

	p2 = intancename;	
	while (*p1)
		*p2++ = *p1++;
	*p2++ = '\0';

	//	And create full name with path
	//-----------------------------------------
	string	log_file;
	LogPath(log_file);
	log_file += slash;
	log_file += servname;
	log_file += "_";
	log_file += intancename;
	log_file += ".log";
	return log_file;
}
#ifdef WIN32
//+------------------------------------------------------------------
/**
 *	This method does not exist in visual cpp
 *
 *    The  strcasecmp()  and  strncasecmp()  functions  are  case-
 *    insensitive  versions  of   strcmp()  and  strncmp() respec-
 *    tively, described below.  They assume  the  ASCII  character
 *    set  and ignore differences in case when comparing lower and
 *    upper case characters.
 */
//+------------------------------------------------------------------
int StarterUtil::strcasecmp(const char *s1, const char *s2)
{
	if (strlen(s1) != strlen(s2))
		return 1;

	for ( ; *s1 ; s1++, s2++)
		if (tolower(*s1) != tolower(*s2))
			return 1;
	return 0;
}
#endif










//====================================================================
//
//	The controlled Servers Object management
//
//====================================================================

//+------------------------------------------------------------------
/**
 *	Get host device servers list from database.
 *
 *	@param dbase	Database device as a DeviceProxy for not implemented API commands.
 */
//+------------------------------------------------------------------
vector<string>	StarterUtil::get_host_ds_list()
{
	//	Read server info from database.
	vector<string>		servnames;
	for (unsigned int i=0 ; i<hostnames.size() ; i++)
	{
		Tango::DeviceData	argin;
		argin << hostnames[i];
		cout << "DbGetHostServerList for " << hostnames[i] << endl;
		Tango::DeviceData	argout = dbase->command_inout("DbGetHostServerList", argin);
		vector<string>	tmp;
		argout >> tmp;

		//	Check servers really used (Erase this one and database server
		vector<string>::iterator pos;
		for (pos=tmp.begin() ; pos<tmp.end() ; pos++)
		{
			int	idx = (*pos).find_first_of("/");
			if (idx>0)
			{
				//	Get process name only in lower case before compeare
				string	s = (*pos).substr(0, idx);
				transform(s.begin(), s.end(), s.begin(), ::tolower);
				if (s=="starter"  ||  s=="databaseds" || s=="logconsumer")
				{
					tmp.erase(pos);
					pos--;	//	because erase decrease size !
				}
			}
		}
		//	Copy to global vector
		for (unsigned int j=0 ; j<tmp.size() ; j++)
			servnames.push_back(tmp[j]);
	}
	cout << "----------------------------------------" << endl;
	cout << servnames.size() << " servers found" << endl;
	for (unsigned int j=0 ; j<servnames.size() ; j++)
		cout << "\t" <<  servnames[j]	<< endl;

	return servnames;
}
//+------------------------------------------------------------------
/**
 *	Read DS info from database to know if it is controled
 *		and it's starting level.
 *
 *	@param	devname	device to get info.
 *	@param	server	object to be updated from db read.
 */
//+------------------------------------------------------------------
void StarterUtil::get_server_info(ControledServer *server)
{
	try
	{
		//	Read server info from database.
		Tango::DeviceData	argin;
		argin << server->name;
		Tango::DeviceData	argout = dbase->command_inout("DbGetServerInfo", argin);
		vector<string>	result;
		argout >> result;
		server->controled     = (atoi(result[2].c_str())==0)? false: true;
		server->startup_level =  atoi(result[3].c_str());
	}
	catch(Tango::DevFailed &e)
	{
		Tango::Except::print_exception(e);
		server->controled = false;
		server->startup_level = 0;
	}
	//cout << server->name << " - " << ((server->controled)? "true": "false");
	//cout << " ----> Startup level " << server->startup_level <<endl;
}
//+------------------------------------------------------------------
/**
 *	Allocate and fill the servers controled object
 */
//+------------------------------------------------------------------
void StarterUtil::build_server_ctrl_object(vector<ControledServer> *servers)
{
	bool trace = false;
	if (trace)	cout << "build_server_ctrl_object()" << endl;
	unsigned int	i;
	vector<string>	result_from_db;
	for (i=0 ; i<hostnames.size() ; i++)
	{
		//	Call for servers and their info for each host
		cout << "Call for servers on " << hostnames[i] << endl;
		Tango::DeviceData	argin;
		argin << hostnames[i];
		Tango::DeviceData	argout = dbase->command_inout("DbGetHostServersInfo", argin);
		argout >> result_from_db;
	}

	if (trace)	cout << "--------------  Check if list of servers modified  --------------" << endl;
	
	//	Check servers really used (erase this one and database server
	vector<string>::iterator pos;
	vector<string>	result;
	for (pos=result_from_db.begin() ; pos<result_from_db.end() ; pos+=3)
	{
		int	idx = (*pos).find_first_of("/");
		if (idx>0)
		{
			//	Get process name only in lower case before compeare
			string	s = (*pos).substr(0, idx);
			transform(s.begin(), s.end(), s.begin(), ::tolower);
			if (s!="starter"  &&  s!="databaseds" && s!="logconsumer")
			{
				result.push_back(*pos);		//	Server name
				result.push_back(*(pos+1));	//	Controlled/Not Controlled
				result.push_back(*(pos+2));	//	Startup Level
			}
		}
	}

	//	Check if some servers have disappeared
	vector<ControledServer>::iterator it;
	bool	redo = true;	//	Iterators management seems to have changed
	                        //	between vc6 and vc8  (??)
	while (redo)
	{
		redo = false;
		for (it=servers->begin() ; it<servers->end() ; it++)
		{
			string	s1(it->name);
			bool	found = false;
			for (i=0 ; !found && i<result.size() ; i+=3)
				found = (s1==result[i]);

			if (!found)
			{
				if (trace)	cout << s1 << " has disappeared" << endl;
				//	if disappeared then stop thread and remove reference
				it->thread_data->set_stop_thread();
				servers->erase(it);
				redo = true;
				break;	//	get out of loop (vector size has changed.
			}
		}
	}
	//	Check if new servers appeared
	for (pos=result.begin() ; pos<result.end() ; )
	{
		string	name(*pos++);
		ControledServer	*p_serv = get_server_by_name(name, *servers);
		if (p_serv==NULL)
		{
			if (trace)	cout << name << " appeared " << endl;
			//	Create a new server instance
			ControledServer	server;

			server.name = name;
			server.admin_name = "dserver/" + server.name;
			server.dev = NULL;
			server.controled = (atoi((*pos++).c_str())==0)? false: true;
			server.startup_level =  atoi((*pos++).c_str());
			server.running = true;

			//	Add a thread to ping server
			server.thread_data = new PingThreadData(server.name);
			server.thread = new PingThread(server.thread_data, server.name);
			server.thread->start();

			servers->push_back(server);
			ms_sleep(50);
		}
		else
		{
			//	Update levels
			p_serv->controled     = (atoi((*pos++).c_str())==0)? false: true;
			p_serv->startup_level =  atoi((*pos++).c_str());
		}
	}
}

//+------------------------------------------------------------------
/**
 *	search a server in ControledServer array by it's name .
 *
 *	@param servname	Server searched name.
 */
//+------------------------------------------------------------------
ControledServer *StarterUtil::get_server_by_name(string &servname, vector<ControledServer> &servers)
{
	for (unsigned int i=0 ; i<servers.size() ; i++)
	{
		ControledServer	*server = &servers[i];
		if (server->name == servname)
			return server;
	}
	return NULL;
}

//+----------------------------------------------------------------------------
//
// method : 		StarterUtil::start_timer()
// 
// description : 	initilize a timer
//
//-----------------------------------------------------------------------------
void StarterUtil::start_timer()
{
#ifndef WIN32
	gettimeofday(&before, NULL);
#else
#endif /* WIN32 */
}

//+----------------------------------------------------------------------------
//
// method : 		StarterUtil::get_timer_value()
// 
// description : 	Return the current value of the timer
//
//-----------------------------------------------------------------------------
int	StarterUtil::get_timer_value()
{
#ifndef WIN32
	gettimeofday(&after, NULL);
	elapsed = (after.tv_sec-before.tv_sec)*1000000;
	elapsed = (after.tv_usec-before.tv_usec) + elapsed;
#else
	elapsed = -1;
#endif /* WIN32 */
	return elapsed;
}
//+----------------------------------------------------------------------------
//
// method : 		StarterUtil::
// 
// description : 	Return true if Notify Daemon is alive
//
//-----------------------------------------------------------------------------
void StarterUtil::import_notifyd()
{
	//	Get info about notify daemon
	Tango::DeviceData	argin;
	argin << notifyd_name;
	Tango::DeviceData	import_argout =
				dbase->command_inout("DbImportEvent", argin);
	const Tango::DevVarLongStringArray	*lsa;
	import_argout >> lsa;

	//	store ior
	string	factory_ior      = string((lsa->svalue)[1]);

	//	Try to connect
	Tango::Util		*tu = Tango::Util::instance();
	CORBA::ORB_ptr	_orb = tu->get_orb();

	CORBA::Object *event_factory_obj =
		 _orb -> string_to_object(factory_ior.c_str());

	if (event_factory_obj -> _non_existent())
		event_factory_obj = CORBA::Object::_nil();

	// Narrow the CORBA_Object reference to an EventChannelFactory
	// reference so we can invoke its methods
	//CORBA::Object	*_eventChannelFactory =	event_factory_obj;

	//	Test the connection
	ch_factory =
	CosNotifyChannelAdmin::EventChannelFactory::_narrow(event_factory_obj);
}
//+----------------------------------------------------------------------------
//
// method : 		StarterUtil::
// 
// description : 	Return true if Notify Daemon is alive
//
//-----------------------------------------------------------------------------
Tango::DevState StarterUtil::is_notifyd_alive()
{
	try
	{
		if (ch_factory==NULL)
			import_notifyd();

		CosNotifyChannelAdmin::ChannelIDSeq	*ch_id = ch_factory->get_all_channels();
		delete ch_id;

		cout2 << notifyd_name << "EventChannelFactory is ON" << endl;
 		return Tango::DevState(Tango::ON);
	}
	catch (...)
	{
		cerr << "Failed to narrow the EventChannelFactory on " << notifyd_name << endl;
		cerr << " make sure the notifd process is running on this host" << endl;
		ch_factory = NULL;
 		return Tango::DevState(Tango::FAULT);
	}
	//	Never used.
	return Tango::DevState(Tango::ON);
}












//+----------------------------------------------------------------------------
/**
 *	Create a thread to start  polling.
 */
//+----------------------------------------------------------------------------
PollingState::PollingState(string devname)
{
	device_name = devname;
}
//+----------------------------------------------------------------------------
/**
 *	Execute the thread commands
 */
//+----------------------------------------------------------------------------
void *PollingState::run_undetached(void *ptr)
{
	//	Check if dserver exported
	bool		done = false;
	Tango::Util *tg = Tango::Util::instance();
	Tango::DServer		*adm_dev = NULL;
	for (int i=0 ; !done && i<10 ; i++)
	{
		try
		{
			ms_sleep(500);
			cout << "Try if DServer exported " << i << "/10" << endl;
			adm_dev = tg->get_dserver_device();
			adm_dev->ping();
			done = true;
		}
		catch(Tango::DevFailed &e)
		{
			Tango::Except::print_exception(e);
		}
	}
	//	If dserver not started , do nothing
	if (adm_dev!=NULL)
	{
		long	upd_period = 1000;
		add_obj_polling(adm_dev,"attribute", "State",          upd_period);
		add_obj_polling(adm_dev,"attribute", "HostState",      upd_period);
		add_obj_polling(adm_dev,"attribute", "RunningServers", upd_period);
		add_obj_polling(adm_dev,"attribute", "StoppedServers", upd_period);
	}
	return NULL;
}
 
//+------------------------------------------------------------------
/**
 *	Add the Polling object.
 */
//+------------------------------------------------------------------
void PollingState::add_obj_polling(Tango::DServer *adm_dev,string type, string objname, int upd_period)
{
	Tango::DevVarLongStringArray	*lsa = new Tango::DevVarLongStringArray();
	lsa->lvalue.length(1);
	lsa->svalue.length(3);
	lsa->lvalue[0] = upd_period;
	lsa->svalue[0] = CORBA::string_dup(device_name.c_str());
	lsa->svalue[1] = CORBA::string_dup(type.c_str());
	lsa->svalue[2] = CORBA::string_dup(objname.c_str());
	CORBA::Any	any;
	any <<= lsa;
	try {
		CORBA::Any	*any_out =
			adm_dev->command_inout("AddObjPolling", any);
		delete any_out;
	}
	catch (Tango::DevFailed &e) {

		string	reason(e.errors[0].reason);
		if (reason=="API_AlreadyPolled")
		{
			//	If already polled -> just update period.
			//----------------------------------------------
			try {
				CORBA::Any	*any_out =
					adm_dev->command_inout("UpdObjPollingPeriod", any);
				delete any_out;
				//cout <<"UpdObjPollingPeriod(" << objname <<
				//			", " << upd_period << " ms)" << endl;
			}
			catch (Tango::DevFailed &e) {
				Tango::Except::print_exception(e);
			}
		}
		else
			Tango::Except::print_exception(e);
	}
}
//+----------------------------------------------------------------------------



}	//	namespace
