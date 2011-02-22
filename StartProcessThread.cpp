static const char *RcsId = "$Header$";
//+=============================================================================
//
// file :         Starter.cpp
//
// description :  C++ source for the Starter start process thread.
//
// project :      TANGO Device Server
//
// $Author$
//
// $Revision$
//
// $Log$
// Revision 3.8  2007/02/01 09:17:44  pascal_verdier
// Monitor added on some shared data.
// Wait 3 seconds added at startup for first ping timeout.
//
// Revision 3.7  2006/11/20 06:58:37  pascal_verdier
// Mutex on start process data added.
//
// Revision 3.6  2006/11/10 14:53:28  pascal_verdier
// Remove vc8 warnings.
//
// Revision 3.5  2006/06/13 19:38:15  pascal_verdier
// Minor changes.
//
// Revision 3.4  2006/06/06 12:01:26  pascal_verdier
// Bug in log file fixed.
//
// Revision 3.3  2006/06/05 07:20:36  pascal_verdier
// Server startup is now delayed (with timeout) in a startup level.
// New state MOVING added.
// At startup, starter loop until properties have been read.
//
// Revision 3.2  2006/04/24 08:58:10  pascal_verdier
// *** empty log message ***
//
// Revision 3.1  2006/04/24 07:06:27  pascal_verdier
// A thread is started for each level when at servers startup.
//
//
// copyleft :     European Synchrotron Radiation Facility
//                BP 220, Grenoble 38043
//                FRANCE
//
#ifdef WIN32
#	include <process.h>
#	include <direct.h>
#	include <io.h>
#else
#	include <sys/wait.h>
#	include <sys/time.h>
#endif

#include <fcntl.h>

#include <tango.h>
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
void *StartProcessThread::run_undetached(void *ptr)
{
	unsigned int	i;
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
		//cout << "Starting " <<
		//	processes[i]->servname << "/" << processes[i]->instancename << endl;

		start_process(processes[i]);

		//	Sleep a bit between startups			 		
		//	to do not overload database
		bool	started = false;
		time_t	t0 = time(NULL);
		time_t	t1 = t0;
		Tango::DeviceProxy	*serv;
		while (started==false && (t1-t0)<starter->serverStartupTimeout)
		{
#		ifdef WIN32
			_sleep(1000);
#		else
			sleep(1);
#		endif
			//	Check if server has been started
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
		delete processes[i]->adminname;
		delete processes[i]->logfile;
	}

//	starter->starting_processes--;
	//	remove in level vector to start another level
	starter->start_proc_data->remove_current_level();
	return NULL;
}
#ifndef	WIN32
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
	switch(fork())
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
				setpgrp();

				//	Call setsid() to do NOT stop children if Starter is killed.
				//--------------------------------------------------------------
				setsid();

				//	Close the stderr and re-open it on a log file.
				//-------------------------------------------------
				close(2);
				remove(process->logfile);
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


#else	//	WIN32


//+----------------------------------------------------------
//	WIN 32 methods to fork a sub process
//+----------------------------------------------------------
void StartProcessThread::start_process(NewProcess *process)
{
	StartWinThread	*win_thread = new StartWinThread(process);
	win_thread->start();
}

//+----------------------------------------------------------
//	WIN 32 Thread to fork a sub process
//		If batch file, the spawnv is blocking !!!
//+----------------------------------------------------------
void *StartWinThread::run_undetached(void *ptr)
{
	//	Check if batch file
	string	str_server(process->servname);
	if (str_server.find(".bat") != string::npos)
	{
		char	*argv[3];
		int		i = 0;
		argv[i++] = process->servname;
		argv[i++] = process->instancename;
		argv[i++] = NULL;

		cout << "Forking "   << process->servname << endl;
		cout << "log file: " << process->logfile << endl;

		//	Write a starting message in log file
		ofstream	ofs1(process->logfile);
		ofs1 << "Starting " << argv[0] << "....." << endl << endl;
		ofs1.close();

		//	Close the stderr and re-open it on a log file.
		//-------------------------------------------------
		int	ret;
		if ((ret=_spawnv(_P_WAIT, argv[0], argv))<0)
		{
			//	Write error in log file
			//-------------------------------
			ofstream	ofs(process->logfile);
			ofs << "Exec(" << argv[0] << ") failed " << endl;
			ofs << strerror(errno) << endl;
			ofs << "_spawnv has returned " << ret << endl;
			ofs.close();
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

		string	cmd_line(process->servname);
		cmd_line += " ";
		cmd_line += process->instancename;

		//
		// Get handles for standard output and errors
		//
		SECURITY_ATTRIBUTES sa;
		sa.bInheritHandle = 1;
		sa.nLength = sizeof(sa);
		sa.lpSecurityDescriptor = NULL;

		HANDLE hStdError  = CreateFile( process->logfile, 
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

		if( !CreateProcess(
				NULL ,            // pointer to name of executable module
				(char*)cmd_line.c_str(), // pointer to command line string
				&sa,              // process security attributes
				&sa,              // thread security attributes
				TRUE,             // handle inheritance flag
				DETACHED_PROCESS, // creation flags
				NULL,             // pointer to new environment block
				NULL,             // pointer to current directory name
				&si,              // pointer to STARTUPINFO
				&pi               // pointer to PROCESS_INFORMATION
				) )
		{
			_sleep(2000);
			cerr << "Error when trying to create process " << cmd_line <<endl << strerror(errno) << endl;

			//	Write a starting message in log file
			ofstream	ofs1(process->logfile);
			ofs1 << "Error when trying to create process " << cmd_line <<endl << strerror(errno) << endl;
			ofs1.close();
		}
		else
		{
			cout << "...OK (" << cmd_line << ")" << endl;
		}
	}
	return NULL;
}

#endif	//	WIN32

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
