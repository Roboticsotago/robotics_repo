#!/bin/sh

BASENAME=memory-and-EEPROM

pandoc -s --latex-engine=xelatex --variable=geometry:a4paper --include-in-header=fontoptions.tex --include-in-header=fullrule.tex $BASENAME.md -o $BASENAME.pdf
