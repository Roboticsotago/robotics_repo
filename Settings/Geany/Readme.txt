(Better) Arduino integration in Geany
CME 2016-07-18

Geany seems to have two kinds of file type handling:
 - Built-in filetypes
 - Custom filetypes

Custom filetypes are simpler to define, but less powerful.

Built-in filetypes 
 

Tags: I think these are things like function names that Geany can index for ease of navigation using the Symbols panel.

tag_parser: e.g. "C"

build_settings, build-menu sections 

http://www.geany.org/manual/dev/index.html#custom-filetypes

http://www.geany.org/manual/dev/index.html#build-menu-configuration

http://geany.org/manual/dev/#symbols-and-tags-files

--

Files to perhaps edit:

~/.config/geany/filetype_extensions.conf

	Add Arduino file type to the Programming group.

<<
...
[Extensions]
# <CME>
Arduino=*.ino;*.pde;
# </CME>
...
[Groups]
# <CME>
Programming=Arduino;Clojure;CUDA;Cython;Genie;Scala;
# </CME>
...
>>

Alternatively, could we just add .ino to the list of C++ suffixes in filetype_extensions.conf?

	Yes, but then it just inherits the C++ build/toolchain commands, which we don't want.

Create a new:
filedefs/filetypes.ino
	NOTE: can include a build-menu section (see above)

Create a new:
templates/files/sketch.ino

MIME type:
text/x-arduino


Files added (to be installed):
~/.config/geany/filetype_extensions.conf
~/.config/geany/filedefs/filetypes.Arduino.conf
~/.config/geany/templates/files/sketch.ino

Hmm, it's reading the file_type_extensions.conf file (cos if I add *.pde to the C++ list it gets set as that type), but it's ignoring my Arduino definition line from the same.

<<
sbis4114:geany admin$ /Applications/Geany.app/Contents/MacOS/geany -v 
Geany-INFO: Geany 1.28, en_US.UTF-8
Geany-INFO: GTK 2.24.30, GLib 2.48.0
Geany-INFO: System data dir: /Applications/Geany.app/Contents/Resources/share/geany
Geany-INFO: User config dir: /Users/admin/.config/geany
Geany-INFO: System plugin path: /Applications/Geany.app/Contents/Resources/lib/geany
Geany-INFO: Added filetype Clojure (61).
Geany-INFO: Added filetype CUDA (62).
Geany-INFO: Added filetype Cython (63).
Geany-INFO: Added filetype Genie (64).
Geany-INFO: Added filetype Graphviz (65).
Geany-INFO: Added filetype JSON (66).
Geany-INFO: Added filetype Scala (67).
Geany-INFO: Could not find filetype 'Arduino'.
Geany-INFO: Filetype 'Arduino' not found for group 'Programming'!
Geany-INFO: /Users/admin/.config/geany/filetype_extensions.conf : Conf (UTF-8)
>>

Ah, it needs to be called filetypes.Arduino.conf

[ ] Problem: New file should be called sketch.ino, but it's getting named as untitled.ino
[ ] Symbols (function names, constants, etc.) are not being picked up, even when using C as the file type.  Ah, they don't get scanned until you save the file!

--

Geany/ino/Arduino on Mac OS X

Sebastian discovered that ino doesn't work with the latest version of the Ardunio IDE installation, on which it depends.

lsusb equivalent on Mac OS X:

https://github.com/jlhonora/lsusb

ioreg -p IOUSB -l -w 0 -x -c IOUSBDevice -r

	-p -> I/O kit plane (dev type)
	-l -> list properties (including handy attributes such as idVendor, idProduct, "USB Vendor Name", "USB Product Name", bDeviceClass, bDeviceSubClass
	-w 0 -> no text wrapping
	-x -> show numbers as hexadecimal
	-a -> archive format (relatively nasty XML output)
	-c IOUSBDevice -> limit to IOUSBDevice classes (and subclasses)
	-r -> root subtrees according to filter criteria

Ugh, will still have to parse a lump of text to get the useful data out...

system_profiler SPUSBDataType

Would be preferable not to require installation of additional scripts/programs/packages.


/dev/cu.usbmodem1a21
/dev/tty.usbmodem1a21
USBDroid	20A0:4150


Tcl OS detection:

if {$tcl_platform(os) eq "Darwin"} {...}


# New toplevel for .boards? Or just use the main "." frame?

toplevel .boards
wm title .boards {Available Arduino Devices}


label .boards.label -text {Choose the device to program:}
pack .boards.label -expand 0 -fill x

# NOTE: column #0 is always the tree column (even if not shown).
ttk::treeview .boards.tree -columns {<tree> Device USB_ID Model Type} -selectmode browse -show headings
pack .boards.tree -expand 1 -fill both
.boards.tree heading #0 -anchor w -text <tree>
.boards.tree heading #1 -anchor w -text Device:
.boards.tree heading #2 -anchor w -text {USB ID:}
.boards.tree heading #3 -anchor w -text {Model ID:}
.boards.tree heading #4 -anchor w -text {Device Type:}

# To insert a row:
.boards.tree insert {} end -text the_text -values [list dev id model type]

# A more realistic one:
.boards.tree insert {} end -text NoMatter -values [list /dev/ttyACM0 dead:beef uno {Arduino Uno}]

# TODO: column widths

# TODO: scrolling

# Buttons
button .boards.cancel_button -text Cancel -command {}
button .boards.ok_button -text OK -command {}

pack .boards.ok_button .boards.cancel_button -side right


