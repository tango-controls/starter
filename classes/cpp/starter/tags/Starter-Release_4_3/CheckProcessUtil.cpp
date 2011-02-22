static const char *RcsId = "$Header$";
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
// $Revision$
//
// $Log$
// Revision 3.4  2008/04/10 12:15:05  jensmeyer
// Added compile options for MacOSX and FreeBSD
//
// Revision 3.3  2008/04/09 14:39:57  pascal_verdier
// Better trace on pread failed
//
// Revision 3.2  2008/03/03 13:26:15  pascal_verdier
// is_process_running() method added.
//
// Revision 3.1  2008/02/29 15:15:05  pascal_verdier
// Checking running processes by system call added.
//
//
// copyleft :     European Synchrotron Radiation Facility
//                BP 220, Grenoble 38043
//                FRANCE
//
//-=============================================================================



#include <CheckProcessUtil.h>

#ifndef	TIME_VAR
#ifndef WIN32

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

#endif	/*	WIN32		*/
#endif	/*	TIME_VAR	*/


namespace Starter_ns
{

#ifdef WIN32
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
	delete ch;
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
	delete ch;
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
	PROCESS_BASIC_INFORMATION	pbi;
	WCHAR	*wc = string2wchar("ntdll.dll");
	HINSTANCE	hLibrary = GetModuleHandle(wc);
	delete wc;
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

	__INFOBLOCK	block;
    __PEB 		PEB;
	char		*c_cmdline = NULL;
    DWORD		dwSize=0;

	// Now walk the snapshot of processes, and
	pbi.PebBaseAddress = (PPEB)0x7ffdf000;
	do
	{
		// Retrieve the priority class.
		DWORD	dwPriorityClass = 0;
		HANDLE	hProcess = OpenProcess( PROCESS_ALL_ACCESS, FALSE, pe32.th32ProcessID );
		if( hProcess != NULL )
		{
	        if (lpfnNtQueryInformationProcess != NULL)
    	        (*lpfnNtQueryInformationProcess)(hProcess, ProcessBasicInformation, &pbi, sizeof(pbi), &dwSize);

			string cmdline("");
			dwPriorityClass = GetPriorityClass( hProcess );
			if( !dwPriorityClass )
				errorCodeToString(GetLastError(), "GetPriorityClass" );

			if (ReadProcessMemory(hProcess, 
					pbi.PebBaseAddress,
					&PEB,
					sizeof(PEB),
					&dwSize))
			{
				if (ReadProcessMemory(hProcess, 
						(LPVOID)PEB.dwInfoBlockAddress,
						&block,
						sizeof(block),
						&dwSize))
				{
					WCHAR	*buff = new WCHAR[block.wMaxLength+1];
    	        	if (ReadProcessMemory(hProcess, 
									(LPVOID)block.dwCmdLineAddress, 
									buff, 
									block.wMaxLength, 
									&dwSize))
					{
						cmdline = wchar2string(buff, dwSize);
						delete buff;
					}
					else
						errorCodeToString(GetLastError(), "3-ReadProcessMemory()" );
				}
				else
					errorCodeToString(GetLastError(), "2-ReadProcessMemory()" );
			}
			else
				errorCodeToString(GetLastError(), "1-ReadProcessMemory()" );
			
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

			//	add pid and cmd line
			process->pid  = pe32.th32ProcessID;
			process->line = cmdline;
			proc_list.push_back(process);
		}
	} while ( Process32Next(hProcessSnap, &pe32) );

	CloseHandle(hProcessSnap);
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



#else	//	WIN32




//=============================================================
//=============================================================
void ProcessData::read_process_list_from_sys()
{
	//	clear previous list
	for (int i=0 ; i<proc_list.size() ; i++)
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
		}
		Tango::Except::throw_exception(
				(const char *)"READ_PROCESS_LIST_FAILED",
				desc,
				(const char *)"Starter::get_process_list()");
		
	} 

	struct dirent	*ent;
	while (ent = readdir (proc))
	{
		if (isdigit (ent->d_name[0]))
		{
			//	Get PID
			Process	*process = new Process();
			process->pid = atoi(ent->d_name);

			try
			{
				manageProcFiles(process);
				//	Add process object to vector
				proc_list.push_back(process);
			}
			catch(...)
			{
				cout << "Excepion catch during manageProcFiles for pid = "
					<< process->pid << endl;
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
void  ProcessData::manageProcFiles(Process *process)
{
#if (defined linux) || (defined __darwin__) || (defined __freebsd__)

	//	Read command line file
	TangoSys_MemStream	fname;
	fname << "/proc/" << process->pid <<"/cmdline";

	//	Read file
	ifstream	ifs((char *)fname.str().c_str());
	if (ifs)
	{
		TangoSys_MemStream	sstr;
		sstr << ifs.rdbuf() << ends;
		ifs.close();

		//	Get command line
		process->line = sstr.str();
		//	Replace NULL with SPACE char
		string::size_type	pos;
		while ((pos=process->line.find('\0'))!=string::npos)
			process->line.replace(pos, 1, " ");
	}
	else
		cerr << fname.str() << ":	" << strerror(errno) << endl;


#else	//	solaris

	//	Read psinfo file		
	TangoSys_MemStream	fname;
	fname << "/proc/" << process->pid <<"/psinfo";

	int				fid;
	struct psinfo	ps;	
	if ((fid=open(fname.str().c_str(), O_RDONLY))!=-1)
	{
		read(fid, (void *) &ps, sizeof(struct psinfo));
		close(fid);
		process->name = ps.pr_fname;

		uid_t	euid = geteuid();
		if ((euid==0) || (euid == ps.pr_euid))
		{
			/*
			 * To get the argv vector and environment variables
			 * for the process you need to be either root or the owner of the process.
			 * Otherwise you will not be able to open the processes memory.
			 */
			int fdesc;
			TangoSys_MemStream	filepath;
			filepath << "/proc/" << process->pid <<"/as";
			if ( (fdesc = open(filepath.str().c_str(), O_RDONLY|O_NONBLOCK)) < 0 )
			{
#ifdef TRACE
				TangoSys_MemStream	tms;
				tms << "Cannot open " << filepath.str() << ":	" <<
					strerror(errno) << endl;
				cerr << tms.str();
				Tango::Except::throw_exception(
						(const char *)"PROCESS_READ_FAILED",
						(const char *) tms.str().c_str(),
						(const char *)"Starter::manageProcFiles()");
#endif
			}
			else
			{
				//	Allocate a pointer array
				size_t	size = sizeof(char *) * (ps.pr_argc+1);
				char **argvp = (char **) malloc(size);

				//	And initialize
				if (pread(fdesc, argvp, size, ps.pr_argv)>0)
				{
					TangoSys_MemStream	line;

					//	If argv[n] read -> append to command line
					char	buff[0x100];
					for (int n=0; n<ps.pr_argc; n++)
						if (pread(fdesc, buff ,0xFF, (off_t)argvp[n])>0)
							line << buff << " ";

					process->line = line.str();
				}
				else
				{
					TangoSys_MemStream	tms;
					tms << "pread failed when getting command line arguments " <<
						" from memory for process  " << process->name  << " (" <<
						filepath.str() << ")\n" << strerror(errno) << endl;
					cerr << tms.str();
					Tango::Except::throw_exception(
							(const char *)"PROCESS_READ_FAILED",
							(const char *) tms.str().c_str(),
							(const char *)"Starter::manageProcFiles()");
				}
				free(argvp);
				close(fdesc);
			}
		}
		else
		{
			//	Not Owner -> get only the  ps.pr_psargs
			process->line = ps.pr_psargs;
			close(fid);
		}
	}
#endif
}
#endif	//	WIN32



//=============================================================
/**
 *	Not only cpp
 *	Check for other than java and python processes
 */
//=============================================================
void ProcessData::check_cpp_process(Process* process)
{
	//	Remove path
#ifndef WIN32
	process->name = name_from_path(process->line_args[0]);
#endif

	for (int i=1 ; i<process->line_args.size() ; i++)
		process->proc_args.push_back(process->line_args[i]);
}
//=============================================================
/**
 *	Check for  java processes
 */
//=============================================================
bool ProcessData::check_java_process(Process* process)
{
#ifdef WIN32
	if (process->name!="java")
		return false;
#else
	if (name_from_path(process->line_args[0])!="java")
		return false;
#endif
	//	Check for class name
	bool	jvm_arg = true;
	for (int i=1 ; i<process->line_args.size() ; i++)
	{
		if (jvm_arg && process->line_args[i].c_str()[0]!='-')
		{
			jvm_arg = false;	//	end of jvm args
			
			//	To get class name, remove package name
			string	full_name(process->line_args[i]);
			string::size_type	start = 0;
			string::size_type	end;
			while ((end=full_name.find('.', start))!=string::npos)
				start = end+1;
			//	Get last one
			process->name = full_name.substr(start);
		}
		else
		if (jvm_arg==false)
			process->proc_args.push_back(process->line_args[i]);
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
#ifdef WIN32
	if (process->name!="python")
		return false;
#else
	if (name_from_path(process->line_args[0])!="python")
		return false;
#endif

	//	To get python module name
	string	full_name(process->line_args[1]);
	string::size_type	start = 0;
	string::size_type	end;
	while ((end=full_name.find('/', start))!=string::npos)
		start = end+1;
	while ((end=full_name.find('\\', start))!=string::npos)
		start = end+1;

	//	Get last one
	end = full_name.find(".py", start);
	process->name = full_name.substr(start, (end-start));

	for (int i=2 ; i<process->line_args.size() ; i++)
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




//=============================================================
/**
 *	Public method to update and build process process 
 */
//=============================================================
void ProcessData::update_process_list()
{
	omni_mutex_lock sync(*this);

	TimeVal	t0, t1;
	GetTime(t0);
	read_process_list_from_sys();
	GetTime(t1);
#ifdef TRACE
	cout << "	Reading process list = " << Elapsed(t0, t1) << " ms" << endl;
#endif

	for (int i=0 ; i<proc_list.size() ; i++)
	{
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
					//	et last arg and concat with new one
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
		process->line_args.push_back(s);


#ifndef WIN32
		process->name = process->line_args[0];
#endif

		//	Check if java or python process
		if (check_java_process(process)==false)
			if(check_python_process(process)==false)
				check_cpp_process(process);
#ifdef TRACE2
		cout << process->pid << "	" << process->name;
		if (process->proc_args.size()>0)
			cout << " " << process->proc_args[0];
		cout << endl;
#endif
	}

}
//=============================================================
//=============================================================
int ProcessData::get_server_pid(string argin)
{
	omni_mutex_lock sync(*this);
	for (int i=0 ; i<proc_list.size() ; i++)
	{
		Process	*process = proc_list[i];
		// server is a process with at least one arg
		if (process->proc_args.size()>0)
		{
			string	servname(process->name);
			servname += "/";
			servname += process->proc_args[0];
			//cout << servname << "==" << argin << endl;
			if (servname == argin)
				return process->pid;
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
	for (int i=0 ; i<proc_list.size() ; i++)
	{
		Process	*process = proc_list[i];
		// server is a process with at least one arg
		if (process->proc_args.size()>0)
		{
			string	servname(process->name);
			servname += "/";
			servname += process->proc_args[0];
			//cout << servname << "==" << argin << endl;
			if (servname == argin)
				return true;
		}
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
	for (int i=0 ; i<proc_list.size() ; i++)
	{
		Process	*process = proc_list[i];
		if (process->name == argin)
			return true;
	}
	return false;
}
//=============================================================
//=============================================================
vector<Process> ProcessData::get_process_list()
{
	omni_mutex_lock sync(*this);
	
	//	copy list
	vector<Process>	ret;
	for (int i=0 ; i<proc_list.size() ; i++)
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
	return data->is_server_running(argin);
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
//=============================================================
vector<Process> CheckProcessUtil::get_process_list()
{
	return data->get_process_list();
}
//=============================================================
//=============================================================
void *CheckProcessUtil::run_undetached(void *ptr)
{
	while (true)
	{
		try
		{
			data->update_process_list();
		}
		catch(Tango::DevFailed &e)
		{
			Tango::Except::print_exception(e);
		}
		

		//	And wait for next loop
		{
			omni_mutex_lock sync(*data);
			data->wait(2000);
		}
	}
	return NULL;
}
//=============================================================
//=============================================================


}	//	namespace
