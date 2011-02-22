import unittest
import PyTango
import sys
import os
import time
import TestUtil
import posix 
        
#-------------------------------------------------------------------------------------------------------------------
#
#        New test class
#
#-------------------------------------------------------------------------------------------------------------------

class DBTest(TestUtil.TestUtil):
    def __init__(self,tg_host,dev_name,methodName='runTest'):
        unittest.TestCase.__init__(self,methodName)
        self.tg_host = tg_host
        self.db = PyTango.Database()
        self.db_dev = PyTango.DeviceProxy(dev_name)
        self.dev_name = 'et/db/test'
        
#---------------------------------------------------------------------------------------------------------------------
                            
    def setUp(self):
		dev_info = PyTango.DbDevInfo()
		dev_info.name = self.dev_name
		dev_info._class = 'TheClass'
		dev_info.server = 'Server/instance'
		
		self.db.add_device(dev_info)
		
                
#---------------------------------------------------------------------------------------------------------------------
               
    def ImpExpDev(self):
        """Import/Export device"""
        
        di = self.db.import_device(self.dev_name)
        
        self.assertEqual(di.name,self.dev_name)
        self.assertEqual(di.exported,0)
        
        dev_export = PyTango.DbDevExportInfo()
        dev_export.name = self.dev_name
        dev_export.ior = "IOR:xxxxxx"
        dev_export.host = "TheHost"
        dev_export.version = "999"
        dev_export.pid = 0
        
        self.db.export_device(dev_export)
        
        di = self.db.import_device(self.dev_name)
        
        self.assertEqual(di.name,self.dev_name)
        self.assertEqual(di.exported,1)
        self.assertEqual(di.ior,"IOR:xxxxxx")

#---------------------------------------------------------------------------------------------------------------------
               
    def AddDelServDev(self):
        """Add/Delete server - Add/Delete device - Put Server Info - Put/Get device alias"""
        
        serv_name = "TheServer/ac-dc"
        class_name = "TheClass"
        new_dev_name = "et/db/new-test"
        
        dev1 = PyTango.DbDevInfo()
        dev1.name = "et/dbtest/01"
        dev1._class = class_name
        dev2 = PyTango.DbDevInfo()
        dev2._class = class_name
        dev2.name = "et/dbtest/02"
        dev3 = PyTango.DbDevInfo()
        dev3.name = "dserver/" + serv_name
        dev3._class = "DServer"
        
        devs = [dev1,dev2,dev3]
        
        self.db.add_server(serv_name,devs)
        
        new_dev = PyTango.DbDevInfo()
        new_dev.name = new_dev_name
        new_dev._class = class_name
        new_dev.server = serv_name
        
        self.db.add_device(new_dev)
        
        svrInfo = [serv_name,"Here","1","5"]
        self.db_dev.command_inout("DbPutServerInfo",svrInfo)
        
        self.db.put_device_alias(new_dev_name,"ADev")
        self.db.delete_device_alias("ADev")
        
        self.db.delete_device(new_dev_name)
        
        self.db.delete_server(serv_name)     

#---------------------------------------------------------------------------------------------------------------------
               
    def PutAttAlias(self):
        """Put/Delete attribute alias"""
        
        PutAliasInfo = ["et/db/test/attr","TheAttr"]
        
        self.db_dev.command_inout("DbPutAttributeAlias",PutAliasInfo)
        
        self.db_dev.command_inout("DbDeleteAttributeAlias","TheAttr")
        
#---------------------------------------------------------------------------------------------------------------------
               
    def ImpExpEvent(self):
        """Import/Export event"""
        
        ExpEvent = ["et/the/event","IOR:xxxxxxxx","OneHost","123","3"]
        
        self.db_dev.command_inout("DbExportEvent",ExpEvent)
        
        ImpEvent = self.db_dev.command_inout("DbImportEvent","et/the/event")
        
        self.assertEqual(ImpEvent[0][0],1)
        self.assertEqual(ImpEvent[0][1],123)
        
        self.assertEqual(ImpEvent[1][0],"et/the/event")
        self.assertEqual(ImpEvent[1][1],"IOR:xxxxxxxx")
        self.assertEqual(ImpEvent[1][2],"3")
        self.assertEqual(ImpEvent[1][3],"OneHost")
        
#---------------------------------------------------------------------------------------------------------------------
               
    def ObjProp(self):
        """Put/Get/Delete/History on object properties"""
        
        prop = {"AProp":["PropValue"]}
        obj_name = "OneObj"
        
        self.db.put_property(obj_name,prop)
        
        ret_prop = self.db.get_property(obj_name,["AProp"])
        prop_val = ret_prop["AProp"]
        self.assertEqual(len(prop_val),1)
        self.assertEqual(prop_val[0],"PropValue")
        
        
        self.db.delete_property(obj_name,["AProp"])
        
        hi = [obj_name,"AProp"]
        prop_hist = self.db_dev.command_inout("DbGetPropertyHist",hi)
        
        size = len(prop_hist)
        self.assert_(size >= 7)
        self.assertEqual(prop_hist[size - 7],"AProp")
        if prop_hist[size - 5] == "1":
        	self.assertEqual(prop_hist[size - 4],"PropValue")
        	self.assertEqual(prop_hist[size - 3],"AProp")
        	self.assertEqual(prop_hist[size - 1],"0")
        
#---------------------------------------------------------------------------------------------------------------------
               
    def DevProp(self):
        """Put/Get/Delete/History on device properties"""
        
        prop_name = "DevProp"
        prop = {prop_name:["Prop1","Prop2"]}
        dev_name = "et/db/01"
        
        self.db.put_device_property(dev_name,prop)
        
        ret_prop = self.db.get_device_property(dev_name,[prop_name])
        prop_val = ret_prop[prop_name]
        self.assertEqual(len(prop_val),2)
        self.assertEqual(prop_val[0],"Prop1")
        self.assertEqual(prop_val[1],"Prop2")
        
        
        self.db.delete_device_property(dev_name,[prop_name])
        
        hi = [dev_name,prop_name]
        prop_hist = self.db_dev.command_inout("DbGetDevicePropertyHist",hi)
        
        size = len(prop_hist)
        self.assert_(size >= 8)
        self.assertEqual(prop_hist[size - 8],prop_name)
        if prop_hist[size - 6] == "2":
			self.assertEqual(prop_hist[size - 5],"Prop1")
			self.assertEqual(prop_hist[size - 4],"Prop2")
			self.assertEqual(prop_hist[size - 3],prop_name)
			self.assertEqual(prop_hist[size - 1],"0")
			
#---------------------------------------------------------------------------------------------------------------------
               
    def ClassProp(self):
        """Put/Get/Delete/History on class properties"""
        
        prop_name = "ClProp"
        prop = {prop_name:["Prop1","Prop2"]}
        cl_name = "TheClass"
        
        self.db.put_class_property(cl_name,prop)
        
        ret_prop = self.db.get_class_property(cl_name,[prop_name])
        prop_val = ret_prop[prop_name]
        self.assertEqual(len(prop_val),2)
        self.assertEqual(prop_val[0],"Prop1")
        self.assertEqual(prop_val[1],"Prop2")
        
        
        self.db.delete_class_property(cl_name,[prop_name])
        
        hi = [cl_name,prop_name]
        prop_hist = self.db_dev.command_inout("DbGetClassPropertyHist",hi)
        
        size = len(prop_hist)
        self.assert_(size >= 8)
        self.assertEqual(prop_hist[size - 8],prop_name)
        if prop_hist[size - 6] == "2":
			self.assertEqual(prop_hist[size - 5],"Prop1")
			self.assertEqual(prop_hist[size - 4],"Prop2")
			self.assertEqual(prop_hist[size - 3],prop_name)
			self.assertEqual(prop_hist[size - 1],"0")
			
			
#---------------------------------------------------------------------------------------------------------------------
               
    def DevAttProp(self):
        """Put/Get/Delete/History on device attribute properties"""
        
        prop_name = "AttProp"
        att_name = "TheAttr"
        att_prop = {prop_name:["Property"]}
        prop = {att_name:att_prop}
        dev_name = "et/db/01"
        
        self.db.put_device_attribute_property(dev_name,prop)
        
        ret_prop = self.db.get_device_attribute_property(dev_name,[att_name])
        prop_val = ret_prop[att_name][prop_name]
        self.assertEqual(len(prop_val),1)
        self.assertEqual(prop_val[0],"Property")
        
        deleted_prop = {att_name:[prop_name]}
        self.db.delete_device_attribute_property(dev_name,deleted_prop)
        
        hi = [dev_name,att_name,prop_name]
        prop_hist = self.db_dev.command_inout("DbGetDeviceAttributePropertyHist",hi)
        
        size = len(prop_hist)
        self.assert_(size >= 9)
        self.assertEqual(prop_hist[size - 9],att_name)
        self.assertEqual(prop_hist[size - 8],prop_name)
        if prop_hist[size - 6] == "1":
			self.assertEqual(prop_hist[size - 5],"Property")
			self.assertEqual(prop_hist[size - 4],att_name)
			self.assertEqual(prop_hist[size - 3],prop_name)
			self.assertEqual(prop_hist[size - 1],"0")
			
			
#---------------------------------------------------------------------------------------------------------------------
               
    def ClassAttProp(self):
        """Put/Get/Delete/History on class attribute properties"""
        
        prop_name = "AttProp"
        att_name = "TheAttr"
        att_prop = {prop_name:["WhyNot"]}
        prop = {att_name:att_prop}
        cl_name = "TheClass"
        
        self.db.put_class_attribute_property(cl_name,prop)
        
        ret_prop = self.db.get_class_attribute_property(cl_name,[att_name])
        prop_val = ret_prop[att_name][prop_name]
        self.assertEqual(len(prop_val),1)
        self.assertEqual(prop_val[0],"WhyNot")
        
        deleted_prop = {att_name:[prop_name]}
        self.db.delete_class_attribute_property(cl_name,deleted_prop)
        
        hi = [cl_name,att_name,prop_name]
        prop_hist = self.db_dev.command_inout("DbGetClassAttributePropertyHist",hi)
        
        size = len(prop_hist)
        self.assert_(size >= 9)
        self.assertEqual(prop_hist[size - 9],att_name)
        self.assertEqual(prop_hist[size - 8],prop_name)
        if prop_hist[size - 6] == "1":
			self.assertEqual(prop_hist[size - 5],"WhyNot")
			self.assertEqual(prop_hist[size - 4],att_name)
			self.assertEqual(prop_hist[size - 3],prop_name)
			self.assertEqual(prop_hist[size - 1],"0")

#---------------------------------------------------------------------------------------------------------------------

    def DelAllAttProp(self):
        """Delete all properties for a list of device attribute(s)"""
        
        prop_name = "AttProp"
        att_name = "TheAttr"
        another_prop_name = "AnotherProp"
        att_prop = {prop_name:["WhyNot"],another_prop_name:["1","2","3"]}
        prop = {att_name:att_prop}
        dev_name = "et/db/01"

        self.db.put_device_attribute_property(dev_name,prop)
        ret = self.db.get_device_attribute_property(dev_name,[att_name])
        self.assertEqual(len(ret[att_name]),2)
        self.assertEqual(len(ret[att_name][prop_name]),1)
        self.assertEqual(len(ret[att_name][another_prop_name]),3)

        hi = [dev_name,"dummy",att_name,"bidon"]
        self.db_dev.command_inout("DbDeleteAllDeviceAttributeProperty",hi)

        ret = self.db.get_device_attribute_property(dev_name,[att_name])
        self.assertEqual(len(ret[att_name]),0)
			                       
#---------------------------------------------------------------------------------------------------------------------
               


    def tearDown(self):
		self.db.delete_device(self.dev_name)
                
#-------------------------------------------------------------------------------------------------------------------
#
#        Test sequencement
#
#-------------------------------------------------------------------------------------------------------------------  

if __name__ == '__main__':
#	if len(sys.argv) != 2:
#		print "DbTst usage = DbTst <DataBase DS name (exec/inst)>"
#		sys.exit(-1)
		
	tg_host = "kidiboo:11111"
	dev_name = "sys/database/3"
	
	runner = unittest.TextTestRunner(verbosity=3)
	TestUtil.start_test(runner,DBTest,tg_host,dev_name,"ImpExpDev")
	TestUtil.start_test(runner,DBTest,tg_host,dev_name,"AddDelServDev")
	TestUtil.start_test(runner,DBTest,tg_host,dev_name,"PutAttAlias")
	TestUtil.start_test(runner,DBTest,tg_host,dev_name,"ImpExpEvent")
	TestUtil.start_test(runner,DBTest,tg_host,dev_name,"ObjProp")
	TestUtil.start_test(runner,DBTest,tg_host,dev_name,"DevProp")
	TestUtil.start_test(runner,DBTest,tg_host,dev_name,"ClassProp")
	TestUtil.start_test(runner,DBTest,tg_host,dev_name,"DevAttProp")
	TestUtil.start_test(runner,DBTest,tg_host,dev_name,"ClassAttProp")
	TestUtil.start_test(runner,DBTest,tg_host,dev_name,"DelAllAttProp")
               
