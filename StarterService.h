//=============================================================================
//
// file :         StarterService.h
//
// description :  Include for the StarterService.cpp
//                It is used on Windows to put the starter as windows service.
//			
// project :      TANGO Device Server
//
// $Author$
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
// $Log$
//
//=============================================================================


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
