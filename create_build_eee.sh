#! /bin/sh
build_dir=build_eee

echo "---------------------------"
echo "Vytvarim:${build_dir}"
echo "---------------------------"
mkdir -p ${build_dir}
rm -rf ${build_dir}/*

export PRJ_DIR=$(pwd)
echo "PRJ_DIR:${PRJ_DIR}"

# cmake -B $build_dir -D CMAKE_TOOLCHAIN_FILE="./cmake/teensy41.toolchain.cmake" -D T41_SRC_TYPE=EEE -D CMAKE_BUILD_TYPE=Debug

# ld: warning: t41sdt has a LOAD segment with RWX permissions
# cmake -B $build_dir -D CMAKE_TOOLCHAIN_FILE="./cmake/arm13.toolchain.cmake" -D T41_SRC_TYPE=EEE -D CMSIS_TYPE=LIB_V4_PJRC -D CMAKE_BUILD_TYPE=Debug

# ld: warning: t41sdt has a LOAD segment with RWX permissions
# cmake -B $build_dir -D CMAKE_TOOLCHAIN_FILE="./cmake/arm13.toolchain.cmake" -D T41_SRC_TYPE=EEE -D CMSIS_TYPE=LIB_V4_ARM -D CMAKE_BUILD_TYPE=Debug

# cmake -B $build_dir -D CMAKE_TOOLCHAIN_FILE="./cmake/arm13.toolchain.cmake" -D T41_SRC_TYPE=EEE -D CMSIS_TYPE=LIB_V6 -D CMAKE_BUILD_TYPE=Debug

cmake -B $build_dir -D CMAKE_TOOLCHAIN_FILE="./cmake/arm13.toolchain.cmake" -D T41_SRC_TYPE=EEE -D CMSIS_TYPE=SRC_V6 -D CMAKE_BUILD_TYPE=Debug

cp script/load_to_teensy.sh ${build_dir}