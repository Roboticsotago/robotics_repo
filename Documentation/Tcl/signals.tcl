#!/usr/bin/env tclsh

# This program demonstrates how to detect and respond to signals within a Tcl program.

# Signals are a simple inter-process communication (IPC) mechanism available on unix-like systems such as Linux - something like a Facebook "poke" for processes, but with more "flavours".  The process receiving the signal is generally free to choose how to respond, although there are well-defined semantics for most signal types.  Signals are what the `kill` and `killall` commands use to end a given process - using the TERM signal by default, or the KILL signal if you specify the -9 flag.  They are also the mechanism by which typing Ctrl-C or Ctrl-Z in the terminal work (Ctrl-C sends SIGINT - interrupt, and Ctrl-Z sends SIGSTP).

# Signals are also used by the operating system to tell a process when it's done something wrong - such as the infamous segmentation fault signal SEGV when an illegal memory access was attempted.

# By defining your own signal handling routines, you can enhance your program's functionality, for example by ensuring a graceful shutdown when the process is killed.

# You can do other neat stuff with signals, such as tell a process to suspend (pause) or resume its operations, using SIGSTOP and SIGCONT respectively.  Some programs use signals to trigger reloading their configuration files from disk, or to dump out a status report.

# Note that handling signals in Tcl does not require the Tcl event loop to be running.

# Signals can be identified by name or by number.  "kill -l" will list these , and have a look at the Wikipedia entry for a description of each:

# https://en.wikipedia.org/wiki/Unix_signal#POSIX_signals

# Signal names are often seen with the SIG prefix, e.g. SIGSEGV for SEGV.  


# The "signal" command is part of the Tclx extension package, so load that first:
package require Tclx

puts stderr "Signals demo running as process ID [pid]..."

# Here's how to associate a signal with a simple action:
signal trap SIGUSR1 {puts stderr "Got SIGUSR1!"}

# Then, in another shell:
# kill -s USR1 12118 # <-- or whatever PID was reported

# You can generate signals in Tcl as well using Tcl's "kill" command - including sending a signal to yourself!

kill SIGUSR1 [pid]


# For more complex signal handlers, it might be helpful to define a procedure:

proc handle_sigint {} {
	puts stderr "Got SIGINT!"
	# ...do some cleanup...
	puts stderr "Going bye..."
	exit
}

signal trap SIGINT handle_sigint

# You could run the main (infinite) loop of your program, and the signal would still allow it to be exited gracefully.

kill SIGINT [pid]


# You can detect which signal was sent using the substitution that "signal trap" does, replacing %S with the name of the signal:

proc handle_signal {sig} {
	puts stderr "Got signal ${sig}!"
}

signal trap {SIGINT SIGUSR1 SIGUSR2} {handle_signal %S}

kill SIGUSR2 [pid]


# The shell command `pidof` can tell you the PID of a given process name (not necessarily unique).
# kill -s STOP `pidof firefox`

# You can define a couple of shell functions to make this even more convenient:

# pause(){ kill -s STOP `pidof $1`; }
# resume(){ kill -s CONT `pidof $1`; }

# Then you can just do "pause firefox", "resume chromium", etc. :)
