#!/usr/bin/env python2.7
import SimpleCV
import sys
import os
sys.path.append(os.environ['HOME'] + '/robotics_repo/Documentation/Computer Vision/infoscimechatronics')
import cvutils

#ser = serial.Serial('/dev/serial/by-id/usb-www.freetronics.com_Eleven_64935343233351909241-if00')

#camera = SimpleCV.Camera(0, {"width":320,"height":240})
#os.system(os.environ['HOME'] + '/robotics_repo/Projects/2017/SoccerBots/uvcdynctrl-settings.tcl')

#lab_grey_sample = cvutils.calibrate_white_balance(camera)
#print lab_grey_sample
calibrated_grey_sample = (221.5625, 219.75, 225.8125)
#calibrated_grey_sample = (212.8181818181818, 205.9090909090909, 205.0909090909091)
#lab_goal_blue = cvutils.calibrate_colour_match(camera, lab_grey_sample)
#print lab_goal_blue
#print cvutils.calibrate_colour_match(camera, lab_grey_sample)
calibrated_goal_sample = ((108.0, 2.0), (159.0, 30.0), (90.5, 26.5))



def col_grey():
	return calibrated_grey_sample
	
def col_blue():
	return calibrated_goal_sample
