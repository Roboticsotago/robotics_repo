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

Could we just add .ino to the list of C++ suffixes in filetype_extensions.conf?

	Yes, but then it just inherits the C++ build/toolchain commands, which we don't want.

Create a new:
filedefs/filetypes.ino
	NOTE: can include a build-menu section (see above)

Create a new:
templates/files/sketch.ino

MIME type:
text/x-arduino
