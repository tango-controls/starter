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


namespace Starter
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
	hostnames     = host_names;
	notifyd_name  = "notifd/factory/";
	notifyd_name += host_names[0];	//	Just one.
	ch_factory   = NULL;
}
//+------------------------------------------------------------------
/**
 *	Extract server name from input parameter (servname/instance).
 */
//+------------------------------------------------------------------
char *StarterUtil::get_server_name(char *argin)
{
	static char servname[50];
	char	*p1 = argin;
	char	*p2 = servname;	
	while (*p1 && *p1!='/')
		*p2++ = *p1++;
	*p2++ = '\0';
	return servname;
}
//+------------------------------------------------------------------
/**
 *	Extract instance name from input parameter (servname/instance).
 */
//+------------------------------------------------------------------
char *StarterUtil::get_instance_name(char *argin)
{
	static char instancename[50];
	char	*p1 = argin;
	char	*p2 = instancename;	
	while (*p1 && *p1!='/')
		p1++;
	p1++;
	while (*p1)
		*p2++ = *p1++;
	*p2++ = '\0';
	
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
char *StarterUtil::check_exe_file(char *servname, vector<string>v_path)
{
	static char	*result = NULL;
	if (result==NULL)
		free(result);


	int	i;	
	for (i=0 ; i<v_path.size() ; i++)
	{
		string	filename(v_path[i]);
		filename += slash;
		filename += servname;
#ifdef	WIN32
		filename += ".exe";
#endif
		cout2 << "Checking " << filename << endl;
	
		//	Check if exist
		ifstream	ifs(filename.c_str());
		if (ifs)
		{
			ifs.close();
			result = (char *)malloc(strlen(filename.c_str())+1);
			strcpy(result, filename.c_str());
			return result;
		}
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
	log_file += "/";
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
	for (int i=0 ; i<hostnames.size() ; i++)
	{
		Tango::DeviceData	argin;
		argin << hostnames[i];
		cout2 << "DbGetHostServerList for " << hostnames[i] << endl;
		Tango::DeviceData	argout = dbase->command_inout("DbGetHostServerList", argin);
		vector<string>	tmp;
		argout >> tmp;

		//	Check servers really used (Erase this one and database server
		Tango::Util *tg = Tango::Util::instance();
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
		for (int j=0 ; j<tmp.size() ; j++)
			servnames.push_back(tmp[j]);
	}
	cout2 << "----------------------------------------" << endl;
	cout2 << servnames.size() << " servers found" << endl;
	for (int j=0 ; j<servnames.size() ; j++)
		cout2 << "\t" <<  servnames[j]	<< endl;

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
void StarterUtil::get_server_info(ServerControled *server)
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
void StarterUtil::build_server_ctrl_object(vector<ServerControled> *servers)
{
	cout << "build_server_ctrl_object()" << endl;
	//	The first time, check if DbGetHostServersInfo command
	//	is implemented in this database server version.
	//-------------------------------------------------------------
	static bool	one_call = true;
	vector<string>	result;
	if (one_call)
	{
		try
		{
			for (int i=0 ; i<hostnames.size() ; i++)
			{
				//	Call for servers and their info for each host
				Tango::DeviceData	argin;
				argin << hostnames[i];
				Tango::DeviceData	argout = dbase->command_inout("DbGetHostServersInfo", argin);
				vector<string>	tmp;
				argout >> tmp;
				//	Copy to global vector
				for (int j=0 ; j<tmp.size() ; j++)
					result.push_back(tmp[j]);
			}

		}
		catch(Tango::DevFailed &e)
		{
			Tango::Except::print_exception(e);
			//	Check if it is due to a prvious DtabaseDs version
			string  reason(e.errors[0].reason);
			if (reason=="API_CommandNotFound")
				one_call = false;
			else
				throw e;
		}
	}
	if (one_call)
	{
		//	Check servers really used (erase this one and database server
		Tango::Util *tg = Tango::Util::instance();
		vector<string>::iterator pos;
		for (pos=result.begin() ; pos<result.end() ; pos+=3)
		{
			int	idx = (*pos).find_first_of("/");
			if (idx>0)
			{
				//	Get process name only in lower case before compeare
				string	s = (*pos).substr(0, idx);
				transform(s.begin(), s.end(), s.begin(), ::tolower);
				if (s=="starter"  ||  s=="databaseds" || s=="logconsumer")
				{
					result.erase(pos);
					result.erase(pos);
					result.erase(pos);
					pos -=3;	//	because erase decrease size !
				}
			}
		}
		//	If nb servers not modified
		//--------------------------------------------------------------------
		int		nb_serv = result.size() / 3; //	name & 2 info
		int		i;
		bool	modif = false;
		if (nb_serv!=servers->size())
			modif = true;
		else
			//	Check if list of servers modified
			//--------------------------------------------------------
			for (i=0 ; i<servers->size() ; i++)
				if (i<result.size())
					if ((*servers)[i].name!=result[3*i])
						modif = true;
		//	If modified re-create object.
		//--------------------------------------------------------
		if (modif)
		{
			servers->clear();
			//	Create Server  controled object
			//---------------------------------------------------------
			cout2 << "------------->   Allocate memory for object !!!" << endl;
			for (i=0 ; i<result.size() ; i+=3)
			{
				ServerControled	server;

				//	get names
				//-------------------------
				server.name = result[i];
				server.admin_name = "dserver/" + server.name;
				server.dev = NULL;

				servers->push_back(server);
			}
		}
		//	Set if controlled or not
		for (i=0 ; i<servers->size() ; i++)
		{
			(*servers)[i].controled     = (atoi(result[3*i+1].c_str())==0)? false: true;
			(*servers)[i].startup_level =  atoi(result[3*i+2].c_str());
		}
	}
	if (one_call==false)
	{
		build_server_ctrl_object_multi_call(servers);
		return;
	}
}
//+------------------------------------------------------------------
/**
 *	Allocate and fill the servers controled object
 */
//+------------------------------------------------------------------
void StarterUtil::build_server_ctrl_object_multi_call(vector<ServerControled> *servers)
{
	//	Get the list of the servers running on this host
	//----------------------------------------------------
	vector<string>	servnames;
	servnames  = get_host_ds_list();

	//	Get the number of servers (if none, nothing to do)
	//--------------------------------------------------------
	if (servnames.size()==0)
		return;

	//	If nb servers not modified
	//--------------------------------------------------------------------
	int		i;
	bool	modif = false;
	if (servnames.size()!=servers->size())
		modif = true;
	else
		//	Check if list of servers modified
		//--------------------------------------------------------
		for (i=0 ; i<servnames.size() ; i++)
			if (i<servers->size())
				if ((*servers)[i].name!=servnames[i])
					modif = true;

	//	If modified re-create object.
	//--------------------------------------------------------
	if (modif)
	{
		servers->clear();
		//	Create Server  controled object
		//---------------------------------------------------------
		cout2 << "------------->   Allocate memory for object !!!" << endl;
		for (i=0 ; i<servnames.size() ; i++)
		{
			ServerControled	server;

			//	get names
			//-------------------------
			server.name = servnames[i];
			server.admin_name = "dserver/" + server.name;
			server.dev = NULL;

			servers->push_back(server);
		}
	}

	//	Set if controlled or not
	for (i=0 ; i<servnames.size() ; i++)
		get_server_info(&((*servers)[i]));
}




//+------------------------------------------------------------------
/**
 *	search a server in ServerControled array by it's name .
 *
 *	@param servname	Server searched name.
 */
//+------------------------------------------------------------------
ServerControled *StarterUtil::get_server_by_name(string servname, vector<ServerControled> &servers)
{
	for (int i=0 ; i<servers.size() ; i++)
	{
		ServerControled	*server = &servers[i];
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
	CORBA::Object	*_eventChannelFactory =	event_factory_obj;

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
		cout << "Failed to narrow the EventChannelFactory on " << notifyd_name << endl;
		cout << " make sure the notifd process is running on this host" << endl;
		ch_factory = NULL;
 		return Tango::DevState(Tango::FAULT);
	}
	//	Never used.
	return Tango::DevState(Tango::ON);
}












//+----------------------------------------------------------------------------
/**
 *	Consdstructor for Shared data object 
 */
//+----------------------------------------------------------------------------
SharedData::SharedData()
{
	//	Get the polling thread ID
	//Tango::Util *tg = Tango::Util::instance();
	//polling_id = tg->get_polling_thread_object();
	
}
//+----------------------------------------------------------------------------
/**
 *	Return the elapsed time from last command.
 */
//+----------------------------------------------------------------------------
time_t SharedData::get_timer()
{
	omni_thread	*this_thread = omni_thread::self();
	cout2 << "Thread : " << this_thread << " - SharedData::get_timer()" << endl;
	omni_mutex_lock sync(*this);
	//	A Command has been received -> reset timer
	time_t	retval = time(NULL) - last_cmd_time;
	cout2 << "Thread : " << this_thread << " SharedData::get_timer() done" << endl;
	return retval;
}
//+----------------------------------------------------------------------------
/**
 *	Set the timer to curent time (a command has been received).
 */
//+----------------------------------------------------------------------------
void SharedData::set_timer()
{
	omni_thread	*this_thread = omni_thread::self();
	cout2 << "Thread : " << this_thread << " - SharedData::set_timer()" << endl;
	omni_mutex_lock sync(*this);
	//	Do it only if command is not form polling thread
	//omni_thread	*this_thread = omni_thread::self();
	//cout2 << "SharedData::set_timer() - thread: " << this_thread << endl;
	//if (this_thread != polling_id)
	//{
		//	A Command has been received -> reset timer
		last_cmd_time = time(NULL);
		signal();
	//}
	//else
	//	cout2 << "SharedData::set_timer() not done" << endl;
	cout2 << "Thread : " << this_thread << " - SharedData::set_timer() done" << endl;
}
//+----------------------------------------------------------------------------
/**
 *	Create a thread to start or stop state polling.
 *	Start will be done from astor.
 *	Stop will be done from astor or from a timeout without command.
 */
//+----------------------------------------------------------------------------
PollingState::PollingState(SharedData *sd, long timeout)
{
	shared      = sd;
	cmd_timeout = timeout;
}
//+----------------------------------------------------------------------------
/**
 *	Execute the thread loop.
 *	This thread is awaken when a command has been received 
 *	and falled asleep when no command has been received from a long time.
 */
//+----------------------------------------------------------------------------
void *PollingState::run_undetached(void *ptr)
{
	Tango::Util 	*tg = Tango::Util::instance();
	Tango::DServer	*dev = tg->get_dserver_device();
	CORBA::Any		any;
	omni_thread	*this_thread = omni_thread::self();

	polling_on = false;
	while(true)
	{
		time_t	delay = shared->get_timer();
		cout1 << " PollingState::run() alive.  -  " << delay  << endl;

		//	If a command has been received and polling not started
		//			then START it
		if (delay<cmd_timeout  && polling_on==false)
		{
			cout1 << "	Start polling " << endl;
			try
			{
				dev->command_inout("StartPolling", any);
				polling_on = true;
			}
			catch(Tango::DevFailed &e)
			{
				Tango::Except::print_exception(e);
			}
		}
#ifdef NO_EVENTS
		else
		//	If a command has not been received for a long time 
		//	and polling started   -  > then STOP it
		if (delay>=cmd_timeout  && polling_on==true)
		{
			cout1 << "	Stop polling " << endl;
			try
			{
				dev->command_inout("StopPolling", any);
				polling_on = false;
			}
			catch(Tango::DevFailed &e)
			{
				Tango::Except::print_exception(e);
			}
		}
		
		//	And wait a bit if pollin on 
		//	Or wait until next command.
		{
			omni_mutex_lock sync(*shared);
			if (polling_on==true)
				shared->wait(cmd_timeout * 1000);
			else
				shared->wait();
		}
		cout2 << "Thread : " << this_thread << " awaken" << endl;
#else
		//	Since events are used DO NOT STOP POLLING
		cout << "PollingState::run_undetached() exiting" << endl;
		return NULL;
#endif
		
	}
	return NULL;
}
 
//+----------------------------------------------------------------------------



}	//	namespace
