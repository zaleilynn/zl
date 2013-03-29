# -*- coding: utf-8 -*-

import os
import ConfigParser
import logging
import time

logger = logging.getLogger('agent')

class Config:
    def __init__(self):
        self.attributes = {}

    def Init(self, config_file_name):
        while not os.path.exists(config_file_name):
            time.sleep(2)
        
        cf = ConfigParser.ConfigParser()
        cf.read(config_file_name)

        value = cf.get('agent', 'ip')
        if(value == ''):
            logger.error('no ip specified')
            return False
        self.attributes['ip'] = value

        value = cf.get('agent', 'app')
        if(value == ''):
            logger.error('no app specified')
            return False
        self.attributes['app'] = value
        return True

    def Get(self, key):
        return self.attributes[key]

if __name__ == '__main__':
    config = Config()
    config.Init('conf')
    print config.Get('app')
    print config.Get('ip')

