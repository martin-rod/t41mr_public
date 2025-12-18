# Linux build for T41 Software Defined Transceiver Arduino Sketch

## Features

1. Build T41 with Cmake. Then it is possible to use other IDEs, e.g. Clion.
2. Build T41 on Linux in console.
3. Load to Teensy without Arduiono IDE installation using `teensy_loader_cli`.
4. Build T41 on GitHub Action.
5. Build T41 without Arduiono IDE installation.
6. Tested on Debian 11 `bullseye`, Debian 12 `bookworm` and GitHub `ubuntu-latest`.
7. Build result was tested `T41-EP 4SQRP` kit (v9-v11 AG1P Ron version)

## Issues

1. Build depends on library order in T41_LIB. Some symbol is probably defined two times.

## Requirements

1. Standard `C developer` environment - cmake, make, git, ...
2. Download <https://www.pjrc.com/teensy/00-teensy.rules> the Linux udev rules (link at the top of this page) and copy the file to /etc/udev/rules.d.
   `sudo cp 00-teensy.rules /etc/udev/rules.d/`

## How to build

```
git clone https://github.com/martin-rod/t41mr_public.git
git submodule update --init --recursive
```

### SDT version

```
./create_build_sdt.sh
cd build_sdt
make
load_to_teensy.sh
```

### EEE version

```
./create_build_eee.sh
cd build_eee
make
load_to_teensy.sh
```

Add working copy of T41EEE to the directory T41EEEo
```
git subtree add --prefix=T41EEEo https://github.com/Greg-R/T41EEE main
```

Update working copy of T41EEE to the directory T41EEEo
```
git subtree pull --prefix=T41EEEo https://github.com/Greg-R/T41EEE main
```

CPU speed is set to 528 MHz ...

## T41 sources

<https://groups.io/g/SoftwareControlledHamRadio>

<https://github.com/Greg-R/T41EEE>


## Additional sources

<https://downloads.arduino.cc/arduino-ide/arduino-ide_2.2.1_Linux_64bit.zip>

<https://downloads.arduino.cc/arduino-ide/arduino-ide_2.2.1_Windows_64bit.exe>

<https://www.pjrc.com/teensy/loader.html>

<https://www.pjrc.com/teensy/loader_cli.html>

<https://www.pjrc.com/teensy/td_download.html>

<https://github.com/newdigate/teensy-cmake-macros>

## Arduino IDE linux installation for T41

### Arduino IDE

1. Crete directory (for example in ~/wrk_ham/t41)
```
   mkdir ~/wrk_ham/t41/arduino
   cd ~/wrk_ham/t41/arduino
```

2. Donwload https://downloads.arduino.cc/arduino-ide/arduino-ide_2.2.1_Linux_64bit.zip

Extract to ~/wrk_ham/t41/arduino/. Files will be in ~/wrk_ham/t41/arduino/arduino-ide_2.2.1_Linux_64bit.

3. Run IDE
```
   cd ~/wrk_ham/t41/arduino/arduino-ide_2.2.1_Linux_64bit
   ./arduino-ide
```

Go to menu File/Preferences and add:

Sketchbook location: ~/wrk_ham/t41/arduino/

Additional boards manager URLs: https://www.pjrc.com/teensy/package_teensy_index.json

### Libraries

1. Add lib from IDE (or from git ...)
   Adafruit-GFX-Library (https://github.com/adafruit/Adafruit-GFX-Library)
   This step creates directory `~/wrk_ham/t41/arduino/libraries`
   Adafruit_BusIO as dependencies (https://github.com/adafruit/Adafruit_BusIO.git)

2. `cd ~/wrk_ham/t41/arduino/libraries`

3. Add lib from git
   `git clone https://github.com/chipaudette/OpenAudio_ArduinoLibrary`

4. Add lib from git
   `git clone https://github.com/etherkit/Si5351Arduino`

5. Add lib from git
   `git clone https://github.com/brianlow/Rotary`

6. Add lib from  git for EEE version
   `git clone https://github.com/bblanchon/ArduinoJson.git`

7. Select .ino file, board and build in IDE ...

### Libraries modification

Edit .gitmodules
```
[submodule "libraries/OpenAudio_ArduinoLibrary"]
	path = libraries/OpenAudio_ArduinoLibrary
	url = git@github.com-mr:martin-rod/OpenAudio_ArduinoLibrary.git
	branch = mr_v6_tst
[submodule "libraries/Si5351Arduino"]
	path = libraries/Si5351Arduino
	url = git@github.com-mr:martin-rod/Si5351Arduino
	branch = mr_v6_tst
```

and / or

```
git config --file=.gitmodules submodule.Submod.url https://github.com/username/ABC.git
git config --file=.gitmodules submodule.Submod.branch Development
git submodule sync
git submodule update --init --recursive --remote
or
git submodule update --init --remote libraries/OpenAudio_ArduinoLibrary
```

### T41 flash erase

<https://groups.io/g/SoftwareControlledHamRadio/topic/how_to_complete_flash_erase/104720544>

1. Turn on Teensy before next step. The orange LED must light
2. Press and hold the button on the Teensy (near the orange LED)
3. Release the button after the first flash of the red LED (the red LED is under the Audio Adapter on the Teensy)
4. The red LED stays on for about 30 seconds
5. The orange LED flashes and the Teensy is in factory settings

### Serial port
dmesg

```
[ 9856.898460] usb 1-9.3: USB disconnect, device number 115
[ 9861.742655] usb 1-9.3: new high-speed USB device number 117 using xhci_hcd
[ 9861.855411] usb 1-9.3: New USB device found, idVendor=16c0, idProduct=048b, bcdDevice= 2.80
[ 9861.855426] usb 1-9.3: New USB device strings: Mfr=1, Product=2, SerialNumber=3
[ 9861.855432] usb 1-9.3: Product: Dual Serial
[ 9861.855437] usb 1-9.3: Manufacturer: Teensyduino
[ 9861.855441] usb 1-9.3: SerialNumber: 11111111
[ 9861.860007] cdc_acm 1-9.3:1.0: ttyACM1: USB ACM device
[ 9861.860687] cdc_acm 1-9.3:1.2: ttyACM2: USB ACM device
```

`gtkterm` wiht /dev/ttyACM1 15200 8N1 or `tio -m INLCRNL  /dev/ttyACM1`

### Compiler

#### Inlude

gcc
```
rod@t440-d12:~/wrk_ham/t41/t41mr/build_eee (mr_config_20250420 *$%=)$ echo | /home/rod/wrk_ham/t41/t41mr/teensy/arm-gnu-toolchain-13.2.Rel1-x86_64-arm-none-eabi/bin/arm-none-eabi-gcc -E -Wp,-v -xc /dev/null
ignoring duplicate directory "/home/rod/wrk_ham/t41/t41mr/teensy/arm-gnu-toolchain-13.2.Rel1-x86_64-arm-none-eabi/bin/../lib/gcc/../../lib/gcc/arm-none-eabi/13.2.1/include"
ignoring nonexistent directory "/home/rod/wrk_ham/t41/t41mr/teensy/arm-gnu-toolchain-13.2.Rel1-x86_64-arm-none-eabi/bin/../arm-none-eabi/usr/local/include"
ignoring duplicate directory "/home/rod/wrk_ham/t41/t41mr/teensy/arm-gnu-toolchain-13.2.Rel1-x86_64-arm-none-eabi/bin/../lib/gcc/../../lib/gcc/arm-none-eabi/13.2.1/include-fixed"
ignoring duplicate directory "/home/rod/wrk_ham/t41/t41mr/teensy/arm-gnu-toolchain-13.2.Rel1-x86_64-arm-none-eabi/bin/../lib/gcc/../../lib/gcc/arm-none-eabi/13.2.1/../../../../arm-none-eabi/include"
ignoring duplicate directory "/home/rod/wrk_ham/t41/t41mr/teensy/arm-gnu-toolchain-13.2.Rel1-x86_64-arm-none-eabi/bin/../arm-none-eabi/include"
#include "..." search starts here:
#include <...> search starts here:
 /home/rod/wrk_ham/t41/t41mr/teensy/arm-gnu-toolchain-13.2.Rel1-x86_64-arm-none-eabi/bin/../lib/gcc/arm-none-eabi/13.2.1/include
 /home/rod/wrk_ham/t41/t41mr/teensy/arm-gnu-toolchain-13.2.Rel1-x86_64-arm-none-eabi/bin/../lib/gcc/arm-none-eabi/13.2.1/include-fixed
 /home/rod/wrk_ham/t41/t41mr/teensy/arm-gnu-toolchain-13.2.Rel1-x86_64-arm-none-eabi/bin/../lib/gcc/arm-none-eabi/13.2.1/../../../../arm-none-eabi/include
End of search list.
# 0 "/dev/null"
# 0 "<built-in>"
# 0 "<command-line>"
# 1 "/dev/null"
```

g++
```
rod@t440-d12:~/wrk_ham/t41/t41mr/build_eee (mr_config_20250420 *$%=)$ echo | /home/rod/wrk_ham/t41/t41mr/teensy/arm-gnu-toolchain-13.2.Rel1-x86_64-arm-none-eabi/bin/arm-none-eabi-g++ -E -Wp,-v -xc++ /dev/null
ignoring duplicate directory "/home/rod/wrk_ham/t41/t41mr/teensy/arm-gnu-toolchain-13.2.Rel1-x86_64-arm-none-eabi/bin/../lib/gcc/../../lib/gcc/arm-none-eabi/13.2.1/../../../../arm-none-eabi/include/c++/13.2.1"
ignoring duplicate directory "/home/rod/wrk_ham/t41/t41mr/teensy/arm-gnu-toolchain-13.2.Rel1-x86_64-arm-none-eabi/bin/../lib/gcc/../../lib/gcc/arm-none-eabi/13.2.1/../../../../arm-none-eabi/include/c++/13.2.1/arm-none-eabi"
ignoring duplicate directory "/home/rod/wrk_ham/t41/t41mr/teensy/arm-gnu-toolchain-13.2.Rel1-x86_64-arm-none-eabi/bin/../lib/gcc/../../lib/gcc/arm-none-eabi/13.2.1/../../../../arm-none-eabi/include/c++/13.2.1/backward"
ignoring duplicate directory "/home/rod/wrk_ham/t41/t41mr/teensy/arm-gnu-toolchain-13.2.Rel1-x86_64-arm-none-eabi/bin/../lib/gcc/../../lib/gcc/arm-none-eabi/13.2.1/include"
ignoring nonexistent directory "/home/rod/wrk_ham/t41/t41mr/teensy/arm-gnu-toolchain-13.2.Rel1-x86_64-arm-none-eabi/bin/../arm-none-eabi/usr/local/include"
ignoring duplicate directory "/home/rod/wrk_ham/t41/t41mr/teensy/arm-gnu-toolchain-13.2.Rel1-x86_64-arm-none-eabi/bin/../lib/gcc/../../lib/gcc/arm-none-eabi/13.2.1/include-fixed"
ignoring duplicate directory "/home/rod/wrk_ham/t41/t41mr/teensy/arm-gnu-toolchain-13.2.Rel1-x86_64-arm-none-eabi/bin/../lib/gcc/../../lib/gcc/arm-none-eabi/13.2.1/../../../../arm-none-eabi/include"
ignoring duplicate directory "/home/rod/wrk_ham/t41/t41mr/teensy/arm-gnu-toolchain-13.2.Rel1-x86_64-arm-none-eabi/bin/../arm-none-eabi/include"
#include "..." search starts here:
#include <...> search starts here:
 /home/rod/wrk_ham/t41/t41mr/teensy/arm-gnu-toolchain-13.2.Rel1-x86_64-arm-none-eabi/bin/../lib/gcc/arm-none-eabi/13.2.1/../../../../arm-none-eabi/include/c++/13.2.1
 /home/rod/wrk_ham/t41/t41mr/teensy/arm-gnu-toolchain-13.2.Rel1-x86_64-arm-none-eabi/bin/../lib/gcc/arm-none-eabi/13.2.1/../../../../arm-none-eabi/include/c++/13.2.1/arm-none-eabi
 /home/rod/wrk_ham/t41/t41mr/teensy/arm-gnu-toolchain-13.2.Rel1-x86_64-arm-none-eabi/bin/../lib/gcc/arm-none-eabi/13.2.1/../../../../arm-none-eabi/include/c++/13.2.1/backward
 /home/rod/wrk_ham/t41/t41mr/teensy/arm-gnu-toolchain-13.2.Rel1-x86_64-arm-none-eabi/bin/../lib/gcc/arm-none-eabi/13.2.1/include
 /home/rod/wrk_ham/t41/t41mr/teensy/arm-gnu-toolchain-13.2.Rel1-x86_64-arm-none-eabi/bin/../lib/gcc/arm-none-eabi/13.2.1/include-fixed
 /home/rod/wrk_ham/t41/t41mr/teensy/arm-gnu-toolchain-13.2.Rel1-x86_64-arm-none-eabi/bin/../lib/gcc/arm-none-eabi/13.2.1/../../../../arm-none-eabi/include
End of search list.
# 0 "/dev/null"
# 0 "<built-in>"
# 0 "<command-line>"
# 1 "/dev/null"
```

???
```
/home/rod/wrk_ham/t41/t41mr/teensy/arm-gnu-toolchain-13.2.Rel1-x86_64-arm-none-eabi/arm-none-eabi/include/c++/13.2.1/tr1
/home/rod/wrk_ham/t41/t41mr/teensy/arm-gnu-toolchain-13.2.Rel1-x86_64-arm-none-eabi/arm-none-eabi/include/c++/13.2.1
/home/rod/wrk_ham/t41/t41mr/teensy/arm-gnu-toolchain-13.2.Rel1-x86_64-arm-none-eabi/arm-none-eabi/include
/home/rod/wrk_ham/t41/t41mr/teensy/arm-gnu-toolchain-13.2.Rel1-x86_64-arm-none-eabi/arm-none-eabi/include/c++/13.2.1/arm-none-eabi
/home/rod/wrk_ham/t41/t41mr/teensy/arm-gnu-toolchain-13.2.Rel1-x86_64-arm-none-eabi/arm-none-eabi/include/sys
/home/rod/wrk_ham/t41/t41mr/teensy/arm-gnu-toolchain-13.2.Rel1-x86_64-arm-none-eabi/arm-none-eabi/include/ssp

```

#### Define

From TDUINO:
```
__arm__
__IMXRT1062__
TEENSYDUINO=159
ARDUINO=10607
ARDUINO_TEENSY41
F_CPU=528000000
USB_DUAL_SERIAL
LAYOUT_US_ENGLISH
```

From toolchain:
```
rod@t440-d12:~/wrk_ham/t41/t41mr (maro_trace *$%=)$ echo | /home/rod/wrk_ham/t41/t41mr/teensy/arm-gnu-toolchain-13.2.Rel1-x86_64-arm-none-eabi/bin/arm-none-eabi-gcc  -dM -E - > define_c.txt
rod@t440-d12:~/wrk_ham/t41/t41mr (maro_trace *$%=)$ echo | /home/rod/wrk_ham/t41/t41mr/teensy/arm-gnu-toolchain-13.2.Rel1-x86_64-arm-none-eabi/bin/arm-none-eabi-g++  -dM -E - > define_cpp.txt
```
### QtCreator

The problem is that with the "Ibus" input method, "Ctrl-shift-u" is by default configured to the "Unicode Code Point" shortcut. You can try this: Type ctrl-shift-u, then an (underlined) u appears. If you then type a unicode code point number in hex (e.g. 21, the ASCII/unicode CP for !) and press enter, it is replaced with the corresponding character.

https://superuser.com/questions/358749/how-to-disable-ctrlshiftu

Solution (GUI)

This shortcut can be changed or disabled using the ibus-setup utility:

    Run ibus-setup from the terminal (or open IBus Preferences).
    Go to “Emoji”.
    Next to “Unicode code point:”, click on the three dots (i.e. ...).
    In the dialog, click “Delete”, then “OK”.
    Close the IBus Preferences window.

### Vim

Move trailing comments to the line above:
```
%s:^\(.*\)\s\+\(//.*\):\2\r\1:
```

### Notes

`/home/rod/wrk_ham/t41/t41mr/teensy/tools/teensy-compile/11.3.1/arm/bin/arm-none-eabi-objdump -x libSPI.a | c++filt -n | less -RN`

`git submodule update --init --recursive`

# USB

## CAT



## USB Audio Patches

<https://github.com/K7MDL2/KEITHSDR/wiki/48KHz-USB-Audio>

<https://github.com/laiudm/laiudm-Teensy4-192k-USB-Audio/tree/main>
<https://github.com/laiudm/laiudm-Teensy4-192k-USB-Audio/blob/main/DevelopersGuide.md>

## USB Audio linux commands

List devices:

```
arecord -L
arecord -l
aplay -L
aplay -l
```

Play audio from T41 to PC sound card:
```
arecord -D hw:CARD=Audio2xSerial,DEV=0  -v -f S16_LE -c2 -r192000  - | aplay
```

Record audio from T41 to WAV file:

```
arecord -D hw:CARD=Audio2xSerial,DEV=0  -v -f S16_LE -c2 -r192000  -d 10 -t wav 192k_test.wav
```

Play audio from PC to T41 Teensy Audio Adapter (sgtl5000):

```
aplay -D hw:CARD=Audio2xSerial,DEV=0 -v -f S16_LE -c2 -r192000 192k_test.wav
```

Files for test setup:

```
sandbox/usb_audio.cpp
sandobx/192k_test.wav
```

# Coverity

download Coverity Build Tool

```
https://scan.coverity.com/download/cxx/linux64
```

extract and add to PATH

```
export PATH=/home/rod/wrk_ham/t41/coverity/cov-analysis-linux64-2024.12.1/bin/:$PATH
```

because -fpermissive
```
export COVERITY_UNSUPPORTED_COMPILER_INVOCATION=1
```

run coverity

```
./create_build_eee_o.sh
cd /home/rod/wrk_ham/t41/t41mr/build_eee_o
cov-build --dir cov-int make -j 4
tar czvf t41_eee_o.tgz cov-int
```
and upload ...

```
https://scan.coverity.com/projects/martin-rod-t41mr_public
```

# Memory

from https://www.pjrc.com/store/teensy41.html

![Memory](/doc/teensy41_memory.png "Memory")

## Program / Flash Memory

Teensy 4.1 has 8 Mbyte of flash memory intended for storing your code. The flash memory can also store read-only variables and arrays. A portion of the flash memory may be used for file storage using the LittleFS library. The top 256K of this memory is reserved for EEPROM emulation data and the LED blink restore program.

## RAM

1024K of memory is available for variables and data. Half of this memory (RAM1) is accessed as tightly coupled memory for maximum performance. The other half (RAM2) is optimized for access by DMA. Normally large arrays & data buffers are placed in RAM2, to save the ultra-fast RAM1 for normal variables.

```
<ITCM>  00000000 .. 0000ffff  RAM1
<DTCM>  20000000 .. 2006ffff  RAM1
<RAM>   20200000 .. 2027ffff  RAM2
<FLASH> 60000000 .. 607fffff
<PSRAM> 70000000 .. 707fffff
```

### Static Allocation Keywords

    When the compiler builds your program, all global variables, static variables, and compiled code is assigned to dedicated locations in memory. This is called static allocation, because the memory addresses are fixed. By default, allocation tries to use the ultra-fast DTCM & ITCM memory. The following keywords allow control over where the compiler will place your variables and code within the memory.

        DMAMEM - Variables defined with DMAMEM are placed at the beginning of RAM2. Normally buffers and large arrays are placed here. These variables can not be initialized, your program must write their initial values, if needed.

        EXTMEM - Variables defined with EXTMEM are placed in the optional PSRAM memory chip soldered to the QSPI memory expansion area on bottom side of Teensy 4.1. These variables can not be initialized, your program must write their initial values, if needed.

        PROGMEM & F() - Variables defined with PROGMEM, and strings surrounded by F() are placed only in the flash memory. They can be accessed normally, special functions normally used on 8 bit boards are not required to read PROGMEM variables.

        FASTRUN - Functions defined with "FASTRUN" are allocated in the beginning of RAM1. A copy is also stored in Flash and copied to RAM1 at startup. These functions are accessed by the Cortex-M7 ITCM bus, for the fastest possible performance. By default, functions without any memory type defined are treated as FASTRUN. A small amount of memory is typically unused, because the ITCM bus must access a memory region which is a multiple of 32K.

        FLASHMEM - Functions defined with "FLASHMEM" executed directly from Flash. If the Cortex-M7 cache is not already holding a copy of the function, a delay results while the Flash memory is read into the M7's cache. FLASHMEM should be used on startup code and other functions where speed is not important.

### Dynamic Allocation

    As your program runs, it may use all of the RAM which was not reserved by static allocation. Because the specific memory address for each variable is computed as your program runs, this is called dynamic memory allocation.

        Local Variables - Local variables, and also return addresses from function calls and the saved state from interrupts are placed on a stack which starts from the top of RAM1 and grown downward. The amount of space for local variable is the portion of RAM1 not used by FASTRUN code and the initialized and zeroed variables.

        Heap - Memory allocated by C++ "new" and C malloc(), and Arduino String variables are placed in RAM2, starting immediately after the DMAMEM variables.

        External Heap - If PSRAM has been added, extmem_malloc() may be used to allocate this memory, started immediately after the EXTMEM variables. When no PSRAM is present, extmem_malloc() automatically allocated memory from the normal heap in RAM2.

## RTC RAM

    16 bytes of memory are located within the RTC. If a coin cell is connected to VBAT, contents of this memory is preserved while power is off. This memory is accessed as 32 bit registers LPGPR0-LPGPR3.


# ARM tool chain

https://developer.arm.com/downloads/-/arm-gnu-toolchain-downloads

https://developer.arm.com/Tools%20and%20Software/GNU%20Toolchain#Support-and-Training

https://gcc.gnu.org/onlinedocs/gcc-14.3.0/gcc/Option-Summary.html

# License

*********************************************************************************************

  This comment block must appear in the load page (e.g., main() or setup()) in any source code
  that uses code presented as whole or part of the T41-EP source code.

  (c) Frank Dziock, DD4WH, 2020_05_8
  "TEENSY CONVOLUTION SDR" substantially modified by Jack Purdum, W8TEE, and Al Peter, AC8GY

  This software is made available under the GNU GPLv3 license agreement. If commercial use of this
  software is planned, we would appreciate it if the interested parties contact Jack Purdum, W8TEE,
  and Al Peter, AC8GY.

  Any and all other uses, written or implied, by the GPLv3 license are forbidden without written
  permission from Jack Purdum, W8TEE, and Al Peter, AC8GY.

