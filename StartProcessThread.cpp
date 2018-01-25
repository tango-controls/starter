//+=============================================================================
//
// file :         StartProcessThread.cpp
//
// description :  C++ source for the Starter start process thread.
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
//+=============================================================================


#include <tango.h>


#ifdef _TG_WINDOWS_
#	include <process.h>
#	include <direct.h>
#	include <io.h>
#else
#	include <sys/wait.h>
#endif

#include <fcntl.h>

#include <Starter.h>

namespace Starter_ns
{
//+------------------------------------------------------------------
/**
 * Thread constructor
 */
//+------------------------------------------------------------------
StartProcessThread::StartProcessThread(vector<NewProcess *> v_np, int level, Starter *st)
{
	for (unsigned int i=0 ; i<v_np.size() ; i++)
		processes.push_back(v_np[i]);
	thread_level = level;
	starter      = st;
}
//+------------------------------------------------------------------
/**
 * Execute the fork of the sub process in a thread.
 */
//+------------------------------------------------------------------
void StartProcessThread::run(TANGO_UNUSED(void *ptr))
{
	unsigned int	i;
	CheckProcessUtil	*process_util = starter->util->proc_util;
	//	Check if a previous level thread is terminated
	if (thread_level>1)
	{
		while (starter->start_proc_data->get_current_level()!=thread_level &&
               starter->start_proc_data->level_is_still_active(thread_level) )
		{
			//cout << "	level " << thread_level <<
			//	"	wait for end for level " << starter->start_proc_data->get_current_level() << endl;
			ms_sleep(500);
		}
	}

	//	Start the level process start loop
	for (i=0 ; i<processes.size() &&
                starter->start_proc_data->level_is_still_active(thread_level) ; i++)
	{
		//	Build server name from admin name and check if stopped before start it
		char *p = processes[i]->adminName + strlen("dserver/");
		string servname(p);
		if (process_util->is_server_running(servname)==false) {
			start_process(processes[i]);


			//	Sleep a bit between start ups
			//	to do not overload database
			bool started = false;
			time_t t0 = time(NULL);
			time_t t1 = t0;
			Tango::DeviceProxy *serv;
			bool failed = false;
			while (failed == false && started == false &&
					(t1 - t0) < starter->serverStartupTimeout) {
#		ifdef _TG_WINDOWS_
				_sleep(1000);
#		else
				sleep(1);
#		endif


				//	do not test before 4 seconds to be sure
				//	process list has been updated.
				t1 = time(NULL);
				if ((t1 - t0) >= 4) {
					//	Check if server running or failed
					if (process_util->is_server_running(servname) == false)
						failed = true;
					else {
						//	if  not failed check if start up is terminated
						try {
							serv = new Tango::DeviceProxy(processes[i]->adminName);
							serv->ping();
							started = true;
						}
						catch (Tango::DevFailed &) {
							//cout << e.errors[0].desc << endl;
						}
						delete serv;
						t1 = time(NULL);
					}
				}
			}
			if (started) {
				TangoSys_OMemStream out_stream;
				out_stream << servname << " started";
				starter->util->log_starter_info(out_stream.str());

				//cout << "----- " << serverName << " started in " << (t1-t0) << " sec." << endl;
			} else if (failed)
				cout << "----- " << servname << " failed in " << (t1 - t0) << " sec." << endl;
			else
				cout << "----- " << servname << " Timeout = " << (t1 - t0) << endl;

			//	Wait a bit betwee 2 startup (not for last one)
			if (thread_level > 0 && i < processes.size() - 1) {
				ms_sleep(TIME_BETWEEN_STARTUPS);
			}
		}
    }

	//	Check if a time to wait between two start up levels in seconds as been set
	//	Except for level 0 which is a single start up
	if (thread_level>0 && starter->interStartupLevelWait>0 &&
        starter->start_proc_data->level_is_still_active(thread_level))
	{
		//cout << "Thread level " << thread_level <<
		//	" wait for " << starter->interStartupLevelWait << endl;
		ms_sleep(starter->interStartupLevelWait*1000);
	}

	//	Free the process structure field
	for (i=0 ; i<processes.size() ; i++)
	{
		delete[] processes[i]->serverName;
		delete[] processes[i]->instanceName;
		delete[] processes[i]->adminName;
		delete[] processes[i]->logFileName;
		delete processes[i];
	}

	//	remove in level vector to start another level
	starter->start_proc_data->remove_level(thread_level);
}
#ifndef	_TG_WINDOWS_
//+------------------------------------------------------------------
/**
 *	Start one process
 */
//+------------------------------------------------------------------
void StartProcessThread::start_process(NewProcess *process)
{
	char	*argv[4];
	int		i = 0;
	argv[i++] = process->serverName;
	argv[i++] = process->instanceName;
	//argv[i++] = "-v5";
	argv[i] = NULL;

	//	Fork a child process to start the device server
	int	fork_id = fork();
	switch(fork_id)
	{
	case -1:
		cerr << "Fork Failed !" << endl;
		break;
	case 0:
	    //  Double fork to prevent defunct child processes
		switch(fork())
		{
		case -1:
			cerr << "Fork Failed !" << endl;
			break;

		case 0:
				// Change process group and close control tty
#if defined(__darwin__) || defined( __MACOS__)

				setpgrp();

#elif defined (__freebsd__)

				setpgrp(0,setsid());

#else
				setpgrp();

				//	Call setsid() to do NOT stop children if Starter is killed.
				setsid();
#endif
				// Close standard out
				close(1);
                open("/dev/null", O_RDWR | O_CREAT, 0664);

				//	Close the stderr and re-open it on a log file.
                close(2);
                starter->util->manage_log_file_history(
                        process->logFileName, starter->keepLogFiles);
                open(process->logFileName, O_RDWR | O_CREAT, 0664);

                //	Start the execution of the device server
                if (execvp(argv[0], argv)<0)
                {
                    ofstream	of(process->logFileName);
                    of << "Exec(" << argv[0] << ") failed " << endl;
                    of << strerror(errno) << endl;
                    of.close();
                }
				_exit(0);

		default:
			//cout << fork_id << " exit()" << endl;
			_exit(0);
		}
		break;

	default:
		int res_wait;
		wait(&res_wait);
		break;
	}
}


#else	//	_TG_WINDOWS_


//+----------------------------------------------------------
//	WIN 32 methods to fork a sub process
//+----------------------------------------------------------
void StartProcessThread::start_process(NewProcess *process)
{
	/*****
	On Win32 problem with permission denied !
	It seems that this not necessary because there is nothing in file.

	starter->util->manage_log_file_history(
			process->logfile, starter->keepLogFiles);
	*/
	StartWinThread	*win_thread = new StartWinThread(process, starter);
	win_thread->start();
}

//+------------------------------------------------------------------
/**
 *	Set the path between cotes for windows.
 *
 *      @param  serverName	server name
 */
//+------------------------------------------------------------------
string StartWinThread::get_server_name_with_cotes(string serverName)
{
	string::size_type	pos = serverName.find_last_of("/\\");
	if (pos!=string::npos)
	{
		string	str("\"");
		str += serverName.substr(0, pos);
		str += "\"";
		str += serverName.substr(pos);

		return str;
	}
	else
		return serverName;
}
//+----------------------------------------------------------
//	WIN 32 Thread to fork a sub process
//		If batch file, the spawnv is blocking !!!
//+----------------------------------------------------------
void StartWinThread::run(void *ptr)
{
	//	Check if batch file
	string str_server(process->serverName);
	string str_with_cotes = get_server_name_with_cotes(process->serverName);
	string cmd(str_with_cotes);
	cmd += "  ";
	cmd += process->instanceName;
	cout << "system(" << cmd << ");" << endl;
	system(cmd.c_str());
	return;
}

#endif	//	_TG_WINDOWS_

//+------------------------------------------------------------------
//+------------------------------------------------------------------
int StartProcessShared::get_starting_processes()
{
	omni_mutex_lock sync(*this);
	return starting_processes;
}
//+------------------------------------------------------------------
/**
 *	StartProcessShared::push_back_level(int level)
 */
//+------------------------------------------------------------------
void StartProcessShared::push_back_level(int level)
{
	omni_mutex_lock sync(*this);
	start_process_thread_levels.push_back(level);
	starting_processes++;
}
//+------------------------------------------------------------------
/**
 *	StartProcessShared::get_current_level()
 */
//+------------------------------------------------------------------
int StartProcessShared::get_current_level()
{
	omni_mutex_lock sync(*this);
	return start_process_thread_levels[0];
}
//+------------------------------------------------------------------
/**
 *	StartProcessShared::level_is_still_active(int level)
 */
//+------------------------------------------------------------------
bool StartProcessShared::level_is_still_active(int level)
{
	omni_mutex_lock sync(*this);
	vector<int>::iterator it = start_process_thread_levels.begin();
	for (  ; it<start_process_thread_levels.end() ; it++)
    	if (*it==level) {
        	//cout << "Level " << level << " is still active" << endl;
        	return true;
    	}
        //cout << "Level " << level << " is NOT still active" << endl;
   return false;
}
//+------------------------------------------------------------------
/**
 *	StartProcessShared::remove_level(int level)
 */
//+------------------------------------------------------------------
void StartProcessShared::remove_level(int level)
{
	omni_mutex_lock sync(*this);
	vector<int>::iterator it = start_process_thread_levels.begin();
	for (  ; it<start_process_thread_levels.end() ; it++) {
        if (*it==level) {
            //cout << "StartProcessShared::remove_level " << level << endl;
            start_process_thread_levels.erase(it);
            starting_processes--;
            return;
        }
	}
}

};	//	namespace
