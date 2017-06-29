#!/bin/sh
pd -noaudio -nomidi motor-control.pd attacker_main.pd sensor_input.pd &
sleep 5
python2.7 blob_tracking.py | pdsend 7001 & 
cu -l /dev/serial/by-id/usb-1a86_USB2.0-Serial-if00-port0 -s 115200 | pdsend 7000
#TODO: change ttyUSB0 to device ID
