#!/bin/sh
# Runs the cu pipeline to stream the Mega's sensor readings to the Pd control program for the SoccerBots.
# NOTE: will have to change the USB device name for the goalie!
cu -l /dev/serial/by-id/usb-1a86_USB2.0-Serial-if00-port0 -s 115200 | pdsend 7000

