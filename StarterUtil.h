//=============================================================================
//
// file :        StarterUtil.h
//
// description : Include for the StarterUtil class.
//
// project :	Starter for Tango Administration
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
//=============================================================================

#ifndef _STARTER_UTIL_H
#define _STARTER_UTIL_H

#include <tango.h>
#include <COS/CosNotifyChannelAdmin.hh>
#include <PingThread.h>
#include <CheckProcessUtil.h>

/**
 * $Author$
 * $Revision$
 */

/**
 * Class Description:
 *	This class is a group of utility methods used by Starter Device Server.
 */

namespace Starter_ns
{

 //	Add your own constants definitions here.
 //-----------------------------------------------
#ifndef _TG_WINDOWS_
typedef unsigned char boolean;
#endif

class ControlledServer {
private:
    Tango::DevState		state;
    time_t moving_time; // When moving has been detected
public:
    string name;
    string admin_name;
    bool   controlled;
    short  startup_level;
    PingThreadData *thread_data;
    PingThread *thread;
    bool   stopped;
    bool   auto_start;
    int    nbInstances;
    time_t started_time;
    time_t failure_time;
    void set_state(Tango::DevState st);
    Tango::DevState get_state() { return state; };
    time_t get_moving_duration() { return time(NULL)-moving_time; }
};

//	Millisecond sleep platform independent.
//--------------------------------------------
#	ifdef _TG_WINDOWS_
#	define		ms_sleep(ms)	_sleep(ms);
#	else
#	define		ms_sleep(ms)	{\
		struct timespec ts;     \
		ts.tv_sec = ms / 1000;  \
		ts.tv_nsec = (ms - (ts.tv_sec * 1000)) * 1000000; \
		nanosleep(&ts,NULL); \
	}
#	endif

//	Definitions for separators
//----------------------------------
#ifdef _TG_WINDOWS_
#	define	slash	'\\'
#	define	TmpRoot	"c:\\temp"
#else
#	define	slash	'/'
#	define	TmpRoot	"/var/tmp"
#endif
#define	LogPath(s,home)		\
		s  = home;	\
		s += slash;		\
		s += "ds.log";

#define STARTER_LOG_DEPTH	400
#define STARTER_STAT_DEPTH	2000


class Starter;

class StarterUtil
{
public :
	string				notifyd_name;
	string				log_home;
	string				starter_log_file;	//	History log file
	string				starter_stat_file;	//	Statistics file
	CheckProcessUtil	*proc_util;

/**
 *	the event channel factory used to test if notifd is alive.
 */
CosNotifyChannelAdmin::EventChannelFactory_var	ch_factory;
/**
 *	@Constructor methods
 *	Constructor methods
 */
//@{
/**
 *	Default constructor.
 */
StarterUtil(Tango::DeviceProxy *dev, vector<string> host_name, string logHome);
//@}
/**
 *	@name methods
 *	Tools methods
 */
//@{
/**
 *	Extract server name from input parameter (servname/instance).
 */
string removeFQDN(string s);
/**
 *	Extract server name from input parameter (servname/instance).
 *
 *      @param  argin     servname/instance
 */
char *get_server_name(char *argin);
/**
 *	Extract instance name from input parameter (servname/instance).
 *
 *      @param  argin     servname/instance
 */
char *get_instance_name(char *argin);
/**
 *	Check if executable file exists
 *	and return its full name with good path.
 *
 *	@param	servname	Server name
 *	@param	fullpath	Path from property
 */
char *check_exe_file(char *servname, vector<string> v_path);
char *check_exe_file(string filename);
/**
 *	Build the error log file name from server name and domain.
 *
 *      @param  server   name of the server
 */
string build_log_file_name(char *);
/**
 *	Rename log file list
 *	@param	filename	log file name
 */
vector<string> get_log_file_list(string filename);
/**
 *	returns log file
 *	@param	filename	file's name to get the date and rename.
 */
void manage_log_file_history(char *filename, int nb_max);
/**
 *	Get the last modification on a file and return it in a string.
 *	@param	filename	file's name to get the date.
 */
char *get_file_date(char *filename);
/**
 *	Log info for starter.
 */
void log_starter_info(string message);
/**
 *	Log statistics for specified server obsevrved by starter..
 */
void log_starter_statistics(ControlledServer *);
/**
 *	Resetstatistics for all servers.
 */
void reset_starter_stat_file(vector<ControlledServer> *servers);
/**
 *	Format the date and time in the argin value (Ux format) as string.
 */
char *strtime(time_t t);
/**
 *	search a server in ControlledServer array by it's name in an array.
 *
 *	@param servname	Server searched name.
 *	@param servers	Array of structure to search name.
 */
ControlledServer *get_server_by_name(string&, vector<ControlledServer>&);
/**
 *	Get host device servers list from database.
 *
 *	@param dbase	Database device as a DeviceProxy for not implemented API commands.
 *	@param hostname	Host's name to get the device servers list.
 */
vector<string>	get_host_ds_list();
/**
 *	Read DS info from database to know if it is controlled
 *		and it's starting level.
 *
 *	@param	devname	device to get info.
 *	@param	server	object to be updated from db read.
 */
void get_server_info(ControlledServer *);
/**
 *	Allocate and fill the servers controlled object
 */
void build_server_ctrl_object(vector<ControlledServer> *servers);
/**
 *	check if Notify Daemon is alive.
 */
Tango::DevState is_notifyd_alive();
void import_notifyd();

//@}
private:
	static int		elapsed;
	vector<string>	hostnames;
	/**
	 *	Database device (as DeviceProxy) for not implemented API commands.
	 */
	Tango::DeviceProxy	*dbase;

#ifndef _TG_WINDOWS_
	static struct timeval	before, after;
#else
#endif /* _TG_WINDOWS_ */
};





//=========================================================
/**
 *	Shared data between DS and thread.
 */
//=========================================================
class SharedData: public Tango::TangoMonitor
{
private:
	time_t	last_cmd_time;
	/**
	 *	Polling thread ID
	 */
	omni_thread			*polling_id;

public:
	SharedData();
/**
 *	Get the last command time
 */
time_t get_timer();
/**
 *	Set the last command time
 */
void set_timer();
};

}	//	namespace

#endif	// _STARTER_UTIL_H
