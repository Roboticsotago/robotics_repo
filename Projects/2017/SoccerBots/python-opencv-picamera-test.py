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

target_framerate = 10
# 0.08 s frame capture with 50 Hz target


# Had originally thought 1640 x 922 (scaled down to 820 x 461) would be suitable
# However: "PiCameraResolutionRounded: frame size rounded up from 820x461 to 832x464"
# Perhaps 1640 x 1232 capture, scaled down by 4 to 410 x 308?
# Nope: "PiCameraResolutionRounded: frame size rounded up from 410x308 to 416x320"
# 640 x 360 for 16:9?
# Argh: "PiCameraResolutionRounded: frame size rounded up from 640x360 to 640x368" (360 is not divisible by 16)
capture_res = (1640,922)
#capture_res = (3280,2464)
#capture_res = (1640,1232)
capture_res = (640,480)
capture_res = (320,240)
capture_res = (128,96)
# Nope, even then: PiCameraResolutionRounded: frame size rounded up from 3280x2464 to 3296x2464
# Hmm, but at full res, we can't capture faster than 15 Hz.
# Argh, 1640 is not divisible by 16 either!: "PiCameraResolutionRounded: frame size rounded up from 1640x1232 to 1664x1232"
# So maybe we have to capture at full res (3280x2464)? At least those are div by 16...
#resized_res = (capture_res[0]/4, capture_res[1]/4)
#resized_res = (640,480)
#resized_res = (1640,1232)
# But we need a resolution where x and y are both divisible by 16 that has a usefully scaled down version that's also divisible by 16.
# I think given that the capture resolutions are so limited, don't worry about those or the integer scaling part.
# But we will need the resized resolution to be the correct aspect ratio and divisble by 16 in both axes.
# Ah, here are some:
# 16:9 modes:
resized_res = (768,432)
#resized_res = (512,288)
resized_res = (256,144)
resized_res = (640,480)
resized_res = (320,240)
resized_res = (128,96)
# 4:3 modes:
#resized_res = (512,384)
#camera = PiCamera()

#camera = PiCamera(sensor_mode=4, resolution=capture_res, framerate=target_framerate)
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

# That's it for the camera setup.

#camera.start_preview(alpha=127)
#sleep(60)

# array wrapper:
rawCapture = PiRGBArray(camera)
# If resizing, you likely have to tell the array how big to be:
#rawCapture = PiRGBArray(camera, size=resized_res)

sleep(0.1)

def capture_still():
	# The camera provides different ports for still images and video, specified via use_video_port. Specify False for better quality still images, or True when streaming video.
	camera.capture(rawCapture, format="bgr", use_video_port=False, resize=resized_res)
	image = rawCapture.array
	cv2.imshow("Image", image)
	# Image doesn't display until this:
	cv2.waitKey(0)

	# Oh, wait, do we have to clear it before continuing?!
	rawCapture.truncate(0)
	# Yes, that was it!

#capture_still()

#exit()

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
goal_hsv = (102, 190, 100)
goal_hsv_tolerance = (12, 65, 100)
# Um, does bad stuff happen if the range results in a min below 0 or a max above 255?
# Should the range be applied additively or multiplicatively?
def lower(hsv, hsv_tolerance):
	return [float(hsv[0])-hsv_tolerance[0], float(hsv[1])-hsv_tolerance[1], float(hsv[2])-hsv_tolerance[2]]
def upper(hsv, hsv_tolerance):
	return [float(hsv[0])+hsv_tolerance[0], float(hsv[1])+hsv_tolerance[1], float(hsv[2])+hsv_tolerance[2]]

goal_hsv_lo = numpy.array(lower(goal_hsv, goal_hsv_tolerance), dtype="uint8")
goal_hsv_hi = numpy.array(upper(goal_hsv, goal_hsv_tolerance), dtype="uint8")

# capture frames from the camera
# CME: can we specify resize with use_video_port=True? Seems so
# Hmm, why is the video capture noticeably dimmer than the still?
# And why is the frame rate so jittery? Hmm, it seems to be slow to capture the next frame every second frame. Setting the capture frame rate low makes it more obvious.
# Hmm, it's not just every second frame that's pausing...and how long the pause is when capturing the new frame depends on the timing of the loop!
# Sometimes there's a delay of over 2 s while restarting the loop!
# Ah-ha: looks like it's the resizing that causes the jitter.
# Note that when use_video_port=False, reading each image takes about 0.3 s!
print("Press 'q' to quit...")
# , resize=resized_res
for frame in camera.capture_continuous(rawCapture, format="bgr", use_video_port=True):
#while True:
	#camera.capture_sequence(rawCapture, format="bgr", use_video_port=True, resize=resized_res)
	#print(time() - end_time)
	#print("start loop")
	now = time()
	elapsed_time = now  - then
	sys.stderr.write('Frame rate: ' + str(round(1.0/elapsed_time,1)) + ' Hz\n')

	# grab the raw NumPy array representing the image, then initialize the timestamp
	# and occupied/unoccupied text
	#print("copy array")
	image = frame.array
	#image = rawCapture.array
 
	# show the frame
	#print("show raw frame")
	#cv2.imshow("Frame", image)
	#print(" wait >")
	#key = cv2.waitKey(16) & 0xFF
	# if the `q` key was pressed, break from the loop
	#if key == ord("q"):
	#	break
	
	# Let's try some basic colour matching:
	# Should we convert to HSV first?
	# Hmm, it seems that the chroma resolution is quite poor at certain camera settings (low res)...or maybe it's just the lower res exaggerating it.
	#print("BGR -> HSV...")
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
