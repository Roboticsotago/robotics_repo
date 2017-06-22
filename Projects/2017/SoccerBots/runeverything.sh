#!/bin/sh
pd -noaudio -nomidi motor-control.pd attacker_main.pd sensor_input.pd &
python2.7 initial_blob_tracking.py | pdsend 7001 &
cu -l ttyUSB0 -s 115200 | pdsend 7000
#TODO: change ttyUSB0 to device ID
