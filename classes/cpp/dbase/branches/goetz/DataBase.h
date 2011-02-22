//=============================================================================
//
// file :               DataBase.h
//
// description :        Include file for the DataBase class. This class 
//			implements the device server for the database.
//			All clients and server which need to access the 
//			TANGO database will go via this class.
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
// Revision 1.4  1999/11/26 10:28:10  goetz
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

#ifndef _DATABASE_H
#define _DATABASE_H

#include <tango_ds.h>
#include <mysql.h>

#define Tango_DevErr_DataBase 1

class DataBase: public DeviceImpl
{
public :
	DataBase(DeviceClass *,const char *,const char *);
	~DataBase() {};
	
	static string DataBase::db_name; // (global) name of current incarnation of database

	Tango_DevVarStringArray *Info();
	Tango_DevVarStringArray *GetClassList(Tango_DevString);
	Tango_DevVarStringArray *GetDeviceList(Tango_DevVarStringArray *);
	Tango_DevVarStringArray *GetDeviceClassList(Tango_DevString);

	void ExportDevice(Tango_DevVarStringArray *);
	void UnExportDevice(Tango_DevString);
	void UnExportServer(Tango_DevString);
	Tango_DevVarLongStringArray *ImportDevice(Tango_DevString);
	Tango_DevVarLongStringArray *ImportDeviceList(Tango_DevVarStringArray *);

	Tango_DevVarStringArray *GetDeviceProperty(Tango_DevVarStringArray *);
	Tango_DevVarStringArray *GetAttributeProperty(Tango_DevVarStringArray *);
	Tango_DevVarStringArray *GetClassProperty(Tango_DevVarStringArray *);
	void PutDeviceProperty(Tango_DevVarStringArray *);
	void PutAttributeProperty(Tango_DevVarStringArray *);
	void PutClassProperty(Tango_DevVarStringArray *);

	void AddServer(Tango_DevVarStringArray *);
	void AddDevice(Tango_DevVarStringArray *); 

	virtual Tango_DevState State();
	virtual string &Status();
	
	virtual void init_device();

protected :	

	MYSQL mysql;
};

#endif /* _DATABASE_H */
