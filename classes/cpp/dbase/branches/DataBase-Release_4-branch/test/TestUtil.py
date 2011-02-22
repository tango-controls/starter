import unittest
import PyTango
import sys
import os
import user

def start_test(runner,cl,*args):
    test_res = runner.run(cl(*args))
    if len(test_res.errors) != 0 or len(test_res.failures) != 0:
#        admin = PyTango.DeviceProxy("dserver/pool/test")
#        admin.command_inout("kill")
        sys.exit(-1)		
		
		
        
class TestUtil(unittest.TestCase):       
    def check_empty_attribute(self,dev,att_name):
        try:
            c_list = dev.read_attribute(att_name)
            if len(c_list.value) != 0:
                self.assert_(False,"The %s attribute is not empty !! It contains: %s" % (att_name,c_list.value))
        except PyTango.DevFailed,e:
            except_value = sys.exc_info()[1]
            self.assertEqual(except_value[0]["reason"],"API_EmptyDeviceAttribute")
            self.assertEqual(except_value[0]["desc"],"cannot extract, no data in DeviceAttribute object ")
			
#---------------------------------------------------------------------------------------------------------  
            
    def attribute_error(self,dev,att_name,err,pr = False):
        try:
            c_list = dev.read_attribute(att_name)
            self.assert_(False,"The %s attribute is not in fault!!" % (att_name))
        except PyTango.DevFailed,e:
            except_value = sys.exc_info()[1]
            if pr == True:
                print "\nERROR desc"
                print "origin =",except_value[0]["origin"]
                print "desc =",except_value[0]["desc"]
                print "origin =",except_value[0]['origin']
            self.assertEqual(except_value[0]["reason"],err)
			
#---------------------------------------------------------------------------------------------------------  
            
    def wr_attribute_error(self,dev,att_val,err,pr = False):
        try:
            dev.write_attribute(att_val)
            self.assert_(False,"The %s attribute is not in fault!!" % (att_val.name))
        except PyTango.DevFailed,e:
            except_value = sys.exc_info()[1]
            if pr == True:
                print "\nERROR desc"
                print "origin =",except_value[0]["origin"]
                print "desc =",except_value[0]["desc"]
                print "origin =",except_value[0]['origin']
            self.assertEqual(except_value[0]["reason"],err)
			
#---------------------------------------------------------------------------------------------------------  
            
    def _write_attribute(self,dev,att_name,att_val):
        val = PyTango.AttributeValue()
        val.name = att_name
        val.value = att_val
        dev.write_attribute(val)

#---------------------------------------------------------------------------------------------------------  

    def wrong_argument(self,dev,cmd_name,arg_list,err,pr = False):
        try:
            dev.command_inout(cmd_name,arg_list)
            self.assert_(False,"The %s command succeed with stupid arguments!!" % (cmd_name))
        except PyTango.DevFailed,e:
            except_value = sys.exc_info()[1]
            if pr == True:
                print "\nERROR desc"
                print "origin =",except_value[0]["origin"]
                print "desc =",except_value[0]["desc"]
                print "origin =",except_value[0]['origin']
            self.assertEqual(except_value[0]["reason"],err)
            
#---------------------------------------------------------------------------------------------------------  
           
