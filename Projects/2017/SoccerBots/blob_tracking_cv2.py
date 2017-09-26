#!/usr/bin/env python2

# Demo/test of the Python OpenCV bindings (wot I built from source) with the picamera (Python-MMAL) module.
# Based on http://www.pyimagesearch.com/2015/03/30/accessing-the-raspberry-pi-camera-with-opencv-and-python/

# Dave Jones's excellent picamera library:
from picamera.array import PiRGBArray
from picamera import PiCamera

# The OpenCV library:
import cv2

from time import sleep,time
import sys
import numpy


# Set up camera and a NumPy array wrapper suitable for transferring the image data into OpenCV

target_framerate = 20
# 0.08 s frame capture with 50 Hz target

# Hmm, at 128 x 96 the chroma subsampling is pretty bad. Can we influence the format, or do we just have to increase the resolution?
#capture_res = (128,96)
capture_res = (256,192)

camera = PiCamera(resolution=capture_res, framerate=target_framerate)
camera.iso = 400
camera.shutter_speed = 20000
#camera.shutter_speed = 15000
print("calibrating gain...")
sleep(2)
camera.exposure_mode = 'off'
print("camera.analog_gain = ",camera.analog_gain)

# Now fix the white balace:
print("Hold a white card in front of the camera now!")
sleep(5)
g = camera.awb_gains
print("Thanks; camera.awb_gains = ",g)
camera.awb_mode = 'off'
camera.awb_gains = g

# array wrapper:
rawCapture = PiRGBArray(camera)

def calibrate_target_colour():
	print("Calibrating target colour")
	print("Put the target colour in the capture frame")
	camera.start_preview(alpha=200)
	sleep(3)
	i=1
	h_avg=0
	s_avg=0
	v_avg=0
	h_min=180
	h_max=0
	s_min=255
	s_max=0
	v_min=255
	v_max=0
	
	for frame in camera.capture_continuous(rawCapture, format="bgr", use_video_port=True):
		# grab the raw NumPy array representing the image, then initialize the timestamp
		# and occupied/unoccupied text
		#print("copy array")
		image = frame.array
		hsv = cv2.cvtColor(image, cv2.COLOR_BGR2HSV)
		average_colour = [hsv[:, :, c].mean() for c in range(hsv.shape[-1])]
		#average_colour = hsv[capture_res[0]/2,capture_res[1]/2]
		print(average_colour, i)
		rawCapture.truncate(0)
		if  i==1:
			h_avg = average_colour[0]
			s_avg = average_colour[1]
			v_avg = average_colour[2]
		else:
			h_avg = (i-1)/float(i)*h_avg + average_colour[0]/float(i)
			s_avg = (i-1)/float(i)*s_avg + average_colour[1]/float(i)
			v_avg = (i-1)/float(i)*v_avg + average_colour[2]/float(i)
			
		current_h_min = hsv[:,:,0].min()
		current_h_max = hsv[:,:,0].max()
		current_s_min = hsv[:,:,1].min()
		current_s_max = hsv[:,:,1].max()
		current_v_min = hsv[:,:,2].min()
		current_v_max = hsv[:,:,2].max()
		if current_h_min < h_min: h_min = current_h_min
		if current_h_max > h_max: h_max = current_h_max
		if current_s_min < s_min: s_min = current_s_min
		if current_s_max > s_max: s_max = current_s_max
		if current_v_min < v_min: v_min = current_v_min
		if current_v_max > v_max: v_max = current_v_max
		
		i = i + 1
		if i >= 60:
			average_colour[0] = h_avg
			average_colour[1] = s_avg
			average_colour[2] = v_avg
			print(average_colour)
			return [h_min,s_min,v_min], [h_max,s_max,v_max]
			break;

# That's it for the camera setup.
average_colour=(0,0,0)
sleep(0.1)

then = 0
now = 0
end_time = 0

# For colour matching testing:
# Helper for converting a decimal <r,g,b> triple into an integer [b,g,r] array:
def bgr(r,g,b):
		return [int(round(b*255)), int(round(g*255)), int(round(r*255))]

blue_lo = bgr(0.1, 0.24, 0.4)
blue_hi = bgr(0.2, 0.4, 0.6)
lower = numpy.array(blue_lo, dtype="uint8")
upper = numpy.array(blue_hi, dtype="uint8")

# What about HSV, which worked pretty well for the original SimpleCV attempt?
# OpenCV docs: For HSV, Hue range is [0,179], Saturation range is [0,255] and Value range is [0,255]
# Goal blue H was about 102/180, S=190/255, V=50/255 (or 50/100?)
goal_hsv = (104.3136,251.977,89.199)
#goal_hsv = average_colour
#goal_hsv = (average_colour[0], 127, 127)
print(average_colour[0])
print(goal_hsv)
goal_hsv_tolerance = (12, 3, 100)
#goal_hsv_tolerance = (12,127,127)
# Um, does bad stuff happen if the range results in a min below 0 or a max above 255?
# Should the range be applied additively or multiplicatively?
def lower(hsv, hsv_tolerance):
	return [float(hsv[0])-hsv_tolerance[0], float(hsv[1])-hsv_tolerance[1], float(hsv[2])-hsv_tolerance[2]]
def upper(hsv, hsv_tolerance):
	return [float(hsv[0])+hsv_tolerance[0], float(hsv[1])+hsv_tolerance[1], float(hsv[2])+hsv_tolerance[2]]

#goal_hsv_lo = numpy.array(lower(goal_hsv, goal_hsv_tolerance), dtype="uint8")
#goal_hsv_hi = numpy.array(upper(goal_hsv, goal_hsv_tolerance), dtype="uint8")
calibration = calibrate_target_colour()
print calibration
goal_hsv_lo = numpy.array(calibration[0], dtype="uint8")
goal_hsv_hi = numpy.array(calibration[1], dtype="uint8")

print("Press 'q' to quit...")
# , resize=resized_res
for frame in camera.capture_continuous(rawCapture, format="bgr", use_video_port=True):
	now = time()
	elapsed_time = now  - then
	sys.stderr.write('Frame rate: ' + str(round(1.0/elapsed_time,1)) + ' Hz\n')

	# grab the raw NumPy array representing the image, then initialize the timestamp
	# and occupied/unoccupied text
	#print("copy array")
	image = frame.array

	hsv = cv2.cvtColor(image, cv2.COLOR_BGR2HSV)
	# Remember, it's (B,G,R) otherwise!
	mask = cv2.inRange(hsv, goal_hsv_lo, goal_hsv_hi)
	#output = cv2.bitwise_and(image, image, mask=mask)

	# show the matching area:
	#print("show masked frame")
	cv2.imshow("Frame", mask)
	#print(" wait >")
	key = cv2.waitKey(16) & 0xFF
	# if the `q` key was pressed, break from the loop
	if key == ord("q"):
		break
	
	# clear the stream in preparation for the next frame
	#print("truncating...")
	rawCapture.truncate(0)
 
	then = now
	
	#print("end loop")
	#end_time = time()

