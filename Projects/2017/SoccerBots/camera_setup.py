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
camera.shutter_speed = 30000
#camera.shutter_speed = 15000


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



