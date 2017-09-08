#!/usr/bin/env python2.7


import SimpleCV
import os
import sys
sys.path.append(os.environ['HOME'] + '/robotics_repo/Documentation/Computer Vision/infoscimechatronics')
import cvutils
import time
import serial
execfile("camera_calibration_state.py")

camera = SimpleCV.Camera(0, {"width":320,"height":240})
os.system(os.environ['HOME'] + '/robotics_repo/Projects/2017/SoccerBots/rpicam-setup.sh')

speed = 0
prev_error = 0
integral = 0
derivative = 0
hunt_dir = 1 
hunt_step = 15
#blobs_threshold = 0.0075
blobs_threshold = 0.00005
times = []
output = False
then = 0
now = 0	
scaling_factor = 0.250 #0.075


def average(numbers):
	x = 0
	for num in numbers:
		x += num
	x = float(x) / len(numbers)
	return x

def x_coordinate_to_angle(coord): 
	#return coord*35.543 #calibrated for LogiTech Camera.
	return coord*33.0 #calibrated for RPi Camera. 

def clip_angle(angle):
	if angle < -90:
		return -90
	if angle > 90:
		return 90
	else:
		return angle

def send2pd(message):
	print(str(message) + ";")
	sys.stdout.flush()

while True:
	now = time.time()
	elapsed_time = now  - then
	sys.stderr.write('Frame rate:' + str(1.0/elapsed_time) + '\n')
	#times.append(elapsed_time)
	#frame_rate = 1 / average(times) not calcultating the correct result
	
	image = cvutils.wb(camera.getImage().scale(scaling_factor), calibrated_grey_sample)
	v,s,h = image.toHSV().splitChannels()
	hue_match = h.colorDistance((calibrated_goal_sample[0][0],calibrated_goal_sample[0][0],calibrated_goal_sample[0][0])).binarize(calibrated_goal_sample[0][1]*3)
	sat_match = s.colorDistance((calibrated_goal_sample[1][0],calibrated_goal_sample[1][0],calibrated_goal_sample[1][0])).binarize(calibrated_goal_sample[1][1]*3)
	matched = ((hue_match / 16) * (sat_match / 16))
	matched.show()
	blobs = matched.findBlobs(threshval=100, minsize=1, maxsize=0, threshblocksize=0, threshconstant=5)
	if blobs is not None:
		blob_size = blobs[-1].area()
		image_size = image.area()
		sys.stderr.write ("Blob size / image size: " + str(blob_size / image_size) + "\n")
		if blob_size / image_size < blobs_threshold:
			sys.stderr.write("Blobs too small! \n")
			send2pd(-180) #404 not found
			time.sleep(0.02)
		else:
			(x,y) = blobs[-1].centroid()
			image.dl().line((x,0), (x,image.height), (255,0,0), antialias=False)
			image.dl().line((0,y), (image.width,y), (0,255,0), antialias=False)
			image.show()
			#sys.stderr.write float(x) / image.width
			converted_coord = float(x) / image.width
			converted_coord = x_coordinate_to_angle(converted_coord*2-1)
			sys.stderr.write("converted_coord: " + str(converted_coord) + ' ' + '\n')
			send2pd(converted_coord)
	else:
		sys.stderr.write("No blobs found! \n")

		send2pd(-180)
		time.sleep(0.1)
		
		
	time.sleep(0.03)
	then = now
	
