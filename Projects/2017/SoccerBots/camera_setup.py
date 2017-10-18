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
capture_res = (128,96)
#capture_res = (256,192)

# How much of the frame to crop to when doing the calibration sampling?
xfrac = 0.1
yfrac = 0.1

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

params = cv2.SimpleBlobDetector_Params()
# Change thresholds
params.minThreshold = 10;    # the graylevel of images
params.maxThreshold = 200;

params.filterByColor = True
params.blobColor = 255

# Filter by Area
params.filterByArea = False
params.minArea = 10000

params.filterByInertia = False
params.filterByConvexity = False

detector = cv2.SimpleBlobDetector(params)

print(target_framerate)

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
		
		# Crop the central region of interest and convert to HSV colour space:
		xres = capture_res[0]
		yres = capture_res[1]
		# TODO: make a function for these?
		Y1 = int(round(yres * (yfrac + 1) / 2.0))
		Y0 = int(round(yres * (1 - yfrac) / 2.0))
		X0 = int(round(xres * (1 - xfrac) / 2.0))
		X1 = int(round(xres * (xfrac + 1) / 2.0))
		hsv = cv2.cvtColor(image[Y0:Y1,X0:X1], cv2.COLOR_BGR2HSV)
		
		# Draw sampling region on the frame to assist with aiming:
		cv2.rectangle(image,(X0,Y0),(X1,Y1),(0,255,0),2)
		cv2.imshow("Frame",image)
		key = cv2.waitKey(16) & 0xFF

		# Compute the average colour for the sampling region:
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
		if i >= 120:
			average_colour[0] = h_avg
			average_colour[1] = s_avg
			average_colour[2] = v_avg
			print(average_colour)
			return [h_min,s_min,v_min], [h_max,s_max,v_max]
			break;
