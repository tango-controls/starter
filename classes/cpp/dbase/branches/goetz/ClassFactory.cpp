static const char *RcsId = "$Header$";
//+=============================================================================
//
// file :               ClassFactory.cpp
//
// description :        C++ source for the class_factory method of the DServer
//			device class. This method is responsible to create
//			all class singletin for a device server. It is called
//			at device server startup
//
// project :            TANGO
//
// author(s) :          A.Gotz + E.Taurel
//
// $Version$
//
// $Log$
// Revision 1.4  2000/02/15 17:51:49  goetz
// testing 1-2-3
//
// Revision 1.1.1.1  2000/02/15 11:20:40  goetz
// Imported sources
//
// Revision 1.1.1.1  1999/12/22 14:49:45  goetz
// Imported sources
//
// Revision 1.1  1999/11/26 10:27:08  goetz
// added DeviceProperty, AttributeProperty and ClassProperty
//
//
// copyleft :           European Synchrotron Radiation Facility
//                      BP 220, Grenoble 38043
//                      FRANCE
//
//-=============================================================================

#include <tango_ds.h>
#include <DataBaseClass.h>

void DServer::class_factory()
{

//
// Create DataBaseClass singleton and store it in DServer object
//

	add_class(DataBaseClass::instance());

}
