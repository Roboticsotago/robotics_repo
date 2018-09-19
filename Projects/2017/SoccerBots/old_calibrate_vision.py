#!/usr/bin/env python2.7
import SimpleCV
import sys
import os
sys.path.append(os.environ['HOME'] + '/robotics_repo/Documentation/Computer Vision/infoscimechatronics')
import cvutils

calibration_file_name = "camera_calibration_state.py"
camera = SimpleCV.Camera(0, {"width":320,"height":240})
os.system(os.environ['HOME'] + '/robotics_repo/Projects/2017/SoccerBots/rpicam-setup.sh')

grey_sample = cvutils.calibrate_white_balance(camera)
blue_sample = cvutils.calibrate_colour_match(camera, grey_sample)

#grey_sample = (221.5625, 219.75, 225.8125)
#blue_sample = ((108.0, 2.0), (159.0, 30.0), (90.5, 26.5))


def file_output():
	output_file = open(calibration_file_name, "w")
	output_file.write("calibrated_grey_sample = " + str(grey_sample) + "\n")
	output_file.write("calibrated_goal_sample = " + str(blue_sample) + "\n")
	output_file.close()
	
file_output()

