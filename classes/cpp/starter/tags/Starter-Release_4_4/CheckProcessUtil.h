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
// $Revision$
//
// $Log$
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
//
// copyleft :    European Synchrotron Radiation Facility
//               BP 220, Grenoble 38043
//               FRANCE
//
//=============================================================================
#ifndef _CHECKPROCESS_UTIL_H
#define _CHECKPROCESS_UTIL_H

#if defined (WIN32)
# pragma warning (disable : 4786)
#endif



#include <tango.h>
#ifdef WIN32
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
	vector<string>	line_args;
	vector<string>	proc_args;
	string	user;
	long	pid;
}
Process;





#ifdef WIN32

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

	string	name_from_path(string full_name);

#ifdef WIN32
	string wchar2string(WCHAR *wch, int size=0x100);
	WCHAR *string2wchar(string str);
	string errorCodeToString(DWORD err_code,  string src);
#else
	void manageProcFiles(Process *process);
#endif

public:
	ProcessData() {};
	~ProcessData();
	void update_process_list();
	bool is_server_running(string argin);
	bool is_process_running(string argin);
	int  get_server_pid(string argin);
	vector<Process>	get_process_list();
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
