# A collection of handy functions built on SimpleCV for our robotics projects

# We need this, obviously:
import SimpleCV
import numpy

# Some colour space conversion functions:

def hue_from_angle(degrees): return degrees / 360.0 * 255
def sat_from_percent(percent): return percent / 100.0 * 255
# Actually doesn't give the right numbers?!


# Generate a solid colour image, matching the resolution and format of the supplied image:
# (I couldn't see any way to create a new blank image using Image)

def solid(image, colour):
	solid = image.copy()
	solid[0:solid.width, 0:solid.height] = colour
	return solid

# Or, now that I know about this:
#def solid(image, colour):
#	solid=image.copy()
#	solid.drawRectangle(0,0,solid.width,solid.height, color=colour)
#	return solid
# No! drawRectangle draws only the border, not a solid area!

# Oh, wait, you pass Image() an (x,y) dimension tuple when creating it.

def solid_(template_image, colour):
	solid = SimpleCV.Image(template_image.width,template_image.height)
	solid[0:solid.width, 0:solid.height] = colour
	return solid
# Bah, problems with numpy array format...

def solid__(width, height, colour):
	solid = SimpleCV.Image(width,height)
	solid[0:width, 0:height] = colour
	return solid


# or would it be better to pass (width,height) as a tuple in our function too?

# e.g.
# image = camera.getImage()
# red = solid(image, (255,0,0))



# Pixel function for highlighting pixels (e.g. to assist in troubleshooting image processing)
# Actually, applying Python-based pixel functions is ridiculously slow, so maybe this is no use...
# Also, scaling the image first seems to bork things.  Maybe different pixel format?  Oh, well..

def highlight(pixels):
	r=pixels[0]
	g=pixels[1]
	b=pixels[2]
	return (int(255*pow(r/255.0, 0.25)), int(255*pow(g/255.0, 0.25)), int(255*pow(b/255.0, 0.25)))



# Correct an image for white balance based on a sample grey point:

def wb(image,grey_sample):
	grey=numpy.mean(grey_sample)
	r_corr = grey / grey_sample[0]
	g_corr = grey / grey_sample[1]
	b_corr = grey / grey_sample[2]
	#print('r_corr=' + str(r_corr) + ', g_corr=' + str(g_corr) + ', b_corr=' + str(b_corr))
	r,g,b=image.splitChannels()
	r = r * float(r_corr)
	g = g * float(g_corr)
	b = b * float(b_corr)
	return SimpleCV.Image.mergeChannels(r,r,g,b)

# Test it:
#camera.live()
## coord: (225,264), color: (158.0, 164.0, 184.0)
#x=225
#y=264
#grey_sample=(158.0, 164.0, 184.0)
#image=camera.getImage()
#image.getPixel(x,y) # Um, row/column transposition?!
#corrected=wb(camera.getImage(), grey_sample)
#corrected.getPixel(x,y)

## How fast (slow!) is it?:

##while True:
#	wb(camera.getImage(), grey_sample).show()

## And with the undistortion?
#while True:
#	wb(camera.getImageUndistort(), grey_sample).show()

## And at reduced resolution?
#while True:
#	wb(camera.getImageUndistort().scale(0.25), grey_sample).show()



# Helper function for setting the white balance manually, based on a series of samples from the camera.

# NOTE: Display.mouseLeft returns the current state of the button, whereas Display.leftButtonDownPosition() returns None unless the button was only JUST pressed. We want Display.mouseX and Display.mouseY instead, I think.

def calibrate_white_balance():
	display=SimpleCV.Display()
	camera=SimpleCV.Camera()
	prev_mouse_state = False
	sample_pixels = []
	print('Press and hold left mouse button over a region that should be white/grey...')
	while display.isNotDone():
		image = camera.getImage()
		image.save(display)
		if display.mouseLeft:
			x = display.mouseX
			y = display.mouseY
			if not prev_mouse_state:
				# Button just pressed - initialise new list:
				sample_pixels = []
				#print(sample_pixels)
			colour_sample = image.getPixel(x,y)
			# Indicate the point being sampled:
			image.drawCircle((x,y),rad=5,color=SimpleCV.Color.VIOLET,thickness=1)
			image.save(display)
			print(str(x) + ', ' + str(y) + ': ' + str(colour_sample))
			sample_pixels.append(colour_sample)
			prev_mouse_state = True
		else:
			if prev_mouse_state:
				# Button just released:
				# Um, how to take the mean of a list of tuples?
				mean_sample = tuple(map(lambda y: sum(y) / float(len(y)), zip(*sample_pixels)))
				print('mean grey sample: ' + str(mean_sample))
				return mean_sample # or rinse and repeat?
			prev_mouse_state = False

# Usage:
#camera=SimpleCV.Camera()
#grey_sample = calibrate_white_balance()
#while True:
#	wb(camera.getImage(), grey_sample).show()
