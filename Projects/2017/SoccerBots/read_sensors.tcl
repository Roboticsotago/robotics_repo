#!/usr/bin/env tclsh

# Read sensor data from the Mega's serial port and write it out to standard output (stdout) for piping into pdsend

# This uses Tcl's event loop to copy serial data only when it becomes available (as opposed to a polling approach).

# See robotics_repo/Documentation/Tcl/serial-basics.tcl for further explanation on the use of serial port channels in Tcl.

puts stderr "Serial reader starting up..."

if {[info hostname] == "Boris"} {
	puts stderr "Robot: Goalie"
	set SERIAL_DEVICE /dev/serial/by-id/usb-Arduino__www.arduino.cc__0042_852313632363516031B2-if00
} else {
	puts stderr "Robot: Attack"
	set SERIAL_DEVICE /dev/serial/by-id/usb-1a86_USB2.0-Serial-if00-port0
}

# Open the channel and store the channel identifier ("handle") for future reference:
set serial_channel [open $SERIAL_DEVICE RDONLY]

# Configure the channel:
chan configure $serial_channel -mode 115200,n,8,1 -translation crlf -buffering line -blocking 0


# Define a callback procedure, called whenever the serial channel has data available to read:
proc read_serial channel {
	set line [gets $channel]
	if {$line != ""} {
		puts $line		
	}
	if {[eof $channel]} {
		close $channel
		set ::done true
	}
}

# Set up the callback:
chan event $serial_channel readable [list read_serial $serial_channel]

# Enter the event loop (exit when done)
vwait ::done
