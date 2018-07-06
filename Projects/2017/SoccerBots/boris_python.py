import serial
import time
serinput = serial.Serial('/dev/serial/by-id/usb-Arduino__www.arduino.cc__0042_852313632363516031B2-if00', 115200)
seroutput = serial.Serial('/dev/serial/by-id/usb-DSpace__www.dspace.org.nz__DSpace_Robot_2.0_55234313335351A0D161-if00', 19200)
left_motor_mask = 0b10000000 #first bit 1 means motor message
right_motor_mask = 0b11000000 #second bit 1 means right motor
kicker_mask = 0b00000000 #first bit 0 means kicker message
speed_threshold = 0.4 #deadzone threshold


ball_angle=0

def readsensors():
    line = str(serinput.readline()).strip(' ;\r\n ').split(" ")
    print(len(line))
    print(line)
    try:
        ball_detected=line[0]
        ball_angle=line[1]
        ball_distance=line[2]
        hall_effect=line[3]
        rangee=line[4]
        compass_heading=line[5]
        calibration_mode=line[6]
        colour=line[7]
    except(serial.SerialException, ValueError, IndexError) as error:
        time.sleep(1)


def motoroutput(binary_input1, binary_input2):
    if binary_input1 is not None:
        x = left_motor(binary_input1).strip("0b")
        print(x)
        seroutput.write(str(x).encode())
    if binary_input2 is not None:
        y = right_motor(binary_input2).strip("0b")
        print(y)
        seroutput.write(str(y).encode())
    else:
        pass


def deadzone(speed, threshold):
    if(speed > 0):
        out = speed + threshold
    elif(speed < 0):
        out = speed - threshold
    if(out<-1):
        return -1
    elif(out > 1):
        return 1
    else:
        return out

def left_motor(speed): # input -1 to +1
    speed = deadzone(speed, speed_threshold)
    if speed >= 0:
        dir_bit_mask = 0b00100000
    else:
        dir_bit_mask = 0b00000000
    speedbits = (int(abs(speed)*255))>>3
    return bin(dir_bit_mask | speedbits | left_motor_mask)

def right_motor(speed):
    speed = deadzone(speed, speed_threshold)
    if speed >= 0:
        dir_bit_mask = 0b00100000
    else:
        dir_bit_mask = 0b00000000
    speedbits = (int(abs(speed)*255))>>3
    return bin(dir_bit_mask | speedbits | right_motor_mask)

def kicker(kick):
    return bin(kicker_mask | kick)


while True:
    motoroutput(-1,-1)
    #readsensors()
    #print(ball_angle)
