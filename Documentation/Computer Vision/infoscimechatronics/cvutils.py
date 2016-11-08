# A collection of handy functions built on SimpleCV for our robotics projects

# TODO: sort out image.show() vs image.save(display) pros/cons and scaling weirdness.

# We need this, obviously:
import SimpleCV
import numpy
import time

# Some colour space conversion functions:
# These might be handy when converting from an HSV colour reported by GIMP to one that SimpleCV can use directly.
def hue_from_angle(degrees): return degrees / 360.0 * 255
def sat_from_percent(percent): return percent / 100.0 * 255
# Actually doesn't give the right numbers?!  TODO: test!


# Generate a solid colour image, matching the resolution and format of the supplied image:
# (I couldn't see any way to create a new blank image using Image - supposedly you can call the constructor with a width and height, but not sure about setting the default colour)

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
# NOTE: assumes RGB - should probably check for that and warn/convert if necessary.
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

def calibrate_white_balance(camera):
	display=SimpleCV.Display()
	#camera=SimpleCV.Camera()
	prev_mouse_state = False
	sample_pixels = []
	image = camera.getImage().scale(0.5)
	image.drawText("Hold mouse button over white/grey", 8,8,color=SimpleCV.Color.YELLOW,fontsize=14)
	image.save(display)
	time.sleep(2)
	print('Press and hold left mouse button over a region that should be white/grey...')
	while display.isNotDone():
		image = camera.getImage().scale(0.5)
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
				return mean_sample # or rinse and repeat?  Maybe not - fn can just be called again if needed.
			prev_mouse_state = False

# Usage:
#camera=SimpleCV.Camera()
#grey_sample = calibrate_white_balance()
#while True:
#	wb(camera.getImage(), grey_sample).show()

# TODO: might be nice to calculate the correct colour temperature so at least the camera could be set to that in a fixed way...might give better colour resolution than doing it in SimpleCV, which seems to have only 8-bit colour depth per channel.


# Function for identifying black regions in the image:
def find_black(image,value_threshold=75,saturation_threshold=90):
	v,s,h = image.toHSV().splitChannels()
	dark = v.binarize(value_threshold)   # had 90 on ProBook cam
	grey = s.binarize(saturation_threshold)
	return (dark/16) * (grey/16)




# Function for calibrating the colour matcher, which will be quite similar to the white-balance sampler.
# Hmm, this should really use WB-corrected images! But how does it know what correction to apply? Might have to add a grey-sample parameter.

def calibrate_colour_match(camera, grey_point):
	display=SimpleCV.Display()
	#camera=SimpleCV.Camera()
	prev_mouse_state = False
	sample_pixels = []
	image = wb(camera.getImage().scale(0.5), grey_point).toHSV()
	image.drawText("Hold mouse button over target", 8,8,color=SimpleCV.Color.YELLOW,fontsize=14)
	image.save(display)
	time.sleep(2)
	print('Press and hold left mouse button over the region whose colour you wish to match...')
	while display.isNotDone():
		image = wb(camera.getImage().scale(0.5), grey_point).toHSV()
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
				# Find the mean hue, saturation, and value...
				sample_pixels_transposed = zip(*sample_pixels)
				mean_sample = tuple(map(lambda y: sum(y) / float(len(y)), zip(*sample_pixels)))
				print('mean sample (HSV): ' + str(mean_sample))
				# ...and also the max and min in order to determine the range to match within each (probably ignore value, ultimately).
				# Could perhaps just take the smaller of the ranges symmetrically around the mean...or the larger...
				# Of course, the min and max are not necessarily equidistant from the mean...maybe better to use the midpoint of the min and max only...though I think it would be nice to recognise the mean somehow...
				# We could get super-fancy and build a probability distribution function, I guess...
				# ...but note that we are kind of assuming that the hue and saturation dimensions are completely independent, which is probably not realistic (even for diffuse reflections?).
				sample_min = tuple(map(min, sample_pixels_transposed))
				sample_max = tuple(map(max, sample_pixels_transposed))
				# Ugh, you can't just subtract two tuples in Python?!
				hue_midpoint = (sample_max[0] + sample_min[0]) / 2.0
				sat_midpoint = (sample_max[1] + sample_min[1]) / 2.0
				val_midpoint = (sample_max[2] + sample_min[2]) / 2.0
				hue_tolerance = sample_max[0] - hue_midpoint
				sat_tolerance = sample_max[1] - sat_midpoint
				val_tolerance = sample_max[2] - val_midpoint
				# Alternatively, mean-based tolerance:
				#hue_tolerance=min(sample_max[0]-mean_sample[0], mean_sample[0]-sample_min[0])
				#sat_tolerance=min(sample_max[1]-mean_sample[1], mean_sample[1]-sample_min[1])
				print('hue midpoint:' + str(hue_midpoint))
				print('hue tolerance:' + str(hue_tolerance))
				print('saturation midpoint:' + str(sat_midpoint))
				print('saturation tolerance:' + str(sat_tolerance))
				print('value midpoint:' + str(val_midpoint))
				print('value tolerance:' + str(val_tolerance))
				# What structure to return? Perhaps a pair of triples, each having the HSV means and thresholds?
				# Or a triple of pairs?
				return ((hue_midpoint,hue_tolerance),(sat_midpoint,sat_tolerance),(val_midpoint,val_tolerance))
				#return mean_sample # or rinse and repeat?
			prev_mouse_state = False


# Hmm, those .5s are suspicious - is something being quantised to integers somewhere?
# Oh, no, it's OK - the min and max will always be integers, therefore the precision of the midpoints will be 0.5.

#h,s,v=sample
#hue_midpoint,hue_tolerance=h
#sat_midpoint,sat_tolerance=s
#v_midpoint,v_tolerance=v

# Can we simply write:


# Nice. Let's do likewise for colour matching:
# TODO: refine saturation matching. I suspect we want to match any saturation below the threshold, not just within a range, because of specular highlights.
def find_colour_1(image, hue, hue_thresh, sat, sat_thresh):
	colour_hue = (hue,hue,hue)
	colour_sat = (sat,sat,sat)
	v,s,h = image.toHSV().splitChannels()
	hue_proximity = h.colorDistance(colour_hue).binarize(hue_thresh)
	sat_proximity = s.colorDistance(colour_sat).binarize(sat_thresh)
	return ((hue_proximity/16.0) * (sat_proximity/16.0))


# Now that we have a function that returns an HSV triple of midpoints and tolerances, let's use those.
# Also, probably more efficient to convert to greyscale somewhere along the line...
def find_colour(image, hsv_midpoints_and_tolerances):
	# Unpack HSV target midpoints and tolerances:
	h,s,v = hsv_midpoints_and_tolerances
	hue_midpoint,hue_tolerance = h
	sat_midpoint,sat_tolerance = s
	#v_midpoint,v_tolerance=v
	# (we ignore value for now)
	colour_hue = (hue_midpoint,hue_midpoint,hue_midpoint)
	colour_sat = (sat_midpoint,sat_midpoint,sat_midpoint)
	v,s,h = image.toHSV().splitChannels(grayscale=True)
	# NOTE: v,s,h at this point are each BGR images..why? Supposedly, Image.splitChannels() has a grayscale parameter, which is True by default.  But even with that explicitly set to true, the resulting images are BGR.
	h = h.toGray()
	s = s.toGray()
	#v = v.toGray()
	# Not sure why the tolerances are coming out too small, but I'm having to multiply them by a minimum of about 3 to get good matching overall.
	hue_proximity = h.colorDistance(colour_hue).binarize(hue_tolerance*3)
	sat_proximity = s.colorDistance(colour_sat).binarize(sat_tolerance*3)
	return ((hue_proximity/16.0) * (sat_proximity/16.0))


# Next we need a way to find the centroid of the matching (white) pixels in an image, e.g. for targeting.

# Also, how much of the horizontal field of vision is occupied by a particular colour, since when the goal is near, the angle is not critical.
# Could probably just use blobs for that, using the bounding box.
