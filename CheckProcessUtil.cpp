//+=============================================================================
//
// file :         CheckProcessUtil.cpp
//
// description :  C++ source for the CheckProcessUtil
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
//-=============================================================================



#include <CheckProcessUtil.h>

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

#ifdef _TG_WINDOWS_
typedef LONG (WINAPI *TNtQueryInformationProcess)(HANDLE,UINT,PVOID,ULONG,PULONG);
#endif

namespace Starter_ns
{
//=============================================================
//=============================================================
ProcessData::ProcessData()
{
#ifdef _TG_WINDOWS_
	//	Under win 2000 or before the process name is shorted to 15 char
	//	If win 2000, take it from command line.
	win2000 = isWin2000();
#endif	/*	_TG_WINDOWS_		*/
}
//=============================================================
//=============================================================
ProcessData::~ProcessData()
{
	//	clear previous list
	for (unsigned int i=0 ; i<proc_list.size() ; i++)
		delete proc_list[i];
	proc_list.clear();
}

#ifdef _TG_WINDOWS_
//=============================================================
//=============================================================
string  ProcessData::parseNameFromCmdLine(string name, string cmdline)
{
	//	Search last position of name
	string::size_type	pos = 0;
	string::size_type	tmp;
	while ((tmp=cmdline.find(name, pos+1))!=string::npos)
		pos = tmp;

	//	Get name before sppace char
	string::size_type	end = cmdline.find(" ", pos);
	if (end==string::npos)
		end = cmdline.find("\t", pos);
	string	full_name = cmdline.substr(pos, end-pos);

	//	Take off extention if any
	end = full_name.find(".");
	if (end==string::npos)
		return full_name;
	else
		return full_name.substr(0, end);
}
//=============================================================
//=============================================================
bool ProcessData::isWin2000(void)
{
	OSVERSIONINFOEX osvi;
	BOOL bOsVersionInfoEx;

	ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
	if(!(bOsVersionInfoEx=GetVersionEx((OSVERSIONINFO *)&osvi)))
	{
		osvi.dwOSVersionInfoSize=sizeof(OSVERSIONINFO);
		if (!GetVersionEx((OSVERSIONINFO *)&osvi) )
			return true;
	}
	double	osversion = osvi.dwMajorVersion + (1.0*osvi.dwMinorVersion/10);
	if (osversion<5.1)
	{
		cout << "/=======================================" << endl;
		cout << "	Windows 2000 or before !!!" << endl;
		cout << "/=======================================" << endl;
		return true;
	}
	else
	{
		cout << "Windows XP or later" << endl;
		return false;
	}
}
//=============================================================
//=============================================================
string ProcessData::wchar2string(WCHAR *wch, int size)
{
	char	*ch = new char[size+1];
	int	i;
	for (i=0 ; wch[i]!=0 && i<size ; i++)
		ch[i] = wch[i];
	ch[i] = 0x0;
	string	str(ch);
	delete [] ch;
	return str;
}
//=============================================================
//=============================================================
WCHAR *ProcessData::string2wchar(string str)
{
	char	*ch  = new char[str.length()+1];
	WCHAR	*wch = new WCHAR[str.length()+1];
	strcpy(ch, str.c_str());

	int	i;
	for (i=0 ; ch[i]!=0 ; i++)
		wch[i] = (short)ch[i];
	wch[i] = 0x0;
	delete [] ch;
	return wch;
}
//=============================================================
//=============================================================
void ProcessData::read_process_list_from_sys()
{
	//	clear previous list
	for (int i=0 ; i<proc_list.size() ; i++)
		delete proc_list[i];
	proc_list.clear();
#ifdef OLD
	// Take a snapshot of all processes in the system.
	HANDLE	hProcessSnap = 
		CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS | TH32CS_SNAPMODULE, 0);
	if( hProcessSnap == INVALID_HANDLE_VALUE)
	{
		string desc = errorCodeToString(GetLastError(), "CreateToolhelp32Snapshot" );
		Tango::Except::throw_exception(
						(const char *)"PROCESS_LIST_FAILED",
						(const char *) desc.c_str(),
						(const char *)"Starter::get_process_list()");
	}

	// Set the size of the structure before using it.
	PROCESSENTRY32	pe32;
	pe32.dwSize = sizeof( PROCESSENTRY32 );

	// Retrieve information about the first process,
	// and exit if unsuccessful
	if( !Process32First( hProcessSnap, &pe32 ) )
	{
		string desc = errorCodeToString(GetLastError(), "Process32First" );  // Show cause of failure
		CloseHandle( hProcessSnap );     // Must clean up the snapshot object!
		Tango::Except::throw_exception(
						(const char *)"PROCESS_LIST_FAILED",
						(const char *) desc.c_str(),
						(const char *)"Starter::get_process_list()");
	}

	//	Get module ntdll
    NTQIP						*lpfnNtQueryInformationProcess;
	WCHAR	*wc = string2wchar("ntdll.dll");
	HINSTANCE	hLibrary = GetModuleHandle(wc);
	delete [] wc;
    if (hLibrary != NULL)
    {
        lpfnNtQueryInformationProcess = (NTQIP *)GetProcAddress(hLibrary, "ZwQueryInformationProcess");
    }
	else
    {
		string	desc = errorCodeToString(GetLastError(), "GetModuleHandle() ");
		Tango::Except::throw_exception(
						(const char *)"PROCESS_LIST_FAILED",
						(const char *) desc.c_str(),
						(const char *)"Starter::get_process_list()");
    }

	do
	{
		// Retrieve the priority class.
		HANDLE	hProcess = OpenProcess( PROCESS_ALL_ACCESS, FALSE, pe32.th32ProcessID );
		if( hProcess != NULL )
		{
			PROCESS_BASIC_INFORMATION	pbi;
			DWORD		dwSize=0;
			if (lpfnNtQueryInformationProcess != NULL)
				(*lpfnNtQueryInformationProcess)(hProcess, ProcessBasicInformation, &pbi, sizeof(pbi), &dwSize);
			cout << pbi.PebBaseAddress <<  "  -  " << dwSize << endl;

			string cmdline("");
			SIZE_T		sizeT=0;
			__PEB 		PEB;
			DWORD dwPriorityClass = GetPriorityClass( hProcess );
			if( dwPriorityClass )
			{
				if (ReadProcessMemory(hProcess, 
						pbi.PebBaseAddress,
						&PEB,
						sizeof(PEB),
						&sizeT))
				{
					cout << hex << PEB.dwInfoBlockAddress << "  -  " << sizeT << endl;
					sizeT = 0;
					__INFOBLOCK	block;
					if (ReadProcessMemory(hProcess, 
							(LPVOID)PEB.dwInfoBlockAddress,
							&block,
							sizeof(block),
							&sizeT))
					{
						sizeT = 0;
						WCHAR	*buff = new WCHAR[block.wMaxLength+1];
    	        		if (ReadProcessMemory(hProcess, 
										(LPVOID)block.dwCmdLineAddress, 
										buff, 
										block.wMaxLength, 
										&sizeT)) {
							cmdline = wchar2string(buff, sizeT);
							cout << cmdline << endl;
						}
						else
							errorCodeToString(GetLastError(), "3-ReadProcessMemory()" );
						delete [] buff;
					}
					else
						errorCodeToString(GetLastError(), "2-ReadProcessMemory()" );
				}
				else
					errorCodeToString(GetLastError(), "1-ReadProcessMemory()" );
			}
			else
				errorCodeToString(GetLastError(), "GetPriorityClass" );
	
			CloseHandle( hProcess );

			//	build process object to be added in vector
			Process	*process = new Process();

			//	Remove extention from exe name
			string	full_name = wchar2string(pe32.szExeFile);
			string::size_type	pos = full_name.find('.');
			if (pos!=string::npos)
				process->name = full_name.substr(0, pos);
			else
				process->name = full_name;

			//	Parse name frome cmd line because file manager truncate it at 15 chars
			if (win2000 && process->name.length()>13)
				process->name = parseNameFromCmdLine(process->name, cmdline);

			//	On win32 -> exe file is case unsesitive
			transform(process->name.begin(), process->name.end(),
				process->name.begin(), ::tolower);


			//	add pid and cmd line
			process->pid  = pe32.th32ProcessID;
			process->line = cmdline;
			proc_list.push_back(process);
		}
	} while ( Process32Next(hProcessSnap, &pe32) );

	CloseHandle(hProcessSnap);
	
#else	//=============================================================================

	// Take a snapshot of all processes in the system.
	HANDLE	hProcessSnap = 
		CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS | TH32CS_SNAPMODULE, 0);
	if( hProcessSnap == INVALID_HANDLE_VALUE)
	{
		string desc = errorCodeToString(GetLastError(), "CreateToolhelp32Snapshot" );
		Tango::Except::throw_exception(
						(const char *)"PROCESS_LIST_FAILED",
						(const char *) desc.c_str(),
						(const char *)"Starter::get_process_list()");
	}

	// Set the size of the structure before using it.
	PROCESSENTRY32	pe32;
	pe32.dwSize = sizeof( PROCESSENTRY32 );

	// Retrieve information about the first process,
	// and exit if unsuccessful
	if( !Process32First( hProcessSnap, &pe32 ) )
	{
		string desc = errorCodeToString(GetLastError(), "Process32First" );  // Show cause of failure
		CloseHandle( hProcessSnap );     // Must clean up the snapshot object!
		Tango::Except::throw_exception(
						(const char *)"PROCESS_LIST_FAILED",
						(const char *) desc.c_str(),
						(const char *)"Starter::get_process_list()");
	}

	//	Get module ntdll
    NTQIP		*lpfnNtQueryInformationProcess;
	HINSTANCE	hLibrary = GetModuleHandleA("ntdll.dll");
    if (hLibrary != NULL)
    {
        lpfnNtQueryInformationProcess = (NTQIP *)GetProcAddress(hLibrary, "ZwQueryInformationProcess");
    }
	else
    {
		string	desc = errorCodeToString(GetLastError(), "GetModuleHandle() ");
		Tango::Except::throw_exception(
						(const char *)"PROCESS_LIST_FAILED",
						(const char *) desc.c_str(),
						(const char *)"Starter::get_process_list()");
    }

	do
	{
		// Retrieve the priority class.
		//HANDLE	hProcess = OpenProcess( PROCESS_ALL_ACCESS, FALSE, pe32.th32ProcessID );
		HANDLE	hProcess = OpenProcess(
			PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pe32.th32ProcessID );
		if( hProcess != NULL )
		{
			string strLine("");

			//	Get PEB address
			PVOID	pebAddress = getPebAddress(hProcess);

			//	Get the address of parameters
			PVOID	rtlUserProcParamsAddress = getPebStructure(hProcess, pebAddress);
			if (rtlUserProcParamsAddress!=NULL)
			{
				//	Read the command line in UNICODE structure
				UNICODE_STRING	commandLine =
					getUnicodeCommandLine(hProcess, rtlUserProcParamsAddress);
					
				//	Convert the command line
				WCHAR *commandLineContents = new WCHAR[commandLine.Length];
				if (ReadProcessMemory(hProcess, commandLine.Buffer,
					commandLineContents, commandLine.Length, NULL))
				{
					strLine = wchar2string(
						commandLineContents, commandLine.Length/2);
				}
				else
					errorCodeToString(GetLastError(), "Cannot convert the command line" );
				delete [] commandLineContents;
			}
			else
				errorCodeToString(GetLastError(), "Cannot get the address of parameters" );


			CloseHandle( hProcess );

			//	build process object to be added in vector
			Process	*process = new Process();

			//	Remove extention from exe name
			string	full_name = wchar2string(pe32.szExeFile);
			string::size_type	pos = full_name.find('.');
			if (pos!=string::npos)
				process->name = full_name.substr(0, pos);
			else
				process->name = full_name;

			//	Parse name frome cmd line because file manager truncate it at 15 chars
			if (win2000 && process->name.length()>13)
				process->name = parseNameFromCmdLine(process->name, strLine);

			//	On win32 -> exe file is case unsesitive
			transform(process->name.begin(), process->name.end(),
				process->name.begin(), ::tolower);


			//	add pid and cmd line
			process->pid  = pe32.th32ProcessID;
			process->line = strLine;
			proc_list.push_back(process);
		}
	} while ( Process32Next(hProcessSnap, &pe32) );
	CloseHandle(hProcessSnap);
#endif
}

//============================================================================ 
//============================================================================
UNICODE_STRING ProcessData::getUnicodeCommandLine(HANDLE hProcess, PVOID paramAddress)
{
	UNICODE_STRING	commandLine;

#if (_MSC_VER >= 1600)	//	VC10
	RTL_USER_PROCESS_PARAMETERS	*pProcessParam =
		(RTL_USER_PROCESS_PARAMETERS *) paramAddress;
	ReadProcessMemory(hProcess,
		(PCHAR) &pProcessParam->CommandLine,
		&commandLine, sizeof(commandLine), NULL);
#else
	ReadProcessMemory(hProcess,
		(PCHAR) paramAddress + 0x40,
		&commandLine, sizeof(commandLine), NULL);
#endif
	return commandLine;
}
//============================================================================ 
//============================================================================
PVOID ProcessData::getPebStructure(HANDLE hProcess, PVOID pebAddress)
{
	PVOID	rtlUserProcParamsAddress = NULL;

#if (_MSC_VER >= 1600)	//	VC10
	PEB		*peb = (PEB *)pebAddress;
	ReadProcessMemory(hProcess,
		(PCHAR)&peb->ProcessParameters,
		&rtlUserProcParamsAddress, sizeof(PVOID), NULL);
#else
	ReadProcessMemory(hProcess,
		(PCHAR)pebAddress+0x10,
		&rtlUserProcParamsAddress, sizeof(PVOID), NULL);
#endif
	return rtlUserProcParamsAddress;
}
//============================================================================ 
//============================================================================
PVOID  ProcessData::getPebAddress(HANDLE pHandle)
{
	NTQIP	*ntQueryInformationProcess = 
		(NTQIP *)GetProcAddress(GetModuleHandleA("ntdll.dll"), "NtQueryInformationProcess");
	PROCESS_BASIC_INFORMATION	pbi;
	ntQueryInformationProcess(pHandle, ProcessBasicInformation, &pbi, sizeof(pbi), NULL);
	return pbi.PebBaseAddress;
}


// ============================================================================
// Win32ProcessManager::errorCodeToString
// ============================================================================
string ProcessData::errorCodeToString (DWORD err_code,  string src)
{
	WCHAR	*buff;
	string	msg;

	if(err_code != ERROR_SUCCESS)
	{
		FormatMessage( 
			FORMAT_MESSAGE_ALLOCATE_BUFFER | 
			FORMAT_MESSAGE_FROM_SYSTEM | 
			FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			err_code,
			0, // Default language
			(LPTSTR) &buff,	
			0,
			NULL 
			);
		msg = src + string(" failed : ");
		msg += wchar2string(buff);

		// Free the buffer.
		LocalFree(buff);
	}
	else
		msg = "No Error";

	cerr << msg << endl;

	return msg;
}



#else	//	_TG_WINDOWS_




//=============================================================
//=============================================================
void ProcessData::read_process_list_from_sys()
{
	//	clear previous list
	for (unsigned int i=0 ; i<proc_list.size() ; i++)
		delete proc_list[i];
	proc_list.clear();

	//	build processes list
	DIR		*proc = opendir ("/proc") ;
	if(proc == NULL)
	{
		string	desc;
		//	error
		switch(errno)
		{
		case EACCES: desc = "Permission denied.";
			break;
		case EMFILE: desc = "Too many file descriptors in use by process.";
			break;
		case ENFILE: desc = "Too many file are currently open in the system.";
			break;
		case ENOENT: desc = "Directory does not exist or NAME is an empty string.";
			break;
		case ENOMEM: desc = "Insufficient memory to complete the operation.";
			break;
		case ENOTDIR:desc  = "NAME is not a directory.";
			break;
        default:desc  = "Undefined error.";
			break;
		}
		Tango::Except::throw_exception(
				(const char *)"READ_PROCESS_LIST_FAILED",
				desc,
				(const char *)"Starter::get_process_list()");
		
	} 

	struct dirent	*ent;
	while ( (ent=readdir(proc)) )
	{
		if (isdigit (ent->d_name[0]))
		{
			//	Get PID
			Process	*process = new Process();
			process->pid = atoi(ent->d_name);
			try
			{
				//	if process can be read, add process object to vector
				if (manageProcFiles(process))
					proc_list.push_back(process);
				else
					delete process;
			}
			catch(Tango::DevFailed &e)
			{
				cout << "Exception catch during manageProcFiles for pid = "
					<< process->pid << endl;
				cout << e.errors[0].desc;
				delete process;
			}
			catch(...)
			{
				cout << "Exception catch during manageProcFiles for pid = "
					<< process->pid << endl;
				delete process;
			}
		}
	}
	closedir(proc);
}


//=============================================================
/**
 *	Manage the /proc files
 */
//=============================================================
bool  ProcessData::manageProcFiles(Process *process)
{
#if (defined __GLIBC__) || (defined __darwin__) || (defined __freebsd__)

	//	Read command line file
	TangoSys_OMemStream	fname;
	fname << "/proc/" << process->pid <<"/cmdline";

	//	Read file
	ifstream	ifs((char *)fname.str().c_str());
	if (ifs)
	{
		TangoSys_OMemStream	sstr;
		sstr << ifs.rdbuf() << ends;
		ifs.close();

		//	Get command line
		process->line = sstr.str();
		//	Replace NULL with SPACE char
		string::size_type	pos;
		while ((pos=process->line.find('\0'))!=string::npos)
			process->line.replace(pos, 1, " ");
		return true;
	}
	else
	{
		cerr << fname.str() << ":	" << strerror(errno) << endl;
		return false;
	}
#else	//	solaris

	//	Read psinfo file		
	TangoSys_OMemStream	fname;
	fname << "/proc/" << process->pid <<"/psinfo";

	int				fid;
	struct psinfo	ps;	
	if ((fid=open(fname.str().c_str(), O_RDONLY))!=-1)
	{
		read(fid, (void *) &ps, sizeof(struct psinfo));
		close(fid);
		process->name = ps.pr_fname;
		
		//	Check if a real process or a shell (cannot be a server)
		if (process->name=="ssh" ||
			process->name=="bash" ||
			process->name=="sh")
			return false;

		uid_t	euid = geteuid();
		if ((euid==0) || (euid == ps.pr_euid))
		{
			/*
			 * To get the argv vector and environment variables
			 * for the process you need to be either root or the owner of the process.
			 * Otherwise you will not be able to open the processes memory.
			 */
			int fdesc;
			TangoSys_OMemStream	filepath;
			filepath << "/proc/" << process->pid <<"/as";
			if ( (fdesc = open(filepath.str().c_str(), O_RDONLY|O_NONBLOCK)) < 0 )
			{
				TangoSys_OMemStream	tms;
				tms << "Cannot open " << filepath.str() << ":	" <<
					strerror(errno) << endl;
#ifdef TRACE
				cerr << tms.str();
#endif
				Tango::Except::throw_exception(
						(const char *)"PROCESS_READ_FAILED",
						(const char *) tms.str().c_str(),
						(const char *)"Starter::manageProcFiles()");
			}
			else
			{
				//	Allocate a pointer array
				size_t	size = sizeof(char *) * (ps.pr_argc+1);
				char **argvp = (char **) malloc(size);

				//	And initialize
				if (pread(fdesc, argvp, size, ps.pr_argv)>0)
				{
					TangoSys_OMemStream	line;

					//	If argv[n] read -> append to command line
					char	buff[0x100];
					for (int n=0; n<ps.pr_argc; n++)
						if (pread(fdesc, buff ,0xFF, (off_t)argvp[n])>0)
							line << buff << " ";

					process->line = line.str();
					free(argvp);
					close(fdesc);
				}
				else
				{
					free(argvp);
					close(fdesc);
					TangoSys_OMemStream	tms;
					tms << "pread failed when getting command line arguments " <<
						" from memory for process  " << process->name  << " (" <<
						filepath.str() << ")\n" << strerror(errno) << endl;
					cerr << tms.str();
					Tango::Except::throw_exception(
							(const char *)"PROCESS_READ_FAILED",
							(const char *) tms.str().c_str(),
							(const char *)"Starter::manageProcFiles()");
				}
			}
		}
		else
		{
			//	Not Owner -> get only the  ps.pr_psargs
			process->line = ps.pr_psargs;
			close(fid);
		}
	}
	else
	{
		TangoSys_OMemStream	tms;
		tms << "open(" << fname.str() << ")  failed\n" <<  strerror(errno) << endl;
		cerr << tms.str();
		Tango::Except::throw_exception(
						(const char *)"PROCESS_READ_FAILED",
						(const char *) tms.str().c_str(),
						(const char *)"Starter::manageProcFiles()");
	}
	return true;
#endif
}
#endif	//	_TG_WINDOWS_



//=============================================================
/**
 *	Not only cpp
 *	Check for other than java and python processes
 */
//=============================================================
void ProcessData::check_cpp_process(Process* process)
{
	//	Remove path
#ifndef _TG_WINDOWS_
	if (process->line_args.size()==0)
		process->name = "";
	else
		process->name = name_from_path(process->line_args[0]);
#endif

	for (unsigned int i=1 ; i<process->line_args.size() ; i++)
		process->proc_args.push_back(process->line_args[i]);
}
//=============================================================
/**
 *	Check for  java processes
 */
//=============================================================
bool ProcessData::check_java_process(Process *process)
{
	if (process->line_args.size()==0)
		return false;
#ifdef _TG_WINDOWS_
	if (process->name!="java" &&
		process->name!="javaw")
		return false;
#else
	if (name_from_path(process->line_args[0])!="java")
		return false;
#endif

	//	Parse class and instance name
	bool	found=false;
	for (unsigned long i=process->line_args.size()-1 ; !found && i>0 ; i--)
	{
		if (process->line_args[i]!="" && process->line_args[i].c_str()[0]!='-')
		{
			if (i>1)	 // -1 because last one is instance
			{
				//	To get class name, remove package name of previous arg
				string	full_name(process->line_args[i-1]);
				string::size_type	start = 0;
				string::size_type	end;
				while ((end=full_name.find('.', start))!=string::npos)
					start = end+1;
                string name = full_name.substr(start);

                if (name!="jar") {
                    //	Get last one
                    process->name = name;
                }
                else {
                    // if jar file get file name without extension
                    full_name = full_name.substr(0, start-1);
                    start = full_name.rfind("."); // last in package
                    if (start==string::npos)
                        start = full_name.rfind("/");// last in path
                    if (start==string::npos)
                        start = 0; // only name
                    else
                        start++; // after / or .
                    process->name = full_name.substr(start);
                }
				//	and take this one as instance 
				process->proc_args.push_back(process->line_args[i]);
				found = true;
			}
		}
	}
	return true;
}
//=============================================================
/**
 *	Check for  python processes
 */
//=============================================================
bool ProcessData::check_python_process(Process* process)
{
	if (process->line_args.size()==0)
		return false;

	//	Check id process is python (must start with "python")
	string	processName;
#ifdef _TG_WINDOWS_
	processName = process->name;
//	if (process->name!="python")
//		return false;
#else
	processName = name_from_path(process->line_args[0]);
//	if (name_from_path(process->line_args[0])!="python")
//		return false;
#endif
	if (processName.find("python")!=0)
		return false;
	

	if (process->line_args.size()<2)
		return false;	//	No module loaded

	//	To get python module name
	bool found_py_module = false;
	unsigned int args_idx = 1;
	for (; args_idx < process->line_args.size()-1 ; args_idx++)
	{
	    const string &arg = process->line_args[args_idx];
	    
	    if (arg.empty())
	        continue;
	    
	    if (arg[0] == '-')
	    {
	        // special python arguments that receive and additional parameter
	        if (arg.size() > 1 && (arg[1] == 'Q' || arg[1] == 'W'))
	            args_idx++; 
	        continue;
	    }
	    
	    // we reached the python file in execution
        found_py_module = true;
	    
        string::size_type start = arg.rfind('/');
        if (start == string::npos)
            start = arg.rfind('\\');
        
        if (start == string::npos)
            start = 0;
        else
            start++;

	    string::size_type end = arg.rfind(".py");
        if (end == string::npos) {
        	//	Check if starts package name
        	string fullName = arg.substr(start);
			string::size_type index = fullName.rfind('.');
			if (index==string::npos)
	        	process->name = fullName;
			else
				process->name = fullName.substr(++index);
        }
        else
            process->name = arg.substr(start, end-start);
        // everything from now is an argument
        args_idx++;
        break;
	}

    if (!found_py_module)
        return false;

	for (unsigned int i=args_idx ; i<process->line_args.size() ; i++)
		process->proc_args.push_back(process->line_args[i]);
	return true;
}
//=============================================================
//=============================================================
string ProcessData::name_from_path(string full_name)
{
	string::size_type	start = 0;
	string::size_type	end;
	while ((end=full_name.find('/', start))!=string::npos)
		start = end+1;
	//	Get last one
	return full_name.substr(start);
}
//=============================================================
//=============================================================
void ProcessData::build_server_names(Process* process)
{
	// server is a process with at least one arg
	if (process->proc_args.size()>0)
	{
		process->servname  = process->name;
		process->servname += "/";
		string	instance(process->proc_args[0]);
		transform(instance.begin(), instance.end(),
				instance.begin(), ::tolower);
		process->servname += instance;
#ifdef _TG_WINDOWS_
	//	Wain32 is case unsensitive
	transform(process->serverName.begin(), process->serverName.end(),
					process->serverName.begin(), ::tolower);
#endif
}
	else
		process->servname  = "";
}
//=============================================================
//=============================================================




//=============================================================
/**
 *	Public method to update and build process process 
 */
//=============================================================
//#define TRACE
void ProcessData::update_process_list()
{
	omni_mutex_lock sync(*this);

	TimeVal	t0, t1;
	GetTime(t0);
	read_process_list_from_sys();
	GetTime(t1);
#ifdef TRACE
	TimeVal	t2, t3;
	double max_t = 0;
	Process	*max_t_proc;
	cout << "	Reading process list = " << Elapsed(t0, t1) << " ms" << endl;
#endif

	for (unsigned int i=0 ; i<proc_list.size() ; i++)
	{
#ifdef TRACE
		GetTime(t2);
#endif
		Process	*process = proc_list[i];

		//	Split on Space char
		string::size_type	start = 0;
		string::size_type	end;
		bool	in_cotes = false;
		while ((end=process->line.find(' ', start))!=string::npos)
		{
			string	s = process->line.substr(start, (end-start));
			start = end+1;
			//	Check if not empty
			if (s!="" && s!=" " && s!="\t")
			{
				//	Check if between cotes
				if (in_cotes==false && s.find('\"')!=string::npos) //	starting
				{
					process->line_args.push_back(s);
					in_cotes = true;
				}
				else
				if (in_cotes==true) // inside
				{
					//	Get last arg and concat with new one
					string	arg = process->line_args.back();
					arg += " " + s;
					//	And replace
					process->line_args.pop_back();
					process->line_args.push_back(arg);

					if (s.find('\"')!=string::npos) // ending
						in_cotes = false;
				}
				else
					process->line_args.push_back(s);
			}
		}
		//	Get last one
		string	s = process->line.substr(start);
		if (s!="")
			process->line_args.push_back(s);


#ifndef _TG_WINDOWS_
		if (process->line_args.empty()==false)
			process->name = process->line_args[0];
		else
			process->name = "";
			
#endif

		//	Check if java or python process
		if (check_java_process(process)==false)
			if(check_python_process(process)==false)
				check_cpp_process(process);
		build_server_names(process);

#ifdef TRACE2
		cout << process->pid << "	" << process->name;
		if (process->proc_args.empty()==false)
			cout << " " << process->proc_args[0];
		cout << endl;
#endif

#ifdef TRACE
		GetTime(t3);
		double	t = Elapsed(t2, t3);
		if (t>max_t)
		{
			max_t = t;
			max_t_proc = process;
		}
#endif

	}
#ifdef TRACE
	GetTime(t1);
	cout << "		total = " << Elapsed(t0, t1) << " ms" << endl;
	cout << "	max:	" << max_t << "  for " << max_t_proc->name << " (" <<
				 max_t_proc->pid << ")" << endl;
#endif

}
//=============================================================
//=============================================================
int ProcessData::get_server_pid(string argin)
{
	omni_mutex_lock sync(*this);
	for (unsigned int i=0 ; i<proc_list.size() ; i++)
	{
		Process	*process = proc_list[i];
		// server is a process with at least one arg
		if (process->proc_args.empty()==false)
		{
			string	servname(process->name);
			servname += "/";
			servname += process->proc_args[0];
			//cout << serverName << "==" << argin << endl;
			if (servname == argin)
				return (int)process->pid;
		}
	}
	return -1;
}
//=============================================================
/**
 * Returs true if server running 
 */
//=============================================================
bool ProcessData::is_server_running(string argin)
{
	omni_mutex_lock sync(*this);
	for (unsigned int i=0 ; i<proc_list.size() ; i++)
	{
		Process	*process = proc_list[i];
        if (!process->servname.empty() && process->servname == argin)
            return true;
	}
	return false;
}
//=============================================================
/**
 * Returs true if process running (do not check instance name)
 */
//=============================================================
bool ProcessData::is_process_running(string argin)
{
	omni_mutex_lock sync(*this);
	for (unsigned int i=0 ; i<proc_list.size() ; i++)
	{
		Process	*process = proc_list[i];
		if (!process->servname.empty() && process->name == argin)
			return true;
	}
	return false;
}
//=============================================================
/**
 * Returs true if process running
 */
//=============================================================
int ProcessData::getNbServerInstances(string argin)
{
	omni_mutex_lock sync(*this);
    int nb = 0;
    for (unsigned int i=0 ; i<proc_list.size() ; i++)
    {
        Process	*process = proc_list[i];
        if (!process->servname.empty() && process->servname==argin)
            nb++;
    }
	return nb;
}
//=============================================================
//=============================================================
vector<Process> ProcessData::get_process_list()
{
	omni_mutex_lock sync(*this);
	
	//	copy list
	vector<Process>	ret;
	for (unsigned int i=0 ; i<proc_list.size() ; i++)
	{
		Process	*p_src = proc_list[i];
		Process	process;
		Process *p_target = &process;
		*p_target = *p_src;
		ret.push_back(process);
	}

	return ret;
}
//=============================================================
//=============================================================






//=============================================================
//=============================================================
int CheckProcessUtil::get_server_pid(string argin)
{
	return data->get_server_pid(argin);
}
//=============================================================
/**
 * Returs true if server running
 */
//=============================================================
bool CheckProcessUtil::is_server_running(string argin)
{
	//	Make sure instance is lower case
	string::size_type	pos = argin.find('/');
	if (pos==string::npos)
		return false;	//	Not a server name
	pos++;
	string	dsname(argin.substr(0, pos));
#ifdef _TG_WINDOWS_
	//	Wain32 is case unsensitive
	transform(dsname.begin(), dsname.end(),
					dsname.begin(), ::tolower);
#endif
	string	instance(argin.substr(pos));
	transform(instance.begin(), instance.end(),
					instance.begin(), ::tolower);
	dsname += instance;
	return data->is_server_running(dsname);
}
//=============================================================
/**
 * Returs true if process running (do not check instance name)
 */
//=============================================================
bool CheckProcessUtil::is_process_running(string argin)
{
	return data->is_process_running(argin);
}
//=============================================================
/**
 * Returs true if process running several times (normally cannot occurs)
 */
//=============================================================
int CheckProcessUtil::getNbServerInstances(string argin)
{
	return data->getNbServerInstances(argin);
}
//=============================================================
//=============================================================
vector<Process> CheckProcessUtil::get_process_list()
{
	return data->get_process_list();
}
//=============================================================
//=============================================================
void *CheckProcessUtil::run_undetached(TANGO_UNUSED(void *ptr))
{
	while (stop_thread==false)
	{
		try
		{
			data->update_process_list();
		}
		catch(Tango::DevFailed &e)
		{
			Tango::Except::print_exception(e);
		}
		

		//	And wait n times for next loop
		for (int i=0 ; i<2 && stop_thread==false ; i++)
		{
			omni_mutex_lock sync(*data);
			data->wait(1000);
		}
	}
	delete data;
	return NULL;
}
//=============================================================
//=============================================================


}	//	namespace


 	  	 
