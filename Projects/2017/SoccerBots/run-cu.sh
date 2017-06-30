#!/bin/sh
# NOTE: this should no longer be needed!  See read_sensors.tcl instead.
# Runs the cu pipeline to stream the Mega's sensor readings to the Pd control program for the SoccerBots.
# NOTE: will have to change the USB device name for the goalie!
#DEVICE=/dev/serial/by-id/usb-1a86_USB2.0-Serial-if00-port0
DEVICE=/dev/ttyUSB0
whoami
echo "Starting cu | pdsend pipeline..."
echo $USER >> /home/pi/run-cu-user.txt
echo $$ > /home/pi/soccerbot-cu.pid
/usr/bin/cu -l $DEVICE -s 115200 | /usr/bin/pdsend 7000
#stty -F /dev/ttyUSB0 raw -echo 115200

