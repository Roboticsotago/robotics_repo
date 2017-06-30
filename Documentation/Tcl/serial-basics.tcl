# Tcl serial port basics
# In Tcl, serial communication is part of a large I/O (Input/Output) framework centred around channels.  File I/O and network I/O also uses channels.

# The general procedure is to open a channel, read from and/or write to it, then close when done.
# You can configure an existing channel using the "chan configure" command.


# Tcl version on the SoccerBot Pi systems is 8.6

set SERIAL_DEVICE /dev/serial/by-id/usb-Arduino__www.arduino.cc__0042_852313632363516031B2-if00

# Open the channel and store the channel identifier ("handle") for future reference:
set serial_channel [open $SERIAL_DEVICE RDONLY]

# Configure the channel:
chan configure $serial_channel -mode 115200,n,8,1 -translation auto -buffering none -blocking 0

# You can read a line of text on demand like so:
gets $serial_channel

# A nicer way is to use Tcl's event loop, and set up an event handler that is called whenever the channel has something to read:

# Callback function:
proc read_serial channel {
	puts [gets $channel]
	if {[eof $channel]} {
		close $channel
		set ::done true
	}
}

# Set up the callback:
chan event $serial_channel readable [list read_serial $serial_channel]

# Enter the event loop (exit when done)
vwait ::done


# That's it for the basics.  We might to want automatically reconnect when the channel is closed or otherwise encounters an error.

proc connect ... {
}

proc disconnect {channel} {
	catch {chan close $channel}
}

proc reconnect {channel} {
	warning "Reconnecting to $channel..."
	catch {chan close $channel}
	connect ...
}
