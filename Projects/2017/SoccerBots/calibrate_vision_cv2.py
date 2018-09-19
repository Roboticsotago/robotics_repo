#!/usr/bin/env python2.7
#Calibration of the camera to do when lighting conditions change
import sys
def debug(msg):
	sys.stderr.write(str(msg) + "\n")

from time import sleep
execfile("camera_setup.py")
debug("calibrating gain...")
sleep(2)
camera.exposure_mode = 'off'
debug("camera.analog_gain = " + str(camera.analog_gain))

# Now fix the white balace:
debug("Hold a white card in front of the camera now!")
sleep(5)
g = camera.awb_gains
debug("Thanks; camera.awb_gains = " + str(g))
camera.awb_mode = 'off'
camera.awb_gains = g
calibration_frames = 240

def calibrate_target_colour():
	debug("Calibrating target colour")
	debug("Put the target colour in the capture frame")
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
		debug(str(average_colour) + str(i))
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
		
		if i >= calibration_frames:
			average_colour[0] = h_avg
			average_colour[1] = s_avg
			average_colour[2] = v_avg
			debug(average_colour)
			return [h_min,s_min,v_min], [h_max,s_max,v_max]
			break;
		
		
calibrated_colour = calibrate_target_colour()	
			
calibration_file_name = "camera_calibration_state_cv2.py"

def file_output(file_name, white_balance, target_colour_min_max):
	filex = open(file_name, "w")
	filex.write("calibrated_white_balance = " + str(white_balance) + "\n")
	filex.write("calibrated_goal_colour = " + str(target_colour_min_max) + "\n")
	filex.close()
	
file_output(calibration_file_name, g, calibrated_colour)
