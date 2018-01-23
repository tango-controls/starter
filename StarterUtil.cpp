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
// Copyright (C) :      2004,2005,2006,2007,2008,2009,2010,2011,2012,2013,2014,2015
//						European Synchrotron Radiation Facility
//                      BP 220, Grenoble 38043
//                      FRANCE
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
// $Date$
//
//-=============================================================================

#include <tango.h>

#include <fcntl.h>
#include <sys/stat.h>
#ifndef _TG_WINDOWS_
#	include <sys/time.h>
#endif


#include <StarterUtil.h>


namespace Starter_ns
{

int StarterUtil::elapsed;
#ifndef _TG_WINDOWS_
struct timeval	StarterUtil::before, StarterUtil::after;
#else
#endif /* _TG_WINDOWS_ */

//+------------------------------------------------------------------
/**
 *	Contructor - Initialize data members.
 */
//+------------------------------------------------------------------
StarterUtil::StarterUtil(Tango::DeviceProxy *database, vector<string> host_names, string logHome)
{
	dbase = database;
	notifyd_name  = "notifd/factory/";
	notifyd_name += host_names[0];
	ch_factory   = NULL;
	log_home     = logHome;

	//	Remove the Fully Qualify Domain Name for tango less than 5.2 compatibility
	for (unsigned int i=0 ; i<host_names.size() ; i++)
		hostnames.push_back(removeFQDN(host_names[i]));

	proc_util = new CheckProcessUtil();
	proc_util->start();
	//	Wait a bit to be sure
	//	that running process list is updated.
#ifdef _TG_WINDOWS_
	_sleep(1000);
#else
	sleep(1);
#endif

	//	Build starter log file name.
	LogPath(starter_log_file, logHome);
	starter_log_file += "/Starter.log";

	LogPath(starter_stat_file, logHome);
	starter_stat_file += "/Starter.stat";

	cout << "---->  starter_log_file  = " << starter_log_file << endl;
	cout << "---->  starter_stat_file = " << starter_stat_file << endl;
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
    string fullName(argin);
    string serverName;
    string::size_type	pos = fullName.find('/');
	if (pos == string::npos)
		serverName = fullName;
	else
		serverName = fullName.substr(0, pos);

	char	*servname = new char[serverName.length()+1];
	strcpy(servname, serverName.c_str());
	return servname;
}
//+------------------------------------------------------------------
/**
 *	Extract instance name from input parameter (servname/instance).
 */
//+------------------------------------------------------------------
char *StarterUtil::get_instance_name(char *argin)
{
    string fullName(argin);
    string instanceName;
    string::size_type	pos = fullName.find('/');
	if (pos == string::npos)
		instanceName = fullName;
	else
		instanceName = fullName.substr(pos+1);

	char	*instancename = new char[instanceName.length()+1];
	strcpy(instancename, instanceName.c_str());
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
	string	filename(name);
#ifdef	_TG_WINDOWS_
	filename += ".exe";
#endif
	//cout << "Checking " << filename << endl;
	ifstream	ifs(filename.c_str());
	if (ifs)
	{
		ifs.close();
		char *exeFullName = new char[strlen(filename.c_str())+1];
		strcpy(exeFullName, filename.c_str());
		return exeFullName;
	}
#ifdef	_TG_WINDOWS_

	//	Check for batch file
	filename = name;
	filename += ".bat";
	//cout << "Checking " << filename << endl;
	ifstream	ifs2(filename.c_str());
	if (ifs2)
	{
		ifs2.close();
		char *exeFullName = new char[strlen(filename.c_str())+1];
		strcpy(exeFullName, filename.c_str());
		return exeFullName;
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
	TangoSys_OMemStream out_stream;
	out_stream << servname << " :  not found in \'StartDsPath\' property:" << endl;
	for (i=0 ; i<v_path.size() ; i++)
		out_stream << " - " << v_path[i] << endl;
	out_stream << ends;
	Tango::Except::throw_exception((const char *)"CANNOT_RUN_FILE",
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
 *	Log info for starter.
 *	@param	message	 mesage to be logged
 */
//+------------------------------------------------------------------
void StarterUtil::log_starter_info(string message)
{
	stringstream	strlog;
	strlog << strtime(time(NULL)) << "\t" << message  << endl;

	//	Read and close log file.
	ifstream	ifs((char *)starter_log_file.c_str());
	if (ifs)
	{
		strlog << ifs.rdbuf();
		ifs.close();
	}
	//	Check for nb lines
	string	str(strlog.str());
	string::size_type	pos = 0;
	int nb = 0;
	while (nb<STARTER_LOG_DEPTH && (pos=str.find('\n', pos+1))!=string::npos)
		nb++;

	if (pos!=string::npos)
		str = str.substr(0, pos);

	//	Write to log file
	ofstream	ofs((char *)starter_log_file.c_str());
	ofs << str;
	ofs.close();
}
//+------------------------------------------------------------------
/**
 *	Reset statistics for starter.
 */
//+------------------------------------------------------------------
void StarterUtil::reset_starter_stat_file(vector<ControlledServer> *servers)
{
	//	build present situation log
	stringstream	strlog;
	vector<ControlledServer>::iterator it;
	for (it=servers->begin() ; it<servers->end() ; ++it)
	{
		if (it->controlled && it->startup_level>0)
		{
			time_t	failure_time = it->failure_time;
			if (failure_time<0)
				failure_time = time(NULL);

			strlog << it->name           << "\t" <<
	                ((it->get_state()==Tango::ON)? "ON\t" : "FAULT\t") <<
	                it->started_time << "\t" <<
						failure_time << "\t" <<
					"false" << endl;
		}
	}

	//	Write an empty statistics file
	ofstream	ofs((char *)starter_stat_file.c_str());
	ofs << strlog.str();
	ofs.close();
}
//+------------------------------------------------------------------
/**
 *	Log statistica for starter.
 *	@param	message	 mesage to be logged
 */
//+------------------------------------------------------------------
void StarterUtil::log_starter_statistics(ControlledServer *server)
{
	stringstream	strlog;
	strlog << server->name           << "\t" <<
	                ((server->get_state()==Tango::ON)? "ON\t" : "FAULT\t") <<
	                server->started_time  << "\t" <<
					server->failure_time  << "\t" <<
					((server->auto_start)? "true" : "false") << endl;
	server->auto_start = false;

	//	Read and close log file.
	ifstream	ifs((char *)starter_stat_file.c_str());
	if (ifs)
	{
		strlog << ifs.rdbuf();
		ifs.close();
	}
	//	Check for nb lines
	string	str(strlog.str());
	string::size_type	pos = 0;
	int nb = 0;
	while (nb<STARTER_STAT_DEPTH && (pos=str.find('\n', pos+1))!=string::npos)
		nb++;

	if (pos!=string::npos)
		str = str.substr(0, pos);

	//	Write to log file
	ofstream	ofs((char *)starter_stat_file.c_str());
	ofs << str;
	ofs.close();
}
//+------------------------------------------------------------------
/**
 *	check if there is not to much log file and rename last one
 *	@param	filename	file's name to get the date and rename.
 */
//+------------------------------------------------------------------
void StarterUtil::manage_log_file_history(char *filename, int nb_max)
{
	string	log_file(filename);

	//	Try to open log file
	ifstream	ifs((char *)log_file.c_str());
	if (!ifs)
		return;	//	Does not exist-> do nothing

	//	Get the log file list
	vector<string>	list =  get_log_file_list(log_file);
	//for (unsigned int i=0 ; i<list.size() ; i++)
	//	cout << list[i] << endl;

	//	Check if too much files -> delete
	while (list.size()>((unsigned int)nb_max-1))	//	-1 because a new one will exist bellow
	{
		//cout << "Removing " << list[0] << endl;
		if (remove(list[0].c_str())<0)
			cerr << "remove failed : " << strerror(errno) << endl;
		list.erase(list.begin());
	}

	//	Build date and time (of file creation) part
	struct stat	info;
	stat(filename, &info);
	struct tm	*st = localtime(&info.st_mtime) ;
	if (st->tm_year>=100)
		st->tm_year -= 100 ;
	char	strdate[32];
	sprintf (strdate, "_[20%02d-%02d-%02d_%02d-%02d-%02d]",
						st->tm_year, st->tm_mon+1, st->tm_mday,
						st->tm_hour, st->tm_min, st->tm_sec ) ;

	//	search position to insert (before extention)
	string	str(filename);
	string::size_type	pos = str.rfind(".log");
	if (pos != string::npos)
		str = str.substr(0, pos);

	char	*new_filename = new char[strlen(filename) + strlen(strdate) +1];
	strcpy(new_filename, str.c_str());
	strcat(new_filename, strdate);
	strcat(new_filename, ".log");
	int ret = rename(filename, new_filename);
	if (ret<0)
		cerr << "Renaming " << filename << " to " << new_filename << " failed : " << strerror(errno) << endl;
	//else
	//	cout << "Renaming " << filename << " to " << new_filename << "  done." << endl;
	delete [] new_filename;
}
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
bool  alphabetical(string a,string b)
{
	//	returns alphabetic order.
	return (a < b);
}
//+------------------------------------------------------------------
/**
 *	Rename log file list
 *	@param	filename	log file name
 */
//+------------------------------------------------------------------
vector<string> StarterUtil::get_log_file_list(string logfile)
{
	vector<string>	list;
	//	Split path and file name
	string::size_type	pos = logfile.rfind(slash);
	if (pos != string::npos)
	{
		string	path = logfile.substr(0, pos);
		pos++;
		//	remove extention
		string	str = logfile.substr(pos);
		pos = str.rfind('.');
		string	filter = str.substr(0, pos);
		filter += "_[";

#ifndef _TG_WINDOWS_
		//cout << "Searching " << filter << "  in " << path << endl;
		DIR		*dir = opendir ((char *)path.c_str()) ;
		if(dir==NULL)
		{
			string	desc;
			//	error
			switch(errno)
			{
				case EACCES: desc = "Permission denied.";
					break;
				case EMFILE: desc = "Too many file descriptors in use by process.";
					break;
				case ENFILE: desc = "Too many file are currently open in the system.";
					break;
				case ENOENT: desc = "Directory does not exist or NAME is an empty string.";
					break;
				case ENOMEM: desc = "Insufficient memory to complete the operation.";
					break;
				case ENOTDIR:desc  = "NAME is not a directory.";
					break;
				default: desc = "Unknown error: ";
						 desc += errno;
			}
			Tango::Except::throw_exception(
					(const char *)"READ_FILE_LIST_FAILED",
					desc,
					(const char *)"StarterUtil::get_log_file_list()");

		}
		struct dirent	*ent;
		while ( (ent=readdir(dir)) )
		{
			string	name(ent->d_name);
			pos = name.find(filter);
			if (pos == 0)
			{
				string	full_name(path);
				full_name += "/";
				full_name += name;
				list.push_back(full_name);
			}
		}
		closedir(dir);
#else
		//cout << "Searching " << filter << "  in " << path << endl;
		path += "\\";
		path += filter;
		path += "*";
		WCHAR	*w_path = ProcessData::string2wchar(path);

		WIN32_FIND_DATA fd;
		DWORD dwAttr = FILE_ATTRIBUTE_DIRECTORY;
		HANDLE hFind = FindFirstFile(w_path, &fd);
		if(hFind != INVALID_HANDLE_VALUE)
		{
			do
			{
				string	s = ProcessData::wchar2string(fd.cFileName);
				list.push_back(s);
			}
			while (FindNextFile( hFind, &fd));
			FindClose( hFind);
		}
		else
		{
			string desc = ProcessData::errorCodeToString(GetLastError(), "FindFirstFile()" );
			cerr << "Invalid Handle value " << desc << endl;
		}
		delete w_path;
#endif
	}
	sort(list.begin(), list.end(), alphabetical);
	return list;
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
	//	Separate server name and instance name.
	/*
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
    */
    string serverName(server);
    unsigned long index = serverName.find('/');
    string binary = serverName.substr(0, index);
    string instance = serverName.substr(++index);

	//	And create full name with path
	string	log_file;
	LogPath(log_file,log_home);
	log_file += slash;
	log_file += binary;
	log_file += "_";
	log_file += instance;
	log_file += ".log";
	return log_file;
}








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
		for (pos=tmp.begin() ; pos<tmp.end() ; ++pos)
		{
			unsigned long idx = (*pos).find_first_of("/");
			if (idx>0)
			{
				//	Get process name only in lower case before compeare
				string	s = (*pos).substr(0, idx);
				transform(s.begin(), s.end(), s.begin(), ::tolower);
				if (s=="starter"  ||
					s=="databaseds" ||
					s=="tangoaccesscontrol" ||
					s=="logconsumer")
				{
					tmp.erase(pos);
					--pos;	//	because erase decrease size !
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
 *	Read DS info from database to know if it is controlled
 *		and it's starting level.
 *
 *	@param	devname	device to get info.
 *	@param	server	object to be updated from db read.
 */
//+------------------------------------------------------------------
void StarterUtil::get_server_info(ControlledServer *server)
{
	try
	{
		//	Read server info from database.
		Tango::DeviceData	argin;
		argin << server->name;
		Tango::DeviceData	argout = dbase->command_inout("DbGetServerInfo", argin);
		vector<string>	result;
		argout >> result;
		server->controlled = atoi(result[2].c_str()) != 0;
		server->startup_level =  (short)atoi(result[3].c_str());
	}
	catch(Tango::DevFailed &e)
	{
		Tango::Except::print_exception(e);
		server->controlled = false;
		server->startup_level = 0;
	}
	//cout << server->name << " - " << ((server->controlled)? "true": "false");
	//cout << " ----> Startup level " << server->startup_level <<endl;
}
//+------------------------------------------------------------------
/**
 *	Allocate and fill the servers controlled object
 */
//+------------------------------------------------------------------
void StarterUtil::build_server_ctrl_object(vector<ControlledServer> *servers)
{
	bool trace = false;
	if (trace)	cout << "build_server_ctrl_object()" << endl;
	unsigned int	i;
	vector<string>	result_from_db;
	for (i=0 ; i<hostnames.size() ; i++)
	{
		//	Call for servers and their info for each host
		//cout << "Call for servers on " << hostnames[i] << endl;
		Tango::DeviceData	argin;
		argin << hostnames[i];
		Tango::DeviceData	argout = dbase->command_inout("DbGetHostServersInfo", argin);
		argout >> result_from_db;
	}

	if (trace)	cout << "--------------  Check if list of servers modified  --------------" << endl;

	//	Check servers really used (erase this one and database server)
	vector<string>::iterator pos;
	vector<string>	result;
	for (pos=result_from_db.begin() ; pos<result_from_db.end() ; pos+=3)
	{
		unsigned long idx = (*pos).find_first_of("/");
		if (idx>0)
		{
			//	Get process name only in lower case before compare
			string	s = (*pos).substr(0, idx);
			transform(s.begin(), s.end(), s.begin(), ::tolower);
			if (s!="starter"            &&
				s!="databaseds"         &&
				s!="tangoaccesscontrol" &&
				s!="logconsumer")
			{
				result.push_back(*pos);		//	Server name
				result.push_back(*(pos+1));	//	Controlled/Not Controlled
				result.push_back(*(pos+2));	//	Startup Level
			}
		}
	}

	//	Check if some servers have disappeared
	vector<ControlledServer>::iterator it;
	bool	redo = true;	//	Iterators management seems to have changed
	                        //	between vc6 and vc8  (??)
	while (redo)
	{
		redo = false;
		for (it=servers->begin() ; it<servers->end() ; ++it)
		{
			string	s1(it->name);
			bool	found = false;
			for (i=0 ; !found && i<result.size() ; i+=3)
				found = (s1==result[i]);

			if (!found)
			{
				if (trace)  cout << s1 << " has disappeared" << endl;
				//	if disappeared then stop thread and remove reference
				it->thread_data->set_stop_thread();
				it->thread->join(0);
				servers->erase(it);
				redo = true;
				break;	//	get out of loop (vector size has changed).
			}
		}
	}
	//	Check if new servers appeared
	for (pos=result.begin() ; pos<result.end() ; )
	{
		string	name(*pos++);
		ControlledServer *p_serv = get_server_by_name(name, *servers);
		if (p_serv==NULL)
		{
			if (trace)	cout << name << " appeared " << endl;
			//	Create a new server instance
			ControlledServer	server;

			server.name = name;
			server.admin_name = "dserver/" + server.name;
			server.controlled = atoi((*pos++).c_str()) != 0;
			server.startup_level = (short)atoi((*pos++).c_str());
			server.set_state(Tango::FAULT);
			server.stopped       = false;
			server.auto_start    = false;
			server.started_time  = time(NULL);
			server.failure_time  = -1;

			//	Add a thread to ping server
			server.thread_data = new PingThreadData(server.name);
			server.thread =
				new PingThread(server.thread_data, server.name, proc_util);
			server.thread->start();

			servers->push_back(server);
			ms_sleep(50);	//	wait for server updated.
		}
		else
		{
			//	Update levels
			p_serv->controlled = atoi((*pos++).c_str()) != 0;
			p_serv->startup_level = (short) atoi((*pos++).c_str());
		}
	}
}

//+------------------------------------------------------------------
/**
 *	search a server in ControlledServer array by it's name .
 *
 *	@param servname	Server searched name.
 */
//+------------------------------------------------------------------
ControlledServer *StarterUtil::get_server_by_name(string &servname, vector<ControlledServer> &servers)
{
	string serverName(servname);
	transform(serverName.begin(), serverName.end(), serverName.begin(), ::tolower);
	for (unsigned int i=0 ; i<servers.size() ; i++)
	{
		ControlledServer	*server = &servers[i];
		string ctrlName(server->name);
		transform(ctrlName.begin(), ctrlName.end(), ctrlName.begin(), ::tolower);
		if (ctrlName == serverName)
			return server;
	}
	return NULL;
}
//+----------------------------------------------------------------------------
//+----------------------------------------------------------------------------
    void ControlledServer::set_state(Tango::DevState st) {
        if (st==Tango::MOVING && state!=Tango::MOVING) {
            //  Has just been switched to MOVING
            moving_time = time(NULL);
        }
        state = st;
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
	string	notify_procname("notifd");
	Tango::DevState	notifd_state;
	if (proc_util->is_process_running(notify_procname))
	{
		try
		{
			//cout << notify_procname << " is running" << endl;
			if (ch_factory==NULL)
				import_notifyd();

			CosNotifyChannelAdmin::ChannelIDSeq	*ch_id =
							ch_factory->get_all_channels();
			delete ch_id;

			cout2 << notifyd_name << "EventChannelFactory is ON" << endl;

			notifd_state = Tango::ON;
		}
		catch (...)
		{
			cerr << notify_procname << " is running  BUT not responding" << endl;
			//cerr << "Failed to narrow the EventChannelFactory on " << notifyd_name << endl;
			ch_factory = NULL;
 			notifd_state = Tango::UNKNOWN;
		}
	}
	else
	{
		//cout << notify_procname << " is NOT running" << endl;
 		notifd_state = Tango::FAULT;
	}
	return notifd_state;
}
//-----------------------------------------------------------------------------

}	//	namespace
