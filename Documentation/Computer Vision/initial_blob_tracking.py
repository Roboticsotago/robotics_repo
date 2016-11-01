import SimpleCV
import sys
sys.path.append('/home/pi/robotics_repo/Documentation/Computer Vision/infoscimechatronics')
import cvutils
import time

camera = SimpleCV.Camera()
os.system('/home/pi/robotics_repo/Projects/2017/SoccerBots/uvcdynctrl-settings.tcl')

lab_grey_sample = cvutils.calibrate_white_balance()
lab_goal_blue = cvutils.calibrate_colour_match(lab_grey_sample)

times = []

def average(numbers):
        x = 0
        for num in numbers:
                x += num
        x = x/len(numbers)
        return x

while True:
        start_time = time.clock()
	image = cvutils.wb(camera.getImage().scale(0.25), lab_grey_sample)
	v,s,h = image.toHSV().splitChannels()
	hue_match = h.colorDistance((lab_goal_blue[0][0],lab_goal_blue[0][0],lab_goal_blue[0][0])).binarize(lab_goal_blue[0][1]*3)
	sat_match = s.colorDistance((lab_goal_blue[1][0],lab_goal_blue[1][0],lab_goal_blue[1][0])).binarize(lab_goal_blue[1][1]*3)
	matched = ((hue_match / 16) * (sat_match / 16))
	matched.show()
	blobs = matched.findBlobs(100, 1)
	if blobs is not None:
		(x,y) = blobs[-1].centroid()
		image.dl().line((x,0), (x,image.height), (255,0,0), antialias=False)
		image.dl().line((0,y), (image.width,y), (0,255,0), antialias=False)
		image.show()
		blob_size = blobs[-1].area()
                image_size = image.area()
                print blob_size / image_size
	else:
		print "No blobs found!"
	end_time = time.clock()
	elapsed_time = end_time - start_time
	times.append(elapsed_time)
	frame_rate = 1 / average(times)
	image.drawText(('Frame rate:' + str(frame_rate)), 0, 0)
	image.show()


        
