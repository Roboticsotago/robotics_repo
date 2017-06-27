#!/usr/bin/env python2.7
import SimpleCV
import os
import sys
sys.path.append(os.environ['HOME'] + '/robotics_repo/Documentation/Computer Vision/infoscimechatronics')
import cvutils
import time
import serial
#sys.path.append(os.environ['HOME'] + '/robotics_repo/Projects/2017/SoccerBots')
import camera_calibration_state

#ser = serial.Serial('/dev/serial/by-id/usb-www.freetronics.com_Eleven_64935343233351909241-if00')

camera = SimpleCV.Camera(0, {"width":320,"height":240})
os.system(os.environ['HOME'] + '/robotics_repo/Projects/2017/SoccerBots/uvcdynctrl-settings.tcl')

#lab_grey_sample = cvutils.calibrate_white_balance(camera)
#lab_goal_blue = cvutils.calibrate_colour_match(camera, lab_grey_sample)
lab_grey_sample = camera_calibration_state.col_grey()
lab_goal_blue = camera_calibration_state.col_blue()
#print lab_grey_sample

speed = 0
#global current_angle
#current_angle = 0
prev_error = 0
integral = 0
derivative = 0
#global hunt_dir
hunt_dir = 1 
hunt_step = 15
#blobs_threshold = 0.0075
blobs_threshold = 0.00005
times = []
output = False
	

def average(numbers):
	x = 0
	for num in numbers:
		x += num
	x = float(x) / len(numbers)
	return x

def x_coordinate_to_angle(coord): 
	#return coord*35.543 #calibrated for LogiTech Camera.
	return coord*33.0 #calibrated for RPi Camera. 

def plant(control): #control input from -1...1 so -90...90 deg / sec
	current_angle += speed
	speed = control * 90
	return current_angle
	
#Sends serial data to connected Arduino, input data between -90 to 90, output data to servo is 0 to 180 deg
def servo(target_angle):
	current_angle = get_current_angle() + 90
	sys.stderr.write("current angle: " + str(current_angle) + ' ')
	sys.stderr.write("target angle:: " + str(target_angle) + '\n')
	for i in range(abs(int(target_angle))):
		if target_angle > 0:
			if current_angle < 180:
				ser.write('+')
		if target_angle < 0:
			if current_angle > 0:
				ser.write('-')
	#time.sleep(0.2)
	
def get_current_angle():
	global ser
	ser.write("?")
	return int(ser.readline().strip()) - 90
	
def clip_angle(angle):
	if angle < -90:
		return -90
	if angle > 90:
		return 90
	else:
		return angle
	
def servo_abs(target_angle):
	servo(target_angle - get_current_angle())

def seek():
	sys.stderr.write("Seek called & hunt_dir = " + str(hunt_dir) + "\n")
	global hunt_step
	global hunt_dir
	current_angle = get_current_angle()
	if current_angle >= 90 or current_angle <= -90:
		hunt_dir *= -1	
	servo_abs(clip_angle(current_angle + hunt_dir * hunt_step))
	time.sleep(0.05)
	
def control(target):
	kp = 1
	ki = 1
	kd = 1
	error = target - current_angle
	integral += error
	derivative = error - prev_error
	prev_error = error
	control = kp * error + ki * integral + kd * derivative
	sys.stderr.write(control)
	sys.stderr.write(error * kp)
	sys.stderr.write(integral * ki)
	sys.stderr.write(derivative * kd)
	sys.stderr.write(plant(control))
	return plant(control)

def send2pd(message):
	print(str(message) + ";")
	sys.stdout.flush()

while True:
	start_time = time.clock()
	image = cvutils.wb(camera.getImage().scale(0.075), lab_grey_sample)
	v,s,h = image.toHSV().splitChannels()
	hue_match = h.colorDistance((lab_goal_blue[0][0],lab_goal_blue[0][0],lab_goal_blue[0][0])).binarize(lab_goal_blue[0][1]*3)
	sat_match = s.colorDistance((lab_goal_blue[1][0],lab_goal_blue[1][0],lab_goal_blue[1][0])).binarize(lab_goal_blue[1][1]*3)
	matched = ((hue_match / 16) * (sat_match / 16))
	matched.show()
	blobs = matched.findBlobs(100, 1)
	if blobs is not None:
		blob_size = blobs[-1].area()
		image_size = image.area()
		sys.stderr.write ("Blob size / image size: " + str(blob_size / image_size) + "\n")
		if blob_size / image_size < blobs_threshold:
			sys.stderr.write("Blobs too small! \n")
			send2pd(-180) #404 not found
			#seek()
		else:
			(x,y) = blobs[-1].centroid()
			image.dl().line((x,0), (x,image.height), (255,0,0), antialias=False)
			image.dl().line((0,y), (image.width,y), (0,255,0), antialias=False)
			image.show()
			#sys.stderr.write float(x) / image.width
			converted_coord = float(x) / image.width
			converted_coord = x_coordinate_to_angle(converted_coord*2-1)
			sys.stderr.write("converted_coord: " + str(converted_coord) + ' ' + '\n')
			#servo(converted_coord*0.3)
			send2pd(converted_coord)
	else:
		sys.stderr.write("No blobs found! \n")
		#seek()
		send2pd(-180)
	end_time = time.clock()
	elapsed_time = end_time - start_time
	times.append(elapsed_time)
	frame_rate = 1 / average(times)
	sys.stderr.write('Frame rate:' + str(frame_rate) + '\n')
	#image.show()
