//=============================================================================
//
// file :         DataBaseClass.h
//
// description :  Include for the DataBaseClass root class.
//                This class is represents the singleton class for
//                the DataBase device class.
//                It contains all properties and methods which the 
//                DataBase requires only once e.g. the commands.
//			
// project :      TANGO Device Server
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
// $Log$
// Revision 2.34  2010/09/21 11:43:21  taurel
// - Add GPL stuff
//
// Revision 2.33  2009/04/06 13:11:08  pascal_verdier
// DbMySqlSelect command added.
//
// Revision 2.32  2009/01/30 08:17:51  taurel
// - Added DbGetDeviceWideList command
//
// Revision 2.31  2008/03/26 17:31:00  taurel
// - Committed after merge with the Release 4 branch
//
// Revision 2.30.2.2  2008/02/28 17:07:37  taurel
// - Added the StoredProcedureRelease attribute
//
// Revision 2.30.2.1  2008/01/25 12:29:50  taurel
// - Add a new command DbDeleteAllDeviceAttributeProperty
// - The DbImportDevice now also returns the device class
// - Fix bug in the DbGetHostServerList command
//
// Revision 2.30  2007/11/06 07:24:07  taurel
// - Added the DbGetDataForServerCache command (with timing stats)
// - Add timing stats for the DbPutClassProperty command
//
// Revision 2.29  2006/09/28 11:18:20  pascal_verdier
// DbGetClassForDevice and DbGetClassInheritanceForDevice commands added.
//
// Revision 2.28  2006/06/22 15:25:32  jlpons
// Added history commands
//
// Revision 2.27  2005/10/05 07:35:54  pascal_verdier
// Bug fixed in Timing_Info attribute read.
// ResetTimingValues command added.
// Windows compatibility added for timing attributes.
//
// Revision 2.26  2005/07/25 08:49:18  andy_gotz
// replaced the timing attributes for the import device with a spectrum of timing
// stats for a list of commands.
//
// Revision 2.25  2005/07/21 20:12:28  andy_gotz
// added attributes to return timing information for the DbImportDevice command
//
// Revision 2.24  2005/02/15 10:03:31  pascal_verdier
// Bug fixed in DbGetDeviceInfo command
//
// Revision 2.23  2005/02/11 12:58:49  pascal_verdier
// Is now a TANGO 5 server.
// DbGetDeviceInfo command added.
//
// Revision 2.22  2005/01/10 10:35:25  pascal_verdier
// Updated to Tango 5.
//
// Revision 2.21  2004/10/22 10:33:55  taurel
// Added four new commands DbPutClassAttributeProperty2,
// DbGetClassAttributeProperty2, DbPutDeviceAttributeProperty2
// and DbGetDeviceAttributeProperty2. These commands support array attribute
// properties. For compatibility reason, keep the old commands.
//
// Revision 2.20  2004/06/21 12:20:12  pascal_verdier
// DbGetExportdDeviceListForClass and DbGetDeviceAlias commands added.
//
// Revision 2.19  2004/03/08 12:41:56  pascal_verdier
// Attribute alias management added.
//
// Revision 2.18  2004/01/26 13:24:57  pascal_verdier
// DbGetServerNameList and DbGetInstanceNameList commands added.
//
// Revision 2.17  2003/12/11 10:22:10  pascal_verdier
// DbGetHostServersInfo command added.
//
// Revision 2.16  2003/08/13 12:02:23  andy_gotz
// added DbPutAttributeAlias and DbGetAttributeAlias commands needed by
// the AttributeProxy object. Note this needs a new table attribute_alias
// in mysql.
//
// Revision 2.15  2003/03/20 14:55:15  andy_gotz
// added support for device aliases; fixed bug in updating server info; tagged server name onto end of import info; implemented support for importing and exporting events;
//
// Revision 2.14  2003/01/16 14:30:40  goetz
// ported Makefile to omniorb
//
// Revision 2.13  2002/11/26 10:00:58  goetz
// added delete_class_attribute_property; changed Solaris to CC; added pid to import_device
//
// Revision 2.12  2002/09/16 08:39:37  goetz
// added GetObjectList and GetPropertyList commands
//
// Revision 2.11  2002/02/04 17:09:09  goetz
// updated Windows port
//
// Revision 2.9  2001/07/12 12:15:19  goetz
// changed db_get_class_list() and db_delete_device_attribute_property()
//
// Revision 2.8  2001/07/04 05:17:03  goetz
// dserver device domain,family,member corrected; wildcards for DbGetClassList
//
// Revision 2.7  2001/07/04 04:42:25  goetz
// delete all properties before updating them
//
// Revision 2.6  2001/03/22 12:56:52  goetz
// fixed bug in DbAddDevice command, device name now unique in device table
//
// Revision 2.5  2001/03/06 12:05:44  goetz
// added DbGetDeviceExportedList; DbExportDevice updates host info in server table
//
// Revision 2.4  2001/03/06 11:01:56  goetz
// added DbGetDeviceExportedList command; DbExportDevice updates host in server table
//
//=============================================================================
//
//  		This file is generated by POGO
//	(Program Obviously used to Generate tango Object)
//
//             (c) - Pascal Verdier - ESRF
//=============================================================================

#ifndef _DATABASECLASS_H
#define _DATABASECLASS_H

#include <tango.h>
#include <DataBase.h>

namespace DataBase_ns {
//=====================================
//	Define classes for attributes
//=====================================
class Timing_infoAttrib: public Tango::SpectrumAttr
{
public:
	Timing_infoAttrib():SpectrumAttr("Timing_info", Tango::DEV_STRING, Tango::READ, 64) {};
	~Timing_infoAttrib() {};
	
	virtual void read(Tango::DeviceImpl *dev,Tango::Attribute &att)
	{(static_cast<DataBase *>(dev))->read_Timing_info(att);}
	virtual bool is_allowed(Tango::DeviceImpl *dev,Tango::AttReqType ty)
	{return (static_cast<DataBase *>(dev))->is_Timing_info_allowed(ty);}
};

class Timing_indexAttrib: public Tango::SpectrumAttr
{
public:
	Timing_indexAttrib():SpectrumAttr("Timing_index", Tango::DEV_STRING, Tango::READ, 64) {};
	~Timing_indexAttrib() {};
	
	virtual void read(Tango::DeviceImpl *dev,Tango::Attribute &att)
	{(static_cast<DataBase *>(dev))->read_Timing_index(att);}
	virtual bool is_allowed(Tango::DeviceImpl *dev,Tango::AttReqType ty)
	{return (static_cast<DataBase *>(dev))->is_Timing_index_allowed(ty);}
};

class Timing_callsAttrib: public Tango::SpectrumAttr
{
public:
	Timing_callsAttrib():SpectrumAttr("Timing_calls", Tango::DEV_DOUBLE, Tango::READ, 64) {};
	~Timing_callsAttrib() {};
	
	virtual void read(Tango::DeviceImpl *dev,Tango::Attribute &att)
	{(static_cast<DataBase *>(dev))->read_Timing_calls(att);}
	virtual bool is_allowed(Tango::DeviceImpl *dev,Tango::AttReqType ty)
	{return (static_cast<DataBase *>(dev))->is_Timing_calls_allowed(ty);}
};

class Timing_maximumAttrib: public Tango::SpectrumAttr
{
public:
	Timing_maximumAttrib():SpectrumAttr("Timing_maximum", Tango::DEV_DOUBLE, Tango::READ, 64) {};
	~Timing_maximumAttrib() {};
	
	virtual void read(Tango::DeviceImpl *dev,Tango::Attribute &att)
	{(static_cast<DataBase *>(dev))->read_Timing_maximum(att);}
	virtual bool is_allowed(Tango::DeviceImpl *dev,Tango::AttReqType ty)
	{return (static_cast<DataBase *>(dev))->is_Timing_maximum_allowed(ty);}
};

class Timing_minimumAttrib: public Tango::SpectrumAttr
{
public:
	Timing_minimumAttrib():SpectrumAttr("Timing_minimum", Tango::DEV_DOUBLE, Tango::READ, 64) {};
	~Timing_minimumAttrib() {};
	
	virtual void read(Tango::DeviceImpl *dev,Tango::Attribute &att)
	{(static_cast<DataBase *>(dev))->read_Timing_minimum(att);}
	virtual bool is_allowed(Tango::DeviceImpl *dev,Tango::AttReqType ty)
	{return (static_cast<DataBase *>(dev))->is_Timing_minimum_allowed(ty);}
};

class Timing_averageAttrib: public Tango::SpectrumAttr
{
public:
	Timing_averageAttrib():SpectrumAttr("Timing_average", Tango::DEV_DOUBLE, Tango::READ, 64) {};
	~Timing_averageAttrib() {};
	
	virtual void read(Tango::DeviceImpl *dev,Tango::Attribute &att)
	{(static_cast<DataBase *>(dev))->read_Timing_average(att);}
	virtual bool is_allowed(Tango::DeviceImpl *dev,Tango::AttReqType ty)
	{return (static_cast<DataBase *>(dev))->is_Timing_average_allowed(ty);}
};

class StoredProcedureReleaseAttrib: public Tango::Attr
{
public:
	StoredProcedureReleaseAttrib():Attr("StoredProcedureRelease", Tango::DEV_STRING, Tango::READ) {};
	~StoredProcedureReleaseAttrib() {};
	
	virtual void read(Tango::DeviceImpl *dev,Tango::Attribute &att)
	{(static_cast<DataBase *>(dev))->read_StoredProcedureRelease(att);}
	virtual bool is_allowed(Tango::DeviceImpl *dev,Tango::AttReqType ty)
	{return (static_cast<DataBase *>(dev))->is_StoredProcedureRelease_allowed(ty);}
};

//=========================================
//	Define classes for commands
//=========================================
class DbMySqlSelectCmd : public Tango::Command
{
public:
	DbMySqlSelectCmd(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out,
				   const char        *in_desc,
				   const char        *out_desc,
				   Tango::DispLevel  level)
	:Command(name,in,out,in_desc,out_desc, level)	{};

	DbMySqlSelectCmd(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out)
	:Command(name,in,out)	{};
	~DbMySqlSelectCmd() {};
	
	virtual CORBA::Any *execute (Tango::DeviceImpl *dev, const CORBA::Any &any);
	virtual bool is_allowed (Tango::DeviceImpl *dev, const CORBA::Any &any)
	{return (static_cast<DataBase *>(dev))->is_DbMySqlSelect_allowed(any);}
};



class DbDeleteAllDeviceAttributePropertyClass : public Tango::Command
{
public:
	DbDeleteAllDeviceAttributePropertyClass(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out,
				   const char        *in_desc,
				   const char        *out_desc,
				   Tango::DispLevel  level)
	:Command(name,in,out,in_desc,out_desc, level)	{};

	DbDeleteAllDeviceAttributePropertyClass(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out)
	:Command(name,in,out)	{};
	~DbDeleteAllDeviceAttributePropertyClass() {};
	
	virtual CORBA::Any *execute (Tango::DeviceImpl *dev, const CORBA::Any &any);
	virtual bool is_allowed (Tango::DeviceImpl *dev, const CORBA::Any &any)
	{return (static_cast<DataBase *>(dev))->is_DbDeleteAllDeviceAttributeProperty_allowed(any);}
};



class DbGetDataForServerCacheCmd : public Tango::Command
{
public:
	DbGetDataForServerCacheCmd(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out,
				   const char        *in_desc,
				   const char        *out_desc,
				   Tango::DispLevel  level)
	:Command(name,in,out,in_desc,out_desc, level)	{};

	DbGetDataForServerCacheCmd(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out)
	:Command(name,in,out)	{};
	~DbGetDataForServerCacheCmd() {};
	
	virtual CORBA::Any *execute (Tango::DeviceImpl *dev, const CORBA::Any &any);
	virtual bool is_allowed (Tango::DeviceImpl *dev, const CORBA::Any &any)
	{return (static_cast<DataBase *>(dev))->is_DbGetDataForServerCache_allowed(any);}
};



class ResetTimingValuesClass : public Tango::Command
{
public:
	ResetTimingValuesClass(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out,
				   const char        *in_desc,
				   const char        *out_desc,
				   Tango::DispLevel  level)
	:Command(name,in,out,in_desc,out_desc, level)	{};

	ResetTimingValuesClass(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out)
	:Command(name,in,out)	{};
	~ResetTimingValuesClass() {};
	
	virtual CORBA::Any *execute (Tango::DeviceImpl *dev, const CORBA::Any &any);
	virtual bool is_allowed (Tango::DeviceImpl *dev, const CORBA::Any &any)
	{return (static_cast<DataBase *>(dev))->is_ResetTimingValues_allowed(any);}
};



class DbUnExportServer : public Tango::Command
{
public:
	DbUnExportServer(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out,
				   const char        *in_desc,
				   const char        *out_desc,
				   Tango::DispLevel  level)
	:Command(name,in,out,in_desc,out_desc, level)	{};

	DbUnExportServer(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out)
	:Command(name,in,out)	{};
	~DbUnExportServer() {};
	
	virtual CORBA::Any *execute (Tango::DeviceImpl *dev, const CORBA::Any &any);
	virtual bool is_allowed (Tango::DeviceImpl *dev, const CORBA::Any &any)
	{return (static_cast<DataBase *>(dev))->is_DbUnExportServer_allowed(any);}
};



class DbUnExportEventCmd : public Tango::Command
{
public:
	DbUnExportEventCmd(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out,
				   const char        *in_desc,
				   const char        *out_desc,
				   Tango::DispLevel  level)
	:Command(name,in,out,in_desc,out_desc, level)	{};

	DbUnExportEventCmd(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out)
	:Command(name,in,out)	{};
	~DbUnExportEventCmd() {};
	
	virtual CORBA::Any *execute (Tango::DeviceImpl *dev, const CORBA::Any &any);
	virtual bool is_allowed (Tango::DeviceImpl *dev, const CORBA::Any &any)
	{return (static_cast<DataBase *>(dev))->is_DbUnExportEvent_allowed(any);}
};



class DbUnExportDevice : public Tango::Command
{
public:
	DbUnExportDevice(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out,
				   const char        *in_desc,
				   const char        *out_desc,
				   Tango::DispLevel  level)
	:Command(name,in,out,in_desc,out_desc, level)	{};

	DbUnExportDevice(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out)
	:Command(name,in,out)	{};
	~DbUnExportDevice() {};
	
	virtual CORBA::Any *execute (Tango::DeviceImpl *dev, const CORBA::Any &any);
	virtual bool is_allowed (Tango::DeviceImpl *dev, const CORBA::Any &any)
	{return (static_cast<DataBase *>(dev))->is_DbUnExportDevice_allowed(any);}
};



class DbPutServerInfoCmd : public Tango::Command
{
public:
	DbPutServerInfoCmd(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out,
				   const char        *in_desc,
				   const char        *out_desc,
				   Tango::DispLevel  level)
	:Command(name,in,out,in_desc,out_desc, level)	{};

	DbPutServerInfoCmd(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out)
	:Command(name,in,out)	{};
	~DbPutServerInfoCmd() {};
	
	virtual CORBA::Any *execute (Tango::DeviceImpl *dev, const CORBA::Any &any);
	virtual bool is_allowed (Tango::DeviceImpl *dev, const CORBA::Any &any)
	{return (static_cast<DataBase *>(dev))->is_DbPutServerInfo_allowed(any);}
};



class DbPutProperty : public Tango::Command
{
public:
	DbPutProperty(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out,
				   const char        *in_desc,
				   const char        *out_desc,
				   Tango::DispLevel  level)
	:Command(name,in,out,in_desc,out_desc, level)	{};

	DbPutProperty(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out)
	:Command(name,in,out)	{};
	~DbPutProperty() {};
	
	virtual CORBA::Any *execute (Tango::DeviceImpl *dev, const CORBA::Any &any);
	virtual bool is_allowed (Tango::DeviceImpl *dev, const CORBA::Any &any)
	{return (static_cast<DataBase *>(dev))->is_DbPutProperty_allowed(any);}
};



class DbPutDeviceProperty : public Tango::Command
{
public:
	DbPutDeviceProperty(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out,
				   const char        *in_desc,
				   const char        *out_desc,
				   Tango::DispLevel  level)
	:Command(name,in,out,in_desc,out_desc, level)	{};

	DbPutDeviceProperty(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out)
	:Command(name,in,out)	{};
	~DbPutDeviceProperty() {};
	
	virtual CORBA::Any *execute (Tango::DeviceImpl *dev, const CORBA::Any &any);
	virtual bool is_allowed (Tango::DeviceImpl *dev, const CORBA::Any &any)
	{return (static_cast<DataBase *>(dev))->is_DbPutDeviceProperty_allowed(any);}
};



class DbPutDeviceAttributeProperty2Cmd : public Tango::Command
{
public:
	DbPutDeviceAttributeProperty2Cmd(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out,
				   const char        *in_desc,
				   const char        *out_desc,
				   Tango::DispLevel  level)
	:Command(name,in,out,in_desc,out_desc, level)	{};

	DbPutDeviceAttributeProperty2Cmd(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out)
	:Command(name,in,out)	{};
	~DbPutDeviceAttributeProperty2Cmd() {};
	
	virtual CORBA::Any *execute (Tango::DeviceImpl *dev, const CORBA::Any &any);
	virtual bool is_allowed (Tango::DeviceImpl *dev, const CORBA::Any &any)
	{return (static_cast<DataBase *>(dev))->is_DbPutDeviceAttributeProperty2_allowed(any);}
};



class DbPutDeviceAttributeProperty : public Tango::Command
{
public:
	DbPutDeviceAttributeProperty(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out,
				   const char        *in_desc,
				   const char        *out_desc,
				   Tango::DispLevel  level)
	:Command(name,in,out,in_desc,out_desc, level)	{};

	DbPutDeviceAttributeProperty(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out)
	:Command(name,in,out)	{};
	~DbPutDeviceAttributeProperty() {};
	
	virtual CORBA::Any *execute (Tango::DeviceImpl *dev, const CORBA::Any &any);
	virtual bool is_allowed (Tango::DeviceImpl *dev, const CORBA::Any &any)
	{return (static_cast<DataBase *>(dev))->is_DbPutDeviceAttributeProperty_allowed(any);}
};



class DbPutDeviceAliasCmd : public Tango::Command
{
public:
	DbPutDeviceAliasCmd(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out,
				   const char        *in_desc,
				   const char        *out_desc,
				   Tango::DispLevel  level)
	:Command(name,in,out,in_desc,out_desc, level)	{};

	DbPutDeviceAliasCmd(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out)
	:Command(name,in,out)	{};
	~DbPutDeviceAliasCmd() {};
	
	virtual CORBA::Any *execute (Tango::DeviceImpl *dev, const CORBA::Any &any);
	virtual bool is_allowed (Tango::DeviceImpl *dev, const CORBA::Any &any)
	{return (static_cast<DataBase *>(dev))->is_DbPutDeviceAlias_allowed(any);}
};



class DbPutClassProperty : public Tango::Command
{
public:
	DbPutClassProperty(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out,
				   const char        *in_desc,
				   const char        *out_desc,
				   Tango::DispLevel  level)
	:Command(name,in,out,in_desc,out_desc, level)	{};

	DbPutClassProperty(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out)
	:Command(name,in,out)	{};
	~DbPutClassProperty() {};
	
	virtual CORBA::Any *execute (Tango::DeviceImpl *dev, const CORBA::Any &any);
	virtual bool is_allowed (Tango::DeviceImpl *dev, const CORBA::Any &any)
	{return (static_cast<DataBase *>(dev))->is_DbPutClassProperty_allowed(any);}
};



class DbPutClassAttributeProperty2Cmd : public Tango::Command
{
public:
	DbPutClassAttributeProperty2Cmd(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out,
				   const char        *in_desc,
				   const char        *out_desc,
				   Tango::DispLevel  level)
	:Command(name,in,out,in_desc,out_desc, level)	{};

	DbPutClassAttributeProperty2Cmd(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out)
	:Command(name,in,out)	{};
	~DbPutClassAttributeProperty2Cmd() {};
	
	virtual CORBA::Any *execute (Tango::DeviceImpl *dev, const CORBA::Any &any);
	virtual bool is_allowed (Tango::DeviceImpl *dev, const CORBA::Any &any)
	{return (static_cast<DataBase *>(dev))->is_DbPutClassAttributeProperty2_allowed(any);}
};



class DbPutClassAttributeProperty : public Tango::Command
{
public:
	DbPutClassAttributeProperty(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out,
				   const char        *in_desc,
				   const char        *out_desc,
				   Tango::DispLevel  level)
	:Command(name,in,out,in_desc,out_desc, level)	{};

	DbPutClassAttributeProperty(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out)
	:Command(name,in,out)	{};
	~DbPutClassAttributeProperty() {};
	
	virtual CORBA::Any *execute (Tango::DeviceImpl *dev, const CORBA::Any &any);
	virtual bool is_allowed (Tango::DeviceImpl *dev, const CORBA::Any &any)
	{return (static_cast<DataBase *>(dev))->is_DbPutClassAttributeProperty_allowed(any);}
};



class DbPutAttributeAliasCmd : public Tango::Command
{
public:
	DbPutAttributeAliasCmd(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out,
				   const char        *in_desc,
				   const char        *out_desc,
				   Tango::DispLevel  level)
	:Command(name,in,out,in_desc,out_desc, level)	{};

	DbPutAttributeAliasCmd(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out)
	:Command(name,in,out)	{};
	~DbPutAttributeAliasCmd() {};
	
	virtual CORBA::Any *execute (Tango::DeviceImpl *dev, const CORBA::Any &any);
	virtual bool is_allowed (Tango::DeviceImpl *dev, const CORBA::Any &any)
	{return (static_cast<DataBase *>(dev))->is_DbPutAttributeAlias_allowed(any);}
};



class DbInfo : public Tango::Command
{
public:
	DbInfo(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out,
				   const char        *in_desc,
				   const char        *out_desc,
				   Tango::DispLevel  level)
	:Command(name,in,out,in_desc,out_desc, level)	{};

	DbInfo(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out)
	:Command(name,in,out)	{};
	~DbInfo() {};
	
	virtual CORBA::Any *execute (Tango::DeviceImpl *dev, const CORBA::Any &any);
	virtual bool is_allowed (Tango::DeviceImpl *dev, const CORBA::Any &any)
	{return (static_cast<DataBase *>(dev))->is_DbInfo_allowed(any);}
};



class DbImportEventCmd : public Tango::Command
{
public:
	DbImportEventCmd(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out,
				   const char        *in_desc,
				   const char        *out_desc,
				   Tango::DispLevel  level)
	:Command(name,in,out,in_desc,out_desc, level)	{};

	DbImportEventCmd(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out)
	:Command(name,in,out)	{};
	~DbImportEventCmd() {};
	
	virtual CORBA::Any *execute (Tango::DeviceImpl *dev, const CORBA::Any &any);
	virtual bool is_allowed (Tango::DeviceImpl *dev, const CORBA::Any &any)
	{return (static_cast<DataBase *>(dev))->is_DbImportEvent_allowed(any);}
};



class DbImportDevice : public Tango::Command
{
public:
	DbImportDevice(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out,
				   const char        *in_desc,
				   const char        *out_desc,
				   Tango::DispLevel  level)
	:Command(name,in,out,in_desc,out_desc, level)	{};

	DbImportDevice(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out)
	:Command(name,in,out)	{};
	~DbImportDevice() {};
	
	virtual CORBA::Any *execute (Tango::DeviceImpl *dev, const CORBA::Any &any);
	virtual bool is_allowed (Tango::DeviceImpl *dev, const CORBA::Any &any)
	{return (static_cast<DataBase *>(dev))->is_DbImportDevice_allowed(any);}
};



class DbGetServerNameListCmd : public Tango::Command
{
public:
	DbGetServerNameListCmd(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out,
				   const char        *in_desc,
				   const char        *out_desc,
				   Tango::DispLevel  level)
	:Command(name,in,out,in_desc,out_desc, level)	{};

	DbGetServerNameListCmd(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out)
	:Command(name,in,out)	{};
	~DbGetServerNameListCmd() {};
	
	virtual CORBA::Any *execute (Tango::DeviceImpl *dev, const CORBA::Any &any);
	virtual bool is_allowed (Tango::DeviceImpl *dev, const CORBA::Any &any)
	{return (static_cast<DataBase *>(dev))->is_DbGetServerNameList_allowed(any);}
};



class DbGetServerList : public Tango::Command
{
public:
	DbGetServerList(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out,
				   const char        *in_desc,
				   const char        *out_desc,
				   Tango::DispLevel  level)
	:Command(name,in,out,in_desc,out_desc, level)	{};

	DbGetServerList(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out)
	:Command(name,in,out)	{};
	~DbGetServerList() {};
	
	virtual CORBA::Any *execute (Tango::DeviceImpl *dev, const CORBA::Any &any);
	virtual bool is_allowed (Tango::DeviceImpl *dev, const CORBA::Any &any)
	{return (static_cast<DataBase *>(dev))->is_DbGetServerList_allowed(any);}
};



class DbGetServerInfoCmd : public Tango::Command
{
public:
	DbGetServerInfoCmd(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out,
				   const char        *in_desc,
				   const char        *out_desc,
				   Tango::DispLevel  level)
	:Command(name,in,out,in_desc,out_desc, level)	{};

	DbGetServerInfoCmd(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out)
	:Command(name,in,out)	{};
	~DbGetServerInfoCmd() {};
	
	virtual CORBA::Any *execute (Tango::DeviceImpl *dev, const CORBA::Any &any);
	virtual bool is_allowed (Tango::DeviceImpl *dev, const CORBA::Any &any)
	{return (static_cast<DataBase *>(dev))->is_DbGetServerInfo_allowed(any);}
};



class DbGetPropertyList : public Tango::Command
{
public:
	DbGetPropertyList(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out,
				   const char        *in_desc,
				   const char        *out_desc,
				   Tango::DispLevel  level)
	:Command(name,in,out,in_desc,out_desc, level)	{};

	DbGetPropertyList(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out)
	:Command(name,in,out)	{};
	~DbGetPropertyList() {};
	
	virtual CORBA::Any *execute (Tango::DeviceImpl *dev, const CORBA::Any &any);
	virtual bool is_allowed (Tango::DeviceImpl *dev, const CORBA::Any &any)
	{return (static_cast<DataBase *>(dev))->is_DbGetPropertyList_allowed(any);}
};



class DbGetPropertyHistClass : public Tango::Command
{
public:
	DbGetPropertyHistClass(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out,
				   const char        *in_desc,
				   const char        *out_desc,
				   Tango::DispLevel  level)
	:Command(name,in,out,in_desc,out_desc, level)	{};

	DbGetPropertyHistClass(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out)
	:Command(name,in,out)	{};
	~DbGetPropertyHistClass() {};
	
	virtual CORBA::Any *execute (Tango::DeviceImpl *dev, const CORBA::Any &any);
	virtual bool is_allowed (Tango::DeviceImpl *dev, const CORBA::Any &any)
	{return (static_cast<DataBase *>(dev))->is_DbGetPropertyHist_allowed(any);}
};



class DbGetProperty : public Tango::Command
{
public:
	DbGetProperty(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out,
				   const char        *in_desc,
				   const char        *out_desc,
				   Tango::DispLevel  level)
	:Command(name,in,out,in_desc,out_desc, level)	{};

	DbGetProperty(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out)
	:Command(name,in,out)	{};
	~DbGetProperty() {};
	
	virtual CORBA::Any *execute (Tango::DeviceImpl *dev, const CORBA::Any &any);
	virtual bool is_allowed (Tango::DeviceImpl *dev, const CORBA::Any &any)
	{return (static_cast<DataBase *>(dev))->is_DbGetProperty_allowed(any);}
};



class DbGetObjectListCmd : public Tango::Command
{
public:
	DbGetObjectListCmd(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out,
				   const char        *in_desc,
				   const char        *out_desc,
				   Tango::DispLevel  level)
	:Command(name,in,out,in_desc,out_desc, level)	{};

	DbGetObjectListCmd(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out)
	:Command(name,in,out)	{};
	~DbGetObjectListCmd() {};
	
	virtual CORBA::Any *execute (Tango::DeviceImpl *dev, const CORBA::Any &any);
	virtual bool is_allowed (Tango::DeviceImpl *dev, const CORBA::Any &any)
	{return (static_cast<DataBase *>(dev))->is_DbGetObjectList_allowed(any);}
};



class DbGetInstanceNameListCmd : public Tango::Command
{
public:
	DbGetInstanceNameListCmd(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out,
				   const char        *in_desc,
				   const char        *out_desc,
				   Tango::DispLevel  level)
	:Command(name,in,out,in_desc,out_desc, level)	{};

	DbGetInstanceNameListCmd(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out)
	:Command(name,in,out)	{};
	~DbGetInstanceNameListCmd() {};
	
	virtual CORBA::Any *execute (Tango::DeviceImpl *dev, const CORBA::Any &any);
	virtual bool is_allowed (Tango::DeviceImpl *dev, const CORBA::Any &any)
	{return (static_cast<DataBase *>(dev))->is_DbGetInstanceNameList_allowed(any);}
};



class DbGetHostServersInfoCmd : public Tango::Command
{
public:
	DbGetHostServersInfoCmd(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out,
				   const char        *in_desc,
				   const char        *out_desc,
				   Tango::DispLevel  level)
	:Command(name,in,out,in_desc,out_desc, level)	{};

	DbGetHostServersInfoCmd(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out)
	:Command(name,in,out)	{};
	~DbGetHostServersInfoCmd() {};
	
	virtual CORBA::Any *execute (Tango::DeviceImpl *dev, const CORBA::Any &any);
	virtual bool is_allowed (Tango::DeviceImpl *dev, const CORBA::Any &any)
	{return (static_cast<DataBase *>(dev))->is_DbGetHostServersInfo_allowed(any);}
};



class DbGetHostServerList : public Tango::Command
{
public:
	DbGetHostServerList(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out,
				   const char        *in_desc,
				   const char        *out_desc,
				   Tango::DispLevel  level)
	:Command(name,in,out,in_desc,out_desc, level)	{};

	DbGetHostServerList(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out)
	:Command(name,in,out)	{};
	~DbGetHostServerList() {};
	
	virtual CORBA::Any *execute (Tango::DeviceImpl *dev, const CORBA::Any &any);
	virtual bool is_allowed (Tango::DeviceImpl *dev, const CORBA::Any &any)
	{return (static_cast<DataBase *>(dev))->is_DbGetHostServerList_allowed(any);}
};



class DbGetHostList : public Tango::Command
{
public:
	DbGetHostList(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out,
				   const char        *in_desc,
				   const char        *out_desc,
				   Tango::DispLevel  level)
	:Command(name,in,out,in_desc,out_desc, level)	{};

	DbGetHostList(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out)
	:Command(name,in,out)	{};
	~DbGetHostList() {};
	
	virtual CORBA::Any *execute (Tango::DeviceImpl *dev, const CORBA::Any &any);
	virtual bool is_allowed (Tango::DeviceImpl *dev, const CORBA::Any &any)
	{return (static_cast<DataBase *>(dev))->is_DbGetHostList_allowed(any);}
};



class DbGetExportdDeviceListForClassCmd : public Tango::Command
{
public:
	DbGetExportdDeviceListForClassCmd(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out,
				   const char        *in_desc,
				   const char        *out_desc,
				   Tango::DispLevel  level)
	:Command(name,in,out,in_desc,out_desc, level)	{};

	DbGetExportdDeviceListForClassCmd(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out)
	:Command(name,in,out)	{};
	~DbGetExportdDeviceListForClassCmd() {};
	
	virtual CORBA::Any *execute (Tango::DeviceImpl *dev, const CORBA::Any &any);
	virtual bool is_allowed (Tango::DeviceImpl *dev, const CORBA::Any &any)
	{return (static_cast<DataBase *>(dev))->is_DbGetExportdDeviceListForClass_allowed(any);}
};



class DbGetDeviceServerClassListCmd : public Tango::Command
{
public:
	DbGetDeviceServerClassListCmd(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out,
				   const char        *in_desc,
				   const char        *out_desc,
				   Tango::DispLevel  level)
	:Command(name,in,out,in_desc,out_desc, level)	{};

	DbGetDeviceServerClassListCmd(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out)
	:Command(name,in,out)	{};
	~DbGetDeviceServerClassListCmd() {};
	
	virtual CORBA::Any *execute (Tango::DeviceImpl *dev, const CORBA::Any &any);
	virtual bool is_allowed (Tango::DeviceImpl *dev, const CORBA::Any &any)
	{return (static_cast<DataBase *>(dev))->is_DbGetDeviceServerClassList_allowed(any);}
};



class DbGetDevicePropertyList : public Tango::Command
{
public:
	DbGetDevicePropertyList(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out,
				   const char        *in_desc,
				   const char        *out_desc,
				   Tango::DispLevel  level)
	:Command(name,in,out,in_desc,out_desc, level)	{};

	DbGetDevicePropertyList(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out)
	:Command(name,in,out)	{};
	~DbGetDevicePropertyList() {};
	
	virtual CORBA::Any *execute (Tango::DeviceImpl *dev, const CORBA::Any &any);
	virtual bool is_allowed (Tango::DeviceImpl *dev, const CORBA::Any &any)
	{return (static_cast<DataBase *>(dev))->is_DbGetDevicePropertyList_allowed(any);}
};



class DbGetDevicePropertyHistClass : public Tango::Command
{
public:
	DbGetDevicePropertyHistClass(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out,
				   const char        *in_desc,
				   const char        *out_desc,
				   Tango::DispLevel  level)
	:Command(name,in,out,in_desc,out_desc, level)	{};

	DbGetDevicePropertyHistClass(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out)
	:Command(name,in,out)	{};
	~DbGetDevicePropertyHistClass() {};
	
	virtual CORBA::Any *execute (Tango::DeviceImpl *dev, const CORBA::Any &any);
	virtual bool is_allowed (Tango::DeviceImpl *dev, const CORBA::Any &any)
	{return (static_cast<DataBase *>(dev))->is_DbGetDevicePropertyHist_allowed(any);}
};



class DbGetDeviceProperty : public Tango::Command
{
public:
	DbGetDeviceProperty(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out,
				   const char        *in_desc,
				   const char        *out_desc,
				   Tango::DispLevel  level)
	:Command(name,in,out,in_desc,out_desc, level)	{};

	DbGetDeviceProperty(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out)
	:Command(name,in,out)	{};
	~DbGetDeviceProperty() {};
	
	virtual CORBA::Any *execute (Tango::DeviceImpl *dev, const CORBA::Any &any);
	virtual bool is_allowed (Tango::DeviceImpl *dev, const CORBA::Any &any)
	{return (static_cast<DataBase *>(dev))->is_DbGetDeviceProperty_allowed(any);}
};



class DbGetDeviceMemberList : public Tango::Command
{
public:
	DbGetDeviceMemberList(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out,
				   const char        *in_desc,
				   const char        *out_desc,
				   Tango::DispLevel  level)
	:Command(name,in,out,in_desc,out_desc, level)	{};

	DbGetDeviceMemberList(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out)
	:Command(name,in,out)	{};
	~DbGetDeviceMemberList() {};
	
	virtual CORBA::Any *execute (Tango::DeviceImpl *dev, const CORBA::Any &any);
	virtual bool is_allowed (Tango::DeviceImpl *dev, const CORBA::Any &any)
	{return (static_cast<DataBase *>(dev))->is_DbGetDeviceMemberList_allowed(any);}
};



class DbGetDeviceWideListCmd : public Tango::Command
{
public:
	DbGetDeviceWideListCmd(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out,
				   const char        *in_desc,
				   const char        *out_desc,
				   Tango::DispLevel  level)
	:Command(name,in,out,in_desc,out_desc, level)	{};

	DbGetDeviceWideListCmd(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out)
	:Command(name,in,out)	{};
	~DbGetDeviceWideListCmd() {};
	
	virtual CORBA::Any *execute (Tango::DeviceImpl *dev, const CORBA::Any &any);
	virtual bool is_allowed (Tango::DeviceImpl *dev, const CORBA::Any &any)
	{return (static_cast<DataBase *>(dev))->is_DbGetDeviceWideList_allowed(any);}
};



class DbGetDeviceListCmd : public Tango::Command
{
public:
	DbGetDeviceListCmd(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out,
				   const char        *in_desc,
				   const char        *out_desc,
				   Tango::DispLevel  level)
	:Command(name,in,out,in_desc,out_desc, level)	{};

	DbGetDeviceListCmd(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out)
	:Command(name,in,out)	{};
	~DbGetDeviceListCmd() {};
	
	virtual CORBA::Any *execute (Tango::DeviceImpl *dev, const CORBA::Any &any);
	virtual bool is_allowed (Tango::DeviceImpl *dev, const CORBA::Any &any)
	{return (static_cast<DataBase *>(dev))->is_DbGetDeviceList_allowed(any);}
};



class DbGetDeviceInfoClass : public Tango::Command
{
public:
	DbGetDeviceInfoClass(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out,
				   const char        *in_desc,
				   const char        *out_desc,
				   Tango::DispLevel  level)
	:Command(name,in,out,in_desc,out_desc, level)	{};

	DbGetDeviceInfoClass(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out)
	:Command(name,in,out)	{};
	~DbGetDeviceInfoClass() {};
	
	virtual CORBA::Any *execute (Tango::DeviceImpl *dev, const CORBA::Any &any);
	virtual bool is_allowed (Tango::DeviceImpl *dev, const CORBA::Any &any)
	{return (static_cast<DataBase *>(dev))->is_DbGetDeviceInfo_allowed(any);}
};



class DbGetDeviceFamilyList : public Tango::Command
{
public:
	DbGetDeviceFamilyList(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out,
				   const char        *in_desc,
				   const char        *out_desc,
				   Tango::DispLevel  level)
	:Command(name,in,out,in_desc,out_desc, level)	{};

	DbGetDeviceFamilyList(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out)
	:Command(name,in,out)	{};
	~DbGetDeviceFamilyList() {};
	
	virtual CORBA::Any *execute (Tango::DeviceImpl *dev, const CORBA::Any &any);
	virtual bool is_allowed (Tango::DeviceImpl *dev, const CORBA::Any &any)
	{return (static_cast<DataBase *>(dev))->is_DbGetDeviceFamilyList_allowed(any);}
};



class DbGetDeviceExportedListCmd : public Tango::Command
{
public:
	DbGetDeviceExportedListCmd(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out,
				   const char        *in_desc,
				   const char        *out_desc,
				   Tango::DispLevel  level)
	:Command(name,in,out,in_desc,out_desc, level)	{};

	DbGetDeviceExportedListCmd(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out)
	:Command(name,in,out)	{};
	~DbGetDeviceExportedListCmd() {};
	
	virtual CORBA::Any *execute (Tango::DeviceImpl *dev, const CORBA::Any &any);
	virtual bool is_allowed (Tango::DeviceImpl *dev, const CORBA::Any &any)
	{return (static_cast<DataBase *>(dev))->is_DbGetDeviceExportedList_allowed(any);}
};



class DbGetDeviceDomainList : public Tango::Command
{
public:
	DbGetDeviceDomainList(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out,
				   const char        *in_desc,
				   const char        *out_desc,
				   Tango::DispLevel  level)
	:Command(name,in,out,in_desc,out_desc, level)	{};

	DbGetDeviceDomainList(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out)
	:Command(name,in,out)	{};
	~DbGetDeviceDomainList() {};
	
	virtual CORBA::Any *execute (Tango::DeviceImpl *dev, const CORBA::Any &any);
	virtual bool is_allowed (Tango::DeviceImpl *dev, const CORBA::Any &any)
	{return (static_cast<DataBase *>(dev))->is_DbGetDeviceDomainList_allowed(any);}
};



class DbGetDeviceClassList : public Tango::Command
{
public:
	DbGetDeviceClassList(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out,
				   const char        *in_desc,
				   const char        *out_desc,
				   Tango::DispLevel  level)
	:Command(name,in,out,in_desc,out_desc, level)	{};

	DbGetDeviceClassList(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out)
	:Command(name,in,out)	{};
	~DbGetDeviceClassList() {};
	
	virtual CORBA::Any *execute (Tango::DeviceImpl *dev, const CORBA::Any &any);
	virtual bool is_allowed (Tango::DeviceImpl *dev, const CORBA::Any &any)
	{return (static_cast<DataBase *>(dev))->is_DbGetDeviceClassList_allowed(any);}
};



class DbGetDeviceAttributePropertyHistCmd : public Tango::Command
{
public:
	DbGetDeviceAttributePropertyHistCmd(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out,
				   const char        *in_desc,
				   const char        *out_desc,
				   Tango::DispLevel  level)
	:Command(name,in,out,in_desc,out_desc, level)	{};

	DbGetDeviceAttributePropertyHistCmd(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out)
	:Command(name,in,out)	{};
	~DbGetDeviceAttributePropertyHistCmd() {};
	
	virtual CORBA::Any *execute (Tango::DeviceImpl *dev, const CORBA::Any &any);
	virtual bool is_allowed (Tango::DeviceImpl *dev, const CORBA::Any &any)
	{return (static_cast<DataBase *>(dev))->is_DbGetDeviceAttributePropertyHist_allowed(any);}
};



class DbGetDeviceAttributeProperty2Cmd : public Tango::Command
{
public:
	DbGetDeviceAttributeProperty2Cmd(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out,
				   const char        *in_desc,
				   const char        *out_desc,
				   Tango::DispLevel  level)
	:Command(name,in,out,in_desc,out_desc, level)	{};

	DbGetDeviceAttributeProperty2Cmd(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out)
	:Command(name,in,out)	{};
	~DbGetDeviceAttributeProperty2Cmd() {};
	
	virtual CORBA::Any *execute (Tango::DeviceImpl *dev, const CORBA::Any &any);
	virtual bool is_allowed (Tango::DeviceImpl *dev, const CORBA::Any &any)
	{return (static_cast<DataBase *>(dev))->is_DbGetDeviceAttributeProperty2_allowed(any);}
};



class DbGetDeviceAttributeProperty : public Tango::Command
{
public:
	DbGetDeviceAttributeProperty(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out,
				   const char        *in_desc,
				   const char        *out_desc,
				   Tango::DispLevel  level)
	:Command(name,in,out,in_desc,out_desc, level)	{};

	DbGetDeviceAttributeProperty(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out)
	:Command(name,in,out)	{};
	~DbGetDeviceAttributeProperty() {};
	
	virtual CORBA::Any *execute (Tango::DeviceImpl *dev, const CORBA::Any &any);
	virtual bool is_allowed (Tango::DeviceImpl *dev, const CORBA::Any &any)
	{return (static_cast<DataBase *>(dev))->is_DbGetDeviceAttributeProperty_allowed(any);}
};



class DbGetDeviceAttributeList : public Tango::Command
{
public:
	DbGetDeviceAttributeList(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out,
				   const char        *in_desc,
				   const char        *out_desc,
				   Tango::DispLevel  level)
	:Command(name,in,out,in_desc,out_desc, level)	{};

	DbGetDeviceAttributeList(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out)
	:Command(name,in,out)	{};
	~DbGetDeviceAttributeList() {};
	
	virtual CORBA::Any *execute (Tango::DeviceImpl *dev, const CORBA::Any &any);
	virtual bool is_allowed (Tango::DeviceImpl *dev, const CORBA::Any &any)
	{return (static_cast<DataBase *>(dev))->is_DbGetDeviceAttributeList_allowed(any);}
};



class DbGetDeviceAliasList : public Tango::Command
{
public:
	DbGetDeviceAliasList(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out,
				   const char        *in_desc,
				   const char        *out_desc,
				   Tango::DispLevel  level)
	:Command(name,in,out,in_desc,out_desc, level)	{};

	DbGetDeviceAliasList(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out)
	:Command(name,in,out)	{};
	~DbGetDeviceAliasList() {};
	
	virtual CORBA::Any *execute (Tango::DeviceImpl *dev, const CORBA::Any &any);
	virtual bool is_allowed (Tango::DeviceImpl *dev, const CORBA::Any &any)
	{return (static_cast<DataBase *>(dev))->is_DbGetDeviceAliasList_allowed(any);}
};



class DbGetDeviceAliasCmd : public Tango::Command
{
public:
	DbGetDeviceAliasCmd(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out,
				   const char        *in_desc,
				   const char        *out_desc,
				   Tango::DispLevel  level)
	:Command(name,in,out,in_desc,out_desc, level)	{};

	DbGetDeviceAliasCmd(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out)
	:Command(name,in,out)	{};
	~DbGetDeviceAliasCmd() {};
	
	virtual CORBA::Any *execute (Tango::DeviceImpl *dev, const CORBA::Any &any);
	virtual bool is_allowed (Tango::DeviceImpl *dev, const CORBA::Any &any)
	{return (static_cast<DataBase *>(dev))->is_DbGetDeviceAlias_allowed(any);}
};



class DbGetClassPropertyList : public Tango::Command
{
public:
	DbGetClassPropertyList(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out,
				   const char        *in_desc,
				   const char        *out_desc,
				   Tango::DispLevel  level)
	:Command(name,in,out,in_desc,out_desc, level)	{};

	DbGetClassPropertyList(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out)
	:Command(name,in,out)	{};
	~DbGetClassPropertyList() {};
	
	virtual CORBA::Any *execute (Tango::DeviceImpl *dev, const CORBA::Any &any);
	virtual bool is_allowed (Tango::DeviceImpl *dev, const CORBA::Any &any)
	{return (static_cast<DataBase *>(dev))->is_DbGetClassPropertyList_allowed(any);}
};



class DbGetClassPropertyHistClass : public Tango::Command
{
public:
	DbGetClassPropertyHistClass(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out,
				   const char        *in_desc,
				   const char        *out_desc,
				   Tango::DispLevel  level)
	:Command(name,in,out,in_desc,out_desc, level)	{};

	DbGetClassPropertyHistClass(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out)
	:Command(name,in,out)	{};
	~DbGetClassPropertyHistClass() {};
	
	virtual CORBA::Any *execute (Tango::DeviceImpl *dev, const CORBA::Any &any);
	virtual bool is_allowed (Tango::DeviceImpl *dev, const CORBA::Any &any)
	{return (static_cast<DataBase *>(dev))->is_DbGetClassPropertyHist_allowed(any);}
};



class DbGetClassProperty : public Tango::Command
{
public:
	DbGetClassProperty(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out,
				   const char        *in_desc,
				   const char        *out_desc,
				   Tango::DispLevel  level)
	:Command(name,in,out,in_desc,out_desc, level)	{};

	DbGetClassProperty(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out)
	:Command(name,in,out)	{};
	~DbGetClassProperty() {};
	
	virtual CORBA::Any *execute (Tango::DeviceImpl *dev, const CORBA::Any &any);
	virtual bool is_allowed (Tango::DeviceImpl *dev, const CORBA::Any &any)
	{return (static_cast<DataBase *>(dev))->is_DbGetClassProperty_allowed(any);}
};



class DbGetClassList : public Tango::Command
{
public:
	DbGetClassList(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out,
				   const char        *in_desc,
				   const char        *out_desc,
				   Tango::DispLevel  level)
	:Command(name,in,out,in_desc,out_desc, level)	{};

	DbGetClassList(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out)
	:Command(name,in,out)	{};
	~DbGetClassList() {};
	
	virtual CORBA::Any *execute (Tango::DeviceImpl *dev, const CORBA::Any &any);
	virtual bool is_allowed (Tango::DeviceImpl *dev, const CORBA::Any &any)
	{return (static_cast<DataBase *>(dev))->is_DbGetClassList_allowed(any);}
};



class DbGetClassInheritanceForDeviceClass : public Tango::Command
{
public:
	DbGetClassInheritanceForDeviceClass(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out,
				   const char        *in_desc,
				   const char        *out_desc,
				   Tango::DispLevel  level)
	:Command(name,in,out,in_desc,out_desc, level)	{};

	DbGetClassInheritanceForDeviceClass(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out)
	:Command(name,in,out)	{};
	~DbGetClassInheritanceForDeviceClass() {};
	
	virtual CORBA::Any *execute (Tango::DeviceImpl *dev, const CORBA::Any &any);
	virtual bool is_allowed (Tango::DeviceImpl *dev, const CORBA::Any &any)
	{return (static_cast<DataBase *>(dev))->is_DbGetClassInheritanceForDevice_allowed(any);}
};



class DbGetClassForDeviceClass : public Tango::Command
{
public:
	DbGetClassForDeviceClass(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out,
				   const char        *in_desc,
				   const char        *out_desc,
				   Tango::DispLevel  level)
	:Command(name,in,out,in_desc,out_desc, level)	{};

	DbGetClassForDeviceClass(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out)
	:Command(name,in,out)	{};
	~DbGetClassForDeviceClass() {};
	
	virtual CORBA::Any *execute (Tango::DeviceImpl *dev, const CORBA::Any &any);
	virtual bool is_allowed (Tango::DeviceImpl *dev, const CORBA::Any &any)
	{return (static_cast<DataBase *>(dev))->is_DbGetClassForDevice_allowed(any);}
};



class DbGetClassAttributePropertyHistCmd : public Tango::Command
{
public:
	DbGetClassAttributePropertyHistCmd(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out,
				   const char        *in_desc,
				   const char        *out_desc,
				   Tango::DispLevel  level)
	:Command(name,in,out,in_desc,out_desc, level)	{};

	DbGetClassAttributePropertyHistCmd(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out)
	:Command(name,in,out)	{};
	~DbGetClassAttributePropertyHistCmd() {};
	
	virtual CORBA::Any *execute (Tango::DeviceImpl *dev, const CORBA::Any &any);
	virtual bool is_allowed (Tango::DeviceImpl *dev, const CORBA::Any &any)
	{return (static_cast<DataBase *>(dev))->is_DbGetClassAttributePropertyHist_allowed(any);}
};



class DbGetClassAttributeProperty2Cmd : public Tango::Command
{
public:
	DbGetClassAttributeProperty2Cmd(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out,
				   const char        *in_desc,
				   const char        *out_desc,
				   Tango::DispLevel  level)
	:Command(name,in,out,in_desc,out_desc, level)	{};

	DbGetClassAttributeProperty2Cmd(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out)
	:Command(name,in,out)	{};
	~DbGetClassAttributeProperty2Cmd() {};
	
	virtual CORBA::Any *execute (Tango::DeviceImpl *dev, const CORBA::Any &any);
	virtual bool is_allowed (Tango::DeviceImpl *dev, const CORBA::Any &any)
	{return (static_cast<DataBase *>(dev))->is_DbGetClassAttributeProperty2_allowed(any);}
};



class DbGetClassAttributeProperty : public Tango::Command
{
public:
	DbGetClassAttributeProperty(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out,
				   const char        *in_desc,
				   const char        *out_desc,
				   Tango::DispLevel  level)
	:Command(name,in,out,in_desc,out_desc, level)	{};

	DbGetClassAttributeProperty(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out)
	:Command(name,in,out)	{};
	~DbGetClassAttributeProperty() {};
	
	virtual CORBA::Any *execute (Tango::DeviceImpl *dev, const CORBA::Any &any);
	virtual bool is_allowed (Tango::DeviceImpl *dev, const CORBA::Any &any)
	{return (static_cast<DataBase *>(dev))->is_DbGetClassAttributeProperty_allowed(any);}
};



class DbGetClassAttributeList : public Tango::Command
{
public:
	DbGetClassAttributeList(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out,
				   const char        *in_desc,
				   const char        *out_desc,
				   Tango::DispLevel  level)
	:Command(name,in,out,in_desc,out_desc, level)	{};

	DbGetClassAttributeList(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out)
	:Command(name,in,out)	{};
	~DbGetClassAttributeList() {};
	
	virtual CORBA::Any *execute (Tango::DeviceImpl *dev, const CORBA::Any &any);
	virtual bool is_allowed (Tango::DeviceImpl *dev, const CORBA::Any &any)
	{return (static_cast<DataBase *>(dev))->is_DbGetClassAttributeList_allowed(any);}
};



class DbGetAttributeAliasList : public Tango::Command
{
public:
	DbGetAttributeAliasList(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out,
				   const char        *in_desc,
				   const char        *out_desc,
				   Tango::DispLevel  level)
	:Command(name,in,out,in_desc,out_desc, level)	{};

	DbGetAttributeAliasList(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out)
	:Command(name,in,out)	{};
	~DbGetAttributeAliasList() {};
	
	virtual CORBA::Any *execute (Tango::DeviceImpl *dev, const CORBA::Any &any);
	virtual bool is_allowed (Tango::DeviceImpl *dev, const CORBA::Any &any)
	{return (static_cast<DataBase *>(dev))->is_DbGetAttributeAliasList_allowed(any);}
};



class DbGetAttributeAliasCmd : public Tango::Command
{
public:
	DbGetAttributeAliasCmd(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out,
				   const char        *in_desc,
				   const char        *out_desc,
				   Tango::DispLevel  level)
	:Command(name,in,out,in_desc,out_desc, level)	{};

	DbGetAttributeAliasCmd(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out)
	:Command(name,in,out)	{};
	~DbGetAttributeAliasCmd() {};
	
	virtual CORBA::Any *execute (Tango::DeviceImpl *dev, const CORBA::Any &any);
	virtual bool is_allowed (Tango::DeviceImpl *dev, const CORBA::Any &any)
	{return (static_cast<DataBase *>(dev))->is_DbGetAttributeAlias_allowed(any);}
};



class DbGetAliasDeviceCmd : public Tango::Command
{
public:
	DbGetAliasDeviceCmd(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out,
				   const char        *in_desc,
				   const char        *out_desc,
				   Tango::DispLevel  level)
	:Command(name,in,out,in_desc,out_desc, level)	{};

	DbGetAliasDeviceCmd(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out)
	:Command(name,in,out)	{};
	~DbGetAliasDeviceCmd() {};
	
	virtual CORBA::Any *execute (Tango::DeviceImpl *dev, const CORBA::Any &any);
	virtual bool is_allowed (Tango::DeviceImpl *dev, const CORBA::Any &any)
	{return (static_cast<DataBase *>(dev))->is_DbGetAliasDevice_allowed(any);}
};



class DbExportEventCmd : public Tango::Command
{
public:
	DbExportEventCmd(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out,
				   const char        *in_desc,
				   const char        *out_desc,
				   Tango::DispLevel  level)
	:Command(name,in,out,in_desc,out_desc, level)	{};

	DbExportEventCmd(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out)
	:Command(name,in,out)	{};
	~DbExportEventCmd() {};
	
	virtual CORBA::Any *execute (Tango::DeviceImpl *dev, const CORBA::Any &any);
	virtual bool is_allowed (Tango::DeviceImpl *dev, const CORBA::Any &any)
	{return (static_cast<DataBase *>(dev))->is_DbExportEvent_allowed(any);}
};



class DbExportDevice : public Tango::Command
{
public:
	DbExportDevice(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out,
				   const char        *in_desc,
				   const char        *out_desc,
				   Tango::DispLevel  level)
	:Command(name,in,out,in_desc,out_desc, level)	{};

	DbExportDevice(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out)
	:Command(name,in,out)	{};
	~DbExportDevice() {};
	
	virtual CORBA::Any *execute (Tango::DeviceImpl *dev, const CORBA::Any &any);
	virtual bool is_allowed (Tango::DeviceImpl *dev, const CORBA::Any &any)
	{return (static_cast<DataBase *>(dev))->is_DbExportDevice_allowed(any);}
};



class DbDeleteServerInfoCmd : public Tango::Command
{
public:
	DbDeleteServerInfoCmd(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out,
				   const char        *in_desc,
				   const char        *out_desc,
				   Tango::DispLevel  level)
	:Command(name,in,out,in_desc,out_desc, level)	{};

	DbDeleteServerInfoCmd(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out)
	:Command(name,in,out)	{};
	~DbDeleteServerInfoCmd() {};
	
	virtual CORBA::Any *execute (Tango::DeviceImpl *dev, const CORBA::Any &any);
	virtual bool is_allowed (Tango::DeviceImpl *dev, const CORBA::Any &any)
	{return (static_cast<DataBase *>(dev))->is_DbDeleteServerInfo_allowed(any);}
};



class DbDeleteServerCmd : public Tango::Command
{
public:
	DbDeleteServerCmd(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out,
				   const char        *in_desc,
				   const char        *out_desc,
				   Tango::DispLevel  level)
	:Command(name,in,out,in_desc,out_desc, level)	{};

	DbDeleteServerCmd(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out)
	:Command(name,in,out)	{};
	~DbDeleteServerCmd() {};
	
	virtual CORBA::Any *execute (Tango::DeviceImpl *dev, const CORBA::Any &any);
	virtual bool is_allowed (Tango::DeviceImpl *dev, const CORBA::Any &any)
	{return (static_cast<DataBase *>(dev))->is_DbDeleteServer_allowed(any);}
};



class DbDeleteProperty : public Tango::Command
{
public:
	DbDeleteProperty(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out,
				   const char        *in_desc,
				   const char        *out_desc,
				   Tango::DispLevel  level)
	:Command(name,in,out,in_desc,out_desc, level)	{};

	DbDeleteProperty(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out)
	:Command(name,in,out)	{};
	~DbDeleteProperty() {};
	
	virtual CORBA::Any *execute (Tango::DeviceImpl *dev, const CORBA::Any &any);
	virtual bool is_allowed (Tango::DeviceImpl *dev, const CORBA::Any &any)
	{return (static_cast<DataBase *>(dev))->is_DbDeleteProperty_allowed(any);}
};



class DbDeleteDeviceProperty : public Tango::Command
{
public:
	DbDeleteDeviceProperty(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out,
				   const char        *in_desc,
				   const char        *out_desc,
				   Tango::DispLevel  level)
	:Command(name,in,out,in_desc,out_desc, level)	{};

	DbDeleteDeviceProperty(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out)
	:Command(name,in,out)	{};
	~DbDeleteDeviceProperty() {};
	
	virtual CORBA::Any *execute (Tango::DeviceImpl *dev, const CORBA::Any &any);
	virtual bool is_allowed (Tango::DeviceImpl *dev, const CORBA::Any &any)
	{return (static_cast<DataBase *>(dev))->is_DbDeleteDeviceProperty_allowed(any);}
};



class DbDeleteDeviceAttributePropertyCmd : public Tango::Command
{
public:
	DbDeleteDeviceAttributePropertyCmd(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out,
				   const char        *in_desc,
				   const char        *out_desc,
				   Tango::DispLevel  level)
	:Command(name,in,out,in_desc,out_desc, level)	{};

	DbDeleteDeviceAttributePropertyCmd(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out)
	:Command(name,in,out)	{};
	~DbDeleteDeviceAttributePropertyCmd() {};
	
	virtual CORBA::Any *execute (Tango::DeviceImpl *dev, const CORBA::Any &any);
	virtual bool is_allowed (Tango::DeviceImpl *dev, const CORBA::Any &any)
	{return (static_cast<DataBase *>(dev))->is_DbDeleteDeviceAttributeProperty_allowed(any);}
};



class DbDeleteDeviceAttributeCmd : public Tango::Command
{
public:
	DbDeleteDeviceAttributeCmd(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out,
				   const char        *in_desc,
				   const char        *out_desc,
				   Tango::DispLevel  level)
	:Command(name,in,out,in_desc,out_desc, level)	{};

	DbDeleteDeviceAttributeCmd(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out)
	:Command(name,in,out)	{};
	~DbDeleteDeviceAttributeCmd() {};
	
	virtual CORBA::Any *execute (Tango::DeviceImpl *dev, const CORBA::Any &any);
	virtual bool is_allowed (Tango::DeviceImpl *dev, const CORBA::Any &any)
	{return (static_cast<DataBase *>(dev))->is_DbDeleteDeviceAttribute_allowed(any);}
};



class DbDeleteDeviceAliasCmd : public Tango::Command
{
public:
	DbDeleteDeviceAliasCmd(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out,
				   const char        *in_desc,
				   const char        *out_desc,
				   Tango::DispLevel  level)
	:Command(name,in,out,in_desc,out_desc, level)	{};

	DbDeleteDeviceAliasCmd(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out)
	:Command(name,in,out)	{};
	~DbDeleteDeviceAliasCmd() {};
	
	virtual CORBA::Any *execute (Tango::DeviceImpl *dev, const CORBA::Any &any);
	virtual bool is_allowed (Tango::DeviceImpl *dev, const CORBA::Any &any)
	{return (static_cast<DataBase *>(dev))->is_DbDeleteDeviceAlias_allowed(any);}
};



class DbDeleteDeviceCmd : public Tango::Command
{
public:
	DbDeleteDeviceCmd(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out,
				   const char        *in_desc,
				   const char        *out_desc,
				   Tango::DispLevel  level)
	:Command(name,in,out,in_desc,out_desc, level)	{};

	DbDeleteDeviceCmd(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out)
	:Command(name,in,out)	{};
	~DbDeleteDeviceCmd() {};
	
	virtual CORBA::Any *execute (Tango::DeviceImpl *dev, const CORBA::Any &any);
	virtual bool is_allowed (Tango::DeviceImpl *dev, const CORBA::Any &any)
	{return (static_cast<DataBase *>(dev))->is_DbDeleteDevice_allowed(any);}
};



class DbDeleteClassProperty : public Tango::Command
{
public:
	DbDeleteClassProperty(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out,
				   const char        *in_desc,
				   const char        *out_desc,
				   Tango::DispLevel  level)
	:Command(name,in,out,in_desc,out_desc, level)	{};

	DbDeleteClassProperty(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out)
	:Command(name,in,out)	{};
	~DbDeleteClassProperty() {};
	
	virtual CORBA::Any *execute (Tango::DeviceImpl *dev, const CORBA::Any &any);
	virtual bool is_allowed (Tango::DeviceImpl *dev, const CORBA::Any &any)
	{return (static_cast<DataBase *>(dev))->is_DbDeleteClassProperty_allowed(any);}
};



class DbDeleteClassAttributePropertyCmd : public Tango::Command
{
public:
	DbDeleteClassAttributePropertyCmd(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out,
				   const char        *in_desc,
				   const char        *out_desc,
				   Tango::DispLevel  level)
	:Command(name,in,out,in_desc,out_desc, level)	{};

	DbDeleteClassAttributePropertyCmd(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out)
	:Command(name,in,out)	{};
	~DbDeleteClassAttributePropertyCmd() {};
	
	virtual CORBA::Any *execute (Tango::DeviceImpl *dev, const CORBA::Any &any);
	virtual bool is_allowed (Tango::DeviceImpl *dev, const CORBA::Any &any)
	{return (static_cast<DataBase *>(dev))->is_DbDeleteClassAttributeProperty_allowed(any);}
};



class DbDeleteClassAttributeCmd : public Tango::Command
{
public:
	DbDeleteClassAttributeCmd(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out,
				   const char        *in_desc,
				   const char        *out_desc,
				   Tango::DispLevel  level)
	:Command(name,in,out,in_desc,out_desc, level)	{};

	DbDeleteClassAttributeCmd(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out)
	:Command(name,in,out)	{};
	~DbDeleteClassAttributeCmd() {};
	
	virtual CORBA::Any *execute (Tango::DeviceImpl *dev, const CORBA::Any &any);
	virtual bool is_allowed (Tango::DeviceImpl *dev, const CORBA::Any &any)
	{return (static_cast<DataBase *>(dev))->is_DbDeleteClassAttribute_allowed(any);}
};



class DbDeleteAttributeAliasCmd : public Tango::Command
{
public:
	DbDeleteAttributeAliasCmd(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out,
				   const char        *in_desc,
				   const char        *out_desc,
				   Tango::DispLevel  level)
	:Command(name,in,out,in_desc,out_desc, level)	{};

	DbDeleteAttributeAliasCmd(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out)
	:Command(name,in,out)	{};
	~DbDeleteAttributeAliasCmd() {};
	
	virtual CORBA::Any *execute (Tango::DeviceImpl *dev, const CORBA::Any &any);
	virtual bool is_allowed (Tango::DeviceImpl *dev, const CORBA::Any &any)
	{return (static_cast<DataBase *>(dev))->is_DbDeleteAttributeAlias_allowed(any);}
};



class DbAddServer : public Tango::Command
{
public:
	DbAddServer(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out,
				   const char        *in_desc,
				   const char        *out_desc,
				   Tango::DispLevel  level)
	:Command(name,in,out,in_desc,out_desc, level)	{};

	DbAddServer(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out)
	:Command(name,in,out)	{};
	~DbAddServer() {};
	
	virtual CORBA::Any *execute (Tango::DeviceImpl *dev, const CORBA::Any &any);
	virtual bool is_allowed (Tango::DeviceImpl *dev, const CORBA::Any &any)
	{return (static_cast<DataBase *>(dev))->is_DbAddServer_allowed(any);}
};



class DbAddDevice : public Tango::Command
{
public:
	DbAddDevice(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out,
				   const char        *in_desc,
				   const char        *out_desc,
				   Tango::DispLevel  level)
	:Command(name,in,out,in_desc,out_desc, level)	{};

	DbAddDevice(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out)
	:Command(name,in,out)	{};
	~DbAddDevice() {};
	
	virtual CORBA::Any *execute (Tango::DeviceImpl *dev, const CORBA::Any &any);
	virtual bool is_allowed (Tango::DeviceImpl *dev, const CORBA::Any &any)
	{return (static_cast<DataBase *>(dev))->is_DbAddDevice_allowed(any);}
};



//
// The DataBaseClass singleton definition
//

class
#ifdef WIN32
	__declspec(dllexport)
#endif
	DataBaseClass : public Tango::DeviceClass
{
public:
//	properties member data

//	add your own data members here
//------------------------------------

public:
	Tango::DbData	cl_prop;
	Tango::DbData	cl_def_prop;
	Tango::DbData	dev_def_prop;

//	Method prototypes
	static DataBaseClass *init(const char *);
	static DataBaseClass *instance();
	~DataBaseClass();
	Tango::DbDatum	get_class_property(string &);
	Tango::DbDatum	get_default_device_property(string &);
	Tango::DbDatum	get_default_class_property(string &);
	
protected:
	DataBaseClass(string &);
	static DataBaseClass *_instance;
	void command_factory();
	void get_class_property();
	void attribute_factory(vector<Tango::Attr *> &);
	void write_class_property();
	void set_default_property();
	string get_cvstag();
	string get_cvsroot();

private:
	void device_factory(const Tango::DevVarStringArray *);
};


}	//	namespace DataBase_ns

#endif // _DATABASECLASS_H
