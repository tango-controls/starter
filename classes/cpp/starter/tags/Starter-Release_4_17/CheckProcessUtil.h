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
// Revision 3.10  2010/09/21 12:18:58  pascal_verdier
// GPL Licence added to header.
//
// Revision 3.9  2010/02/09 15:09:49  pascal_verdier
// Define  _TG_WINDOWS_  replace WIN32.
// LogFileHome property added.
//
// Revision 3.8  2008/09/23 14:19:40  pascal_verdier
// Log files history added.
//
// Revision 3.7  2008/06/18 08:17:03  pascal_verdier
// Pb with case unsensitive on win32 fixed.
//
// Revision 3.6  2008/06/04 09:08:03  pascal_verdier
// javaw process control added.
// Java -cp classpath parsing mmodified.
//
// Revision 3.5  2008/05/15 08:07:18  pascal_verdier
// TangoSys_MemStream replaced by TangoSys_OMemStream
// (for leaking problem under win32)
//
// Revision 3.4  2008/04/11 07:26:45  jensmeyer
// Corrected compile options again!
//
// Revision 3.3  2008/04/10 12:15:05  jensmeyer
// Added compile options for MacOSX and FreeBSD
//
// Revision 3.2  2008/03/03 13:26:15  pascal_verdier
// is_process_running() method added.
//
// Revision 3.1  2008/02/29 15:15:05  pascal_verdier
// Checking running processes by system call added.
//
//=============================================================================
#ifndef _CHECKPROCESS_UTIL_H
#define _CHECKPROCESS_UTIL_H

#if defined (_TG_WINDOWS_)
# pragma warning (disable : 4786)
#endif



#include <tango.h>
#ifdef _TG_WINDOWS_
#	include <process.h>
#	include <direct.h>
#	include <io.h>
#	include <tlhelp32.h>
#else
#		include <sys/wait.h>
#		include <sys/time.h>
#		include <dirent.h>
#		include <sys/types.h>
#		include <fcntl.h>
#		include <sys/stat.h>
#		include <pwd.h>
#	if (!defined linux) && (!defined __darwin__) && (!defined __freebsd__)
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

	typedef enum _PROCESSINFOCLASS
	{
    	ProcessBasicInformation
	}
	PROCESSINFOCLASS;

	typedef LONG (WINAPI NTQIP)(HANDLE, PROCESSINFOCLASS, PVOID, ULONG, PULONG);

	typedef struct _PEB
	{
    	BYTE Reserved1[2];
    	BYTE BeingDebugged;
    	BYTE Reserved2[229];
    	PVOID Reserved3[59];
    	ULONG SessionId;
	}
	PEB, *PPEB;

	typedef struct _PROCESS_BASIC_INFORMATION
	{
    	PVOID Reserved1;
    	PPEB PebBaseAddress;
    	PVOID Reserved2[2];
	//    ULONG_PTR UniqueProcessId;
    	ULONG* UniqueProcessId;
    	PVOID Reserved3;
	}
	PROCESS_BASIC_INFORMATION;

	typedef struct ___PEB
	{
    	DWORD   dwFiller[4];
    	DWORD   dwInfoBlockAddress;
	}
	__PEB;

	typedef struct ___INFOBLOCK
	{
    	DWORD   dwFiller[16];
    	WORD    wLength;
    	WORD    wMaxLength;
    	DWORD   dwCmdLineAddress;
	}
	__INFOBLOCK;

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
#else
	bool manageProcFiles(Process *process);
#endif

public:
	ProcessData();
	~ProcessData();
	void update_process_list();
	bool is_server_running(string argin);
	bool is_process_running(string argin);
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
