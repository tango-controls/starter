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
// copyleft :    European Synchrotron Radiation Facility
//               BP 220, Grenoble 38043
//               FRANCE
//
//=============================================================================
#ifndef _STARTER_UTIL_H
#define _STARTER_UTIL_H

#include <tango.h>

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
	bool	running;
	short	startup_level;
	Tango::DeviceProxy	*dev;
}
ServerControled;

//	Definitions for separators
//----------------------------------
#ifdef WIN32
#	define	slash	'\\'
#	define	TmpRoot	"\\temp"
#else
#	define	slash	'/'
#	define	TmpRoot	"/tmp"
#endif
#define	LogPath(s)		\
		s = TmpRoot;	\
		s += slash;		\
		s += "ds.log";

class StarterUtil
{
public :
	string	notifyd_name;

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
/**
 *	Build the error log file name from server name and domain.
 *
 *      @param  server   name of the server
 */
string build_log_file_name(char *);
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
 *	search a server in ServerControled array by it's name in an array.
 *
 *	@param servname	Server searched name.
 *	@param servers	Array of structure to search name.
 */
ServerControled *get_server_by_name(string, vector<ServerControled>&);
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
void get_server_info(ServerControled *);
/**
 *	Allocate and fill the servers controled object
 */
void build_server_ctrl_object(vector<ServerControled> *servers);
void build_server_ctrl_object_multi_call(vector<ServerControled> *servers);
/**
 *	check if Notify Daemon is alive.
 */
Tango::DevState is_notifyd_alive();
void import_notifyd();
#ifdef WIN32
/**
 *	This method does not exist in visual cpp
 *
 *    The  strcasecmp()  and  strncasecmp()  functions  are  case-
 *    insensitive  versions  of   strcmp()  and  strncmp() respec-
 *    tively, described below.  They assume  the  ASCII  character
 *    set  and ignore differences in case when comparing lower and
 *    upper case characters.
 */
int StarterUtil::strcasecmp(const char *s1, const char *s2);
#endif

void start_timer();
int	get_timer_value();

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
	/**
	 *	Shared data
	 */
	SharedData		*shared;
	/**
	 *	status of polling
	 */
	Tango::DevBoolean	polling_on;
	/**
	 *	Timeout without command to fall asleep loop.
	 */
	Tango::DevLong		cmd_timeout;
public:
/**
 *	Create a thread to start or stop state polling.
 *	Start will be done from astor.
 *	Stop will be done from astor or from a timeout without command.
 *
 *	@param	shared pointer on shared data between thread and DS.
 *	@param	timeout	timeout value in seconds.
 */
	PollingState(SharedData *shared, long timeout);

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
