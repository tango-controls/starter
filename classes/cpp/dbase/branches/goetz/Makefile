#
#		Makefile to generate a Tango DataBase device server
#

TANGO_BASE = /segfs/tango
MYSQL_BASE = /usr/local

ifdef solaris
CC = c++
BIN_DIR = solaris
ORBACUS_BASE = /segfs/tango/ORB/ORBacus/solaris_gcc
JTC_BASE = /segfs/tango/JTC/solaris_gcc
TANGO_LIB = $(TANGO_BASE)/lib/solaris
endif

ifdef linux
CC = c++
BIN_DIR = linux
ORBACUS_BASE = /segfs/tango/ORB/ORBacus/linux
JTC_BASE = /segfs/tango/JTC/linux
TANGO_LIB = $(TANGO_BASE)/lib/linuxx86
endif

INCLUDE_DIRS = -I $(ORBACUS_BASE)/include/OB \
	       -I $(ORBACUS_BASE)/include \
	       -I $(JTC_BASE)/include/JTC \
	       -I $(JTC_BASE)/include  \
	       -I $(TANGO_BASE)/include \
               -I $(MYSQL_BASE)/include/mysql \
	       -I .

LIB_DIRS = -L $(ORBACUS_BASE)/lib \
	   -L $(JTC_BASE)/lib \
           -L $(MYSQL_BASE)/lib/mysql \
	   -L $(TANGO_LIB)

ifdef solaris
CFLAGS = -g $(INCLUDE_DIRS)
LFLAGS_SERVER = -g $(LIB_DIRS) -lOB -lJTC -lmysqlclient -lposix4 -lsocket \
		-lnsl -lpthread
LFLAGS_CLIENT = -g $(LIB_DIRS) -lOB -lJTC -lposix4 -lsocket -lnsl -lpthread
endif

ifdef linux
CFLAGS = -O $(INCLUDE_DIRS) -DDEBUG
LFLAGS_SERVER = -O $(LIB_DIRS) -lOB -lJTC -lmysqlclient -ltango  -lpthread 
LFLAGS_CLIENT = -O $(LIB_DIRS) -lOB -lJTC -ltango  -lpthread 
endif

all: DataBaseds db_client

DataBaseds: main.o /tmp/DataBaseClass.o /tmp/DataBase.o /tmp/ClassFactory.o
	$(CC) main.o /tmp/DataBaseClass.o /tmp/DataBase.o \
	/tmp/ClassFactory.o $(LFLAGS_SERVER) -o /tmp/DataBaseds
	mv /tmp/DataBaseds .
			
/tmp/DataBaseClass.o: DataBaseClass.cpp
	$(CC) DataBaseClass.cpp $(CFLAGS) -c -o /tmp/DataBaseClass.o
	
/tmp/DataBase.o: DataBase.cpp
	$(CC) DataBase.cpp $(CFLAGS) -c -o /tmp/DataBase.o
	
main.o: main.cpp
	$(CC) main.cpp $(CFLAGS) -c -o main.o
	
/tmp/ClassFactory.o: ClassFactory.cpp
	$(CC) ClassFactory.cpp $(CFLAGS) -c -o /tmp/ClassFactory.o
	
db_client: db_client.o 
	$(CC) db_client.o $(LFLAGS_CLIENT) -o db_client 

db_client.o: db_client.cpp
	$(CC) db_client.cpp $(CFLAGS) -c -o db_client.o

db_ping: db_ping.o 
	$(CC) db_ping.o $(LFLAGS_CLIENT) -o db_ping 

db_ping.o: db_ping.cpp
	$(CC) db_ping.cpp $(CFLAGS) -c -o db_ping.o

taco_tango: taco_tango.o 
	$(CC) taco_tango.o $(LFLAGS_CLIENT) -ldsapig++ -ldbapi -ldsxdr -ldcapi \
	-o taco_tango

taco_tango.o: taco_tango.cpp
	$(CC) taco_tango.cpp $(CFLAGS) -D__STDC__ -c

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
