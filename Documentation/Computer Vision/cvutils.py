# A collection of handy functions built on SimpleCV for our robotics projects

# Do we need to import SimpleCV?  I assume the import statement will ignore/fail gracefully if asked to import something that's already been imported...
import SimpleCV

# Some colour space conversion functions:

def hue_from_angle(degrees): return degrees / 360.0 * 255
def sat_from_percent(percent): return percent / 100.0 * 255
# Actually doesn't give the right numbers?!  TODO: test and correct...


# Generate a solid colour image, matching the resolution and format of the supplied image:
# (I couldn't see any way to create a new blank image using Image)

def solid(image, colour):
	solid = image.copy()
	solid[0:solid.width, 0:solid.height] = colour
	return solid

# Various also-rans follow...

#def solid(image, colour):
#	solid=image.copy()
#	solid.drawRectangle(0,0,solid.width,solid.height, color=colour)
#	return solid
# No! drawRectangle draws only the border, not a solid area!

# Oh, wait, you pass Image() an (x,y) dimension tuple when creating it.
#def solid_(template_image, colour):
#	solid = Image(template_image.width,template_image.height)
#	solid[0:solid.width, 0:solid.height] = colour
#	return solid
# Bah, problems with numpy array format...

#def solid__(width, height, colour):
#	solid = Image(width,height)
#	solid[0:width, 0:height] = colour
#	return solid

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



# The following code relates to white balance correction (it will often be preferable to set the camera to a fixed white-balance, and correct for it in software in a more controlled and uniform fashion).

# This function corrects an image's white balance based on a sample "grey" point:
# (The calibrate_white_balance() function below can be used to sample a "supposed-to-be-grey" pixel by clicking with the mouse)

def wb(image,grey_sample):
	grey=np.mean(grey_sample)
	r_corr = grey / grey_sample[0]
	g_corr = grey / grey_sample[1]
	b_corr = grey / grey_sample[2]
	#print('r_corr=' + str(r_corr) + ', g_corr=' + str(g_corr) + ', b_corr=' + str(b_corr))
	r,g,b=image.splitChannels()
	r = r * float(r_corr)
	g = g * float(g_corr)
	b = b * float(b_corr)
	return Image.mergeChannels(r,r,g,b)

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
	display=Display()
	camera=Camera()
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
			image.drawCircle((x,y),rad=5,color=Color.VIOLET,thickness=1)
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
grey_sample = calibrate_white_balance()
while True:
	wb(camera.getImage(), grey_sample).show()



# Some functions for identifying which regions of the image match a certain colour.  Let's try with black first:

def find_black(image):
	v,s,h = image.toHSV().splitChannels()
	dark = v.binarize(75)	# had 90 on ProBook cam
	grey = s.binarize(90)
	return (dark/16) * (grey/16)

# The same, but with parameters for value and saturation thresholds:

def find_black(image,v_thresh,s_thresh):
	v,s,h = image.toHSV().splitChannels()
	dark = v.binarize(v_thresh)	# had 90 on ProBook cam
	grey = s.binarize(s_thresh)
	return (dark/16) * (grey/16)

# It wouldn't be too hard to identify which regions of the image are (close enough to) grey as well, using just the saturation channel...
# ...although consider that specular highlights will basically appear white/light grey no matter what the actual hue of the surface

def find_grey(image,s_thresh):
	v,s,h = image.toHSV().splitChannels()
	return(s.binarize(s_thresh))


# For finding colours, we might be able to do reasonably well just by examining the hue channel, and perhaps the saturation as well.
# TODO: refine saturation matching. I suspect we want to match any saturation below the threshold, not just within a range, because of specular highlights.
# Specular reflections will likely cause problems, though, as parts of the image will have a very low saturation, high value, and likely completely wrong hue (the hue for greys defaults to red).

# TODO: might be able to speed this up a little by converting things to greyscale images..?
# Image.toGrey() or Image.greyscale()?!

# Note that we don't have to invert() here, when going from the colour distance to proximity to the target hue/saturation, because, bizarrely, binarize() seems to do that as well.

def find_colour(image, hue, hue_thresh, sat, sat_thresh):
	colour_hue = (hue,hue,hue)
	colour_sat = (sat,sat,sat)
	v,s,h = image.toHSV().splitChannels()
	hue_proximity = h.colorDistance(colour_hue).binarize(hue_thresh)
	sat_proximity = s.colorDistance(colour_sat).binarize(sat_thresh)
	return ((hue_proximity/16.0) * (sat_proximity/16.0))

# Also, note that there is a hueDistance() function as well, which might be more efficient than using colorDistance() in these sorts of situations.
# Presumably there is also a saturationDistance() and valueDistance()...?


# Here's a helper function for finding the target hue, hue threshold, saturation, and saturation threshold for matching a colour.
# TODO: implement a clickable colour picker using Display, similar to calibrate_white_balance() above.  Could infer the target centre and thresholds automatically, based on the range of colours selected.

def calibrate_colour_finder():
	image = camera.getImage()
	image.show()
	highlight=solid(image, (255,0,255))
	v,s,h = image.toHSV().splitChannels()
	target_hue = 0
	target_sat = 0
	hue_threshold=0
	sat_threshold=0
	# Hue:
	response = raw_input('\nTarget Hue (or Enter when found) => ')
	while response != '':
		# TODO: handle bogus response robustly...
		target_hue = int(response)
		match = h.colorDistance((target_hue,target_hue,target_hue)).invert()
		((image * 0.1) + (match * 0.1) + (match.binarize(250).invert()/255 * highlight)).show()
		response = raw_input('\nTarget Hue => ')
	response = raw_input('\nHue Threshold => ')
	while response != '':
		# TODO: handle bogus response robustly...
		hue_threshold=int(response)
		#print(type(hue_threshold))
		# Gah, binarize() and it's built-in invert behaviour!! >:^p
		match = h.colorDistance((target_hue,target_hue,target_hue)).binarize(hue_threshold)
		((image * 0.1) + (match/255 * highlight)).show()
		response = raw_input('\nHue Threshold => ')
	response = raw_input('\nTarget Saturation (or Enter when found) => ')
	while response != '':
		# TODO: handle bogus response robustly...
		target_sat = int(response)
		match = s.colorDistance((target_sat,target_sat,target_sat)).invert()
		((image * 0.1) + (match * 0.1) + (match.binarize(250).invert()/255 * highlight)).show()
		response = raw_input('\nTarget Saturation => ')
	response = raw_input('\nSaturation Threshold => ')
	while response != '':
		# TODO: handle bogus response robustly...
		sat_threshold=int(response)
		match = s.colorDistance((target_sat,target_sat,target_sat)).binarize(sat_threshold)
		((image * 0.1) + (match/255 * highlight)).show()
		response = raw_input('\nSaturation Threshold => ')
	print('Target hue:', target_hue)
	print('Hue threshold:', hue_threshold)
	print('Target saturation:', target_sat)
	print('Saturation threshold:', sat_threshold)
	return (target_hue, hue_threshold, target_sat, sat_threshold)

# Try it out:
calibrate_colour_finder()


# Trial hue distances:
h.colorDistance((54,54,54)).show()
# Trial hue threshold:
h.colorDistance((54,54,54)).binarize(48).show()
# Likewise for saturation:
s.colorDistance((150,150,150)).show()
s.colorDistance((150,150,150)).binarize(160).show()

