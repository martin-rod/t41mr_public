### Linux build for T41 Software Defined Transceiver Arduino Sketch

## Features

1. Build T41 with Cmake. Then it is possible to use other IDEs, e.g. Clion.
2. Build T41 on Linux in console.
3. Load to Teensy without Arduiono IDE installation using `teensy_loader_cli`.
4. Build T41 on GitHub Action.
5. Build T41 without Arduiono IDE installation.
6. Tested on Debian 11 `bullseye` and GitHub `ubuntu-latest`.
7. Build result was tested `T41-EP 4SQRP` kit (v9-v11 AG1P Ron version)

## Issues

1. teensy_loader_cli works correctly on the second try. Probably small timeout after reset.
2. Build depends on library order in T41_LIB. Some symbol is probably defined two times.

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

Update working copy T41EEE in directory T41EEEw
```
git subtree add --prefix=T41EEEw https://github.com/Greg-R/T41EEE main
```


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

### Notes

`/home/rod/wrk_ham/t41/t41mr/teensy/tools/teensy-compile/11.3.1/arm/bin/arm-none-eabi-objdump -x libSPI.a | c++filt -n | less -RN`

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

