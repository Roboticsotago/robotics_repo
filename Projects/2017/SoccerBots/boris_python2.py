import serial
import time
import sys
from threading import Thread

serinput = serial.Serial('/dev/serial/by-id/usb-Arduino__www.arduino.cc__0042_852313632363516031B2-if00', 115200)
seroutput = serial.Serial('/dev/serial/by-id/usb-DSpace__www.dspace.org.nz__DSpace_Robot_2.0_55234313335351A0D161-if00', 19200)
left_motor_string = 0b10000000 #first bit 1 means motor message
right_motor_string = 0b11000000 #second bit 1 means right motor
kicker_mask = 0b00000000 #first bit -  0 means kicker message
speed_threshold = 0.4 #deadzone threshold
intercept_time = 1 #time for intercepting the ball

distance_threshold = 0.8

ball_near = 0
homed = 0
inside_goal_zone = 0
goal_detected = 0
ball_detected = 0
ball_angle = 0
ball_distance = 0
hall_effect = 0
rangee = 0
compass_heading = 0
calibration_mode = 0
colour = 0
voltage = 0
angle_to_goal = 0
blob_size = 0

def read_sensors(): #function which constantly reads sensors
	while True:
		serinput.flush()
		line = str(serinput.readline()).strip(' ;\r\n ').split(" ")
		#print(len(line))
		#print(line)
		global ball_near, homed, inside_goal_zone, goal_detected, ball_detected, ball_angle, ball_distance, hall_effect, rangee, compass_heading, calibration_mode, colour, voltage, angle_to_goal, blob_size
		try:
			ball_detected = line[0]
			ball_angle = float(line[1])
			ball_distance = float(line[2])
			hall_effect = line[3]
			rangee = float(line[4])
			compass_heading = float(line[5])
			calibration_mode = line[6]
			colour = float(line[7])
			voltage = float(line[8])
			time.sleep(0.1)
		except(serial.SerialException, ValueError, IndexError) as error:
			#time.sleep(1)
			#print(error)
			pass
			
		#calculate and set ball side variable
		if ball_angle<900:
			if compass_heading-ball_angle<-180:
				temp_ball_side = compass_heading-ball_angle+360
			else:
				temp_ball_side = compass_heading-ball_angle
			if abs(temp_ball_side)<90:
				ball_side = 1
			else:
				ball_side = 0
		
		#print("voltage: " + str(voltage))
		#read angle to goal and blob size from stdin		
		stdin_line = sys.stdin.readline().strip(";").split()
		
		try:
			angle_to_goal = float(stdin_line[0].strip(";"))
		except(IndexError) as error:
			#print(error)
			pass
		try:
			blob_size = stdin_line[1]
		except(IndexError) as error:
			#print(error)
			pass
		
		#print("angle to goal: " + angle_to_goal)
		
		#decide how close the ball is
		#TODO: see if battery voltage affects the ball distance
		if ball_distance < distance_threshold:
			ball_near = True
		else:
			ball_near = False
			
		
		#calculate whether the goal is detected
		if angle_to_goal != -180:
			goal_detected = 1
		else:
			goal_detected = 0
			
		#calculate whether we are inside the goal zone
		if colour > 800:
			inside_goal_zone = True
		else:
			inside_goal_zone = False
		
		
		#calculate whether we are homed
		if (inside_goal_zone==1 and goal_detected==1 and rangee<25 and abs(compass_heading)<90):
			homed = True
		else:
			homed = False
				
		#print("homed in thread " + str(homed))
		#print("gd: " + str(goal_detected) + " in gz: " +  str(inside_goal_zone) + " range: " + str(rangee) + " cmps: " + str(compass_heading))
		
		serinput.flushInput()
			
reading_of_sensors = Thread(target=read_sensors)
reading_of_sensors.start() #start thread to read sensors constantly


def clip(x): #clip x within the range -1..1
	if x<-1:
		return -1
	elif x>1:
		return 1
	else:
		return x
		
def homing():
	global homed
	#print("homing: " + str(homed))
	while homed == False:
		#print(angle_to_goal)
		if angle_to_goal < 10 and angle_to_goal > -10:
			motor_output(-0.5, -0.5)
		if angle_to_goal < -11:
			motor_output(-0.3, -0.7)
		elif angle_to_goal > 11:
			motor_output(-0.7, -0.3)
			
		
def deadzone(speed, threshold):
    if speed>0:
        out = speed + threshold
    elif speed<0:
        out = speed - threshold
    elif speed==0:
		out = 0
    return clip(out)
        
def speed_to_bin(speed): #convert speed to binary number
	speed = deadzone(speed, speed_threshold)
	if speed>=0:
		dir_bit_string = 0b00100000
	else:
		dir_bit_string = 0b00000000
		
	speedbits = (int(abs(speed)*255))>>3
	return dir_bit_string | speedbits 
	
def left_motor(speed):
	return speed_to_bin(speed) | left_motor_string

def right_motor(speed):
	return speed_to_bin(speed) | right_motor_string		


def send_byte(i):
	# For debugging:
	#print(bin(i))
	# Output for real:
	seroutput.write(chr(i))
		
def motor_output(left_speed, right_speed): #drive both motors with left_speed and right_speed as the left and right motor speeds
	send_byte(int(left_motor(left_speed*(voltage*-0.5 + 5))))
	send_byte(int(right_motor(right_speed*(voltage*-0.5 + 5))))
	
	
def turn_to_ball():
	#read_sensors()
	#print(ball_angle)
	while ball_angle != 0:
		if ball_angle<0:	
			#print("turn left")
			motor_output(-0.5,0.5)
		elif ball_angle>0:
			#print("turn right")
			motor_output(0.5,-0.5)
		else:
			#print("0!")
			motor_output(0,0)

def intercept_ball():
	motor_output(1,1)	
	time.sleep(intercept_time)
	motor_output(-0.75,-0.75)
	time.sleep(intercept_time)
	motor_output(0,0)

while True:	
	"""if ball_near == False:
		print("turning to ball")
		turn_to_ball()
		motor_output(0,0)
	elif ball_near == True:
		print("intercepting ball")
		intercept_ball()
		
	if homed == True:
		print("homed")
		pass
	elif homed == False:
		print(rangee)
		print(colour)
		print("homing")
		homing()"""
#	homing()
	motor_output(0,0)
	motor_output(-0.8,-0.2)
	time.sleep(1)
	motor_output(0,0)
	motor_output(-0.2,-0.8)
	time.sleep(1)
			
	
