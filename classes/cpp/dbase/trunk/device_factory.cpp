/**
 *	This  method is not standard for DataBaseClass.cpp file.
 *
 *	Replace with it inside the Pogo Generated file.
 */



//+----------------------------------------------------------------------------
//
// method : 		DataBaseClass::device_factory
// 
// description : 	Create the device object(s) and store them in the 
//			device list
//
// in :		Tango::DevVarStringArray *devlist_ptr : The device name list
//
//-----------------------------------------------------------------------------
void DataBaseClass::device_factory(const Tango::DevVarStringArray *devlist_ptr)
{
	cout << "DataBaseClass::DataBaseClass(): create database device " << DataBase::db_name.c_str() << endl;
	device_list.push_back(new DataBase(this, DataBase::db_name.c_str(),
                                    	"TANGO database device server"));

	export_device(device_list[0],"database");
}
