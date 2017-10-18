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
	
	
def clip8bit(x):
	if isinstance(x, int):
		if x < 0:
			return 0
		elif x > 255:
			return 255
		else: 
			return x
	elif isinstance(x, list):
		for y in range(len(x)):
			if x[y] < 0:
				x[y] = 0
			elif x[y] > 255:
				x[y] = 255
			else: 
				x[y] = x[y]
		return x
	

#goal_hsv_lo = numpy.array(lower(goal_hsv, goal_hsv_tolerance), dtype="uint8")
#goal_hsv_hi = numpy.array(upper(goal_hsv, goal_hsv_tolerance), dtype="uint8")
calibration = calibrate_target_colour()
print calibration
for x in range(3):
	calibration[0][x] = calibration[0][x] - 10
	calibration[1][x] = calibration[1][x] + 10
print calibration

clipped_cal = (clip8bit(calibration[0]),clip8bit(calibration[1]))

print clipped_cal

goal_hsv_lo = numpy.array(clipped_cal[0], dtype="uint8")
goal_hsv_hi = numpy.array(clipped_cal[1], dtype="uint8")
print goal_hsv_lo
print goal_hsv_hi


print("Press 'q' to quit...")
# , resize=resized_res
for frame in camera.capture_continuous(rawCapture, format="bgr", use_video_port=True):
	rawCapture.truncate(0)
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
	
	for keypoint in keypoints:
		print keypoint
		print "Size:", keypoint.size
		print "Co-ordinates:", keypoint.pt
	print "Number of blobs", len(keypoints)
	

	# Draw detected blobs as red circles.
	# cv2.DRAW_MATCHES_FLAGS_DRAW_RICH_KEYPOINTS ensures the size of the circle corresponds to the size of blob
	mask_with_keypoints = cv2.drawKeypoints(mask, keypoints, numpy.array([]), (0,0,255), cv2.DRAW_MATCHES_FLAGS_DRAW_RICH_KEYPOINTS)
	
	# Show keypoints
	cv2.imshow("Keypoints", mask_with_keypoints)
	# Show hsv image
	cv2.imshow("HSV", hsv)
	# Show raw image
	cv2.imshow("Raw Image", image)
	
	blob_size = 0
	largest_blob_index = 0
	n=0
	
	if len(keypoints) == 0:
		print "No blobs detected!"
		key = cv2.waitKey(16) & 0xFF
		if key == ord("q"):
			break
		continue
		
	for keypoint in keypoints:
		if keypoint.size > blob_size:
			blob_size = keypoint.size
			largest_blob_index = n
		n = n+1
		
	largest_blob = keypoints[largest_blob_index]
	print largest_blob_index
	
	if numpy.isnan(largest_blob.pt[0]) | numpy.isnan(largest_blob.pt[1]):
		print "Not a Number"
		continue

	
	#Show largest blob
	
	mask_with_largest_blob = cv2.drawKeypoints(mask, [largest_blob], numpy.array([]), (0,0,255), cv2.DRAW_MATCHES_FLAGS_DRAW_RICH_KEYPOINTS)
	
	blob_x = largest_blob.pt[0]
	blob_y = largest_blob.pt[1]
	print blob_x, blob_y
	
	cv2.line(mask_with_largest_blob,(int(blob_x),0),(int(blob_x),96),(255,0,0),2)	
	cv2.line(mask_with_largest_blob,(0,int(blob_y))	,(128,int(blob_y)),(0,255,0),2)
	
	cv2.imshow("Largest Blob", mask_with_largest_blob)
	
	
	
	#print(" wait >")
	key = cv2.waitKey(16) & 0xFF
	# if the `q` key was pressed, break from the loop
	if key == ord("q"):
		break
	
	# clear the stream in preparation for the next frame
	#print("truncating...")
	
 
	then = now
	
	#print("end loop")
	#end_time = time()

