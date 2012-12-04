#!/usr/bin/env python

from time import sleep

from shuppan import *

class ShuppanOver(ShuppanApi):

    def info_callback(self, *args):
        print "Info : "+str(args)

    def subscribe_callback(self, *args):
        print "Subscribe : "+str(args)

s = ShuppanOver()

s.join("TEST")
sleep(0.5)
s.publish("TEST", "Hello world !")
sleep(5)

del s
