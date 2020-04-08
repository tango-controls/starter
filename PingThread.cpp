//+=============================================================================
//
// file :         StarterUtil.cpp
//
// description :  C++ source for tools used by the Starter device server.
//
// project :      TANGO Device Server
//
// $Author$
//
// Copyright (C) :      2004,2005,2006,2007,2008,2009,2010,2011,2012,2013,2014,2015
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
// $Date$
//
//-=============================================================================

#include <tango.h>

#include <PingThread.h>

#include <utility>

#ifndef    TIME_VAR
#ifndef _TG_WINDOWS_

#	define    TimeVal    struct timeval
#	define    GetTime(t)    gettimeofday(&t, nullptr);
#	define    Elapsed(before, after)    \
        1000.0*(after.tv_sec-before.tv_sec) + \
        ((double)after.tv_usec-before.tv_usec) / 1000

#else

#	define	TimeVal	struct _timeb
#	define	GetTime(t)	_ftime(&t);
#	define	Elapsed(before, after)	\
        1000*(after.time - before.time) + (after.millitm - before.millitm)

#endif    /*	_TG_WINDOWS_		*/
#endif    /*	TIME_VAR	*/

namespace Starter_ns {
    //+----------------------------------------------------------------------------
    /**
     *	Constructor
     */
    //+----------------------------------------------------------------------------
    PingThreadData::PingThreadData(string sname) {
        servname = std::move(sname);
        stop_thread = false;
        last_write_time = time(nullptr);
        state = Tango::ON;
        nbInstances = 0;
    }
    //+----------------------------------------------------------------------------
    /**
     *	Get the server name
     */
    //+----------------------------------------------------------------------------
    string PingThreadData::get_server_name() {
        omni_mutex_lock sync(*this);
        return servname;
    }
    //+----------------------------------------------------------------------------
    /**
     *	command to stop thread
     */
    //+----------------------------------------------------------------------------
    void PingThreadData::set_stop_thread() {
        omni_mutex_lock sync(*this);
        stop_thread = true;
        signal();
    }
    //+----------------------------------------------------------------------------
    /**
     *	get  stop thread status
     */
    //+----------------------------------------------------------------------------
    bool PingThreadData::get_stop_thread() {
        omni_mutex_lock sync(*this);
        return stop_thread;
    }
    //+----------------------------------------------------------------------------
    /**
     *	Return the elapsed time from last write.
     */
    //+----------------------------------------------------------------------------
    time_t PingThreadData::get_last_write_time() {
        omni_mutex_lock sync(*this);
        return last_write_time;
    }
    //+----------------------------------------------------------------------------
    /**
     *	Set the ping result.
     */
    //+----------------------------------------------------------------------------
    void PingThreadData::set_state(Tango::DevState st) {
        omni_mutex_lock sync(*this);
        state = st;
        last_write_time = time(nullptr);
    }
    //+----------------------------------------------------------------------------
    /**
     *	Set the ping result.
     */
    //+----------------------------------------------------------------------------
    Tango::DevState PingThreadData::get_state() {
        omni_mutex_lock sync(*this);
        return state;
    }
    //+----------------------------------------------------------------------------
    /**
     *	Force thread to update data.
     */
    //+----------------------------------------------------------------------------
    void PingThreadData::wake_up() {
        omni_mutex_lock sync(*this);
        signal();
    }
    //+----------------------------------------------------------------------------
    //+----------------------------------------------------------------------------
    void PingThreadData::setNbInstaces(int nb) {
        omni_mutex_lock sync(*this);
        nbInstances = nb;
    }
    //+----------------------------------------------------------------------------
    //+----------------------------------------------------------------------------
    int PingThreadData::getNbInstaces() {
        omni_mutex_lock sync(*this);
        return nbInstances;
    }
    //+----------------------------------------------------------------------------
    //+----------------------------------------------------------------------------






    //+----------------------------------------------------------------------------
    /**
     *	Create a thread to ping server
     *
     *	@param	shared pointer on shared data between thread and DS.
     *	@param	name The pinged server name
     *	@param	timeout	timeout value in milliseconds for ping command.
     */
    //+----------------------------------------------------------------------------
    PingThread::PingThread(PingThreadData *sd, const string& name, CheckProcessUtil *proc_util)
	{
        //  Convert instance to lowercase
        unsigned long idx = name.find('/');
        string exeFile = name.substr(0, idx);
        string instance = name.substr(++idx);
        transform(instance.begin(), instance.end(),
                  instance.begin(), ::tolower);
        servname = exeFile +'/'+instance;

        shared = sd;
        process_util = proc_util;
	}
    //+----------------------------------------------------------------------------
    /**
     *	Execute the thread loop.
     */
    //+----------------------------------------------------------------------------
    void *PingThread::run_undetached(TANGO_UNUSED(void *ptr)) {
         TimeVal before{}, after{};
        Tango::DeviceProxy *pDevice = nullptr;
        Tango::DevState state;
        bool stop_thread = false;
        string adm_devname("dserver/");
        adm_devname += servname;
        while (!stop_thread) {
            GetTime(before);
            //  Make sure server running in one instance
            int nbInstances = process_util->getNbServerInstances(servname);
            shared->setNbInstaces(nbInstances);
            //	Check before if server running or failed
            if (process_util->is_server_running(servname)) {
                //	try to build DeviceProxy
                if (pDevice == nullptr) {
                    try {
                        pDevice = new Tango::DeviceProxy(adm_devname);
                    }
                    catch (Tango::DevFailed &e) {
                        Tango::Except::print_exception(e);
                    }
                    catch (...) {
                        cout << "============================================" << endl;
                        cout << "	Exception catch !!!!!!" << endl;
                        cout << "============================================" << endl;
                    }
                }
                if (pDevice != nullptr) {
                    try {
                        pDevice->ping();
                        state = Tango::ON;
                    }
                    catch (Tango::DevFailed &) {
                        cout << servname << " is running but not responding !!!" << endl;
                        //Tango::Except::print_exception(e);
                        state = Tango::MOVING;
                    }
                } else {
                    state = Tango::FAULT;
                }
            } else {
                state = Tango::FAULT;
            }
            shared->set_state(state);

            //	Compute time to sleep
            GetTime(after)
            double dt = (double) Elapsed(before, after);
            long time_to_sleep = 2000 - (int) dt;
            if (time_to_sleep < 10)
                time_to_sleep = 10;

            //	Check if thread must be stopped.
            stop_thread = shared->get_stop_thread();
            if (!stop_thread) {
                //	And wait for next ping
                omni_mutex_lock sync(*shared);
                shared->wait(time_to_sleep);
            }
            stop_thread = shared->get_stop_thread();
        }
        delete shared;
        delete pDevice;
        return nullptr;
    }
    //+----------------------------------------------------------------------------
    //+----------------------------------------------------------------------------
}    //	namespace
