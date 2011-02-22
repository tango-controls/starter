# Microsoft Developer Studio Project File - Name="tangotest_shared" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=tangotest_shared - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "tangotest_shared.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "tangotest_shared.mak" CFG="tangotest_shared - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "tangotest_shared - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "tangotest_shared - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "tangotest_shared - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "./objs/shared/release"
# PROP Intermediate_Dir "./objs/shared/release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GR /GX /O2 /I "$(OMNI_BASE)\win32\include" /I "$(TANGO_BASE)\win32\include" /I "$(LOG4TANGO_BASE)\win32\include" /I "..\.." /D "NDEBUG" /D "LOG4TANGO_HAS_DLL" /D "TANGO_HAS_DLL" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /FD /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE RSC /l 0x40c /d "NDEBUG"
# ADD RSC /l 0x40c /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 user32.lib advapi32.lib ws2_32.lib omniORB405_rt.lib omniDynamic405_rt.lib COS405_rt.lib omniThread30_rt.lib tango.lib log4tango.lib kernel32.lib gdi32.lib comctl32.lib /nologo /subsystem:console /machine:I386 /out:"shared\release\tangotest.exe" /libpath:"$(TANGO_BASE)\win32_dll\lib" /libpath:"$(OMNI_BASE)\win32_dll\lib" /libpath:"$(LOG4TANGO_BASE)\win32_dll\lib"

!ELSEIF  "$(CFG)" == "tangotest_shared - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "./objs/shared/debug"
# PROP Intermediate_Dir "./objs/shared/debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GR /GX /ZI /Od /I "$(OMNI_BASE)\win32\include" /I "$(TANGO_BASE)\win32\include" /I "$(LOG4TANGO_BASE)\win32\include" /I "..\.." /D "_DEBUG" /D "LOG4TANGO_HAS_DLL" /D "TANGO_HAS_DLL" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /FR /FD /GZ /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE RSC /l 0x40c /d "_DEBUG"
# ADD RSC /l 0x40c /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 omniORB405_rtd.lib omniDynamic405_rtd.lib COS405_rtd.lib omnithread30_rtd.lib tangod.lib log4tangod.lib kernel32.lib gdi32.lib comctl32.lib user32.lib advapi32.lib ws2_32.lib /nologo /subsystem:console /debug /machine:I386 /out:"shared\debug\tangotest.exe" /pdbtype:sept /libpath:"$(TANGO_BASE)\win32_dll\lib" /libpath:"$(OMNI_BASE)\win32_dll\lib" /libpath:"$(LOG4TANGO_BASE)\win32_dll\lib"

!ENDIF 

# Begin Target

# Name "tangotest_shared - Win32 Release"
# Name "tangotest_shared - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\ClassFactory.cpp
# End Source File
# Begin Source File

SOURCE=..\..\main.cpp
# End Source File
# Begin Source File

SOURCE=..\..\TangoTest.cpp
# End Source File
# Begin Source File

SOURCE=..\..\TangoTestClass.cpp
# End Source File
# Begin Source File

SOURCE=..\..\TangoTestStateMachine.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\TangoTest.h
# End Source File
# Begin Source File

SOURCE=..\..\TangoTestClass.h
# End Source File
# End Group
# End Target
# End Project
