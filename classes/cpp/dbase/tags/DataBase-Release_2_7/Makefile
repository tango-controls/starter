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

TANGO_HOME = /segfs/tango
MYSQL_BASE = /usr

ifdef _solaris
OS_VERS = $(shell /csadmin/common/scripts/get_os)
ifdef _gcc
CC = gcc -g
BIN_DIR = $(OS_VERS)_gcc
endif
ifdef _CC
CC = CC -g
BIN_DIR = $(OS_VERS)_CC
endif
TANGO_LIB = $(TANGO_HOME)/lib/$(BIN_DIR)
#MYSQL_BASE = /segfs/tango/database/$(OS_VERS)
MYSQL_BASE = /usr/local
endif

ifdef linux
CC = c++ -g
BIN_DIR   = $(shell /csadmin/common/scripts/get_os)
TANGO_LIB = $(TANGO_HOME)/lib/$(BIN_DIR)
endif

INCLUDE_DIRS = -I$(TANGO_HOME)/include/$(BIN_DIR) \
               -I$(MYSQL_BASE)/mysql/include \
	    	   -I.
#               -I$(MYSQL_BASE)/include/mysql \


LIB_DIRS =	-L $(MYSQL_BASE)/mysql/lib \
		  	-L$(TANGO_HOME)/lib/$(BIN_DIR)	\
			-L $(TANGO_LIB)

ifdef _solaris
ifdef _gcc
STD_LIB=	-lstdc++
endif
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
		-lsocket -lnsl -lm -lpthread $(STD_LIB)

LFLAGS_INIT =   $(LIB_DIRS)	\
		-lmysqlclient	\
		-lz				\
		-lposix4		\
		-lsocket -lnsl -lm -lpthread $(STD_LIB)

endif

ifdef linux
CFLAGS = -O $(INCLUDE_DIRS) -D_REENTRANT
LFLAGS_SERVER = $(LIB_DIRS)	\
			-ltango			\
			-llog4tango		\
			-lomniORB4		\
			-lomniDynamic4	\
			-lomnithread	\
			-lCOS4			\
			-lmysqlclient -lpthread -ldl

endif

CLASS = DataBase
DB_OBJ =	main.o	\
			$(CLASS)Class.o	\
			$(CLASS).o	\
			$(CLASS)StateMachine.o	\
			DataBaseUtils.o		\
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

DataBaseUtils.o: DataBaseUtils.cpp
	$(CC) DataBaseUtils.cpp $(CFLAGS) -c -o DataBaseUtils.o
	
db_bench.o: db_bench.cpp
	$(CC) db_bench.cpp $(CFLAGS) -c -o db_bench.o

db_bench: db_bench.o
	$(CC) db_bench.o $(LFLAGS_CLIENT) -o db_bench

db_init_history.o: db_init_history.cpp
	$(CC) db_init_history.cpp $(CFLAGS) -c -o db_init_history.o

db_init_history: db_init_history.o
	$(CC) db_init_history.o $(LFLAGS_INIT) -o db_init_history
	cp db_init_history $(BIN_DIR)

main.o: main.cpp
	$(CC) main.cpp $(CFLAGS) -c -o main.o
	
ClassFactory.o: ClassFactory.cpp
	$(CC) ClassFactory.cpp $(CFLAGS) -c -o ClassFactory.o


	
clean:
	rm -f *.o core

clobber: clean
	rm -f $(CLASS)ds
	rm -f db_client
	rm -f db_ping
	rm -f taco_tango

install:
	cp $(BIN_DIR)/$(CLASS)ds	$(TANGO_HOME)/bin/$(BIN_DIR)
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
