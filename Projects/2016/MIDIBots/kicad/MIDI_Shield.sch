EESchema Schematic File Version 2
LIBS:power
LIBS:device
LIBS:transistors
LIBS:conn
LIBS:linear
LIBS:regul
LIBS:74xx
LIBS:cmos4000
LIBS:adc-dac
LIBS:memory
LIBS:xilinx
LIBS:special
LIBS:microcontrollers
LIBS:dsp
LIBS:microchip
LIBS:analog_switches
LIBS:motorola
LIBS:texas
LIBS:intel
LIBS:audio
LIBS:interface
LIBS:digital-audio
LIBS:philips
LIBS:display
LIBS:cypress
LIBS:siliconi
LIBS:opto
LIBS:atmel
LIBS:contrib
LIBS:valves
LIBS:dips-s
LIBS:MIDI_Shield-cache
EELAYER 25 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title "Arduino MIDIBot Shield"
Date "3 apr 2016"
Rev ""
Comp "Chris Edwards"
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L PC900V IC1
U 1 1 56E34A6F
P 3700 1550
F 0 "IC1" H 3490 1740 40  0000 C CNN
F 1 "PC900V" H 3880 1360 40  0000 C CNN
F 2 "DIP6" H 3490 1370 30  0000 C CIN
F 3 "~" H 3700 1550 60  0000 C CNN
	1    3700 1550
	1    0    0    -1  
$EndComp
$Comp
L DIPS_04 SW2
U 1 1 56E34B29
P 7400 4050
F 0 "SW2" V 7150 4050 60  0000 C CNN
F 1 "DIPS_04" V 7650 4050 60  0000 C CNN
F 2 "" H 7400 4050 60  0000 C CNN
F 3 "" H 7400 4050 60  0000 C CNN
	1    7400 4050
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR6
U 1 1 56E34C2C
P 7400 4500
F 0 "#PWR6" H 7400 4500 30  0001 C CNN
F 1 "GND" H 7400 4430 30  0001 C CNN
F 2 "" H 7400 4500 60  0000 C CNN
F 3 "" H 7400 4500 60  0000 C CNN
	1    7400 4500
	1    0    0    -1  
$EndComp
Wire Wire Line
	7250 3800 7250 3850
Wire Wire Line
	7350 3800 7350 3850
Wire Wire Line
	7450 3800 7450 3850
Wire Wire Line
	7550 3800 7550 3850
Text Label 7550 3800 1    60   ~ 0
MIDI_CH_1x
Text Label 7450 3800 1    60   ~ 0
MIDI_CH_2x
Text Label 7350 3800 1    60   ~ 0
MIDI_CH_4x
Text Label 7250 3800 1    60   ~ 0
MIDI_CH_8x
Wire Wire Line
	7250 4250 7250 4350
Wire Wire Line
	7250 4350 7550 4350
Wire Wire Line
	7550 4350 7550 4250
Wire Wire Line
	7350 4250 7350 4350
Connection ~ 7350 4350
Wire Wire Line
	7450 4250 7450 4350
Connection ~ 7450 4350
Wire Wire Line
	7400 4350 7400 4500
Connection ~ 7400 4350
$Comp
L DIN_5 P1
U 1 1 56E351B5
P 1800 1650
F 0 "P1" H 1800 1650 70  0000 C CNN
F 1 "MIDI_IN" H 1800 1500 70  0000 C CNN
F 2 "" H 1800 1650 60  0000 C CNN
F 3 "" H 1800 1650 60  0000 C CNN
	1    1800 1650
	1    0    0    -1  
$EndComp
$Comp
L DIN_5 P2
U 1 1 56E351C4
P 4600 3000
F 0 "P2" H 4600 3000 70  0000 C CNN
F 1 "MIDI_THRU" H 4600 2850 70  0000 C CNN
F 2 "" H 4600 3000 60  0000 C CNN
F 3 "" H 4600 3000 60  0000 C CNN
	1    4600 3000
	-1   0    0    -1  
$EndComp
$Comp
L R R1
U 1 1 56E351E3
P 2800 1450
F 0 "R1" V 2880 1450 40  0000 C CNN
F 1 "220R" V 2807 1451 40  0000 C CNN
F 2 "~" V 2730 1450 30  0000 C CNN
F 3 "~" H 2800 1450 30  0000 C CNN
	1    2800 1450
	0    -1   -1   0   
$EndComp
$Comp
L DIODE D1
U 1 1 56E35230
P 3150 1650
F 0 "D1" H 3150 1750 40  0000 C CNN
F 1 "1N914" H 3150 1550 40  0000 C CNN
F 2 "~" H 3150 1650 60  0000 C CNN
F 3 "~" H 3150 1650 60  0000 C CNN
	1    3150 1650
	0    -1   -1   0   
$EndComp
$Comp
L ARDUINO_SHIELD SHIELD1
U 1 1 56E490D8
P 2950 5000
F 0 "SHIELD1" H 2600 5950 60  0000 C CNN
F 1 "ARDUINO_SHIELD" H 3000 4050 60  0000 C CNN
F 2 "" H 2950 5000 60  0000 C CNN
F 3 "" H 2950 5000 60  0000 C CNN
	1    2950 5000
	1    0    0    -1  
$EndComp
Text Label 3900 5600 0    60   ~ 0
MIDI_CH_1x
Text Label 3900 5500 0    60   ~ 0
MIDI_CH_2x
Text Label 3900 5400 0    60   ~ 0
MIDI_CH_4x
Text Label 3900 5300 0    60   ~ 0
MIDI_CH_8x
Text Label 2000 4800 2    60   ~ 0
+5V
Text Label 2000 4600 2    60   ~ 0
RESET
Text Label 2000 4900 2    60   ~ 0
GND
Text Label 2000 5000 2    60   ~ 0
GND
Text Label 3900 4300 0    60   ~ 0
GND
NoConn ~ 2000 4700
Text Label 2000 5100 2    60   ~ 0
+12V
NoConn ~ 3900 4200
Text Label 2000 5800 2    60   ~ 0
SELF_TEST
$Comp
L GND #PWR3
U 1 1 56E493E9
P 4600 1850
F 0 "#PWR3" H 4600 1850 30  0001 C CNN
F 1 "GND" H 4600 1780 30  0001 C CNN
F 2 "" H 4600 1850 60  0000 C CNN
F 3 "" H 4600 1850 60  0000 C CNN
	1    4600 1850
	1    0    0    -1  
$EndComp
$Comp
L R R2
U 1 1 56E49407
P 4600 1300
F 0 "R2" V 4680 1300 40  0000 C CNN
F 1 "280R" V 4607 1301 40  0000 C CNN
F 2 "~" V 4530 1300 30  0000 C CNN
F 3 "~" H 4600 1300 30  0000 C CNN
	1    4600 1300
	1    0    0    -1  
$EndComp
$Comp
L SW_PUSHBUTTON SW1
U 1 1 56E4E614
P 9600 3400
F 0 "SW1" H 9450 3510 50  0000 C CNN
F 1 "SW_PUSHBUTTON" H 9600 3320 50  0000 C CNN
F 2 "" H 9600 3400 60  0000 C CNN
F 3 "" H 9600 3400 60  0000 C CNN
	1    9600 3400
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR8
U 1 1 56E4E625
P 10100 3650
F 0 "#PWR8" H 10100 3650 30  0001 C CNN
F 1 "GND" H 10100 3580 30  0001 C CNN
F 2 "" H 10100 3650 60  0000 C CNN
F 3 "" H 10100 3650 60  0000 C CNN
	1    10100 3650
	1    0    0    -1  
$EndComp
Text Label 9050 3400 2    60   ~ 0
RESET
Wire Wire Line
	9900 3400 10100 3400
Wire Wire Line
	10100 3400 10100 3650
Wire Wire Line
	9300 3400 9050 3400
NoConn ~ 2000 5400
NoConn ~ 2000 5500
NoConn ~ 2000 5600
NoConn ~ 2000 5700
Wire Wire Line
	3050 1450 3350 1450
Wire Wire Line
	2400 1450 2550 1450
Wire Wire Line
	3350 1850 3350 1650
Wire Wire Line
	2400 1850 3350 1850
Wire Wire Line
	2400 1850 2400 1650
Wire Wire Line
	4050 1650 4600 1650
Wire Wire Line
	4600 1650 4600 1850
Wire Wire Line
	4600 1050 4050 1050
Wire Wire Line
	4050 1050 4050 1450
Wire Wire Line
	4050 1550 5100 1550
Text Label 4600 950  2    60   ~ 0
+5V
Wire Wire Line
	4600 1050 4600 950 
Text Notes 6800 2050 0    60   ~ 0
TODO:\n\n[ ] Servo output(s)\n[ ] MOSFET output(s)\n[ ] RX disconnection switch\n[ ] DIN connector grounding (pin 2, one end only? cap? what about chassis ground?)\n[ ] EMI ferrite beads on I/O lines?\n[ ] Polarised SIL connectors for MIDI I/O\n\n[X] 12 V connector (incl. GND)? Just use Arduino barrel jack.\n\n[Y] Reset switch\n[Y] MIDI THRU circuitry (buffer)\n[Y] Self-test button\n[Y] Latency control potentiometer\n[Y] LED + resistor on Pin 13\n
Text Label 5100 1550 0    60   ~ 0
MIDI_IN_TTL
Text Label 3900 5800 0    60   ~ 0
MIDI_IN_TTL
$Comp
L R R3
U 1 1 5700999C
P 3750 2800
F 0 "R3" V 3830 2800 40  0000 C CNN
F 1 "220R" V 3757 2801 40  0000 C CNN
F 2 "~" V 3680 2800 30  0000 C CNN
F 3 "~" H 3750 2800 30  0000 C CNN
	1    3750 2800
	0    -1   -1   0   
$EndComp
Text Label 3500 2700 2    60   ~ 0
+5V
Wire Wire Line
	3500 2800 3500 2700
Text Label 1700 3000 2    60   ~ 0
MIDI_IN_TTL
$Comp
L R R4
U 1 1 57009A3F
P 3750 3000
F 0 "R4" V 3830 3000 40  0000 C CNN
F 1 "220R" V 3757 3001 40  0000 C CNN
F 2 "~" V 3680 3000 30  0000 C CNN
F 3 "~" H 3750 3000 30  0000 C CNN
	1    3750 3000
	0    -1   -1   0   
$EndComp
$Comp
L 74HC14 U1
U 2 1 57009FF1
P 3050 3000
F 0 "U1" H 3200 3100 40  0000 C CNN
F 1 "74HC14" H 3250 2900 40  0000 C CNN
F 2 "~" H 3050 3000 60  0000 C CNN
F 3 "~" H 3050 3000 60  0000 C CNN
	2    3050 3000
	1    0    0    -1  
$EndComp
$Comp
L 74HC14 U1
U 1 1 5700A078
P 2150 3000
F 0 "U1" H 2300 3100 40  0000 C CNN
F 1 "74HC14" H 2350 2900 40  0000 C CNN
F 2 "~" H 2150 3000 60  0000 C CNN
F 3 "~" H 2150 3000 60  0000 C CNN
	1    2150 3000
	1    0    0    -1  
$EndComp
Text Notes 1050 850  0    60   ~ 0
MIDI input
Text Notes 1000 2600 0    60   ~ 0
MIDI thru
$Comp
L SW_PUSHBUTTON SW3
U 1 1 572AEBD4
P 9600 4150
F 0 "SW3" H 9450 4260 50  0000 C CNN
F 1 "SW_PUSHBUTTON" H 9600 4070 50  0000 C CNN
F 2 "" H 9600 4150 60  0000 C CNN
F 3 "" H 9600 4150 60  0000 C CNN
	1    9600 4150
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR9
U 1 1 572AEBDA
P 10100 4400
F 0 "#PWR9" H 10100 4400 30  0001 C CNN
F 1 "GND" H 10100 4330 30  0001 C CNN
F 2 "" H 10100 4400 60  0000 C CNN
F 3 "" H 10100 4400 60  0000 C CNN
	1    10100 4400
	1    0    0    -1  
$EndComp
Text Label 9050 4150 2    60   ~ 0
SELF_TEST
Wire Wire Line
	9900 4150 10100 4150
Wire Wire Line
	10100 4150 10100 4400
Wire Wire Line
	9300 4150 9050 4150
Text Label 2000 5300 2    60   ~ 0
LATENCY_ADJ
$Comp
L LED D2
U 1 1 572AED85
P 7300 5550
F 0 "D2" H 7300 5650 50  0000 C CNN
F 1 "LED" H 7300 5450 50  0000 C CNN
F 2 "" H 7300 5550 60  0000 C CNN
F 3 "" H 7300 5550 60  0000 C CNN
	1    7300 5550
	0    1    1    0   
$EndComp
$Comp
L R R5
U 1 1 572AEEB8
P 7300 5100
F 0 "R5" V 7380 5100 40  0000 C CNN
F 1 "1K" V 7307 5101 40  0000 C CNN
F 2 "" V 7230 5100 30  0000 C CNN
F 3 "" H 7300 5100 30  0000 C CNN
	1    7300 5100
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR5
U 1 1 572AEF85
P 7300 5800
F 0 "#PWR5" H 7300 5800 30  0001 C CNN
F 1 "GND" H 7300 5730 30  0001 C CNN
F 2 "" H 7300 5800 60  0000 C CNN
F 3 "" H 7300 5800 60  0000 C CNN
	1    7300 5800
	1    0    0    -1  
$EndComp
Text Label 3900 4400 0    60   ~ 0
LED
Text Label 7300 4850 0    60   ~ 0
LED
Wire Wire Line
	7300 5800 7300 5750
$Comp
L POT RV1
U 1 1 572AF20F
P 9500 5250
F 0 "RV1" H 9500 5150 50  0000 C CNN
F 1 "10K" H 9500 5250 50  0000 C CNN
F 2 "" H 9500 5250 60  0000 C CNN
F 3 "" H 9500 5250 60  0000 C CNN
	1    9500 5250
	0    -1   -1   0   
$EndComp
Text Label 9200 5250 2    60   ~ 0
LATENCY_ADJ
Text Label 9500 4850 2    60   ~ 0
+5V
$Comp
L GND #PWR7
U 1 1 572AF432
P 9500 5650
F 0 "#PWR7" H 9500 5650 30  0001 C CNN
F 1 "GND" H 9500 5580 30  0001 C CNN
F 2 "" H 9500 5650 60  0000 C CNN
F 3 "" H 9500 5650 60  0000 C CNN
	1    9500 5650
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR2
U 1 1 572AF481
P 1750 7150
F 0 "#PWR2" H 1750 7150 30  0001 C CNN
F 1 "GND" H 1750 7080 30  0001 C CNN
F 2 "" H 1750 7150 60  0000 C CNN
F 3 "" H 1750 7150 60  0000 C CNN
	1    1750 7150
	1    0    0    -1  
$EndComp
Text Notes 1050 6350 0    60   ~ 0
Power
Text Notes 8200 2800 0    60   ~ 0
User interface
Text Label 1750 6800 2    60   ~ 0
GND
Wire Wire Line
	1750 6800 1750 7150
Text Notes 1100 4150 0    60   ~ 0
Arduino shield
Wire Wire Line
	9500 4850 9500 5000
Wire Wire Line
	9500 5500 9500 5650
Wire Wire Line
	9200 5250 9350 5250
Text Notes 8350 5400 0    60   ~ 0
TODO: series resistor?
$Comp
L +5V #PWR1
U 1 1 572B0054
P 1700 6400
F 0 "#PWR1" H 1700 6490 20  0001 C CNN
F 1 "+5V" H 1700 6490 30  0000 C CNN
F 2 "" H 1700 6400 60  0000 C CNN
F 3 "" H 1700 6400 60  0000 C CNN
	1    1700 6400
	1    0    0    -1  
$EndComp
Text Label 1700 6500 0    60   ~ 0
+5V
Wire Wire Line
	1700 6400 1700 6500
Text Notes 800  7450 0    60   ~ 0
TODO: I should probably have used proper power ports from the beginning, rather than labelled ports.
Text Notes 8450 6000 0    60   ~ 0
TODO: IIRC there was another pot planned
Text Notes 8650 3100 0    60   ~ 0
TODO: switch debouncing?
Connection ~ 3150 1450
Connection ~ 3150 1850
Text Notes 2400 6200 0    60   ~ 0
Do we need all these GND labels?\nWon't the parent Arduino board connect these anyway?
$Comp
L GND #PWR4
U 1 1 572B0D3F
P 5500 3300
F 0 "#PWR4" H 5500 3300 30  0001 C CNN
F 1 "GND" H 5500 3230 30  0001 C CNN
F 2 "" H 5500 3300 60  0000 C CNN
F 3 "" H 5500 3300 60  0000 C CNN
	1    5500 3300
	1    0    0    -1  
$EndComp
NoConn ~ 5200 3000
NoConn ~ 4600 2400
Wire Wire Line
	5200 2800 5500 2800
Wire Wire Line
	5500 2800 5500 3300
NoConn ~ 1200 1650
NoConn ~ 1800 1050
Connection ~ 4600 1550
Text Label 3900 5700 0    60   ~ 0
MIDI_OUT_TTL
Text Label 3900 5200 0    60   ~ 0
MOSFET_PWM
Text Label 3900 5100 0    60   ~ 0
STEPPER_DIR
Text Label 3900 4900 0    60   ~ 0
STEPPER_STEP
Text Label 3900 4800 0    60   ~ 0
SERVO_1
Text Label 3900 4700 0    60   ~ 0
SERVO_2
Text Label 3900 4600 0    60   ~ 0
SERVO_3
Text Label 3900 4500 0    60   ~ 0
MOSFET
$EndSCHEMATC
