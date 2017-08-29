#!/usr/bin/env tclsh

# Test program for the DSpace board motor control protocol

puts stderr "Starting up..."

if {[info hostname] == "Boris"} {
	puts stderr "Robot: Goalie"
	set SERIAL_DEVICE /dev/serial/by-id/usb-DSpace__www.dspace.org.nz__DSpace_Robot_2.0_55234313335351A0D161-if00
} else {
	puts stderr "Robot: Attack"
	set SERIAL_DEVICE /dev/serial/by-id/TODO
}

# Define a start procedure to connect to the serial device
proc connect {serial_device} {
	puts stderr "connecting to $serial_device"
	
	# Open the channel and store the channel identifier ("handle") for future reference:
	while {[catch {set serial_channel [open $serial_device RDWR]}]} {
		puts stderr "Couldn't open $serial_device, retrying in 1 second"
		after 1000
	}
	
	# Configure the channel:
	chan configure $serial_channel -mode 115200,n,8,1 -translation binary -buffering none -blocking 0
	
	# Set up the callback:
	chan event $serial_channel readable [list read_serial $serial_channel]
	
	return $serial_channel
}

# Define a callback procedure, called whenever the serial channel has data available to read:
proc read_serial channel {
	global SERIAL_DEVICE
	set line [gets $channel]
	if {$line != ""} {
		puts $line
		# TODO: handle pipe closure, e.g. restarting
		puts -nonewline stderr .	;# Indicate activity
	}
	if {[eof $channel]} {
		puts stderr "end of file on serial channel"
		close $channel
		connect $SERIAL_DEVICE
	}
}

proc send {binary_string} {
	puts -nonewline	 $::serial_channel [binary format B8 $binary_string]
}

proc run_tests {} {
	# Kicker:
	send 00000001
	after 1000
	send 00000000
	after 1000
	
	# Left motor:
	send 10111111
	after 2000
	send 10011111
	after 2000
	send 10000000
	
	# Right motor:
	send 11111111
	after 2000
	send 11011111
	after 2000
	send 11000000
	
	set ::finished 1
}

set ::serial_channel [connect $SERIAL_DEVICE]

puts stderr "About to run tests..."
after 1000 run_tests

# Enter the event loop (exit when done)
vwait ::finished
close $::serial_channel
puts stderr "All done."
