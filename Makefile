#=============================================================================
#
# file :        Makefile
#
# description : Include for the Starter class.
#
# project :     Makefile to generate a Tango server
#
# $Author$
#
# $Revision$
#
# $Log$
# Revision 3.5  2005/10/20 10:48:23  pascal_verdier
# Add stuff for gcc.
#
# Revision 3.4  2005/04/21 07:18:48  pascal_verdier
# Add a little timeout for ping.
# Fix a bug for windows (SIGCHLD).
#
# Revision 3.3  2004/12/10 08:57:19  pascal_verdier
# Tango 5 compatibility (attribute management).
#
# Revision 3.2  2004/06/29 04:24:26  pascal_verdier
# First revision using events.
#
# Revision 3.1  2004/05/19 08:00:03  pascal_verdier
# merge changes from 3.0.1 onto trunk
#
# Revision 3.0.1.5  2004/02/27 09:53:02  pascal_verdier
# - The starter device is now warned by Database server when something change on a server.
# It replace the DbGetHostServersInfo polling.
# - HostState, ControlledRunningServers, ontrolledStoppedServers attributes added.
#
# Revision 3.0.1.4  2003/12/08 08:53:52  pascal_verdier
# Cluster (multi-host) control implemented.
# Control of notify daemon implemented but not tested.
#
# Revision 3.0.1.3  2003/10/15 10:37:08  pascal_verdier
# *** empty log message ***
#
# Revision 3.0.1.2  2003/10/08 09:18:03  pascal_verdier
# *** empty log message ***
#
# Revision 3.0.1.1  2003/09/18 12:02:49  pascal_verdier
# Problem on Windows service startup fixed.
#
# Revision 3.0  2003/06/17 12:06:36  pascal_verdier
# TANGO 3.x server.
# polling bugs fixed.
#
# Revision 2.0  2003/01/09 13:35:50  verdier
# TANGO 2.2
#
# Revision 1.22  2002/12/18 08:09:19  verdier
# omniORB compatibility
#
# Revision 1.6  2002/10/15 18:55:21  verdier
# The host state is now calculated during the State polled command.
#
#
# copyleft :    European Synchrotron Radiation Facility
#               BP 220, Grenoble 38043
#               FRANCE
#
#=============================================================================
#  		This file is generated by POGO
#	(Program Obviously used to Generate tango Object)
#
#         (c) - Software Engineering Group - ESRF
#=============================================================================
#

CLASS    = Starter
RELEASE  = Release_1_0

TANGO_HOME   =  /segfs/tango

ifdef _solaris
CC = CC
OS_VERS=$(shell /csadmin/common/scripts/get_os)
ifdef _gcc
CC = gcc
BIN_DIR = $(OS_VERS)_gcc
endif
ifdef _CC
CC = CC
BIN_DIR = $(OS_VERS)_CC
endif
endif

ifdef linux
CC = c++
AR = ar
BIN_DIR=$(shell /csadmin/common/scripts/get_os)
endif

INCLUDE_DIRS =	-I$(TANGO_HOME)/include/$(BIN_DIR)	-I.

LIB_DIRS     =  -L $(TANGO_HOME)/lib/$(BIN_DIR)

ifdef _solaris
ifdef _gcc
STD_LIB=	-lstdc++
endif

CXXFLAGS =  -O -g -D_PTHREADS $(INCLUDE_DIRS)
LFLAGS =  -g $(LIB_DIRS)  	\
			-ltango			\
			-llog4tango		\
			-lomniORB4 		\
			-lomniDynamic4	\
			-lomnithread	\
			-lCOS4			\
			-lpthread		\
			-lposix4 -lsocket -lnsl $(STD_LIB)
#
endif

ifdef linux
CXXFLAGS =  -g -D_REENTRANT $(INCLUDE_DIRS)
LFLAGS =  -g $(LIB_DIRS)	\
			-ltango			\
			-llog4tango		\
			-lomniDynamic4	\
			-lomniORB4		\
			-lomnithread	\
			-lCOS4			\
			-ldl -lpthread
endif






SVC_OBJS =	$(CLASS).o		\
			$(CLASS)Class.o	\
			$(CLASS)StateMachine.o	\
			$(CLASS)Util.o	\
			ClassFactory.o	\
			main.o
			
SVC_INC = 	$(CLASS)Class.h \
			$(CLASS).h

INSTANCE_NAME =	sys

#%.o: %.cpp $(SVC_INC)
%.o: %.cpp 
	$(CC) $(CXXFLAGS) -c $<
			
all: $(CLASS)

$(CLASS):	$(SVC_OBJS)
	$(CC) $(SVC_OBJS) -o $(CLASS) $(LFLAGS)
	mv $(CLASS) $(BIN_DIR)/$(CLASS)

clean:
	rm -f *.o $(CLASS) core
	
install:
	cp $(BIN_DIR)/$(CLASS) $(TANGO_HOME)/bin/$(BIN_DIR)
	ls -l $(TANGO_HOME)/bin/$(BIN_DIR)


val:
	PATH=$(PATH):/segfs/tango/tmp/valgrind/bin	\
	valgrind -v --num-callers=20 --logfile-fd=9 --leak-check=yes --leak-resolution=high suse72/Starter splash 9>log


#----------------------------------------------------
#	Tag the CVS module corresponding to this class
#----------------------------------------------------
tag:
	@cvstag "$(CLASS)-$(RELEASE)"
	@make   $(CLASS)
	@make show_tag

show_tag:
	@cvstag -d 
