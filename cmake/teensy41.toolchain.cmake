if (NOT DEFINED teensy_toolchain_file_has_executed)
    set(teensy_toolchain_file_has_executed 1 CACHE INTERNAL "")
    set(COMPILER_PATH "$ENV{PRJ_DIR}/teensy/tools/teensy-compile/11.3.1/arm/bin" CACHE INTERNAL "")
endif()

set(build_command_gcc arm-none-eabi-gcc)
set(build_command_g++ arm-none-eabi-g++)
set(build_command_ar arm-none-eabi-gcc-ar)
set(build_command_objcopy arm-none-eabi-objcopy)
set(build_command_objdump arm-none-eabi-objdump)
set(build_command_linker arm-none-eabi-gcc)
set(build_command_size arm-none-eabi-size)

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)
set(CMAKE_TRY_COMPILE_TARGET_TYPE "STATIC_LIBRARY")

SET(CMAKE_CXX_ARCHIVE_CREATE "${COMPILER_PATH}/${build_command_ar} rcs <TARGET> <LINK_FLAGS> <OBJECTS>")
SET(CMAKE_C_ARCHIVE_CREATE "${COMPILER_PATH}/${build_command_ar} rcs <TARGET> <LINK_FLAGS> <OBJECTS>")

set(CMAKE_C_COMPILER ${COMPILER_PATH}/arm-none-eabi-gcc)
set(CMAKE_CXX_COMPILER ${COMPILER_PATH}/arm-none-eabi-g++)

set(CMAKE_CXX_LINK_EXECUTABLE "${CMAKE_C_COMPILER} <FLAGS> <CMAKE_CXX_LINK_FLAGS> <LINK_FLAGS> <OBJECTS> -o <TARGET> <LINK_LIBRARIES>")