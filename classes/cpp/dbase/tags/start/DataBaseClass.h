//=============================================================================
//
// file :               DataBaseClass.h
//
// description :        Include file for the DataBaseClass root class. 
//			This class is represents the singleton class for 
//			the Database Device class. It contains all properties 
//			and methods which the Database requires only once e.g. 
//			the commands. The first version of the database
//			device server uses MySql as the underlying database.
//
// project :            TANGO
//
// author(s) :          A.Gotz + E.Taurel
//
// $Version$
//
// $Log$
// Revision 1.4  2000/02/15 17:51:50  goetz
// testing 1-2-3
//
// Revision 1.1.1.1  2000/02/15 11:20:40  goetz
// Imported sources
//
// Revision 1.1.1.1  1999/12/22 14:49:44  goetz
// Imported sources
//
// Revision 1.4  1999/11/26 10:27:24  goetz
// added DeviceProperty, AttributeProperty and ClassProperty
//
// Revision 1.3  1999/11/05 15:40:46  goetz
// ported to latest version of device pattern, resource now property
//
// Revision 1.2  1999/08/03 14:47:00  goetz
// added DbGetResource and DbPutResource commands and DEBUG cpp flag
//
// Revision 1.1  1999/08/02 11:32:14  goetz
// Initial revision
//
//
// copyleft :           European Synchrotron Radiation Facility
//                      BP 220, Grenoble 38043
//                      FRANCE
//
//=============================================================================

#ifndef _DATABASECLASS_H
#define _DATABASECLASS_H

#include <tango_ds.h>

class DbInfo : public Command
{
public:
	DbInfo(const char *, Tango_CmdArgType, Tango_CmdArgType);
	
	~DbInfo() {};
	
	virtual bool is_allowed (DeviceImpl *, const CORBA_Any &);
	virtual CORBA_Any *execute (DeviceImpl *, const CORBA_Any &);
};

class DbGetClassList : public Command
{
public:
	DbGetClassList(const char *, Tango_CmdArgType, Tango_CmdArgType);
	
	~DbGetClassList() {};
	
	virtual bool is_allowed (DeviceImpl *, const CORBA_Any &);
	virtual CORBA_Any *execute (DeviceImpl *, const CORBA_Any &);
};

class DbGetDeviceList : public Command
{
public:
	DbGetDeviceList(const char *, Tango_CmdArgType, Tango_CmdArgType);
	
	~DbGetDeviceList() {};
	
	virtual bool is_allowed (DeviceImpl *, const CORBA_Any &);
	virtual CORBA_Any *execute (DeviceImpl *, const CORBA_Any &);
};

class DbGetDeviceClassList : public Command
{
public:
	DbGetDeviceClassList(const char *, Tango_CmdArgType, Tango_CmdArgType);
	
	~DbGetDeviceClassList() {};
	
	virtual bool is_allowed (DeviceImpl *, const CORBA_Any &);
	virtual CORBA_Any *execute (DeviceImpl *, const CORBA_Any &);
};

class DbGetDeviceProperty : public Command
{
public:
	DbGetDeviceProperty(const char *, Tango_CmdArgType, Tango_CmdArgType);
	
	~DbGetDeviceProperty() {};
	
	virtual bool is_allowed (DeviceImpl *, const CORBA_Any &);
	virtual CORBA_Any *execute (DeviceImpl *, const CORBA_Any &);
};

class DbPutDeviceProperty : public Command
{
public:
	DbPutDeviceProperty(const char *, Tango_CmdArgType, Tango_CmdArgType);
	
	~DbPutDeviceProperty() {};
	
	virtual bool is_allowed (DeviceImpl *, const CORBA_Any &);
	virtual CORBA_Any *execute (DeviceImpl *, const CORBA_Any &);
};

class DbGetAttributeProperty : public Command
{
public:
	DbGetAttributeProperty(const char *, Tango_CmdArgType, Tango_CmdArgType);
	
	~DbGetAttributeProperty() {};
	
	virtual bool is_allowed (DeviceImpl *, const CORBA_Any &);
	virtual CORBA_Any *execute (DeviceImpl *, const CORBA_Any &);
};

class DbPutAttributeProperty : public Command
{
public:
	DbPutAttributeProperty(const char *, Tango_CmdArgType, Tango_CmdArgType);
	
	~DbPutAttributeProperty() {};
	
	virtual bool is_allowed (DeviceImpl *, const CORBA_Any &);
	virtual CORBA_Any *execute (DeviceImpl *, const CORBA_Any &);
};

class DbGetClassProperty : public Command
{
public:
	DbGetClassProperty(const char *, Tango_CmdArgType, Tango_CmdArgType);
	
	~DbGetClassProperty() {};
	
	virtual bool is_allowed (DeviceImpl *, const CORBA_Any &);
	virtual CORBA_Any *execute (DeviceImpl *, const CORBA_Any &);
};

class DbPutClassProperty : public Command
{
public:
	DbPutClassProperty(const char *, Tango_CmdArgType, Tango_CmdArgType);
	
	~DbPutClassProperty() {};
	
	virtual bool is_allowed (DeviceImpl *, const CORBA_Any &);
	virtual CORBA_Any *execute (DeviceImpl *, const CORBA_Any &);
};

class DbExportDevice : public Command
{
public:
	DbExportDevice(const char *, Tango_CmdArgType, Tango_CmdArgType);
	
	~DbExportDevice() {};
	
	virtual bool is_allowed (DeviceImpl *, const CORBA_Any &);
	virtual CORBA_Any *execute (DeviceImpl *, const CORBA_Any &);
};

class DbUnExportDevice : public Command
{
public:
	DbUnExportDevice(const char *, Tango_CmdArgType, Tango_CmdArgType);
	
	~DbUnExportDevice() {};
	
	virtual bool is_allowed (DeviceImpl *, const CORBA_Any &);
	virtual CORBA_Any *execute (DeviceImpl *, const CORBA_Any &);
};

class DbUnExportServer : public Command
{
public:
	DbUnExportServer(const char *, Tango_CmdArgType, Tango_CmdArgType);
	
	~DbUnExportServer() {};
	
	virtual bool is_allowed (DeviceImpl *, const CORBA_Any &);
	virtual CORBA_Any *execute (DeviceImpl *, const CORBA_Any &);
};

class DbImportDevice : public Command
{
public:
	DbImportDevice(const char *, Tango_CmdArgType, Tango_CmdArgType);
	
	~DbImportDevice() {};
	
	virtual bool is_allowed (DeviceImpl *, const CORBA_Any &);
	virtual CORBA_Any *execute (DeviceImpl *, const CORBA_Any &);
};

class DbAddServer : public Command
{
public:
	DbAddServer(const char *, Tango_CmdArgType, Tango_CmdArgType);
	
	~DbAddServer() {};
	
	virtual bool is_allowed (DeviceImpl *, const CORBA_Any &);
	virtual CORBA_Any *execute (DeviceImpl *, const CORBA_Any &);
};

class DbAddDevice : public Command
{
public:
	DbAddDevice(const char *, Tango_CmdArgType, Tango_CmdArgType);
	
	~DbAddDevice() {};
	
	virtual bool is_allowed (DeviceImpl *, const CORBA_Any &);
	virtual CORBA_Any *execute (DeviceImpl *, const CORBA_Any &);
};

class DbDeleteServer : public Command
{
public:
	DbDeleteServer(const char *, Tango_CmdArgType, Tango_CmdArgType);
	
	~DbDeleteServer() {};
	
	virtual bool is_allowed (DeviceImpl *, const CORBA_Any &);
	virtual CORBA_Any *execute (DeviceImpl *, const CORBA_Any &);
};

class DbDeleteDevice : public Command
{
public:
	DbDeleteDevice(const char *, Tango_CmdArgType, Tango_CmdArgType);
	
	~DbDeleteDevice() {};
	
	virtual bool is_allowed (DeviceImpl *, const CORBA_Any &);
	virtual CORBA_Any *execute (DeviceImpl *, const CORBA_Any &);
};

class DataBaseClass : public DeviceClass
{
public:
	static DataBaseClass *instance();
	~DataBaseClass() {};
	
protected:
	DataBaseClass(string &);
	static DataBaseClass *_instance;	

private:
	void command_factory();
	void device_factory(Tango_DevVarStringArray *);
};

#endif // _DATABASECLASS_H
