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
//
// copyleft :    European Synchrotron Radiation Facility
//               BP 220, Grenoble 38043
//               FRANCE
//
//=============================================================================
#ifndef _PING_THREAD_H
#define _PING_THREAD_H

#include <tango.h>

/**
 * @author	$Author$
 * @version	$Revision$
 */

/**
 * Class Description:
 *	This class is a thread to ping server.
 */

namespace Starter_ns
{

//=========================================================
/**
 *	Shared data between DS and ping thread.
 */
//=========================================================
class PingThreadData: public Tango::TangoMonitor
{
private:
	string			servname;
	bool			stop_thread;
	time_t			last_write_time;
	Tango::DevState	state;

public:
	PingThreadData(string name);
	/**
	 *	Get the server name
	 */
	string get_server_name();
	/**
	 *	command to stop thread
	 */
	void set_stop_thread();
	/**
	 *	get  stop thread status
	 */
	bool get_stop_thread();

	/**
	 *	Get the last command time
	 */
	time_t get_last_write_time();
	/**
	 *	Set the ping result
	 */
	void set_state(Tango::DevState st);
	/**
	 *	get the ping result
	 */
	Tango::DevState get_state();
	/**
	 *	Force thread to update data.
	 */
	void wake_up();
};
//=========================================================
/**
 *	Create a thread to ping server.
 */
//=========================================================
class PingThread: public omni_thread
{
private:
	/**
	 *	Shared data
	 */
	PingThreadData		*shared;
	/**
	 *	The pinged server name
	 */
	string	servname;
	
	long	ping_timeout;

public:
	/**
	 *	Create a thread to ping server
	 *
	 *	@param	shared pointer on shared data between thread and DS.
	 *	@param	name The pinged server name
	 *	@param	timeout	timeout value in milliseconds for ping command.
	 */
		PingThread(PingThreadData *shared, string name);

	/**
	 *	Execute the thread loop.
	 */
		void *run_undetached(void *);
		void start() {start_undetached();}
};

}	//	namespace

#endif	// _PING_THREAD_H
