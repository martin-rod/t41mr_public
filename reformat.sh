#! /bin/bash
T41_SRC_DIR="T41EEEw/T41EEE"

echo "---------------------------"
echo "reformat ${T41_SRC_DIR}"
echo "---------------------------"

export PRJ_DIR=$(pwd)
echo "PRJ_DIR:${PRJ_DIR}"

FORMAT_TOOL="clang-format-19"
FORMAT_FILE_PATH="${PRJ_DIR}/.clang-format"
FORMAT_OPT="-i --verbose --ferror-limit=0 -style=file:${FORMAT_FILE_PATH}"
FORMAT_CMD="${FORMAT_TOOL} ${FORMAT_OPT} "

echo "FORMAT_CMD:${FORMAT_CMD}"

T41_SRC_ARRAY=( \
    "${T41_SRC_DIR}/AudioSignal.h" \
    "${T41_SRC_DIR}/Bearing.cpp" \
    "${T41_SRC_DIR}/Button.cpp" \
    "${T41_SRC_DIR}/ButtonProc.cpp" \
    "${T41_SRC_DIR}/CWCalibrate.cpp" \
    "${T41_SRC_DIR}/CWCalibrate.h" \
    "${T41_SRC_DIR}/CW_Exciter.cpp" \
    "${T41_SRC_DIR}/CWProcessing.cpp" \
    "${T41_SRC_DIR}/Demod.cpp" \
    "${T41_SRC_DIR}/Display.cpp" \
    "${T41_SRC_DIR}/DSP_Fn.cpp" \
    "${T41_SRC_DIR}/Eeprom.cpp" \
    "${T41_SRC_DIR}/Eeprom.h" \
    "${T41_SRC_DIR}/Encoders.cpp" \
    "${T41_SRC_DIR}/FFT.cpp" \
    "${T41_SRC_DIR}/Filter.cpp" \
    "${T41_SRC_DIR}/FIR.cpp" \
    "${T41_SRC_DIR}/Freq_Shift.cpp" \
    "${T41_SRC_DIR}/JSON.cpp" \
    "${T41_SRC_DIR}/JSON.h" \
    "${T41_SRC_DIR}/MenuProc.cpp" \
    "${T41_SRC_DIR}/MyConfigurationFile.h" \
    "${T41_SRC_DIR}/Noise.cpp" \
    "${T41_SRC_DIR}/Process.cpp" \
    "${T41_SRC_DIR}/SDT.h" \
    "${T41_SRC_DIR}/SSBCalibrate.cpp" \
    "${T41_SRC_DIR}/SSBCalibrate.h" \
    "${T41_SRC_DIR}/SSB_Exciter.cpp" \
    "${T41_SRC_DIR}/Process.cpp" \
    "${T41_SRC_DIR}/T41EEE.ino" \
    "${T41_SRC_DIR}/Tune.cpp" \
    "${T41_SRC_DIR}/Utility.cpp" \
)

for src in ${T41_SRC_ARRAY[@]}; do
  git restore ${src}
done

${FORMAT_CMD} ${T41_SRC_ARRAY[@]}

