#-*- coding:utf8 -*-
import sys
import logging
from config import Config
from tool import Tool
import os

if __name__ == '__main__':
    #初始化日志
    logger = logging.getLogger('agent')
    hdlr = logging.FileHandler('agent.log')
    formatter = logging.Formatter('%(asctime)s %(levelname)s %(message)s')
    hdlr.setFormatter(formatter)
    logger.addHandler(hdlr)
    logger.setLevel(logging.NOTSET)

    conf = Config()
    if not conf.Init('/media/CDROM/conf'):
        logger.error('read agent config error')
        exit
    
    tool = Tool()
    if not tool.SetIP(conf.Get("ip")): 
        logger.error('cannot set ip')
        exit

    os.system(conf.Get('app'))
