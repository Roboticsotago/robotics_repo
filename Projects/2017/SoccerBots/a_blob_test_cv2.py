import cv2
import numpy as np;
import camera_setup

# Read image

execfile("camera_setup.py")
camera.capture(rawCapture, format="bgr")
image = rawCapture.array

im = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)

# Set up the detector with default parameters.

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


# Detect blobs.
keypoints = detector.detect(im)
print keypoints

# Draw detected blobs as red circles.
# cv2.DRAW_MATCHES_FLAGS_DRAW_RICH_KEYPOINTS ensures the size of the circle corresponds to the size of blob
im_with_keypoints = cv2.drawKeypoints(im, keypoints, numpy.array([]), (0,0,255), cv2.DRAW_MATCHES_FLAGS_DRAW_RICH_KEYPOINTS)

largest_blob = [0,0]
x = 0
for key in keypoints:
	if key.size > largest_blob[1]:
		largest_blob[0] = x
		largest_blob[1] = key.size
	x = x+1
	
print largest_blob

# Show keypoints
cv2.imshow("Keypoints", im_with_keypoints)
cv2.waitKey(0)
