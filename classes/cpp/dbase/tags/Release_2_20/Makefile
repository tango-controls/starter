#
#		Makefile to generate a Tango DataBase device server
#

TANGO_BASE = /segfs/tango
MYSQL_BASE = /usr
TACO_BASE  = $(DSHOME)

ifdef solaris
CC = /opt/SUNWspro/bin/CC
BIN_DIR = solaris7_CC
#BIN_DIR = V1.6/solaris7
TANGO_LIB = $(TANGO_BASE)/lib/solaris7_CC
#TANGO_LIB = $(TANGO_BASE)/lib/solaris7
MYSQL_BASE = /segfs/tango/database/solaris7
endif

ifdef linux
CC = c++
#BIN_DIR = suse64
#BIN_DIR = suse72_gcc32
BIN_DIR = suse72
TANGO_LIB = $(TANGO_BASE)/lib/$(BIN_DIR)
endif

ifdef hpux10
CC = aCC
BIN_DIR = hpux102
TANGO_LIB = $(TANGO_BASE)/lib/hpux102
endif

INCLUDE_DIRS = -I$(TANGO_BASE)/include/$(BIN_DIR) \
               -I$(MYSQL_BASE)/include/mysql \
	       -I.


LIB_DIRS = -L $(MYSQL_BASE)/lib/mysql \
	   -L $(TANGO_LIB)

ifdef solaris
CFLAGS = $(INCLUDE_DIRS)
LFLAGS_SERVER = $(LIB_DIRS) -lomniORB4 -lomniDynamic4 -lomnithread \
		-lmysqlclient -ltango -llog4tango -lposix4 -lsocket -lnsl -lpthread
#LFLAGS_CLIENT = -Wl,-Bdynamic $(LIB_DIRS) -lOB -lJTC -ltango -lposix4
LFLAGS_CLIENT = $(LIB_DIRS) -lomniORB4 -lomnithread -ltango -lposix4 \
		-lsocket -lnsl -lpthread 
#LFLAGS_TACO = -L$(TACO_BASE)/lib/solaris -ltacog++
LFLAGS_TACO = -L$(TACO_BASE)/lib/solaris -ltaco++ 
#		-ldcapi -ldbapi -ldsxdr 
endif

ifdef linux
#CFLAGS = -g $(INCLUDE_DIRS) -DDEBUG -D_REENTRANT
CFLAGS = -g $(INCLUDE_DIRS) -D_REENTRANT
#LFLAGS_SERVER = $(LIB_DIRS) -ltango -lOB -lJTC -lmysqlclient -lpthread 
LFLAGS_SERVER = -v $(LIB_DIRS) -ltango -llog4tango -lomniORB4 -lomniDynamic4 -lomnithread \
		-lmysqlclient -lpthread -ldl
LFLAGS_CLIENT = -g $(LIB_DIRS) -lomniORB4 -lomniDynamic4 -lomnithread \
		-ltango  -lpthread -ldl
LFLAGS_TACO = -L$(TACO_BASE)/lib/linux/x86 -ltacog++
#		-ldsapig++ -ldcapi -ldbapi -ldsxdr 
endif

CFLAGS_TACO = -I$(TACO_BASE)/include -I$(TACO_BASE)/include++ \
	-I$(TACO_BASE)/include/private

all: DataBaseds 

DataBaseds: main.o DataBaseClass.o DataBase.o ClassFactory.o
	$(CC) main.o DataBaseClass.o DataBase.o \
	ClassFactory.o $(LFLAGS_SERVER) \
	-o DataBaseds
			
DataBaseClass.o: DataBaseClass.cpp
	$(CC) DataBaseClass.cpp $(CFLAGS) -c -o DataBaseClass.o
	
DataBase.o: DataBase.cpp
	$(CC) DataBase.cpp $(CFLAGS) -c -o DataBase.o
	
main.o: main.cpp
	$(CC) main.cpp $(CFLAGS) -c -o main.o
	
ClassFactory.o: ClassFactory.cpp
	$(CC) ClassFactory.cpp $(CFLAGS) -c -o ClassFactory.o
	
clean:
	rm -f *.o core

clobber: clean
	rm -f DataBaseds
	rm -f db_client
	rm -f db_ping
	rm -f taco_tango
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
	  DataBaseClass.cpp \
	  DataBase.cpp \
	  ClassFactory.cpp \
	  db_client.cpp
	  
DBASE_INCL = DataBaseClass.h \
	     DataBase.h
	  
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
