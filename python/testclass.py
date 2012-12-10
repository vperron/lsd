#!/usr/bin/env python

from time import sleep

from lsd import *

class LsdOver(LsdApi):

    def callback(self, *args):
        print "Info : "+str(args)

s = LsdOver()

s.join("TEST")
sleep(0.5)
s.shout("TEST", "Hello world !")
sleep(5)

del s
