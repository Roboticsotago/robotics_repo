import SimpleCV
import os
import sys
sys.path.append(os.environ['HOME'] + '/robotics_repo/Documentation/Computer Vision/infoscimechatronics')
import cvutils
import time
import serial

ser = serial.Serial('/dev/serial/by-id/usb-Arduino__www.arduino.cc__0043_85431303736351D070E0-if00')

camera = SimpleCV.Camera(0, {"width":960,"height":540})
os.system(os.environ['HOME'] + '/robotics_repo/Projects/2017/SoccerBots/uvcdynctrl-settings.tcl')

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
blobs_threshold = 0.0005
times = []
output = True
image_output = True
calibrating = False
scale_factor = 1/64.0

def debug(msg):
	global output
	if output == True:
		sys.stderr.write(str(msg) + '\n')

lab_grey_sample = (155.40740740740742, 150.55555555555554, 160.07407407407408)
lab_goal_blue = ((104, 2), (218.5, 6.5), (127.5, 14.5))

if calibrating:
	grey_sample = cvutils.calibrate_white_balance(camera)
	target_colour = cvutils.calibrate_colour_match(camera, grey_sample)
else:
	grey_sample = lab_grey_sample
	target_colour = lab_goal_blue

debug(target_colour)

def average(numbers):
	x = 0
	for num in numbers:
		x += num
	x = float(x) / len(numbers)
	return x

def x_coordinate_to_angle(coord):
	return coord*35.543
		
def image_debug(img):
	global image_output
	if image_output:
		img.show()
	
#Sends serial data to connected Arduino, input data between -90 to 90, output data to servo is 0 to 180 deg
def servo(target_angle):
	current_angle = get_current_angle() + 90
	debug("current angle: " + str(current_angle) + ' ' + "target angle: " + str(target_angle))
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
	debug('Seek called & hunt_dir = ' + str(hunt_dir))
	global hunt_step
	global hunt_dir
	current_angle = get_current_angle()
	if current_angle >= 90 or current_angle <= -90:
		hunt_dir *= -1	
	servo_abs(clip_angle(current_angle + hunt_dir * hunt_step))
	time.sleep(0.05)

while True:
	start_time = time.clock()
	image = cvutils.wb(camera.getImage().scale(scale_factor), grey_sample)
	v,s,h = image.toHSV().splitChannels()
	hue_match = h.colorDistance((target_colour[0][0],target_colour[0][0],target_colour[0][0])).binarize(target_colour[0][1]*3)
	sat_match = s.colorDistance((target_colour[1][0],target_colour[1][0],target_colour[1][0])).binarize(target_colour[1][1]*3)
	matched = ((hue_match / 16) * (sat_match / 16))
	image_debug(matched)
	blobs = matched.findBlobs(100, 1)
	if blobs is not None:
		blob_size = blobs[-1].area()
		image_size = image.area()
		#print blob_size / image_size
		if blob_size / image_size < blobs_threshold:
			print 'Blobs too small!'
			seek()
		else:
			(x,y) = blobs[-1].centroid()
			image.dl().line((x,0), (x,image.height), (255,0,0), antialias=False)
			image.dl().line((0,y), (image.width,y), (0,255,0), antialias=False)
			image_debug(image)
			#print float(x) / image.width
			converted_coord = float(x) / image.width
			converted_coord = x_coordinate_to_angle(converted_coord*2-1)
			debug('converted_coord: ' + str(converted_coord) + ' ')
			servo(converted_coord*0.3)
	else:
		print 'No blobs found!'
		seek()
	end_time = time.clock()
	elapsed_time = end_time - start_time
	times.append(elapsed_time)
	frame_rate = 1 / average(times)
	sys.stderr.write('Frame rate:' + str(frame_rate) + '\n')
	#image_debug(image)
