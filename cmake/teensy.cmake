set(runtime_ide_version 159)
set(arduino_ide_version 10607)
set(TEENSY_VERSION 41 CACHE STRING "Set to the Teensy version" FORCE)

function(teensy_set_dynamic_properties)
    if (NOT DEFINED teensy_set_dynamic_properties_has_executed)
        set(teensy_set_dynamic_properties_has_executed 1 CACHE INTERNAL "teensy_set_dynamic_properties_has_executed")
        message(STATUS "teensy_set_dynamic_properties()")

        set(CPU_CORE_SPEED 600000000 CACHE STRING "Set 600000000 CPU core speed" FORCE)

        message(CHECK_START "identify root dependency path")
        if (NOT DEFINED TDUINO)
            message(FATAL_ERROR "TDUINO is UNDEFINED")
        else()
            message(STATUS "TDUINO: ${TDUINO}")
        endif()

        if (NOT DEFINED TDUINO_CORE)
            set(TDUINO_CORE "${TDUIONO}/cores/teensy4/")
            set(TDUINO_CORE ${TDUINO_CORE} CACHE INTERNAL "TDUINO_CORE")
            message(STATUS "TDUINO_CORE: ${TDUINO_CORE}")
        else()
            message(STATUS "TDUINO_CORE: ${TDUINO_CORE}")
        endif()

        if (NOT DEFINED CPU_CORE_SPEED)
            message(FATAL_ERROR "CPU_CORE_SPEED is UNDEFINED")
        else()
            message(STATUS "CPU_CORE_SPEED: ${CPU_CORE_SPEED}")
        endif()

        set(build_fcpu ${CPU_CORE_SPEED})

        if (NOT DEFINED COMPILER_PATH)
            message(FATAL_ERROR "COMPILER_PATH is UNDEFINED")
        else()
            message(STATUS "COMPILER_PATH: ${COMPILER_PATH}")
        endif()

        set(build_toolchain ${COMPILER_PATH})

        if (NOT DEFINED ${build_usbtype})
            set(build_usbtype USB_SERIAL)
            set(build_usbtype ${build_usbtype}  CACHE INTERNAL "build_usbtype")
            message(STATUS "build_usbtype: ${build_usbtype}" )
        endif()

        if (NOT DEFINED ${build_usbtype})
            set(build_keylayout US_ENGLISH)
            set(build_keylayout ${build_keylayout}  CACHE INTERNAL "build_keylayout")
            message(STATUS "build_keylayout: ${build_keylayout}" )
        endif()

        if(TEENSY_VERSION EQUAL 41)
            message(STATUS "building for teensy 4.1")
            set(CPU_DEFINE __IMXRT1062__)
            set(LINKER_FILE ${TDUINO_CORE}imxrt1062_t41.ld)
            set(build_board TEENSY41)
            set(build_flags_ld " -Wl,--gc-sections,--relax ")
            set(build_core teensy4)
            set(build_mcu imxrt1062)
            set(build_warn_data_percentage 99)
            set(build_flags_common "-g -Wall -ffunction-sections -fdata-sections ")
            set(build_flags_dep "-MMD")
            set(build_flags_optimize "-O2")
            set(build_flags_cpu "-mthumb -mcpu=cortex-m7 -mfloat-abi=hard -mfpu=fpv5-d16 ")
            set(build_flags_defs "-D${CPU_DEFINE} -DTEENSYDUINO=${runtime_ide_version} ")
            set(build_flags_cpp "-std=gnu++17 -fno-exceptions -fpermissive -fno-rtti -fno-threadsafe-statics -felide-constructors -Wno-error=narrowing")
            set(build_flags_c "")
            set(build_flags_S "-x assembler-with-cpp")
            set(build_flags_libs "-lm -lstdc++")
        else()
            message(FATAL_ERROR "Teensy version not defined")
        endif()

        set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER CACHE INTERNAL "CMAKE_FIND_ROOT_PATH_MODE_PROGRAM")
        set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY CACHE INTERNAL "CMAKE_FIND_ROOT_PATH_MODE_LIBRARY")
        set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY CACHE INTERNAL "CMAKE_FIND_ROOT_PATH_MODE_INCLUDE")
        set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY CACHE INTERNAL "CMAKE_FIND_ROOT_PATH_MODE_PACKAGE")

        set(CPP_COMPILE_FLAGS   "${build_flags_optimize} ${build_flags_common} ${build_flags_dep} ${build_flags_cpp} ${build_flags_cpu} ${build_flags_defs} -DARDUINO=${arduino_ide_version} -DARDUINO_${build_board} -DF_CPU=${build_fcpu} -D${build_usbtype} -DLAYOUT_${build_keylayout}")
        set(C_COMPILE_FLAGS     "${build_flags_optimize} ${build_flags_common} ${build_flags_dep} ${build_flags_c} ${build_flags_cpu} ${build_flags_defs} -DARDUINO=${arduino_ide_version} -DARDUINO_${build_board} -DF_CPU=${build_fcpu} -D${build_usbtype} -DLAYOUT_${build_keylayout}")
        set(S_COMPILE_FLAGS     "${build_flags_optimize} ${build_flags_common} ${build_flags_dep} ${build_flags_S} ${build_flags_cpu} ${build_flags_defs} -DARDUINO=${arduino_ide_version} -DARDUINO_${build_board} -DF_CPU=${build_fcpu} -D${build_usbtype} -DLAYOUT_${build_keylayout}")
        set(LINK_FLAGS          "${build_flags_optimize} ${build_flags_ld} ${build_flags_ldspecs} ${build_flags_cpu} -T${LINKER_FILE} ${build_flags_libs}")

        set(CPP_COMPILE_FLAGS   ${CPP_COMPILE_FLAGS} CACHE INTERNAL "CPP_COMPILE_FLAGS")
        set(C_COMPILE_FLAGS     ${C_COMPILE_FLAGS}   CACHE INTERNAL "C_COMPILE_FLAGS")
        set(S_COMPILE_FLAGS     ${S_COMPILE_FLAGS}   CACHE INTERNAL "S_COMPILE_FLAGS")
        set(LINK_FLAGS          ${LINK_FLAGS}        CACHE INTERNAL "LINK_FLAGS")

        message(STATUS "S_COMPILE_FLAGS: ${S_COMPILE_FLAGS}")
        message(STATUS "C_COMPILE_FLAGS: ${C_COMPILE_FLAGS}")
        message(STATUS "CPP_COMPILE_FLAGS: ${CPP_COMPILE_FLAGS}")
        message(STATUS "LINK_FLAGS: ${LINK_FLAGS}")
    endif()
endfunction()

function(teensy_set_dynamic_properties_eee)
    if (NOT DEFINED teensy_set_dynamic_properties_has_executed)
        set(teensy_set_dynamic_properties_has_executed 1 CACHE INTERNAL "teensy_set_dynamic_properties_has_executed")
        message(STATUS "teensy_set_dynamic_properties()")

        set(CPU_CORE_SPEED 528000000 CACHE STRING "Set 528000000 CPU core speed" FORCE)

        message(CHECK_START "identify root dependency path")
        if (NOT DEFINED TDUINO)
            message(FATAL_ERROR "TDUINO is UNDEFINED")
        else()
            message(STATUS "TDUINO: ${TDUINO}")
        endif()

        if (NOT DEFINED TDUINO_CORE)
            set(TDUINO_CORE "${TDUIONO}/cores/teensy4/")
            set(TDUINO_CORE ${TDUINO_CORE} CACHE INTERNAL "TDUINO_CORE")
            message(STATUS "TDUINO_CORE: ${TDUINO_CORE}")
        else()
            message(STATUS "TDUINO_CORE: ${TDUINO_CORE}")
        endif()

        if (NOT DEFINED CPU_CORE_SPEED)
            message(FATAL_ERROR "CPU_CORE_SPEED is UNDEFINED")
        else()
            message(STATUS "CPU_CORE_SPEED: ${CPU_CORE_SPEED}")
        endif()

        set(build_fcpu ${CPU_CORE_SPEED})

        if (NOT DEFINED COMPILER_PATH)
            message(FATAL_ERROR "COMPILER_PATH is UNDEFINED")
        else()
            message(STATUS "COMPILER_PATH: ${COMPILER_PATH}")
        endif()

        set(build_toolchain ${COMPILER_PATH})

        if (NOT DEFINED ${build_usbtype})
            # set(build_usbtype USB_SERIAL)
            set(build_usbtype USB_DUAL_SERIAL)
            set(build_usbtype ${build_usbtype}  CACHE INTERNAL "build_usbtype")
            message(STATUS "build_usbtype: ${build_usbtype}" )
        endif()

        if (NOT DEFINED ${build_usbtype})
            set(build_keylayout US_ENGLISH)
            set(build_keylayout ${build_keylayout}  CACHE INTERNAL "build_keylayout")
            message(STATUS "build_keylayout: ${build_keylayout}" )
        endif()

        if(TEENSY_VERSION EQUAL 41)
            message(STATUS "building for teensy 4.1")
            set(CPU_DEFINE __IMXRT1062__)
            set(LINKER_FILE ${TDUINO_CORE}imxrt1062_t41.ld)
            set(build_board TEENSY41)
            set(build_flags_ld " -Wl,--gc-sections,--relax ")
            set(build_core teensy4)
            set(build_mcu imxrt1062)
            set(build_warn_data_percentage 99)
            set(build_flags_common "-g -Wall -ffunction-sections -fdata-sections ")
            set(build_flags_dep "-MMD")
#            set(build_flags_optimize "-Os")
            set(build_flags_optimize "-Ofast")
            set(build_flags_cpu "-mthumb -mcpu=cortex-m7 -mfloat-abi=hard -mfpu=fpv5-d16 ")
            set(build_flags_defs "-D${CPU_DEFINE} -DTEENSYDUINO=${runtime_ide_version} ")
            set(build_flags_cpp "-std=gnu++17 -fno-exceptions -fpermissive -fno-rtti -fno-threadsafe-statics -felide-constructors -Wno-error=narrowing")
            set(build_flags_c "")
            set(build_flags_S "-x assembler-with-cpp")
            set(build_flags_libs "-lm -lstdc++")
        else()
            message(FATAL_ERROR "Teensy version not defined")
        endif()

        set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER CACHE INTERNAL "CMAKE_FIND_ROOT_PATH_MODE_PROGRAM")
        set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY CACHE INTERNAL "CMAKE_FIND_ROOT_PATH_MODE_LIBRARY")
        set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY CACHE INTERNAL "CMAKE_FIND_ROOT_PATH_MODE_INCLUDE")
        set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY CACHE INTERNAL "CMAKE_FIND_ROOT_PATH_MODE_PACKAGE")

        set(CPP_COMPILE_FLAGS   "${build_flags_optimize} ${build_flags_common} ${build_flags_dep} ${build_flags_cpp} ${build_flags_cpu} ${build_flags_defs} -DARDUINO=${arduino_ide_version} -DARDUINO_${build_board} -DF_CPU=${build_fcpu} -D${build_usbtype} -DLAYOUT_${build_keylayout}")
        set(C_COMPILE_FLAGS     "${build_flags_optimize} ${build_flags_common} ${build_flags_dep} ${build_flags_c} ${build_flags_cpu} ${build_flags_defs} -DARDUINO=${arduino_ide_version} -DARDUINO_${build_board} -DF_CPU=${build_fcpu} -D${build_usbtype} -DLAYOUT_${build_keylayout}")
        set(S_COMPILE_FLAGS     "${build_flags_optimize} ${build_flags_common} ${build_flags_dep} ${build_flags_S} ${build_flags_cpu} ${build_flags_defs} -DARDUINO=${arduino_ide_version} -DARDUINO_${build_board} -DF_CPU=${build_fcpu} -D${build_usbtype} -DLAYOUT_${build_keylayout}")
        set(LINK_FLAGS          "${build_flags_optimize} ${build_flags_ld} ${build_flags_ldspecs} ${build_flags_cpu} -T${LINKER_FILE} ${build_flags_libs}")

        set(CPP_COMPILE_FLAGS   ${CPP_COMPILE_FLAGS} CACHE INTERNAL "CPP_COMPILE_FLAGS")
        set(C_COMPILE_FLAGS     ${C_COMPILE_FLAGS}   CACHE INTERNAL "C_COMPILE_FLAGS")
        set(S_COMPILE_FLAGS     ${S_COMPILE_FLAGS}   CACHE INTERNAL "S_COMPILE_FLAGS")
        set(LINK_FLAGS          ${LINK_FLAGS}        CACHE INTERNAL "LINK_FLAGS")

        message(STATUS "S_COMPILE_FLAGS: ${S_COMPILE_FLAGS}")
        message(STATUS "C_COMPILE_FLAGS: ${C_COMPILE_FLAGS}")
        message(STATUS "CPP_COMPILE_FLAGS: ${CPP_COMPILE_FLAGS}")
        message(STATUS "LINK_FLAGS: ${LINK_FLAGS}")
    endif()
endfunction()

function(teensy_add_executable TARGET)
    message(STATUS "teensy_add_executable(${TARGET} ${ARGN})")

    foreach(arg IN LISTS ARGN)
        file(GLOB INO_SOURCE RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${arg})
        list(FILTER INO_SOURCE INCLUDE REGEX ".ino$")
        set(TEENSY_EXE_INO_SOURCES ${TEENSY_EXE_INO_SOURCES} ${INO_SOURCE})
    endforeach()

    foreach(SOURCE_INO ${TEENSY_EXE_INO_SOURCES})
        set_property(SOURCE ${SOURCE_INO} PROPERTY LANGUAGE CXX)
        set_source_files_properties(${SOURCE_INO} PROPERTIES COMPILE_FLAGS "${CPP_COMPILE_FLAGS} ${INCLUDE_DIRECTORIES} -x c++")
    endforeach(SOURCE_INO ${TEENSY_EXE_INO_SOURCES})

    add_executable(${TARGET} ${ARGN})

    set_target_properties(${TARGET} PROPERTIES INCLUDE_DIRECTORIES "${TDUINO_CORE}")

    add_custom_command(OUTPUT ${TARGET}.hex
            COMMAND ${COMPILER_PATH}/${build_command_size} ${TARGET}
            COMMAND ${COMPILER_PATH}/${build_command_objcopy} -O ihex -j .eeprom --set-section-flags=.eeprom=alloc,load --no-change-warnings --change-section-lma .eeprom=0 ${TARGET} ${TARGET}.eep
            COMMAND ${COMPILER_PATH}/${build_command_objcopy} -O ihex -R .eeprom ${TARGET} ${TARGET}.hex
            DEPENDS ${TARGET}
            COMMENT "Creating HEX file for ${TARGET}")

    add_custom_target(${TARGET}_hex ALL DEPENDS ${TARGET}.hex)
endfunction()

function(teensy_add_library TARGET LIB_ROOT )
    message(STATUS "teensy_add_library(${TARGET} ${LIB_ROOT} ${ARGN})")

    set(IMPORT_LIB_CPP_SOURCES "")
    set(IMPORT_LIB_C_SOURCES "")
    set(IMPORT_LIB_S_SOURCES "")

    file(GLOB SOURCES_CPP ABSOLUTE "${LIB_ROOT}/*.cpp")
    foreach(SOURCE_CPP ${SOURCES_CPP})
        set(IMPORT_LIB_CPP_SOURCES ${IMPORT_LIB_CPP_SOURCES} ${SOURCE_CPP})
    endforeach(SOURCE_CPP ${SOURCES_CPP})

    file(GLOB SOURCES_C ABSOLUTE "${LIB_ROOT}/*.c")
    foreach(SOURCE_C ${SOURCES_C})
        set(IMPORT_LIB_C_SOURCES ${IMPORT_LIB_C_SOURCES} ${SOURCE_C})
    endforeach(SOURCE_C ${SOURCES_C})

    file(GLOB SOURCES_S ABSOLUTE "${LIB_ROOT}/*.S")
    foreach(SOURCE_S ${SOURCES_S})
        set(IMPORT_LIB_S_SOURCES ${IMPORT_LIB_S_SOURCES} ${SOURCE_S})
    endforeach(SOURCE_S ${SOURCES_S})

    foreach(arg ${ARGN})
        message(status " checking for ${LIB_ROOT}/${arg}")
        if(NOT EXISTS ${LIB_ROOT}/${arg})
            message(FATAL_ERROR "Could not find the Arduino library directory ${LIB_ROOT}/${arg}")
        endif(NOT EXISTS ${LIB_ROOT}/${arg})

        include_directories(AFTER "${LIB_ROOT}/${arg}")

        file(GLOB SOURCES_CPP ABSOLUTE "${LIB_ROOT}/${arg}/*.cpp")
        foreach(SOURCE_CPP ${SOURCES_CPP})
            set(IMPORT_LIB_CPP_SOURCES ${IMPORT_LIB_CPP_SOURCES} ${SOURCE_CPP})
        endforeach(SOURCE_CPP ${SOURCES_CPP})

        file(GLOB SOURCES_C ABSOLUTE "${LIB_ROOT}/${arg}/*.c")
        foreach(SOURCE_C ${SOURCES_C})
            set(IMPORT_LIB_C_SOURCES ${IMPORT_LIB_C_SOURCES} ${SOURCE_C})
        endforeach(SOURCE_C ${SOURCES_C})

        file(GLOB SOURCES_S ABSOLUTE "${LIB_ROOT}/${arg}/*.S")
        foreach(SOURCE_S ${SOURCES_S})
            set(IMPORT_LIB_S_SOURCES ${IMPORT_LIB_S_SOURCES} ${SOURCE_S})
        endforeach(SOURCE_S ${SOURCES_S})
    endforeach()

    message(STATUS IMPORT_LIB_CPP_SOURCES: ${IMPORT_LIB_CPP_SOURCES})
    message(STATUS IMPORT_LIB_C_SOURCES: ${IMPORT_LIB_C_SOURCES})
    message(STATUS IMPORT_LIB_S_SOURCES: ${IMPORT_LIB_S_SOURCES})

    foreach(SOURCE_S ${IMPORT_LIB_S_SOURCES})
        set_property(SOURCE ${SOURCE_S} PROPERTY LANGUAGE C)
    endforeach(SOURCE_S ${SOURCES_S})

    add_library(${TARGET} STATIC ${IMPORT_LIB_CPP_SOURCES} ${IMPORT_LIB_C_SOURCES} ${IMPORT_LIB_S_SOURCES})
endfunction()
