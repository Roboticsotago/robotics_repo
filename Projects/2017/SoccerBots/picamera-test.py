#!/usr/bin/env python
from time import sleep
from picamera import PiCamera

print("Starting camera...")
# Recommended parameters for initialisation: sensor_mode, resolution, framerate, framerate_range, and clock_mode
camera = PiCamera(resolution=(1640,922), framerate=25)
# Pi Camera v2 Modes:
# #2 is 3280 x 2464, full FoV, 4:3
# #4 is 1640 x 1232, full FoV, 4:3
# #5 is 1640 x 922, full H FoV, 16:9
#camera.resolution = (1640, 922)
camera.iso = 400
#camera.start_preview()
# Pause briefly to allow automatic settings to settle:
print("Hold a white card in front of the camera now!")
sleep(5)

# NOTE:
# <<
# Exposure mode 'off' is special: this disables the camera's automatic gain control, fixing the values of digital_gain and analog_gain.
# Please note that these properties are not directly settable (although they can be influenced by setting iso prior to fixing the gains), and default to low values when the camera is first initialized. Therefore it is important to let them settle on higher values before disabling automatic gain control otherwise all frames captured will appear black.
# >>


print("camera.exposure_speed = ",camera.exposure_speed)
# Is this measured in microseconds?
# 1 / (28649 microseconds) is about 35 Hz..?!
#camera.shutter_speed = camera.exposure_speed
# Can't set exposure_speed manually? Only shutter_speed?

print("setting shutter_speed...")
#camera.exposure_speed = 100000
# 1 / (20 Hz) = 50000 microseconds
#camera.shutter_speed = 50000
# 1 / (25 Hz) = 40000 microseconds
#camera.shutter_speed = 40000
# 1 / (50 Hz) = 20000 microseconds
camera.shutter_speed = 20000
# 1 / (100 Hz) = 10000 microseconds
#camera.shutter_speed = 10000

# Give analog and digital gains time to adjust...
print("recalibrating...")
sleep(2)
camera.exposure_mode = 'off'
#print("camera.analog_gain = ",camera.analog_gain)
# ~7.5 indoors under fluorescent lighting
#camera.analog_gain = 8
#camera.analog_gain = Fraction(241, 32)


g = camera.awb_gains
print("camera.awb_gains = ",g)
camera.awb_mode = 'off'
camera.awb_gains = g

#camera.rotation = 180

# TODO: fix focus settings?

print("You can take away the white card now. :)")

#camera.capture('picamera-test.jpg', resize=(820,461))
# .resize()
camera.start_preview(alpha=255)
# Can we use resize with other capture functions as well? Seems so..also with start_recording().
#camera.capture_sequence(['image%02d.jpg' % i for i in range(99)], resize=(820,461))
# What's the best way to capture for sharing with SimpleCV or OpenCV? A shell pipeline? A socket? A NumPy array?
sleep(10)

# Cature to a video file:
# Note that the video file is raw h.264, with no frame rate information!
#camera.start_recording('my_video.h264')
#camera.wait_recording(60)
#camera.stop_recording()
