#=============================================================================
#
# file :        Makefile
#
# description : Include for the Starter class.
#
# project :     Makefile to generate a Tango $(CLASS) device server
#
# $Author$
#
# $Revision$
#
# $Log$
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

TANGO_HOME = /segfs/tango
MYSQL_BASE = /usr
TACO_BASE  = $(DSHOME)

ifdef _solaris
CC = CC
OS_VERS = $(shell /csadmin/common/scripts/get_os)
BIN_DIR = $(OS_VERS)_CC
TANGO_LIB = $(TANGO_HOME)/lib/$(BIN_DIR)
MYSQL_BASE = /segfs/tango/database/$(OS_VERS)
endif

ifdef linux
CC = c++ -g
BIN_DIR   = $(shell /csadmin/common/scripts/get_os)
TANGO_LIB = $(TANGO_HOME)/lib/$(BIN_DIR)
endif

INCLUDE_DIRS = -I$(TANGO_HOME)/include/$(BIN_DIR) \
               -I$(MYSQL_BASE)/include/mysql \
	    	   -I.


LIB_DIRS =	-L $(MYSQL_BASE)/lib/mysql \
		  	-L$(TANGO_HOME)/lib/$(BIN_DIR)	\
			-L $(TANGO_LIB)

ifdef _solaris
CFLAGS = -O $(INCLUDE_DIRS)
LFLAGS_SERVER = $(LIB_DIRS)	\
		-ltango			\
		-llog4tango		\
		-lomniORB4		\
		-lomniDynamic4	\
		-lomnithread	\
		-lmysqlclient	\
		-lCOS4			\
		-lz				\
		-lposix4		\
		-lsocket -lnsl -lpthread

LFLAGS_CLIENT = $(LIB_DIRS) -lomniORB4 -lomnithread -ltango -lposix4 \
		-lsocket -lnsl -lpthread 
LFLAGS_TACO = -L$(TACO_BASE)/lib/solaris -ltaco++ 
endif

ifdef linux
CFLAGS = -O $(INCLUDE_DIRS) -D_REENTRANT
LFLAGS_SERVER = -v $(LIB_DIRS)	\
			-ltango			\
			-llog4tango		\
			-lomniORB4		\
			-lomniDynamic4	\
			-lomnithread	\
			-lCOS4			\
			-lmysqlclient -lpthread -ldl

LFLAGS_CLIENT = -O $(LIB_DIRS) -lomniORB4 -lomniDynamic4 -lomnithread \
		-ltango -llog4tango   -lpthread -ldl
LFLAGS_TACO = -L$(TACO_BASE)/lib/linux/x86 -ltacog++
endif

CFLAGS_TACO = -I$(TACO_BASE)/include -I$(TACO_BASE)/include++ \
	-I$(TACO_BASE)/include/private


CLASS = DataBase
DB_OBJ =	main.o	\
			$(CLASS)Class.o	\
			$(CLASS).o	\
			$(CLASS)StateMachine.o	\
			update_starter.o	\
			ClassFactory.o


all: $(CLASS)ds 

test:
	tango.test

$(CLASS)ds: $(DB_OBJ)
	$(CC) $(DB_OBJ) $(LFLAGS_SERVER) -o $(CLASS)ds
	cp $(CLASS)ds $(BIN_DIR)
			
$(CLASS)Class.o: $(CLASS)Class.cpp
	$(CC) $(CLASS)Class.cpp $(CFLAGS) -c -o $(CLASS)Class.o
	
$(CLASS).o: $(CLASS).cpp
	$(CC) $(CLASS).cpp $(CFLAGS) -c -o $(CLASS).o
	
$(CLASS)StateMachine.o: $(CLASS)StateMachine.cpp
	$(CC) $(CLASS)StateMachine.cpp $(CFLAGS) -c -o $(CLASS)StateMachine.o
	
update_starter.o: update_starter.cpp
	$(CC) update_starter.cpp $(CFLAGS) -c -o update_starter.o
	
db_bench.o: db_bench.cpp
	$(CC) db_bench.cpp $(CFLAGS) -c -o db_bench.o
	
db_bench: db_bench.o
	$(CC) db_bench.o $(LFLAGS_CLIENT) -o db_bench

main.o: main.cpp
	$(CC) main.cpp $(CFLAGS) -c -o main.o
	
ClassFactory.o: ClassFactory.cpp
	$(CC) ClassFactory.cpp $(CFLAGS) -c -o ClassFactory.o
	
clean:
	rm -f *.o core
	rm -Rf SunWS_cache

clobber: clean
	rm -f $(CLASS)ds
	rm -f db_client
	rm -f db_ping
	rm -f taco_tango

install:
	cp $(BIN_DIR)/$(CLASS)ds	$(TANGO_HOME)/bin/$(BIN_DIR)
	ls -l $(TANGO_HOME)/bin/$(BIN_DIR)/$(CLASS)ds
#
# source code version control (RCS)
#
RCSLOCK  =      co -l 
#               RCS check out options
RCSCO    =      co 
#               RCS check in options
RCSCI    =      ci -u -f -s"Rel" -m"$(LOCKMSG)"
#               RCS unlock options
RCSUNLOCK  =    rcs 
#               RCS diff options
RCSDIFF    =    rcsdiff 


DBASE_SRC = main.cpp \
	  $(CLASS)Class.cpp \
	  $(CLASS).cpp \
	  ClassFactory.cpp \
	  db_client.cpp
	  
DBASE_INCL = $(CLASS)Class.h \
	     $(CLASS).h
	  
lock:
		$(RCSLOCK) $(DBASE_SRC)
		$(RCSLOCK) $(DBASE_INCL)

co:
		$(RCSCO) $(DBASE_SRC)
		$(RCSCO) $(DBASE_INCL)

ci:
		$(RCSCI) $(DBASE_SRC)
		$(RCSCI) $(DBASE_INCL)

unlock:
		$(RCSUNLOCK) $(DBASE_SRC)
		$(RCSUNLOCK) $(DBASE_INCL)

diff:
		$(RCSDIFF) $(DBASE_SRC)
		$(RCSDIFF) $(DBASE_INCL)
