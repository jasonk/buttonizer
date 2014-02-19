# Buttonizer #

This is a very simple application that uses a serial cable, a switch and a
couple of LEDs to do control an event on a computer.  It was originally
developed as a panic alarm, after we had a door lock fail in a high-security
facility and it took days to remove the door and gain access the room again.
Luckily nobody was in the room when the door failed, but it would have been
extra bad if they had been, because the facility is high-security enough that
cell phones are not permitted!

You could use this to control other things, but the documentation that follows
will assume you are using it to control a panic switch.

# Serial Cable #

The schematic for the serial cable can be found in
[schematic.png](schematic.png).  It's basically a serial cable with one end
cut off, and has a switch connected to the DCD line, and LEDs connected to the
DTR and RTS lines.  This allows the system to detect button pushes and control
the two LEDs.

# Application #

In addition to the serial cable, you also need the 'buttonizer' command found
in this directory.  Run 'make' to compile it if you don't have it.  The source
code for the application is found in [buttonizer.c](buttonizer.c) and is very
minimal and has no dependencies other than a working compiler.

You run the executable with two arguments: the serial port that the cable is
connected to, and the command you want run when someone presses the button.

# LED Indicators #

When you first start the application the green LED will turn on.  When you
press the button the green LED will turn off and the red LED turns on.  The
command you specified will be run and then one of the LEDs will blink for 30
seconds.  If the green LED blinks then the command ran successfully (presumably
indicating that the alarm was successfuly sent).  If the red LED blinks then
the command failed.

## LED Status Cheat Sheet ##

    Solid Green     - Ready
    Solid Red       - Transmitting Alarm
    Flashing Green  - Alarm transmitted successfully
    Flashing Red    - Alarm transmission failed

# Author / Repo #

    Jason Kohles <email@jasonkohles.com>
    http://github.com/jasonk/buttonizer

# License #

Released under the Creative Commons Attribution Share-Alike 3.0 License
http://creativecommons.org/licenses/by-sa/3.0
