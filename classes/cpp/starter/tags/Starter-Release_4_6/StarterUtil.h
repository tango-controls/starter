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
// $Revision$
//
// $Log$
// Revision 3.11  2008/05/15 08:07:18  pascal_verdier
// TangoSys_MemStream replaced by TangoSys_OMemStream
// (for leaking problem under win32)
//
// Revision 3.10  2008/02/29 15:15:05  pascal_verdier
// Checking running processes by system call added.
//
// Revision 3.9  2007/05/03 06:54:59  pascal_verdier
// Re-try on DServer exported added before polling startup added.
//
// Revision 3.8  2006/04/24 07:06:28  pascal_verdier
// A thread is started for each level when at servers startup.
//
// Revision 3.7  2006/02/09 11:59:19  pascal_verdier
// A ping thread is now started for each server.
//
//
// copyleft :    European Synchrotron Radiation Facility
//               BP 220, Grenoble 38043
//               FRANCE
//
//=============================================================================
#ifndef _STARTER_UTIL_H
#define _STARTER_UTIL_H

#include <tango.h>
#include <PingThread.h>
#include <CheckProcessUtil.h>

/**
 * @author	$Author$
 * @version	$Revision$
 */

/**
 * Class Description:
 *	This class is a group of utility methods used by Starter Device Server.
 */

namespace Starter_ns
{

 //	Add your own constants definitions here.
 //-----------------------------------------------
#ifndef WIN32
typedef unsigned char boolean;
#endif

typedef struct {
	string	name;
	string	admin_name;
	bool	controled;
	short	startup_level;
	PingThreadData	*thread_data;
	PingThread		*thread;
	Tango::DeviceProxy	*dev;
	Tango::DevState		state;
}
ControledServer;

//	Millisecond sleep platform independant.
//--------------------------------------------
#	ifdef WIN32
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
#ifdef WIN32
#	define	slash	'\\'
#	define	TmpRoot	"c:\\temp"
#else
#	define	slash	'/'
#	define	TmpRoot	"/var/tmp"
#endif
#define	LogPath(s)		\
		s = TmpRoot;	\
		s += slash;		\
		s += "ds.log";

class Starter;

class StarterUtil
{
public :
	string				notifyd_name;
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
StarterUtil(Tango::DeviceProxy *dev, vector<string> host_name);
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
 *	Format the date and time in the argin value (Ux format) as string.
 */
char *strtime(time_t t);
/**
 *	search a server in ControledServer array by it's name in an array.
 *
 *	@param servname	Server searched name.
 *	@param servers	Array of structure to search name.
 */
ControledServer *get_server_by_name(string&, vector<ControledServer>&);
/**
 *	Get host device servers list from database.
 *
 *	@param dbase	Database device as a DeviceProxy for not implemented API commands.
 *	@param hostname	Host's name to get the device servers list.
 */
vector<string>	get_host_ds_list();
/**
 *	Read DS info from database to know if it is controled
 *		and it's starting level.
 *
 *	@param	devname	device to get info.
 *	@param	server	object to be updated from db read.
 */
void get_server_info(ControledServer *);
/**
 *	Allocate and fill the servers controled object
 */
void build_server_ctrl_object(vector<ControledServer> *servers);
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

#ifndef WIN32
	static struct timeval	before, after;
#else
#endif /* WIN32 */
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



//=========================================================
/**
 *	Create a thread to start or stop state polling.
 *	Start will be done from astor.
 *	Stop will be done from astor or from a timeout without command.
 */
//=========================================================
class PollingState: public omni_thread
{
private:
	string	device_name;

	/**
	 *	Add the object to polling list.
 	 */
	void add_obj_polling(Tango::DServer *adm_dev, string type, string name, int period);

public:
/**
 *	Create a thread to start  polling.
 */
	PollingState(string);

/**
 *	Execute the thread loop.
 *	This thread is awaken when a command has been received 
 *	and falled asleep when no command has been received from a long time.
 */
	//virtual void run();

	void *run_undetached(void *);
	void start() {start_undetached();}
};

}	//	namespace

#endif	// _STARTER_UTIL_H
