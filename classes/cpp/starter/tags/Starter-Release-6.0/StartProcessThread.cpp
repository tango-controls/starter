static const char *RcsId = "$Header$";
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
// Copyright (C) :      2004,2005,2006,2007,2008,2009,2010,2011,2012,2013,2014
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
#	include <sys/time.h>
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
		while (starter->start_proc_data->get_current_level()!=thread_level)
		{
			//cout << "	level " << thread_level <<
			//	"	wait for end for level " << starter->start_proc_data->get_current_level() << endl;
			ms_sleep(500);
		}
	}
	
	//	Start the level process start loop
	for (i=0 ; i<processes.size() ; i++)
	{
		start_process(processes[i]);

		//	Build server name from admin name
		char	*p = processes[i]->adminname + strlen("dserver/");
		string	servname(p);
		bool	failed = false;

		//	Sleep a bit between startups			 		
		//	to do not overload database
		bool	started = false;
		time_t	t0 = time(NULL);
		time_t	t1 = t0;
		Tango::DeviceProxy	*serv;
		while (failed==false && started==false && (t1-t0)<starter->serverStartupTimeout)
		{
#		ifdef _TG_WINDOWS_
			_sleep(1000);
#		else
			sleep(1);
#		endif


			//	do not test before 4 seconds to be sure
			//	process list has been updated.
			t1 = time(NULL);
			if ((t1-t0)>=4)
			{
				//	Check if server running or failed
				if (process_util->is_server_running(servname)==false)
					failed = true;
				else
				{
					//	if  not failed check if startup is terminated
					serv = NULL;
					try
					{
						serv = new Tango::DeviceProxy(processes[i]->adminname);
						serv->ping();
						started = true;
					}
					catch(Tango::DevFailed &)
					{
						//cout << e.errors[0].desc << endl;
					}
					if (serv!=NULL)
						delete serv;
					t1 = time(NULL);
				}
			}
		}
		if (started)
		{
			TangoSys_OMemStream out_stream;
			out_stream << servname << " started";
			starter->util->log_starter_info(out_stream.str());

			cout << "----- " << servname << " started in " << (t1-t0) << " sec." << endl;
		}
		else
		if (failed)
			cout << "----- " << servname << " failed in " << (t1-t0) << " sec." << endl;
		else
			cout << "----- " << servname << " Timeout = " << (t1-t0) << endl;

		ms_sleep(TIME_BETWEEN_STARTUPS);
	}

	//	Check if a time to wait between two startup levels in seconds as been set
	//	Except for level 0 which is a single startup
	if (thread_level>0 && starter->interStartupLevelWait>0)
	{
		//cout << "Thread level " << thread_level <<
		//	" wait for " << starter->interStartupLevelWait;
		ms_sleep(starter->interStartupLevelWait*1000);
	}

	//	Free the process structure field
	for (i=0 ; i<processes.size() ; i++)
	{
		free(processes[i]->servname);
		free(processes[i]->instancename);
		delete [] processes[i]->adminname;
		delete [] processes[i]->logfile;
	}

//	starter->starting_processes--;
	//	remove in level vector to start another level
	starter->start_proc_data->remove_current_level();
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
	argv[i++] = process->servname;
	argv[i++] = process->instancename;
	//argv[i++] = "-v5";
	argv[i++] = NULL;

	//	Fork a child process to start the device server
	int	fork_id = fork();
	switch(fork_id)
	{
	case -1:
		cerr << "Fork Failed !" << endl;
		break;
	case 0:
		switch(fork())
		{
		case -1:
			cerr << "Fork Failed !" << endl;
			break;
		case 0:
			{
				// Change process group and close control tty
#if defined(__darwin__) || defined( __MACOS__)

				setpgrp();

#elif defined (__freebsd__)

				setpgrp(0,setsid());

#else										
				setpgrp();

				//	Call setsid() to do NOT stop children if Starter is killed.
				//--------------------------------------------------------------
				setsid();
#endif

				//	Close the stderr and re-open it on a log file.
				//-------------------------------------------------
				close(2);
				starter->util->manage_log_file_history(
						process->logfile, starter->keepLogFiles);
				//remove(process->logfile);
				open(process->logfile, O_RDWR | O_CREAT, 0664);

				//	Start the execution of the device server
				//---------------------------------------------
				if (execvp(argv[0], argv)<0)
				{
					ofstream	of(process->logfile);
					of << "Exec(" << argv[0] << ") failed " << endl;
					of << strerror(errno) << endl;
					of.close();
				}
				_exit(0);
			}
			break;
		default:
			cout << fork_id << " exit()" << endl;
			_exit(0);
			break;
		}
		break;

	default:
		int		res_wait;
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
 *      @param  servname	server name
 */
//+------------------------------------------------------------------
string StartWinThread::get_server_name_with_cotes(string servname)
{
	string::size_type	pos = servname.find_last_of("/\\");
	if (pos!=string::npos)
	{
		string	str("\"");
		str += servname.substr(0, pos);
		str += "\"";
		str += servname.substr(pos);
		
		return str;
		
		/*
		string	str("\"");
		str += servname;
		str += "\"";

		string	str("c:\\\"Program Files\"\\Tango\\notify_daemon.bat");
		return str;
		*/
	}
	else
		return servname;
}
//+----------------------------------------------------------
//	WIN 32 Thread to fork a sub process
//		If batch file, the spawnv is blocking !!!
//+----------------------------------------------------------
void StartWinThread::run(void *ptr)
{
	//	Check if batch file
	string	str_server(process->servname);
	string	str_with_cotes = get_server_name_with_cotes(process->servname);
	char	*servname = new char[str_with_cotes.length()+1];
	strcpy(servname, str_with_cotes.c_str());
	
	string cmd(servname);
	cmd += "  ";
	cmd += process->instancename;
	cout << "system(" << cmd << ");" << endl;
	system(cmd.c_str());
	return;

//	Not used any more
/*************************
	if (str_server.find(".bat") != string::npos)
	{
		char	*argv[3];
		int		i = 0;

		argv[i++] = servname;
		argv[i++] = process->instancename;
		argv[i++] = NULL;

		cout << "Forking "   << servname << endl;
		cout << "log file: " << process->logfile << endl;

		//	Write a starting message in log file
		ofstream	ofs(process->logfile);
		ofs << "Starting " << argv[0] << "....." << endl << endl;
		ofs.close();

		//	Close the stderr and re-open it on a log file.
		//-------------------------------------------------
		int	ret;
		if ((ret=_spawnv(_P_WAIT, argv[0], argv))<0)
		{
			//	Write error in log file
			//-------------------------------
			ofstream	ofs1(process->logfile);
			ofs1 << "Exec(" << argv[0] << ") failed " << endl;
			ofs1 << "_spawnv has returned " << ret << endl;
			DWORD	errcode = GetLastError();
		    LPTSTR	lp_err;
			if (::FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER |
								FORMAT_MESSAGE_FROM_SYSTEM     |
								FORMAT_MESSAGE_ARGUMENT_ARRAY,
							NULL,
							errcode,
							LANG_NEUTRAL,
							(LPTSTR)&lp_err,
							0,
							NULL))
			{
				char	str[0x80];
				char	*p = str;
				for (int i=0 ; lp_err[i]!=0 ; i++)
					*p++ = lp_err[i];
				*p = '\0';
				ofs1 << str << " errcode = " << errcode << endl;
				cout << "===============================================" << endl
					 << servname << "  " << process->instancename<< endl
					 << str << " errcode = " << errcode                   << endl
					 << "===============================================" << endl;
				::LocalFree(lp_err);
			}
			ofs1.close();
		}
		else
		{
			Sleep(5000);
			cout << "Fork Succeed." << endl;
		}
	}
	else
	{
		PROCESS_INFORMATION pi;
		STARTUPINFO si;

		string	cmd_line(servname);
		cmd_line += " ";
		cmd_line += process->instancename;

		//
		// Get handles for standard output and errors
		//
		SECURITY_ATTRIBUTES sa;
		sa.bInheritHandle = 1;
		sa.nLength = sizeof(sa);
		sa.lpSecurityDescriptor = NULL;

		int buffSize = (int)strlen(process->logfile) + 1;
		LPWSTR lpw_logfile = new wchar_t[buffSize];
		MultiByteToWideChar(CP_ACP, 0, process->logfile, buffSize, lpw_logfile, buffSize);

		HANDLE hStdError  = CreateFile( lpw_logfile, 
			GENERIC_WRITE,
			FILE_SHARE_WRITE | FILE_SHARE_READ, 
			&sa, //lpSecurityAttributes 
			CREATE_ALWAYS, 
			FILE_ATTRIBUTE_NORMAL,
			0);
		if (hStdError == INVALID_HANDLE_VALUE)
			cout << "ERROR INVALID_HANDLE_VALUE " << strerror(GetLastError()) << endl;

		//	Prepare Startup info
		memset( &si, 0, sizeof(si) );
		si.cb = sizeof(si);
		si.dwFlags     = STARTF_USESHOWWINDOW;
		si.wShowWindow = SW_HIDE;

		buffSize = (int)strlen(cmd_line.c_str()) + 1;
		LPWSTR lpw_cmd = new wchar_t[buffSize];
		MultiByteToWideChar(CP_ACP, 0, cmd_line.c_str(), buffSize, lpw_cmd, buffSize);

		ofstream	ofs(process->logfile);
		ofs << "Creating process " << cmd_line << endl;
		if(CreateProcess(
				NULL ,            // pointer to name of executable module
				lpw_cmd,          // pointer to command line string
				&sa,              // process security attributes
				&sa,              // thread security attributes
				TRUE,             // handle inheritance flag
				DETACHED_PROCESS, // creation flags
				NULL,             // pointer to new environment block
				NULL,             // pointer to current directory name
				&si,              // pointer to STARTUPINFO
				&pi               // pointer to PROCESS_INFORMATION
				)==0)
		{
			_sleep(2000);
			cerr << "Error when trying to create process " << cmd_line << endl;

			//	Write a starting message in log file
			ofstream	ofs1(process->logfile);
			ofs << "Error when trying to create process " << cmd_line <<endl;
			DWORD	errcode = GetLastError();
		    LPTSTR lp_err;
			if (::FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ARGUMENT_ARRAY,
							NULL,
							errcode,
							LANG_NEUTRAL,
							(LPTSTR)&lp_err,
							0,
							NULL))
			{
				char	str[0x80];
				char	*p = str;
				for (int i=0 ; lp_err[i]!=0 ; i++)
					*p++ = lp_err[i];
				*p = '\0';
				ofs << str << " errcode = " << errcode << endl;
				::LocalFree(lp_err);
			}
		}
		else
		{
			ofs << ".......OK (" << cmd_line << ")" << endl;
		}
		delete lpw_cmd;
		delete lpw_logfile;
		ofs.close();
	}
	delete[] servname;
***********************/
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
 *	StartProcessShared::remove_current_level()
 */
//+------------------------------------------------------------------
void StartProcessShared::remove_current_level()
{
	omni_mutex_lock sync(*this);
	vector<int>::iterator it = start_process_thread_levels.begin();
	start_process_thread_levels.erase(it);
	starting_processes--;
}

};	//	namespace
