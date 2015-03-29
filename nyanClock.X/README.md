# nyanClock Firmware

##Caution: Firmware is still under development!
Firmware may not build or be really, really fraught with bugs.  If you are going to use some of this source, make sure you know what you are doing!

##Summary
This is the firmware for the nyanClock.  It looks after time keeping, displaying the seven segment display, MIDI communication to the VS1053B instrument synth and LED lighting.
The target is the PIC18F27J13 microcontroller.

##Building
There are two ways you can build this code.  The easiest way is the to use MPLABX but a command line option is also available.

###MPLABX
Open the nyanClock.X project with MPLABX and click build (after setting nyanClock as the main project)

###Command Line
Go to the nyanClock.X directory.  Run the make utility by typing:
    
    $make -f nbproject/Makefile-default.mk SUBPROJECTS=.build-conf

To build the source for debugging, run the make utility again but type the following:

    $make -f nbproject/Makefile-default.mk dist/default/debug/nyanClock.X.debug.cof

MPLABX automatically generates the "Makefile-default.mk" file and uses this when builing the code.  ".cof" files are the executable used for debugging and when using MPLABX, they are always stored in /dist/default/debug/.  
When adding source files into the project, it is strongly recommended to use MPLABX to create the files.  This ensures that the makefile is automatically updated to include the new source file.  You can also manually add the file and the relevant compiler flags, but I strongly discourage this practice!

##Debugging
There are two ways to run the project in Debug.  The easiest (and perhaps the best) way is to use MPLABX.  A command line option is also available using the MDB debugger.  Using the command line option can take some time to set up if you haven't done so already.  Refer to the [MDB documentation](https://microchip.wikidot.com/mplabx:mdb) for details.  Make sure that you have a debugger ready to use.
*Note: The PIC18F27J13 is not supported by the PICKit2*

###MPLABX
Open the nyanClock.X project in MPLABX.  Click on "Debug Project"

###Command Line
Ensure that you have set your PATH to include "mdb.sh"  Once you have done that, launch mdb from a terminal window:

    $mdb.sh

Set the device to the PIC18F27J13:

    >Device PIC18F27J13

If you are powering the microcontroller with the debug kit, make sure that you power it with 3V3:

    >set poweroptions.powerenable true
    >set voltagevalue 3.3

Set the debugger to the type that you are using (for example, I use the PICKit 3):

    >Hwtool PICKit3

Program the microcontroller with the debug .cof file, nyanClock.X.debug.cof found in nyanClock.X/dist/default/debug/

    >Program nyanClock.X/dist/default/debug/nyanClock.X.debug.cof

Note: the path will depend on where you start MDB from

Finally, run the firmware:

    >run

To set a breakpoint, type the following before running,

    >Break filename: linenumber