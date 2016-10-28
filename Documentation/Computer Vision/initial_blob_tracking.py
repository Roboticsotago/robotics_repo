import SimpleCV
import sys
sys.path.append('.')
import cvutils
import time

camera = SimpleCV.Camera()

lab_grey_sample = (170.53383458646616, 179.7669172932331, 180.33082706766916)
lab_goal_blue = ((103.0, 3.0), (177.0, 11.0), (163.0, 26.0))

while True:
	image = cvutils.wb(camera.getImage().scale(0.25), lab_grey_sample)
	v,s,h = image.toHSV().splitChannels()
	hue_match = h.colorDistance((lab_goal_blue[0][0],lab_goal_blue[0][0],lab_goal_blue[0][0])).binarize(lab_goal_blue[0][1]*3)
	sat_match = s.colorDistance((lab_goal_blue[1][0],lab_goal_blue[1][0],lab_goal_blue[1][0])).binarize(lab_goal_blue[1][1]*3)
	matched = ((hue_match / 16) * (sat_match / 16))
	matched.show()
	blobs = matched.findBlobs()
	largest_blob = blobs[-1]
	largest_blob.draw()
	largest_blob_centroid = largest_blob.centroid()
	largest_blob_centroid.draw()
	image.drawCircle((largest_blob_centroid[0],largest_blob_centroid[1]),rad=5,color=SimpleCV.Color.VIOLET,thickness=1)
	image.show()
	matched.show()
	time.sleep(1)
	
	

