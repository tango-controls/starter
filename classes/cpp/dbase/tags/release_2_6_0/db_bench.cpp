#include <tango.h>

#include <assert.h>
#include <iostream>
#include <stdio.h>
#include <sys/time.h>
#include <signal.h>

/*
 * global variables
 */

static long n_total, n_ok, n_notok;
static long t_min, t_max, t_total, t_avg;

void signal_handler(int signal)
{
/*
 *
 * ----orion.ctrm.esrf.fr PING Statistics----
 * 4 packets transmitted, 4 packets received, 0% packet loss
 * round-trip (us)  min/avg/max = 0/0/1
 *
 */

	t_avg = t_total / n_total;

	printf("\n---- DB_BENCH Statistics----\n");
	printf("%d packets transmitted, %d packets successful, %d errors\n",
                n_total, n_ok, n_notok);
	printf("round-trip (us) min/avg/max = %d/%d/%d\n",t_min,t_avg,t_max);

	exit(0);
}


int main(int argc,char *argv[])
{
	struct timeval t1, t2, dev_timeout;
	long elapsed;

        (void) signal(SIGINT,  signal_handler);
	(void) signal(SIGQUIT, signal_handler);
	(void) signal(SIGTERM, signal_handler);

	n_total = n_ok = n_notok = 0;
	t_max = 0;
	t_min = 0;

	if (argc != 2) {
		cerr << "Usage : client <database_name> " << endl;
		exit(-1);
	}

	char *dbase_name = argv[1];

// Connect to device

	Tango::DeviceProxy *dev;
	
	try {
		dev = new Tango::DeviceProxy(dbase_name);
	}
	catch (CORBA::Exception  &e) {
		//Tango::Util::print_exception(e);
		exit(-1);
	}
	
	cout << "Connected to database device" << dbase_name << endl;
		
// Print choice

	while (1) {
		
		try {
			int j;
			for (j=0; j >= 0 ; j++) {
				CORBA::Any send;
				send <<= dbase_name;
						
				gettimeofday(&t1, NULL);
				CORBA::Any_var received = dev->command_inout("DbImportDevice",send);
				gettimeofday(&t2, NULL);
				elapsed = (t2.tv_sec-t1.tv_sec)*1000000;
				elapsed = (t2.tv_usec-t1.tv_usec) + elapsed;

				n_total++;
				if (t_max == 0) t_max = elapsed;
				if (elapsed > t_max) t_max = elapsed;
				if (t_min == 0) t_min = elapsed;
				if (elapsed < t_min) t_min = elapsed;
				t_total = t_total + elapsed;
				if (j%1000 == 0)
				{
					t_avg = t_total / n_total;

					printf("\n---- DB_BENCH Statistics----\n");
					printf("%d packets transmitted, %d packets successful, %d errors\n",
                				n_total, n_ok, n_notok);
					printf("round-trip (us) min/avg/max = %d/%d/%d\n",t_min,t_avg,t_max);
				}
				n_ok++;
				Tango::DevVarLongStringArray *import_info;
				assert(received >>= import_info);
					
				/*
				cout << "Device import info " << " version " << (import_info->svalue)[2] << " IOR " << (import_info->svalue)[1] << endl;
				cout << "Device exported " << (import_info->lvalue)[0] << endl;
				 */
			}
		}
		catch (CORBA::Exception &e) {
			//Tango::Util::print_exception(e);
		}
	}			
}

