//=============================================================================
//
// file :        UpdateStarter.h
//
// description : include for thread to inform Strater to update from database
//
// project :	Starter for Tango Administration
//
// $Author$
//
// $Revision$
//
// copyleft :    European Synchrotron Radiation Facility
//               BP 220, Grenoble 38043
//               FRANCE
//
//=============================================================================
#ifndef _UPD_STARTER_THREAD_H
#define _UPD_STARTER_THREAD_H

#include <tango.h>

namespace DataBase {

/**
 * @author	$Author$
 * @version	$Revision$
 */

//=========================================================
/**
 *	Shared data between DS and thread.
 */
//=========================================================
class UpdStarterData: public Tango::TangoMonitor
{
private:
	vector<string>	starter_devnames;
public:
	UpdStarterData();
/**
 *	Get the host name to send cmd
 */
vector<string> get_starter_devname();
/**
 *	Set the host name to send cmd
 */
void send_starter_cmd(vector<string> hostname);
};
//=========================================================
/**
 *	Create a thread to prevent starter to update from database
 */
//=========================================================
class UpdateStarter: public omni_thread
{
private:
	/**
	 *	Shared data
	 */
	UpdStarterData	*shared;
public:
/**
 *	Create a thread to prevent starter to update from database
 */
 UpdateStarter(UpdStarterData *);
/**
 *	Execute the thread loop.
 */
	void *run_undetached(void *);
	void start() {start_undetached();}


};

}	//	namespace


#endif	// _UPD_STARTER_THREAD_H

