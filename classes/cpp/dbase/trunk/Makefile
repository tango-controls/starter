#=============================================================================
#
# file :        Makefile
#
# description : Include for the Starter class.
#
# project :     Makefile to generate a Tango $(
#
# $Author$
#
# $Revision$
#
# $Log$
# Revision 2.41  2010/01/06 09:26:50  pascal_verdier
# -lmysqlclient_r added for linux.
#
# Revision 2.40  2010/01/05 11:45:32  pascal_verdier
# mysql lib dir depends on nb bits for linux.
#
# Revision 2.39  2008/05/23 08:34:32  taurel
# - Remove the compiler flag -DGCC_STD for Linux platform
#
# Revision 2.38  2008/03/27 07:54:56  taurel
# - Some changes for Dbserver V4 (threaded mysql lib...)
#
# Revision 2.37.2.1  2007/11/07 09:50:41  taurel
# - First changes to get a fully-threaded DB server
#
# Revision 2.37  2007/11/06 07:25:18  taurel
# - Added the DbGetDataForServerCache command (with timing stats)
# - Add timing stats for the DbPutClassProperty command
#
# Revision 2.36  2007/03/23 14:42:11  pascal_verdier
# *** empty log message ***
#
# Revision 2.35  2007/03/23 14:40:06  pascal_verdier
# Bug on MySql version compiler flag fixed.
#
# Revision 2.34  2006/11/03 16:58:32  jlpons
# Correction for db_init_history
#
# Revision 2.33  2006/10/03 13:25:55  jlpons
# Added rules for db_init_history
#
# Revision 2.32  2006/09/28 11:18:20  pascal_verdier
# DbGetClassForDevice and DbGetClassInheritanceForDevice commands added.
#
# Revision 2.31  2006/06/22 15:25:32  jlpons
# Added history commands
#
# Revision 2.30  2005/10/20 10:47:27  pascal_verdier
# Add stuff for gcc.
#
# Revision 2.29  2005/10/19 08:49:18  pascal_verdier
# little cleaning of dummy things.
#
# Revision 2.28  2005/09/07 14:53:30  pascal_verdier
# *** empty log message ***
#
# Revision 2.27  2005/04/06 11:02:30  pascal_verdier
# Add a compatibility to pogo.
#
# Revision 2.26  2005/02/15 10:03:31  pascal_verdier
# Bug fixed in DbGetDeviceInfo command
#
# Revision 2.24  2004/07/06 13:35:30  pascal_verdier
# DbGetDeviceAlias command added.
#
# Revision 2.23  2004/06/21 12:20:12  pascal_verdier
# DbGetExportdDeviceListForClass and DbGetDeviceAlias commands added.
#
# Revision 2.22  2004/03/08 12:41:56  pascal_verdier
# Attribute alias management added.
#
# Revision 2.21  2004/02/23 10:24:37  pascal_verdier
# A thread has been added.
# This thread is in charge to warn concerned Starter servers to execute DbGetServerInfo.
# Because a new server has been exported (DbExportDevice command has been received),
# or server info have been modified (DbPutServerInfo command has been received).
#
# Revision 2.20  2004/01/26 13:24:57  pascal_verdier
# DbGetServerNameList and DbGetInstanceNameList commands added.
#
#
# copyleft :    European Synchrotron Radiation Facility
#               BP 220, Grenoble 38043
#               FRANCE
#
#=============================================================================

CLASS      = DataBase
MAJOR_VERS = 4
MINOR_VERS = 0
RELEASE    = Release_$(MAJOR_VERS)_$(MINOR_VERS)
OS         = $(shell /csadmin/common/scripts/get_os)

#-----------------------------------------
#	Set default home directories
#-----------------------------------------
#TEST_TANGO_HOME   =  /segfs/tango/tmp/pascal/cppapi
TANGO_HOME = /segfs/tango

ifdef no_debug
	DEBUG = -O
else
	DEBUG = -g
endif

#------------------- Solaris Definitions --------------------
ifdef _solaris
ifdef _gcc
CC = c++
BIN_DIR = $(OS)_gcc
endif
ifdef _CC
CC = CC
BIN_DIR = $(OS)_CC
endif
MYSQL_HOME=/usr/local/mysql
MYSQL_INC_DIR=$(MYSQL_HOME)/include/mysql
MYSQL_LIB_DIR=$(MYSQL_HOME)/lib/mysql
endif



#------------------- Linux Definitions --------------------
ifdef linux
CC = c++ -g
PROCESSOR  = $(shell uname -p)
$(PROCESSOR)=1
ifdef x86_64
NBITS=64
endif
BIN_DIR=$(shell /csadmin/common/scripts/get_os)_$(NBITS)

BIN_DIR   = $(OS)_$(NBITS)
MYSQL_INC_DIR=/usr/include/mysql
MYSQL_LIB_DIR=/usr/lib$(NBITS)/mysql
endif



INCLUDE_DIRS = -I$(TEST_TANGO_HOME)/$(BIN_DIR)/include \
				-I$(MYSQL_INC_DIR) \
               -I$(TANGO_HOME)/release/$(BIN_DIR)/include \
	    	   -I.


OBJS_DIR  = obj/$(BIN_DIR)
TANGO_LIB = $(TANGO_HOME)/lib/$(BIN_DIR)
LIB_DIRS  =	-L $(TEST_TANGO_HOME)/$(BIN_DIR)/lib  \
			-L $(MYSQL_LIB_DIR) \
		  	-L$(TANGO_HOME)/release/$(BIN_DIR)/lib	\
			-L $(TANGO_LIB)

ifdef _solaris
ifdef _gcc
STD_LIB=	-lstdc++
#CXXFLAGS =  $(DEBUG) -D_PTHREADS -DGCC_STD $(INCLUDE_DIRS)
CXXFLAGS =  $(DEBUG) -D_PTHREADS $(INCLUDE_DIRS)
else
CXXFLAGS =  $(DEBUG) -D_PTHREADS $(INCLUDE_DIRS)
endif

LFLAGS_SERVER = $(LIB_DIRS)	\
		-ltango			\
		-llog4tango		\
		-lomniORB4		\
		-lomniDynamic4	\
		-lomnithread	\
		-lmysqlclient_r	\
		-lCOS4			\
		-lz				\
		-lposix4		\
		-lsocket -lnsl -lm -lpthread -ldl $(STD_LIB)

LFLAGS_INIT =   $(LIB_DIRS)	\
		-ltango			\
		-llog4tango		\
		-lomniORB4		\
		-lomniDynamic4	\
		-lomnithread	\
		-lmysqlclient	\
		-lCOS4			\
		-lmysqlclient_r	\
		-lz				\
		-lposix4		\
		-lsocket -lnsl -lm -lpthread $(STD_LIB)

endif

ifdef linux
CXXFLAGS =  $(DEBUG) -D_REENTRANT $(INCLUDE_DIRS)
LFLAGS_SERVER = $(LIB_DIRS)	\
		-ltango			\
		-llog4tango		\
		-lomniORB4		\
		-lomniDynamic4	\
		-lomnithread	\
		-lCOS4			\
		-lmysqlclient_r -lpthread -ldl

LFLAGS_INIT =   $(LIB_DIRS)	\
		-ltango			\
		-llog4tango		\
		-lomniORB4		\
		-lomniDynamic4	\
		-lomnithread	\
		-lCOS4			\
		-lmysqlclient_r	\
		-lpthread -ldl

endif

DB_OBJ =	$(OBJS_DIR)/main.o	\
			$(OBJS_DIR)/$(CLASS)Class.o	\
			$(OBJS_DIR)/$(CLASS).o	\
			$(OBJS_DIR)/$(CLASS)StateMachine.o	\
			$(OBJS_DIR)/DataBaseUtils.o		\
			$(OBJS_DIR)/update_starter.o	\
			$(OBJS_DIR)/ClassFactory.o

DB_INIT_OBJ =	$(OBJS_DIR)/db_init_history.o

all: $(CLASS)ds db_init_history

test:
	tango.test


$(CLASS)ds:	make_obj_dir make_bin_dir $(DB_OBJ)
	$(CC) $(DB_OBJ) -o $(CLASS)ds $(LFLAGS_SERVER)
	@mv $(CLASS)ds bin/$(BIN_DIR)/$(CLASS)ds


$(OBJS_DIR)/%.o: %.cpp 
	$(CC) $(CXXFLAGS) -c $< -o $(OBJS_DIR)/$*.o
			
db_init_history: make_obj_dir make_bin_dir $(DB_INIT_OBJ)
	$(CC) $(DB_INIT_OBJ) -o db_init_history $(LFLAGS_INIT)
	@mv db_init_history bin/$(BIN_DIR)/db_init_history
	
clean:
	rm -f $(OBJS_DIR)/*.o  \
		$(OBJS_DIR)/*.so.o \
		bin/$(BIN_DIR)/$(CLASS)ds \
		core

clobber: clean
	rm -f $(CLASS)ds
	rm -f db_client
	rm -f db_ping
	rm -f taco_tango

make_obj_dir:
	@mkdir -p obj
	@mkdir -p obj/$(BIN_DIR)

make_bin_dir:
	@mkdir -p bin
	@mkdir -p bin/$(BIN_DIR)

install:
	cp bin/$(BIN_DIR)/$(CLASS)ds  $(TANGO_HOME)/bin/$(BIN_DIR)
	ls -l $(TANGO_HOME)/bin/$(BIN_DIR)/$(CLASS)ds

#----------------------------------------------------
#	Tag the CVS module corresponding to this class
#----------------------------------------------------
tag:
	@cvstag "$(CLASS)-$(RELEASE)"
	@make   $(CLASS)
	@make show_tag

show_tag:
	@cvstag -d

