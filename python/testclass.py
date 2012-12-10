#!/usr/bin/env python

from time import sleep

from shuppan import *

class ShuppanOver(ShuppanApi):

    def callback(self, *args):
        print "Info : "+str(args)

s = ShuppanOver()

s.join("TEST")
sleep(0.5)
s.shout("TEST", "Hello world !")
sleep(5)

del s
