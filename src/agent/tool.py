#-*- coding: utf-8 -*-
import logging
import os
import sys
import time


class Tool:
    def SetIP(self,ip):
        setip = 'ifconfig eth0 ' + ip + ' netmask 255.255.0.0'
        gateway = ip[:ip.rfind('.')] + '.254'
        setroute = 'route add default gw ' + gateway
        os.system(setip + ' ; ' + setroute)
        return True

if __name__ == '__main__':
    tool = Tool()
    tool.SetIP('192.168.120.153')
