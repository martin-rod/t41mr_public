#! /bin/sh
build_dir=build_eee_arm13

echo "---------------------------"
echo "Vytvarim:${build_dir}"
echo "---------------------------"
mkdir -p ${build_dir}
rm -rf ${build_dir}/*

export PRJ_DIR=$(pwd)
echo "PRJ_DIR:${PRJ_DIR}"

# ld: warning: t41sdt has a LOAD segment with RWX permissions
# cmake -B $build_dir -D CMAKE_TOOLCHAIN_FILE="./cmake/arm13.toolchain.cmake" -D T41_SRC_TYPE=EEE -D CMSIS_TYPE=LIB_V4_PJRC -D CMAKE_BUILD_TYPE=Debug

# ld: warning: t41sdt has a LOAD segment with RWX permissions
cmake -B $build_dir -D CMAKE_TOOLCHAIN_FILE="./cmake/arm13.toolchain.cmake" -D T41_SRC_TYPE=EEE -D CMSIS_TYPE=LIB_V4_ARM -D CMAKE_BUILD_TYPE=Debug

# CMSIS_TYPE = LIB_V4_PJRC, LIB_V4_ARM, SRC_V4, SRC_V6


cp script/load_to_teensy.sh ${build_dir}