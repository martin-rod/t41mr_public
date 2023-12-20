cmake_minimum_required(VERSION 3.10)
project(t41sdt C CXX)

message("CMAKE_CURRENT_LIST_DIR:${CMAKE_CURRENT_LIST_DIR}")

set(TDUINO "${CMAKE_CURRENT_LIST_DIR}/teensy/hardware/avr/1.58.1")
set(TDUINO_CORE "${TDUINO}/cores/teensy4/")

include(${CMAKE_CURRENT_LIST_DIR}/cmake/teensy.cmake)

message(STATUS "CMAKE_BUILD_TYPE:${CMAKE_BUILD_TYPE}")

if (CMAKE_BUILD_TYPE MATCHES "Debug")
    #add_definitions(-DDEBUG)
elseif (CMAKE_BUILD_TYPE MATCHES "Release")
    #add_definitions(-DNDEBUG)
else ()
    message(FATAL_ERROR "Invalid Cmake build type specified. Options are Debug, Release")
endif ()

#add_compile_definitions(VERSION=1)

string(APPEND CODE_QUALITY_FLAGS " ")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${CODE_QUALITY_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CODE_QUALITY_FLAGS}")

if (CMAKE_BUILD_TYPE MATCHES "Debug")
    message(STATUS "CMAKE_BUILD_TYPE:${CMAKE_BUILD_TYPE}")
    #string(APPEND BUILD_TYPE_FLAGS " " )
elseif (CMAKE_BUILD_TYPE MATCHES "Release")
    message(STATUS "CMAKE_BUILD_TYPE:${CMAKE_BUILD_TYPE}")
    #string(APPEND BUILD_TYPE_FLAGS "-g ")
else ()
    message(FATAL_ERROR "Invalid Cmake build type specified. Options are Debug, Release")
endif ()

string(APPEND CMAKE_C_FLAGS "${BUILD_TYPE_FLAGS}")
string(APPEND CMAKE_CXX_FLAGS "${BUILD_TYPE_FLAGS}")

string(APPEND CMAKE_C_FLAGS "${SANITIZER_FLAGS}")
string(APPEND CMAKE_CXX_FLAGS "${SANITIZER_FLAGS}")

teensy_set_dynamic_properties()

string(APPEND CMAKE_C_FLAGS "${C_COMPILE_FLAGS}")
string(APPEND CMAKE_CXX_FLAGS "${CPP_COMPILE_FLAGS}")
string(APPEND CMAKE_EXE_LINKER_FLAGS "${LINK_FLAGS}")

message("CMAKE_C_FLAGS:${CMAKE_C_FLAGS}")
message("CMAKE_CXX_FLAGS:${CMAKE_CXX_FLAGS}")
message("CMAKE_EXE_LINKER_FLAGS:${CMAKE_EXE_LINKER_FLAGS}")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")

list(APPEND T41_SDT_SRC "SDTVer049.2K/Bearing.cpp")
list(APPEND T41_SDT_SRC "SDTVer049.2K/Button.cpp")
list(APPEND T41_SDT_SRC "SDTVer049.2K/ButtonProc.cpp")
list(APPEND T41_SDT_SRC "SDTVer049.2K/CW_Excite.cpp")
list(APPEND T41_SDT_SRC "SDTVer049.2K/CWProcessing.cpp")
list(APPEND T41_SDT_SRC "SDTVer049.2K/Demod.cpp")
list(APPEND T41_SDT_SRC "SDTVer049.2K/Display.cpp")
list(APPEND T41_SDT_SRC "SDTVer049.2K/DSP_Fn.cpp")
list(APPEND T41_SDT_SRC "SDTVer049.2K/EEPROM.cpp")
list(APPEND T41_SDT_SRC "SDTVer049.2K/Encoders.cpp")
list(APPEND T41_SDT_SRC "SDTVer049.2K/Exciter.cpp")
list(APPEND T41_SDT_SRC "SDTVer049.2K/FFT.cpp")
list(APPEND T41_SDT_SRC "SDTVer049.2K/Filter.cpp")
list(APPEND T41_SDT_SRC "SDTVer049.2K/FIR.cpp")
list(APPEND T41_SDT_SRC "SDTVer049.2K/Freq_Shift.cpp")
list(APPEND T41_SDT_SRC "SDTVer049.2K/Menu.cpp")
list(APPEND T41_SDT_SRC "SDTVer049.2K/MenuProc.cpp")
list(APPEND T41_SDT_SRC "SDTVer049.2K/MyConfigurationFile.h")
list(APPEND T41_SDT_SRC "SDTVer049.2K/Noise.cpp")
list(APPEND T41_SDT_SRC "SDTVer049.2K/Process2.cpp")
list(APPEND T41_SDT_SRC "SDTVer049.2K/Process.cpp")
list(APPEND T41_SDT_SRC "SDTVer049.2K/SDT.h")
list(APPEND T41_SDT_SRC "SDTVer049.2K/SDTVer049.2K.ino")
list(APPEND T41_SDT_SRC "SDTVer049.2K/Tune.cpp")
list(APPEND T41_SDT_SRC "SDTVer049.2K/Utility.cpp")

list(APPEND T41_ARDUINO_LIB "${TDUINO}/libraries/Wire")
list(APPEND T41_ARDUINO_LIB "${TDUINO}/libraries/SPI")
list(APPEND T41_ARDUINO_LIB "${TDUINO}/libraries/Audio")
list(APPEND T41_ARDUINO_LIB "${TDUINO}/libraries/SD/src")
list(APPEND T41_ARDUINO_LIB "${TDUINO}/libraries/SdFat/src")
list(APPEND T41_ARDUINO_LIB "${TDUINO}/libraries/SerialFlash")
list(APPEND T41_ARDUINO_LIB "${TDUINO}/libraries/Time")
list(APPEND T41_ARDUINO_LIB "${TDUINO}/libraries/Metro")
list(APPEND T41_ARDUINO_LIB "${TDUINO}/libraries/Bounce")
list(APPEND T41_ARDUINO_LIB "${TDUINO}/libraries/RA8875/src")
list(APPEND T41_ARDUINO_LIB "${TDUINO}/libraries/EEPROM")

list(APPEND T41_ARDUINO_LIB "${PROJECT_SOURCE_DIR}/libraries/Adafruit_BusIO")
list(APPEND T41_ARDUINO_LIB "${PROJECT_SOURCE_DIR}/libraries/Adafruit-GFX-Library")
list(APPEND T41_ARDUINO_LIB "${PROJECT_SOURCE_DIR}/libraries/OpenAudio_ArduinoLibrary")
list(APPEND T41_ARDUINO_LIB "${PROJECT_SOURCE_DIR}/libraries/Rotary")
list(APPEND T41_ARDUINO_LIB "${PROJECT_SOURCE_DIR}/libraries/Si5351Arduino/src")

list(APPEND T41_ARDUINO_LIB "${TDUINO_CORE}")

include_directories(${T41_ARDUINO_LIB})

teensy_add_library(Core "${TDUINO_CORE}")

teensy_add_library(Wire "${TDUINO}/libraries/Wire")
teensy_add_library(SPI "${TDUINO}/libraries/SPI")
teensy_add_library(Audio "${TDUINO}/libraries/Audio" utility)
teensy_add_library(SD "${TDUINO}/libraries/SD/src")
teensy_add_library(SdFat "${TDUINO}/libraries/SdFat/src" common DigitalIO ExFatLib FatLib FsLib iostream SdCard SpiDriver)
teensy_add_library(SerialFlash "${TDUINO}/libraries/SerialFlash")
teensy_add_library(Time "${TDUINO}/libraries/Time")
teensy_add_library(Metro "${TDUINO}/libraries/Metro")
teensy_add_library(Bounce "${TDUINO}/libraries/Bounce")
teensy_add_library(RA8875 "${TDUINO}/libraries/RA8875/src")
teensy_add_library(EEPROM "${TDUINO}/libraries/EEPROM")

teensy_add_library(Adafruit_BusIO "${PROJECT_SOURCE_DIR}/libraries/Adafruit_BusIO")
teensy_add_library(Adafruit-GFX-Library "${PROJECT_SOURCE_DIR}/libraries/Adafruit-GFX-Library")
teensy_add_library(OpenAudio_ArduinoLibrary "${PROJECT_SOURCE_DIR}/libraries/OpenAudio_ArduinoLibrary")
teensy_add_library(Rotary "${PROJECT_SOURCE_DIR}/libraries/Rotary")
teensy_add_library(Si5351Arduino "${PROJECT_SOURCE_DIR}/libraries/Si5351Arduino/src")

teensy_add_executable(t41sdt ${T41_SDT_SRC})
target_include_directories(t41sdt PUBLIC ${T41_ARDUINO_LIB})

#depends on the order SPI
target_link_libraries(t41sdt
  Adafruit_BusIO
  Adafruit-GFX-Library
  OpenAudio_ArduinoLibrary
  Rotary Si5351Arduino
  Wire
  Audio
  SD
  SdFat
  SPI
  SerialFlash
  Time Metro
  Bounce
  RA8875
  EEPROM
  Core
  arm_cortexM7lfsp_math
  m
)

message("CMAKE_BINARY_DIR:${CMAKE_BINARY_DIR}")
target_link_directories(t41sdt PUBLIC ${CMAKE_BINARY_DIR})

