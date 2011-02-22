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
// Copyright (C) :      2004,2005,2006,2007,2008,2009,2010,2011
//						European Synchrotron Radiation Facility
//                      BP 220, Grenoble 38043
//                      FRANCE
//
// This file is part of Tango.
//
// Tango is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// Tango is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with Tango.  If not, see <http://www.gnu.org/licenses/>.
//
// $Revision$
//
//=============================================================================
#ifndef _UPD_STARTER_THREAD_H
#define _UPD_STARTER_THREAD_H

#include <tango.h>

namespace DataBase_ns {

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

