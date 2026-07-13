#! /bin/sh
build_dir=build_eee_o

echo "---------------------------"
echo "Vytvarim:${build_dir}"
echo "---------------------------"
mkdir -p ${build_dir}
rm -rf ${build_dir}/*

export PRJ_DIR=$(pwd)
echo "PRJ_DIR:${PRJ_DIR}"

cmake -B $build_dir -D CMAKE_TOOLCHAIN_FILE="./cmake/arm15.toolchain.cmake" -D T41_SRC_TYPE=EEE_O -D CMSIS_TYPE=SRC_V6 -D CMAKE_BUILD_TYPE=Debug

cp script/load_to_teensy.sh ${build_dir}
