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


#ifndef _PING_THREAD_H
#define _PING_THREAD_H

#include <tango.h>
#include <CheckProcessUtil.h>

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
    int nbInstances;

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
	time_t  get_last_write_time();
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

	int getNbInstaces();
	void setNbInstaces(int nb);
};
//=========================================================
/**
 *	Create a thread to ping server (admin device).
 */
//=========================================================
class PingThread: public omni_thread
{
private:
	/**
	 *	Shared data
	 */
	PingThreadData	*shared;
	CheckProcessUtil *process_util;
	/**
	 *	The pinged server name
	 */
	string servname;


public:
	/**
	 *	Create a thread to ping server
	 *
	 *	@param	shared pointer on shared data between thread and DS.
	 *	@param	name The pinged server name
	 *	@param	timeout	timeout value in milliseconds for ping command.
	 */
	PingThread(PingThreadData *sd, const string& name, CheckProcessUtil *proc_util);

	/**
	 *	Execute the thread loop.
	 */
	void *run_undetached(void *);
	void start() {start_undetached();}
};

}	//	namespace

#endif	// _PING_THREAD_H
