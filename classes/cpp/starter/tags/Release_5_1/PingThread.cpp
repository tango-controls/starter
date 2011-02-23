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
// Copyright (C) :      2004,2005,2006,2007,2008,2009,2010
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
//
// $Log$
// Revision 1.10  2010/10/08 08:48:50  pascal_verdier
// Include files order changed.
//
// Revision 1.9  2010/09/21 12:18:58  pascal_verdier
// GPL Licence added to header.
//
// Revision 1.8  2010/02/09 15:09:49  pascal_verdier
// Define  _TG_WINDOWS_  replace WIN32.
// LogFileHome property added.
//
// Revision 1.7  2008/12/16 10:44:57  pascal_verdier
// Memory leak fixed.
//
// Revision 1.6  2008/02/29 15:15:05  pascal_verdier
// Checking running processes by system call added.
//
// Revision 1.5  2007/09/25 12:12:00  pascal_verdier
// little memory leaks fixed.
//
// Revision 1.4  2006/11/10 14:53:28  pascal_verdier
// Remove vc8 warnings.
//
// Revision 1.3  2006/04/24 07:06:27  pascal_verdier
// A thread is started for each level when at servers startup.
//
// Revision 1.2  2006/02/10 10:14:24  pascal_verdier
// Recreate DeviceProxy object in case of Exception on ping() (bug in cpp api).
//
// Revision 1.1  2006/02/09 12:00:15  pascal_verdier
// A ping thread is now started for each server.
//
//-=============================================================================

#include <tango.h>

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#ifndef _TG_WINDOWS_
#	include <sys/time.h>
#endif


#include <sstream>
#include <PingThread.h>

#ifndef	TIME_VAR
#ifndef _TG_WINDOWS_

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

#endif	/*	_TG_WINDOWS_		*/
#endif	/*	TIME_VAR	*/

namespace Starter_ns
{
//+----------------------------------------------------------------------------
/**
 *	Constructor
 */
//+----------------------------------------------------------------------------
PingThreadData::PingThreadData(string sname)
{
	servname        = sname;
	stop_thread     = false;
	last_write_time = time(NULL);
	state           = Tango::ON;
}
//+----------------------------------------------------------------------------
/**
 *	Get the server name
 */
//+----------------------------------------------------------------------------
string PingThreadData::get_server_name()
{
	omni_mutex_lock sync(*this);
	return servname;
}
//+----------------------------------------------------------------------------
/**
 *	command to stop thread
 */
//+----------------------------------------------------------------------------
void PingThreadData::set_stop_thread()
{
	omni_mutex_lock sync(*this);
	stop_thread = true;
	signal();
}
//+----------------------------------------------------------------------------
/**
 *	get  stop thread status
 */
//+----------------------------------------------------------------------------
bool PingThreadData::get_stop_thread()
{
	omni_mutex_lock sync(*this);
	return stop_thread;
}

//+----------------------------------------------------------------------------
/**
 *	Return the elapsed time from last write.
 */
//+----------------------------------------------------------------------------
time_t PingThreadData::get_last_write_time()
{
	omni_mutex_lock sync(*this);
	return last_write_time;
}
//+----------------------------------------------------------------------------
/**
 *	Set the ping result.
 */
//+----------------------------------------------------------------------------
void PingThreadData::set_state(Tango::DevState st)
{
	omni_mutex_lock sync(*this);
	state = st;
	last_write_time = time(NULL);
}
//+----------------------------------------------------------------------------
/**
 *	Set the ping result.
 */
//+----------------------------------------------------------------------------
Tango::DevState PingThreadData::get_state()
{
	omni_mutex_lock sync(*this);
	return state;	
}
//+----------------------------------------------------------------------------
/**
 *	Force thread to update data.
 */
//+----------------------------------------------------------------------------
void PingThreadData::wake_up()
{
	omni_mutex_lock sync(*this);
	signal();
}
//+----------------------------------------------------------------------------
//+----------------------------------------------------------------------------






//+----------------------------------------------------------------------------
/**
 *	Execute the thread loop.
 */
//+----------------------------------------------------------------------------
void *PingThread::run_undetached(void *ptr)
{
	bool	trace = false;
	if (trace)	cout << "Start a thread to ping " << servname << endl;
	TimeVal	before, after;
	Tango::DeviceProxy	*dev  = NULL;
	Tango::DevState		state = Tango::FAULT;
	bool				stop_thread = false;
	string				adm_devname("dserver/");
	adm_devname += servname;
	while(!stop_thread)
	{
		GetTime(before);
		//	Check before if server running or failed
		if (process_util->is_server_running(servname))
		{
			//	try to build DeviceProxy
			if (dev==NULL)
			{
				try
				{
					if (dev==NULL)
						dev = new Tango::DeviceProxy(adm_devname);
				}
				catch(Tango::DevFailed &e)
				{
					Tango::Except::print_exception(e);
				}
				catch(...)
				{
					cout << "============================================" << endl;
					cout << "	Exception catched !!!!!!" << endl;
					cout << "============================================" << endl;
				}
			}
			if (dev!=NULL)
			{
				try
				{
					dev->ping();
					state = Tango::ON;
				}
				catch(Tango::DevFailed &)
				{
					cout << servname << " is running but not responding !!!" << endl;
					//Tango::Except::print_exception(e);
					state = Tango::MOVING;
				}
			}
		}
		else
			state = Tango::FAULT;
		shared->set_state(state);

		if (trace) cout << "Ping thread:[" << servname << "]	" <<
				Tango::DevStateName[state]    << endl;
		
		//	Compute time to sleep
		GetTime(after);
		double	dt = (double)Elapsed(before, after);
		long	time_to_sleep = 2000 - (int)dt;
		if (time_to_sleep<10)
			time_to_sleep = 10;

		//	Check if thread must be stopped.
		stop_thread = shared->get_stop_thread();
		if (!stop_thread)
		{
			//	And wait for next ping
			omni_mutex_lock sync(*shared);
			shared->wait(time_to_sleep);
		}
		stop_thread = shared->get_stop_thread();
	}



	delete shared;
	if (dev!=NULL)
		delete dev;
	if (trace)	cout << "Ping thread:[" << servname << "] - leaving...." << endl;
	return NULL;
}
//+----------------------------------------------------------------------------


}	//	namespace
