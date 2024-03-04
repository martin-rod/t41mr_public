#! /bin/bash
set -x

BUILD_DIR=$(pwd)

PRJ_DIR='..'
TOOLS_PATH="${PRJ_DIR}/teensy/tools/teensy-tools/1.59.0"
COMPILER_PATH="${PRJ_DIR}/teensy/tools/teensy-compile/11.3.1/arm/bin"
LOADER_PATH="${PRJ_DIR}/teensy_loader_cli"

T41_ELF='t41sdt'
T41_HEX='t41sdt.hex'

${TOOLS_PATH}/teensy_size ${BUILD_DIR}/${T41_ELF}
${TOOLS_PATH}/teensy_secure encrypthex TEENSY41 ${BUILD_DIR}/${T41_HEX}
${TOOLS_PATH}/stdout_redirect ${BUILD_DIR}/${T41_ELF}.lst ${COMPILER_PATH}/arm-none-eabi-objdump -d -S -C ${BUILD_DIR}/${T41_ELF}

${TOOLS_PATH}/teensy_ports -L

#${TOOLS_PATH}/teensy_post_compile -file=${T41_ELF} -path=${BUILD_DIR} -tools=${TOOLS_PATH} -board=TEENSY41
#${TOOLS_PATH}/teensy_post_compile -v -file=t41sdt -path=${BUILD_DIR} -tools=${TOOLS_PATH} -board=TEENSY41 -port="xyz"
# "/sys/devices/pci0000:00/0000:00:14.0/usb2/2-6 /dev/ttyACM1 (Teensy 4.1) Serial"

# 2x ...
${LOADER_PATH}/teensy_loader_cli -v --mcu=TEENSY41 -s -w ${BUILD_DIR}/${T41_HEX}


