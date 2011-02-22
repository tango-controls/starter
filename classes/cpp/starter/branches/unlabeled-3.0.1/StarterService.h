

#include <tango.h>
#include <ntservice.h>

/**
 *	Class to use Device server as service
 */


class StarterService: public Tango::NTService
{
public:
	StarterService(char *);
	void start(int, char **, Tango::NTEventLogger *);
};
