
static char *Version="Version=$Version$";
//+=============================================================================
//
// file :               DataBaseClass.cpp
//
// description :        C++ source for the DataBaseClass. A singleton
//			class derived from DeviceClass. It implements the
//			command list and all properties and methods required
//			by the DataBase objects once per process.
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
// Revision 1.4  1999/11/26 10:27:08  goetz
// added DeviceProperty, AttributeProperty and ClassProperty
//
// Revision 1.3  1999/11/05 15:34:43  goetz
// ported to latest version of device pattern, resource now property
//
// Revision 1.2  1999/08/03 14:47:00  goetz
// added DbGetProperty and DbPutProperty commands and DEBUG cpp flag
//
// Revision 1.1  1999/08/02 11:32:14  goetz
// Initial revision
//
//
// copyleft :           European Synchrotron Radiation Facility
//                      BP 220, Grenoble 38043
//                      FRANCE
//
//-=============================================================================

#include <tango_ds.h>
#include <DataBase.h>
#include <DataBaseClass.h>

//+----------------------------------------------------------------------------
//
// method : 		DbInfo::DbInfo(string &s, Tango_CmdArgType, Tango_CmdArgType)
// 
// description : 	constructor for the DbInfo command of the 
//			DataBase.
//
//-----------------------------------------------------------------------------

DbInfo::DbInfo(const char *s, Tango_CmdArgType tin, Tango_CmdArgType tout):Command(s,tin,tout)
{
}

//+----------------------------------------------------------------------------
//
// method : 		DbInfo::is_allowed(string &s)
// 
// description : 	method to test whether command is allowed or not in this
//			state. 
//
// returns :		boolean - true == is allowed , false == not allowed
//
//-----------------------------------------------------------------------------

bool DbInfo::is_allowed(DeviceImpl *device, const CORBA_Any &in_any)
{

// command always allowed

	return(true);
}

//+----------------------------------------------------------------------------
//
// method : 		DbInfo::execute(string &s)
// 
// description : 	method to trigger the execution of "DbInfo" 
//			which will return a list of database infos
//
//-----------------------------------------------------------------------------

CORBA_Any *DbInfo::execute(DeviceImpl *device,const CORBA_Any &in_any)
{	
	Tango_DevVarStringArray *info_list;

// call DataBase method which implements this command

	info_list = ((DataBase*)device)->Info();

// return data to the caller

	CORBA_Any *out_any;
	try
	{	
		out_any = new CORBA_Any();
	}
	catch (bad_alloc)
	{
		throw_exception((const char*)"Can't allocate memory in server",
				(const char*)"DbInfo::execute",
				Tango_DevErr_MemoryAllocation,Tango_APILIB,Tango_ERROR);
	}
	(*out_any) <<= info_list;

	return(out_any);
}
//+----------------------------------------------------------------------------
//
// method : 		DbGetClassList::DbGetClassList(string &s, Tango_CmdArgType, Tango_CmdArgType)
// 
// description : 	constructor for the DbGetClassList command of the 
//			DataBase.
//
//-----------------------------------------------------------------------------

DbGetClassList::DbGetClassList(const char *s, Tango_CmdArgType tin, Tango_CmdArgType tout):Command(s,tin,tout)
{
}

//+----------------------------------------------------------------------------
//
// method : 		DbGetClassList::is_allowed(string &s)
// 
// description : 	method to test whether command is allowed or not in this
//			state. 
//
// returns :		boolean - true == is allowed , false == not allowed
//
//-----------------------------------------------------------------------------

bool DbGetClassList::is_allowed(DeviceImpl *device, const CORBA_Any &in_any)
{

// command always allowed

	return(true);
}

//+----------------------------------------------------------------------------
//
// method : 		DbGetClassList::execute(string &s)
// 
// description : 	method to trigger the execution of "DbGetClassList" 
//			which will return the list of classes this server has
//			to serve
//
//-----------------------------------------------------------------------------

CORBA_Any *DbGetClassList::execute(DeviceImpl *device,const CORBA_Any &in_any)
{	
	Tango_DevString server;
	Tango_DevVarStringArray *class_list;

// call DataBase method which implements this command

	if ((in_any >>= server) == false)
	{
		throw_exception((const char*)"Incompatible command argument type, expected type is : string",
				(const char*)"DbGetClassList::execute",
				Tango_DevErr_IncompatibleCmdArgumentType,
				Tango_APILIB,Tango_ERROR);
	}

	class_list = ((DataBase*)device)->GetClassList(server);

// return data to the caller

	CORBA_Any *out_any;
	try
	{	
		out_any = new CORBA_Any();
	}
	catch (bad_alloc)
	{
		throw_exception((const char*)"Can't allocate memory in server",
				(const char*)"DbGetClassList::execute",
				Tango_DevErr_MemoryAllocation,Tango_APILIB,Tango_ERROR);
	}
	(*out_any) <<= class_list;

	return(out_any);
}

//+----------------------------------------------------------------------------
//
// method : 		DbGetDeviceList::DbGetDeviceList(string &s)
// 
// description : 	constructor for the DbGetDeviceList command of the 
//			DataBase.
//
//-----------------------------------------------------------------------------

DbGetDeviceList::DbGetDeviceList(const char *s, Tango_CmdArgType tin, Tango_CmdArgType tout):Command(s,tin,tout)
{
}

//+----------------------------------------------------------------------------
//
// method : 		DbGetDeviceList::is_allowed(string &s)
// 
// description : 	method to test whether command is allowed or not in this
//			state. 
//
// returns :		boolean - true == is allowed , false == not allowed
//
//-----------------------------------------------------------------------------

bool DbGetDeviceList::is_allowed(DeviceImpl *device, const CORBA_Any &in_any)
{

// command always allowed

	return(true);
}

//+----------------------------------------------------------------------------
//
// method : 		DbGetDeviceList::execute(string &s)
// 
// description : 	method to trigger the execution of "DbGetDeviceList" 
//			which will return a list of all the devices of the specified
//			class to be served by this server.
//
//-----------------------------------------------------------------------------

CORBA_Any *DbGetDeviceList::execute(DeviceImpl *device,const CORBA_Any &in_any)
{	
	Tango_DevVarStringArray *server_class, *device_list;

// call DataBase method which implements this command

	if ((in_any >>= server_class) == false)
	{
		throw_exception((const char*)"Incompatible command argument type, expected type is : TangoDevVarStringArray",
				(const char*)"DbGetDeviceList::execute",
				Tango_DevErr_IncompatibleCmdArgumentType,
				Tango_APILIB,Tango_ERROR);
	}

	device_list = ((DataBase*)device)->GetDeviceList(server_class);

// return data to the caller

	CORBA_Any *out_any;
	try
	{	
		out_any = new CORBA_Any();
	}
	catch (bad_alloc)
	{
		throw_exception((const char*)"Can't allocate memory in server",
				(const char*)"DbGetDeviceList::execute",
				Tango_DevErr_MemoryAllocation,Tango_APILIB,Tango_ERROR);
	}
	(*out_any) <<= device_list;

	return(out_any);
}

//+----------------------------------------------------------------------------
//
// method : 		DbGetDeviceClassList::DbGetDeviceClassList(string &s)
// 
// description : 	constructor for the DbGetDeviceClassList command of the 
//			DataBase.
//
//-----------------------------------------------------------------------------

DbGetDeviceClassList::DbGetDeviceClassList(const char *s, Tango_CmdArgType tin, Tango_CmdArgType tout):Command(s,tin,tout)  
{
}

//+----------------------------------------------------------------------------
//
// method : 		DbGetDeviceClassList::is_allowed(string &s)
// 
// description : 	method to test whether command is allowed or not in this
//			state. 
//
// returns :		boolean - true == is allowed , false == not allowed
//
//-----------------------------------------------------------------------------

bool DbGetDeviceClassList::is_allowed(DeviceImpl *device, const CORBA_Any &in_any)
{

// command always allowed

	return(true);
}

//+----------------------------------------------------------------------------
//
// method : 		DbGetDeviceClassList::execute(string &s)
// 
// description : 	method to trigger the execution of the "DbGetDeviceClassList" 
//			which will return a list of all the devices and their
//			classes to be served by this server.
//			command
//
//-----------------------------------------------------------------------------

CORBA_Any *DbGetDeviceClassList::execute(DeviceImpl *device,const CORBA_Any &in_any)
{	
	Tango_DevString server;
	Tango_DevVarStringArray *device_class_list;

// call DataBase method which implements this command

	if ((in_any >>= server) == false)
	{
		throw_exception((const char*)"Incompatible command argument type, expected type is : char*",
				(const char*)"DbGetDeviceClassList::execute",
				Tango_DevErr_IncompatibleCmdArgumentType,
				Tango_APILIB,Tango_ERROR);
	}

	device_class_list = ((DataBase*)device)->GetDeviceClassList(server);

// return data to the caller

	CORBA_Any *out_any;
	try
	{	
		out_any = new CORBA_Any();
	}
	catch (bad_alloc)
	{
		throw_exception((const char*)"Can't allocate memory in server",
				(const char*)"DbGetDeviceClassList::execute",
				Tango_DevErr_MemoryAllocation,Tango_APILIB);
	}
	(*out_any) <<= device_class_list;

	return(out_any);
}

//+----------------------------------------------------------------------------
//
// method : 		DbExportDevice::DbExportDevice(string &s)
// 
// description : 	constructor for the DbExportDevice command of the 
//			DataBase.
//
//-----------------------------------------------------------------------------

DbExportDevice::DbExportDevice(const char *s, Tango_CmdArgType tin, Tango_CmdArgType tout):Command(s,tin,tout)  
{
}

//+----------------------------------------------------------------------------
//
// method : 		DbExportDevice::is_allowed(string &s)
// 
// description : 	method to test whether command is allowed or not in this
//			state. 
//
// returns :		boolean - true == is allowed , false == not allowed
//
//-----------------------------------------------------------------------------

bool DbExportDevice::is_allowed(DeviceImpl *device, const CORBA_Any &in_any)
{

// command always allowed

	return(true);
}

//+----------------------------------------------------------------------------
//
// method : 		DbExportDevice::execute(string &s)
// 
// description : 	method to trigger the execution of the "DbExportDevice" 
//			command. This command will update the information
//			in the TANGO database concerning this device
//
//-----------------------------------------------------------------------------

CORBA_Any *DbExportDevice::execute(DeviceImpl *device,const CORBA_Any &in_any)
{	
	Tango_DevVarStringArray *export_info;

// call DataBase method which implements this command

	if ((in_any >>= export_info) == false)
	{
		throw_exception((const char*)"Incompatible command argument type, expected type is : Tango_DevVarStringArray",
				(const char*)"DbExportDevice::execute",
				Tango_DevErr_IncompatibleCmdArgumentType,
				Tango_APILIB,Tango_ERROR);
	}

        ((DataBase*)device)->ExportDevice(export_info); 

// return data to the caller

	CORBA_Any *out_any;
	try
	{	
		out_any = new CORBA_Any();
	}
	catch (bad_alloc)
	{
		throw_exception((const char*)"Can't allocate memory in server",
				(const char*)"DbExportDevice::execute",
				Tango_DevErr_MemoryAllocation,Tango_APILIB,Tango_ERROR);
	}
	return(out_any);
}

//+----------------------------------------------------------------------------
//
// method : 		DbImportDevice::DbImportDevice(string &s)
// 
// description : 	constructor for the DbImportDevice command of the 
//			DataBase.
//
//-----------------------------------------------------------------------------

DbImportDevice::DbImportDevice(const char *s, Tango_CmdArgType tin, Tango_CmdArgType tout):Command(s,tin,tout)
{
}

//+----------------------------------------------------------------------------
//
// method : 		DbImportDevice::is_allowed(string &s)
// 
// description : 	method to test whether command is allowed or not in this
//			state. 
//
// returns :		boolean - true == is allowed , false == not allowed
//
//-----------------------------------------------------------------------------

bool DbImportDevice::is_allowed(DeviceImpl *device, const CORBA_Any &in_any)
{

// command always allowed

	return(true);
}

//+----------------------------------------------------------------------------
//
// method : 		DbImportDevice::execute(string &s)
// 
// description : 	method to trigger the execution of "DbImportDevice" 
//			which will return the latest information from
//			the database for this device so that it can
//			be imported.
//
//-----------------------------------------------------------------------------

CORBA_Any *DbImportDevice::execute(DeviceImpl *device,const CORBA_Any &in_any)
{	
	Tango_DevString device_name;
	Tango_DevVarLongStringArray *import_info;

// call DataBase method which implements this command

	if ((in_any >>= device_name) == false)
	{
		throw_exception((const char*)"Incompatible command argument type, expected type is : char*",
				(const char*)"DbImportDevice::execute",
				Tango_DevErr_IncompatibleCmdArgumentType,
				Tango_APILIB,Tango_ERROR);
	}

        import_info = ((DataBase*)device)->ImportDevice(device_name); 

// return data to the caller

	CORBA_Any *out_any;
	try
	{	
		out_any = new CORBA_Any();
	}
	catch (bad_alloc)
	{
		throw_exception((const char*)"Can't allocate memory in server",
				(const char*)"DbImportDevice::execute",
				Tango_DevErr_MemoryAllocation,Tango_APILIB,Tango_ERROR);
	}
	(*out_any) <<= import_info;	
	return(out_any);
}

//+----------------------------------------------------------------------------
//
// method : 		DbUnExportDevice::DbUnExportDevice(string &s)
// 
// description : 	constructor for the DbUnExportDevice command of the 
//			DataBase.
//
//-----------------------------------------------------------------------------

DbUnExportDevice::DbUnExportDevice(const char *s, Tango_CmdArgType tin, Tango_CmdArgType tout):Command(s,tin,tout)
{
}

//+----------------------------------------------------------------------------
//
// method : 		DbUnExportDevice::is_allowed(string &s)
// 
// description : 	method to test whether command is allowed or not in this
//			state. 
//
// returns :		boolean - true == is allowed , false == not allowed
//
//-----------------------------------------------------------------------------

bool DbUnExportDevice::is_allowed(DeviceImpl *device, const CORBA_Any &in_any)
{

// command always allowed

	return(true);
}

//+----------------------------------------------------------------------------
//
// method : 		DbUnExportDevice::execute(string &s)
// 
// description : 	method to trigger the execution of "DbUnExportDevice" 
//			which will mark the device as unexported in the
//			TANGO database
//
//-----------------------------------------------------------------------------

CORBA_Any *DbUnExportDevice::execute(DeviceImpl *device,const CORBA_Any &in_any)
{	
	Tango_DevString device_name;

// call DataBase method which implements this command

	if ((in_any >>= device_name) == false)
	{
		throw_exception((const char*)"Incompatible command argument type, expected type is : char*",
				(const char*)"DbUnExportDevice::execute",
				Tango_DevErr_IncompatibleCmdArgumentType,
				Tango_APILIB,Tango_ERROR);
	}

        ((DataBase*)device)->UnExportDevice(device_name); 

// return data to the caller

	CORBA_Any *out_any;
	try
	{	
		out_any = new CORBA_Any();
	}
	catch (bad_alloc)
	{
		throw_exception((const char*)"Can't allocate memory in server",
				(const char*)"DbUnExportDevice::execute",
				Tango_DevErr_MemoryAllocation,Tango_APILIB,Tango_ERROR);
	}
	return(out_any);
}

//+----------------------------------------------------------------------------
//
// method : 		DbUnExportServer::DbUnExportServer(string &s)
// 
// description : 	constructor for the DbUnExportServer command of the 
//			DataBase.
//
//-----------------------------------------------------------------------------

DbUnExportServer::DbUnExportServer(const char *s, Tango_CmdArgType tin, Tango_CmdArgType tout):Command(s,tin,tout)
{
}

//+----------------------------------------------------------------------------
//
// method : 		DbUnExportServer::is_allowed(string &s)
// 
// description : 	method to test whether command is allowed or not in this
//			state. 
//
// returns :		boolean - true == is allowed , false == not allowed
//
//-----------------------------------------------------------------------------

bool DbUnExportServer::is_allowed(DeviceImpl *device, const CORBA_Any &in_any)
{

// command always allowed

	return(true);
}

//+----------------------------------------------------------------------------
//
// method : 		DbUnExportServer::execute(string &s)
// 
// description : 	method to trigger the execution of "DbUnExportDevice" 
//			which will mark the device as unexported in the
//			TANGO database
//
//-----------------------------------------------------------------------------

CORBA_Any *DbUnExportServer::execute(DeviceImpl *device,const CORBA_Any &in_any)
{	
	Tango_DevString server_name;

// call DataBase method which implements this command

	if ((in_any >>= server_name) == false)
	{
		throw_exception((const char*)"Incompatible command argument type, expected type is : char*",
				(const char*)"DbUnExportServer::execute",
				Tango_DevErr_IncompatibleCmdArgumentType,
				Tango_APILIB,Tango_ERROR);
	}

        ((DataBase*)device)->UnExportServer(server_name); 

// return data to the caller

	CORBA_Any *out_any;
	try
	{	
		out_any = new CORBA_Any();
	}
	catch (bad_alloc)
	{
		throw_exception((const char*)"Can't allocate memory in server",
				(const char*)"DbUnExportServer::execute",
				Tango_DevErr_MemoryAllocation,Tango_APILIB,Tango_ERROR);
	}
	return(out_any);
}

//+----------------------------------------------------------------------------
//
// method : 		DbGetDeviceProperty::DbGetDeviceProperty(string &s)
// 
// description : 	constructor for the DbGetProperty command of the 
//			DataBase.
//
//-----------------------------------------------------------------------------

DbGetDeviceProperty::DbGetDeviceProperty(const char *s, Tango_CmdArgType tin, Tango_CmdArgType tout):Command(s,tin,tout)
{
}

//+----------------------------------------------------------------------------
//
// method : 		DbGetDeviceProperty::is_allowed(string &s)
// 
// description : 	method to test whether command is allowed or not in this
//			state. 
//
// returns :		boolean - true == is allowed , false == not allowed
//
//-----------------------------------------------------------------------------

bool DbGetDeviceProperty::is_allowed(DeviceImpl *device, const CORBA_Any &in_any)
{

// command always allowed

	return(true);
}

//+----------------------------------------------------------------------------
//
// method : 		DbGetDeviceProperty::execute(string &s)
// 
// description : 	method to trigger the execution of the "ReadPosition" 
//			command
//
//-----------------------------------------------------------------------------

CORBA_Any *DbGetDeviceProperty::execute(DeviceImpl *device,const CORBA_Any &in_any)
{	
	Tango_DevVarStringArray *Property_names, *Property_list;

// call DataBase method which implements this command

	if ((in_any >>= Property_names) == false)
	{
		throw_exception((const char*)"Incompatible command argument type, expected type is : Tango_DevVarStringArray",
				(const char*)"DbGetDeviceProperty::execute",
				Tango_DevErr_IncompatibleCmdArgumentType,
				Tango_APILIB,Tango_ERROR);
	}

        Property_list = ((DataBase*)device)->GetDeviceProperty(Property_names); 

// return data to the caller

	CORBA_Any *out_any;
	try
	{	
		out_any = new CORBA_Any();
	}
	catch (bad_alloc)
	{
		throw_exception((const char*)"Can't allocate memory in server",
				(const char*)"DbGetDeviceProperty::execute",
				Tango_DevErr_MemoryAllocation,Tango_APILIB,Tango_ERROR);
	}
	(*out_any) <<= Property_list;	
	return(out_any);
}

//+----------------------------------------------------------------------------
//
// method : 		DbPutDeviceProperty::DbPutDeviceProperty(string &s)
// 
// description : 	constructor for the DbPutDeviceProperty command of the 
//			DataBase.
//
//-----------------------------------------------------------------------------

DbPutDeviceProperty::DbPutDeviceProperty(const char *s, Tango_CmdArgType tin, Tango_CmdArgType tout):Command(s,tin,tout)
{
}

//+----------------------------------------------------------------------------
//
// method : 		DbPutDeviceProperty::is_allowed(string &s)
// 
// description : 	method to test whether command is allowed or not in this
//			state. 
//
// returns :		boolean - true == is allowed , false == not allowed
//
//-----------------------------------------------------------------------------

bool DbPutDeviceProperty::is_allowed(DeviceImpl *device, const CORBA_Any &in_any)
{

// command always allowed

	return(true);
}

//+----------------------------------------------------------------------------
//
// method : 		DbPutDeviceProperty::execute(string &s)
// 
// description : 	method to trigger the execution of the "ReadPosition" 
//			command
//
//-----------------------------------------------------------------------------

CORBA_Any *DbPutDeviceProperty::execute(DeviceImpl *device,const CORBA_Any &in_any)
{	
	Tango_DevVarStringArray *Property_list;

// call DataBase method which implements this command

	if ((in_any >>= Property_list) == false)
	{
		throw_exception((const char*)"Incompatible command argument type, expected type is : Tango_DevVarStringArray",
				(const char*)"DbPutDeviceProperty::execute",
				Tango_DevErr_IncompatibleCmdArgumentType,
				Tango_APILIB,Tango_ERROR);
	}

        ((DataBase*)device)->PutDeviceProperty(Property_list); 

// return nothing (empty CORBA_Any) to the caller

	CORBA_Any *out_any;
	try
	{	
		out_any = new CORBA_Any();
	}
	catch (bad_alloc)
	{
		throw_exception((const char*)"Can't allocate memory in server",
				(const char*)"DbPutDeviceProperty::execute() at line __LINE__",
				Tango_DevErr_MemoryAllocation,Tango_APILIB,Tango_ERROR);
	}
	return(out_any);
}

//+----------------------------------------------------------------------------
//
// method : 		DbGetAttributeProperty::DbGetDeviceProperty(string &s)
// 
// description : 	constructor for the DbGetProperty command of the 
//			DataBase.
//
//-----------------------------------------------------------------------------

DbGetAttributeProperty::DbGetAttributeProperty(const char *s, Tango_CmdArgType tin, Tango_CmdArgType tout):Command(s,tin,tout)
{
}

//+----------------------------------------------------------------------------
//
// method : 		DbGetAttributeProperty::is_allowed(string &s)
// 
// description : 	method to test whether command is allowed or not in this
//			state. 
//
// returns :		boolean - true == is allowed , false == not allowed
//
//-----------------------------------------------------------------------------

bool DbGetAttributeProperty::is_allowed(DeviceImpl *device, const CORBA_Any &in_any)
{

// command always allowed

	return(true);
}

//+----------------------------------------------------------------------------
//
// method : 		DbGetAttributeProperty::execute(string &s)
// 
// description : 	method to trigger the execution of the "ReadPosition" 
//			command
//
//-----------------------------------------------------------------------------

CORBA_Any *DbGetAttributeProperty::execute(DeviceImpl *device,const CORBA_Any &in_any)
{	
	Tango_DevVarStringArray *Property_names, *Property_list;

// call DataBase method which implements this command

	if ((in_any >>= Property_names) == false)
	{
		throw_exception((const char*)"Incompatible command argument type, expected type is : Tango_DevVarStringArray",
				(const char*)"DbGetAttributeProperty::execute",
				Tango_DevErr_IncompatibleCmdArgumentType,
				Tango_APILIB,Tango_ERROR);
	}

        Property_list = ((DataBase*)device)->GetAttributeProperty(Property_names); 

// return data to the caller

	CORBA_Any *out_any;
	try
	{	
		out_any = new CORBA_Any();
	}
	catch (bad_alloc)
	{
		throw_exception((const char*)"Can't allocate memory in server",
				(const char*)"DbGetAttributeProperty::execute",
				Tango_DevErr_MemoryAllocation,Tango_APILIB,Tango_ERROR);
	}
	(*out_any) <<= Property_list;	
	return(out_any);
}

//+----------------------------------------------------------------------------
//
// method : 		DbPutAttributeProperty::DbPutDeviceProperty(string &s)
// 
// description : 	constructor for the DbPutAttributeProperty command of the 
//			DataBase.
//
//-----------------------------------------------------------------------------

DbPutAttributeProperty::DbPutAttributeProperty(const char *s, Tango_CmdArgType tin, Tango_CmdArgType tout):Command(s,tin,tout)
{
}

//+----------------------------------------------------------------------------
//
// method : 		DbPutAttributeProperty::is_allowed(string &s)
// 
// description : 	method to test whether command is allowed or not in this
//			state. 
//
// returns :		boolean - true == is allowed , false == not allowed
//
//-----------------------------------------------------------------------------

bool DbPutAttributeProperty::is_allowed(DeviceImpl *device, const CORBA_Any &in_any)
{

// command always allowed

	return(true);
}

//+----------------------------------------------------------------------------
//
// method : 		DbPutAttributeProperty::execute(string &s)
// 
// description : 	method to trigger the execution of the "ReadPosition" 
//			command
//
//-----------------------------------------------------------------------------

CORBA_Any *DbPutAttributeProperty::execute(DeviceImpl *device,const CORBA_Any &in_any)
{	
	Tango_DevVarStringArray *Property_list;

// call DataBase method which implements this command

	if ((in_any >>= Property_list) == false)
	{
		throw_exception((const char*)"Incompatible command argument type, expected type is : Tango_DevVarStringArray",
				(const char*)"DbPutAttributeProperty::execute",
				Tango_DevErr_IncompatibleCmdArgumentType,
				Tango_APILIB,Tango_ERROR);
	}

        ((DataBase*)device)->PutAttributeProperty(Property_list); 

// return nothing (empty CORBA_Any) to the caller

	CORBA_Any *out_any;
	try
	{	
		out_any = new CORBA_Any();
	}
	catch (bad_alloc)
	{
		throw_exception((const char*)"Can't allocate memory in server",
				(const char*)"DbPutAttributeProperty::execute() at line __LINE__",
				Tango_DevErr_MemoryAllocation,Tango_APILIB,Tango_ERROR);
	}
	return(out_any);
}

//+----------------------------------------------------------------------------
//
// method : 		DbGetClassProperty::DbGetDeviceProperty(string &s)
// 
// description : 	constructor for the DbGetProperty command of the 
//			DataBase.
//
//-----------------------------------------------------------------------------

DbGetClassProperty::DbGetClassProperty(const char *s, Tango_CmdArgType tin, Tango_CmdArgType tout):Command(s,tin,tout)
{
}

//+----------------------------------------------------------------------------
//
// method : 		DbGetClassProperty::is_allowed(string &s)
// 
// description : 	method to test whether command is allowed or not in this
//			state. 
//
// returns :		boolean - true == is allowed , false == not allowed
//
//-----------------------------------------------------------------------------

bool DbGetClassProperty::is_allowed(DeviceImpl *device, const CORBA_Any &in_any)
{

// command always allowed

	return(true);
}

//+----------------------------------------------------------------------------
//
// method : 		DbGetClassProperty::execute(string &s)
// 
// description : 	method to trigger the execution of the "ReadPosition" 
//			command
//
//-----------------------------------------------------------------------------

CORBA_Any *DbGetClassProperty::execute(DeviceImpl *device,const CORBA_Any &in_any)
{	
	Tango_DevVarStringArray *Property_names, *Property_list;

// call DataBase method which implements this command

	if ((in_any >>= Property_names) == false)
	{
		throw_exception((const char*)"Incompatible command argument type, expected type is : Tango_DevVarStringArray",
				(const char*)"DbGetClassProperty::execute",
				Tango_DevErr_IncompatibleCmdArgumentType,
				Tango_APILIB,Tango_ERROR);
	}

        Property_list = ((DataBase*)device)->GetClassProperty(Property_names); 

// return data to the caller

	CORBA_Any *out_any;
	try
	{	
		out_any = new CORBA_Any();
	}
	catch (bad_alloc)
	{
		throw_exception((const char*)"Can't allocate memory in server",
				(const char*)"DbGetClassProperty::execute",
				Tango_DevErr_MemoryAllocation,Tango_APILIB,Tango_ERROR);
	}
	(*out_any) <<= Property_list;	
	return(out_any);
}

//+----------------------------------------------------------------------------
//
// method : 		DbPutClassProperty::DbPutDeviceProperty(string &s)
// 
// description : 	constructor for the DbPutClassProperty command of the 
//			DataBase.
//
//-----------------------------------------------------------------------------

DbPutClassProperty::DbPutClassProperty(const char *s, Tango_CmdArgType tin, Tango_CmdArgType tout):Command(s,tin,tout)
{
}

//+----------------------------------------------------------------------------
//
// method : 		DbPutClassProperty::is_allowed(string &s)
// 
// description : 	method to test whether command is allowed or not in this
//			state. 
//
// returns :		boolean - true == is allowed , false == not allowed
//
//-----------------------------------------------------------------------------

bool DbPutClassProperty::is_allowed(DeviceImpl *device, const CORBA_Any &in_any)
{

// command always allowed

	return(true);
}

//+----------------------------------------------------------------------------
//
// method : 		DbPutClassProperty::execute(string &s)
// 
// description : 	method to trigger the execution of the "ReadPosition" 
//			command
//
//-----------------------------------------------------------------------------

CORBA_Any *DbPutClassProperty::execute(DeviceImpl *device,const CORBA_Any &in_any)
{	
	Tango_DevVarStringArray *Property_list;

// call DataBase method which implements this command

	if ((in_any >>= Property_list) == false)
	{
		throw_exception((const char*)"Incompatible command argument type, expected type is : Tango_DevVarStringArray",
				(const char*)"DbPutClassProperty::execute",
				Tango_DevErr_IncompatibleCmdArgumentType,
				Tango_APILIB,Tango_ERROR);
	}

        ((DataBase*)device)->PutClassProperty(Property_list); 

// return nothing (empty CORBA_Any) to the caller

	CORBA_Any *out_any;
	try
	{	
		out_any = new CORBA_Any();
	}
	catch (bad_alloc)
	{
		throw_exception((const char*)"Can't allocate memory in server",
				(const char*)"DbPutClassProperty::execute() at line __LINE__",
				Tango_DevErr_MemoryAllocation,Tango_APILIB,Tango_ERROR);
	}
	return(out_any);
}

//+----------------------------------------------------------------------------
//
// method : 		DbAddServer::DbAddServer(string &s)
// 
// description : 	constructor for the DbAddServer command of the 
//			DataBase.
//
//-----------------------------------------------------------------------------

DbAddServer::DbAddServer(const char *s, Tango_CmdArgType tin, Tango_CmdArgType tout):Command(s,tin,tout)
{
}

//+----------------------------------------------------------------------------
//
// method : 		DbAddServer::is_allowed(string &s)
// 
// description : 	method to test whether command is allowed or not in this
//			state. 
//
// returns :		boolean - true == is allowed , false == not allowed
//
//-----------------------------------------------------------------------------

bool DbAddServer::is_allowed(DeviceImpl *device, const CORBA_Any &in_any)
{

// command always allowed

	return(true);
}

//+----------------------------------------------------------------------------
//
// method : 		DbAddServer::execute(string &s)
// 
// description : 	method to trigger the execution of "DbAddServer" 
//			which will mark the device as unexported in the
//			TANGO database
//
//-----------------------------------------------------------------------------

CORBA_Any *DbAddServer::execute(DeviceImpl *device,const CORBA_Any &in_any)
{	
        Tango_DevVarStringArray *server_device_list;   

// call DataBase method which implements this command

	if ((in_any >>= server_device_list) == false)
	{
		throw_exception((const char*)"Incompatible command argument type, expected type is : char*",
				(const char*)"DbAddServer::execute",
				Tango_DevErr_IncompatibleCmdArgumentType,
				Tango_APILIB,Tango_ERROR);
	}

        ((DataBase*)device)->AddServer(server_device_list); 

// return data to the caller

	CORBA_Any *out_any;
	try
	{	
		out_any = new CORBA_Any();
	}
	catch (bad_alloc)
	{
		throw_exception((const char*)"Can't allocate memory in server",
				(const char*)"DbAddServer::execute",
				Tango_DevErr_MemoryAllocation,Tango_APILIB,Tango_ERROR);
	}
	return(out_any);
}
//+----------------------------------------------------------------------------
//
// method : 		DbAddDevice::DbAddDevice(string &s)
// 
// description : 	constructor for the DbAddDevice command of the 
//			DataBase.
//
//-----------------------------------------------------------------------------

DbAddDevice::DbAddDevice(const char *s, Tango_CmdArgType tin, Tango_CmdArgType tout):Command(s,tin,tout)
{
}

//+----------------------------------------------------------------------------
//
// method : 		DbAddDevice::is_allowed(string &s)
// 
// description : 	method to test whether command is allowed or not in this
//			state. 
//
// returns :		boolean - true == is allowed , false == not allowed
//
//-----------------------------------------------------------------------------

bool DbAddDevice::is_allowed(DeviceImpl *device, const CORBA_Any &in_any)
{

// command always allowed

	return(true);
}

//+----------------------------------------------------------------------------
//
// method : 		DbAddDevice::execute(string &s)
// 
// description : 	method to trigger the execution of "DbUnExportDevice" 
//			which will mark the device as unexported in the
//			TANGO database
//
//-----------------------------------------------------------------------------

CORBA_Any *DbAddDevice::execute(DeviceImpl *device,const CORBA_Any &in_any)
{	
        Tango_DevVarStringArray *server_device;   

// call DataBase method which implements this command

	if ((in_any >>= server_device) == false)
	{
		throw_exception((const char*)"Incompatible command argument type, expected type is : char*",
				(const char*)"DbAddDevice::execute",
				Tango_DevErr_IncompatibleCmdArgumentType,
				Tango_APILIB,Tango_ERROR);
	}

        ((DataBase*)device)->AddDevice(server_device); 

// return data to the caller

	CORBA_Any *out_any;
	try
	{	
		out_any = new CORBA_Any();
	}
	catch (bad_alloc)
	{
		throw_exception((const char*)"Can't allocate memory in server",
				(const char*)"DbAddDevice::execute",
				Tango_DevErr_MemoryAllocation,Tango_APILIB,Tango_ERROR);
	}
	return(out_any);
}
//DataBaseClass *DataBaseClass::_instance = NULL;
static DataBaseClass *an_instance = NULL;

//+----------------------------------------------------------------------------
//
// method : 		DataBaseClass::DataBaseClass(string &s)
// 
// description : 	constructor for the DataBaseClass
//
//-----------------------------------------------------------------------------

DataBaseClass::DataBaseClass(string &s):DeviceClass(s)
{

	cout4 << "DataBaseClass::DataBaseClass(): do nothing " << endl;
}
//+---------------------------------------------------------------------------- //
// method :             DataBaseClass::command_factory()
//                                                                              // description :        constructor for the DataBaseClass
//
//-----------------------------------------------------------------------------
void DataBaseClass::command_factory()
{
	
//
// Initialise the class here (can also be implemented by a call to a
// class-initialise method (What I prefer)
//
	
//
// Add class command(s) to the command_list
//

	cout1 << "DataBaseClass::DataBaseClass(): add command DbInfo " << endl;
	command_list.push_back(new DbInfo("DbInfo",Tango_DEV_VOID,Tango_DEVVAR_STRINGARRAY));
	cout1 << "DataBaseClass::DataBaseClass(): add command DbGetDeviceList " << endl;
	command_list.push_back(new DbGetDeviceList("DbGetDeviceList",Tango_DEVVAR_STRINGARRAY,Tango_DEVVAR_STRINGARRAY));
	cout1 << "DataBaseClass::DataBaseClass(): add command DbGetClassList " << endl;
	command_list.push_back(new DbGetClassList("DbGetClassList",Tango_DEV_STRING, Tango_DEVVAR_STRINGARRAY));
	cout1 << "DataBaseClass::DataBaseClass(): add command DbGetDeviceClassList " << endl;
	command_list.push_back(new DbGetDeviceClassList("DbGetDeviceClassList",Tango_DEV_STRING, Tango_DEVVAR_STRINGARRAY));
	cout1 << "DataBaseClass::DataBaseClass(): add command DbGetDeviceProperty " << endl;
	command_list.push_back(new DbGetDeviceProperty("DbGetDeviceProperty",Tango_DEVVAR_STRINGARRAY,Tango_DEVVAR_STRINGARRAY));
	cout1 << "DataBaseClass::DataBaseClass(): add command DbPutDeviceProperty " << endl;
	command_list.push_back(new DbPutDeviceProperty("DbPutDeviceProperty",Tango_DEVVAR_STRINGARRAY, Tango_DEV_VOID));
	cout1 << "DataBaseClass::DataBaseClass(): add command DbGetAttributeProperty " << endl;
	command_list.push_back(new DbGetAttributeProperty("DbGetAttributeProperty",Tango_DEVVAR_STRINGARRAY,Tango_DEVVAR_STRINGARRAY));
	cout1 << "DataBaseClass::DataBaseClass(): add command DbPutAttributeProperty " << endl;
	command_list.push_back(new DbPutAttributeProperty("DbPutAttributeProperty",Tango_DEVVAR_STRINGARRAY, Tango_DEV_VOID));
	cout1 << "DataBaseClass::DataBaseClass(): add command DbGetClassProperty " << endl;
	command_list.push_back(new DbGetClassProperty("DbGetClassProperty",Tango_DEVVAR_STRINGARRAY,Tango_DEVVAR_STRINGARRAY));
	cout1 << "DataBaseClass::DataBaseClass(): add command DbPutClassProperty " << endl;
	command_list.push_back(new DbPutClassProperty("DbPutClassProperty",Tango_DEVVAR_STRINGARRAY, Tango_DEV_VOID));
	cout1 << "DataBaseClass::DataBaseClass(): add command DbExportDevice " << endl;
	command_list.push_back(new DbExportDevice("DbExportDevice",Tango_DEVVAR_STRINGARRAY, Tango_DEV_VOID));
	cout1 << "DataBaseClass::DataBaseClass(): add command DbImportDevice " << endl;
	command_list.push_back(new DbImportDevice("DbImportDevice",Tango_DEV_STRING, Tango_DEVVAR_STRINGARRAY));
	cout1 << "DataBaseClass::DataBaseClass(): add command DbUnExportDevice " << endl;
	command_list.push_back(new DbUnExportDevice("DbUnExportDevice",Tango_DEV_STRING, Tango_DEV_VOID));
	cout1 << "DataBaseClass::DataBaseClass(): add command DbUnExportServer " << endl;
	command_list.push_back(new DbUnExportServer("DbUnExportServer",Tango_DEV_STRING, Tango_DEV_VOID));
	cout1 << "DataBaseClass::DataBaseClass(): add command DbAddServer " << endl;
	command_list.push_back(new DbAddServer("DbAddServer",Tango_DEVVAR_STRINGARRAY, Tango_DEV_VOID));
	cout1 << "DataBaseClass::DataBaseClass(): add command DbAddDevice " << endl;
	command_list.push_back(new DbAddDevice("DbAddDevice",Tango_DEVVAR_STRINGARRAY, Tango_DEV_VOID));
}
//+---------------------------------------------------------------------------- //
// method :             DataBaseClass::device_factory()
//                                                                              
// description :        device_factory for the DataBaseClass
//                                                                              //-----------------------------------------------------------------------------
void DataBaseClass::device_factory(Tango_DevVarStringArray *unused)
{

//
// Create device(s) and add them into the device list
//

	cout1 << "DataBaseClass::DataBaseClass(): create database device " << DataBase::db_name.c_str() << endl;
	device_list.push_back(new DataBase(this, DataBase::db_name.c_str(),
					   "TANGO database device server"));
							 
}

//

//+----------------------------------------------------------------------------
//
// method : 		DataBaseClass::instance
// 
// description : 	Create the object if not already done. Otherwise, just
//			return a pointer to the object
//
//-----------------------------------------------------------------------------

DataBaseClass *DataBaseClass::instance()
{
	if (an_instance != NULL)
		cout1 << "DataBaseClass *DataBaseClass::Instance(): an_instance " << an_instance << endl;
	if (an_instance == NULL)
	{
		try
		{
			string s("DataBaseClass");
			an_instance = new DataBaseClass(s);
		}
		catch (bad_alloc)
		{
			throw;
		}		
	}		
	return an_instance;
}
