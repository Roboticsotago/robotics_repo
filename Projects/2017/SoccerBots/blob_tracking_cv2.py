#!/usr/bin/env python2

# Demo/test of the Python OpenCV bindings (wot I built from source) with the picamera (Python-MMAL) module.
# Based on http://www.pyimagesearch.com/2015/03/30/accessing-the-raspberry-pi-camera-with-opencv-and-python/

execfile("camera_setup.py")

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
	# Detect blobs.
	keypoints = detector.detect(mask)
	print keypoints

	# Draw detected blobs as red circles.
	# cv2.DRAW_MATCHES_FLAGS_DRAW_RICH_KEYPOINTS ensures the size of the circle corresponds to the size of blob
	mask_with_keypoints = cv2.drawKeypoints(mask, keypoints, numpy.array([]), (0,0,255), cv2.DRAW_MATCHES_FLAGS_DRAW_RICH_KEYPOINTS)
	
	for x in keypoints:
		print x
	
	# Show keypoints
	cv2.imshow("Keypoints", mask_with_keypoints)
	
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

