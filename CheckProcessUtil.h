//=============================================================================
//
// file :        CheckProcess.h
//
// description : Include for the CheckProcess class.
//
// project :	Check Process
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
#ifndef _CHECKPROCESS_UTIL_H
#define _CHECKPROCESS_UTIL_H


#ifdef _WIN32
	#ifndef _WIN32_WINNT	
		#define _WIN32_WINNT 0x500
	#endif
#endif



#include <tango.h>

#ifdef _TG_WINDOWS_
#	include <process.h>
#	include <direct.h>
#	include <io.h>
#	include <tlhelp32.h>
#	include <WinBase.h>
#else
#		include <sys/wait.h>
#		include <sys/time.h>
#		include <dirent.h>
#		include <sys/types.h>
#		include <fcntl.h>
#		include <sys/stat.h>
#		include <pwd.h>
#	if (!defined __GLIBC__) && (!defined __darwin__) && (!defined __freebsd__)
     /* solaris */
#		include <procfs.h>
#		include <fcntl.h>
#  endif
#endif


/**
 * @author	$Author$
 * @version	$Revision$
 */

 //	Add your own constant definitions here.
 //-----------------------------------------------

typedef struct {
	string	line;
	string	name;
	string	servname;
	vector<string>	line_args;
	vector<string>	proc_args;
	long	pid;
}
Process;





#ifdef _TG_WINDOWS_

#	include <Winternl.h>
	typedef LONG (NTAPI NTQIP)	\
		(HANDLE, PROCESSINFOCLASS, PVOID, ULONG, PULONG);

#endif




namespace Starter_ns
{


//=======================================================
//=======================================================
class ProcessData: public Tango::TangoMonitor
{
private:
	vector<Process*>	proc_list;

	void read_process_list_from_sys();
	bool check_java_process(Process* process);
	bool check_python_process(Process* process);
	void check_cpp_process(Process* process);
	void build_server_names(Process* process);

	string	name_from_path(string full_name);

#ifdef _TG_WINDOWS_
	bool win2000;
	bool isWin2000();
	string  parseNameFromCmdLine(string name, string cmdline);
	PVOID  getPebAddress(HANDLE pHandle);
	PVOID  getPebStructure(HANDLE hProcess, PVOID pebAddress);
	UNICODE_STRING  getUnicodeCommandLine(HANDLE hProcess, PVOID paramAddress);
#else
	bool manageProcFiles(Process *process);
#endif

public:
	ProcessData();
	~ProcessData();
	void update_process_list();
	bool is_server_running(string argin);
	bool is_process_running(string argin);
	int getNbServerInstances(string argin);
	int  get_server_pid(string argin);
	vector<Process>	get_process_list();
#ifdef _TG_WINDOWS_
	static string wchar2string(WCHAR *wch, int size=0x100);
	static WCHAR *string2wchar(string str);
	static string errorCodeToString(DWORD err_code,  string src);
#endif
};

//=======================================================
//=======================================================
class CheckProcessUtil: public omni_thread
{
private:
	ProcessData	*data;
	bool	stop_thread;

public:
	CheckProcessUtil() { data=new ProcessData(); stop_thread=false;};
	bool is_server_running(string argin);
	bool is_process_running(string argin);
	int getNbServerInstances(string argin);
	int  get_server_pid(string argin);
	vector<Process> get_process_list();
	void stop_it() { stop_thread= true; };

	/**
	 *	Execute the thread loop.
	 */
	void *run_undetached(void *);
	void start() {start_undetached();}
};

}	// namespace_ns

#endif	// _CHECKPROCESS_H
