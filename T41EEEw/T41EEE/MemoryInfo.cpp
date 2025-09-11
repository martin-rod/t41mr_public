#include "MemoryInfo.h"

// Feb 23, 2023
// https://forum.pjrc.com/index.php?threads/memory-usage-teensy-4-1.72235/

// https://forum.pjrc.com/threads/33443-How-to-display-free-ram
// full https://forum.pjrc.com/index.php?threads/how-to-display-free-ram.33443/

// ./teensy/hardware/avr/1.59.0/cores/teensy4/imxrt1062_t41.ld
// addr2line -e xxx.elf 0x166

extern "C" uint8_t external_psram_size;

FLASHMEM void memInfo() {
  auto sp = (char *)__builtin_frame_address(0);

  constexpr auto RAM_BASE = 0x2020'0000;
  constexpr auto RAM_SIZE = 512 << 10;
  constexpr auto FLASH_BASE = 0x6000'0000;
  constexpr auto FLASH_SIZE = 8 << 20;

  // these extern are defined by the linker, by defining them as arrays the C++ compiler  will use the address of these definitions

  extern char _stext[];
  extern char _etext[];
  extern char _sbss[];
  extern char _ebss[];
  extern char _sdata[];
  extern char _edata[];
  extern char _estack[];
  extern char _heap_start[];
  extern char _heap_end[];
  extern char _itcm_block_count[];
  extern char *__brkval;

  Serial.printf("--- Memory:\n");
  Serial.printf("_stext        %08x\n", _stext);
  Serial.printf("_etext        %08x +%d B +%d KiB\n", _etext, (int32_t)(_etext - _stext), ((int32_t)(_etext - _stext)) >> 10);
  Serial.printf("_sdata        %08x\n", _sdata);
  Serial.printf("_edata        %08x +%d B +%d KiB\n", _edata, (int32_t)(_edata - _sdata), ((int32_t)(_edata - _sdata)) >> 10);
  Serial.printf("_sbss         %08x\n", _sbss);
  Serial.printf("_ebss         %08x +%d B +%d KiB\n", _ebss, (int32_t)(_ebss - _sbss), ((int32_t)(_ebss - _sbss)) >> 10);
  Serial.printf("curr stack    %08x +%d B +%d KiB\n", sp, (int32_t)(sp - _ebss), ((int32_t)(sp - _ebss)) >> 10);
  Serial.printf("_estack       %08x +%d B +%d KiB\n", _estack, (int32_t)(_estack - sp), ((int32_t)(_estack - sp)) >> 10);
  Serial.printf("_heap_start   %08x\n", _heap_start);
  Serial.printf("__brkval      %08x +%d B +%d KiB\n", __brkval, (int32_t)(__brkval - _heap_start),
                ((int32_t)(__brkval - _heap_start)) >> 10);
  Serial.printf("_heap_end     %08x +%d B +%d KiB\n", _heap_end, (int32_t)(_heap_end - __brkval),
                ((int32_t)(_heap_end - __brkval)) >> 10);

  extern char _extram_start[];
  extern char _extram_end[];
  extern char *__brkval;

  Serial.printf("_extram_start %08x\n", _extram_start);
  Serial.printf("_extram_end   %08x +%d B\n", _extram_end, _extram_end - _extram_start);

  Serial.printf("<ITCM>  %08x .. %08x (RAM1 FASTRUN, unused)\n", _stext, _stext + ((unsigned long int)_itcm_block_count << 15) - 1);
  Serial.printf("<DTCM>  %08x .. %08x (RAM1 init. vars, zeroed vars, stack - from top)\n", _sdata, ((unsigned long int)_estack) - 1);
  Serial.printf("<RAM>   %08x .. %08x (RAM2 DMAMEM, heap - from bottom)\n", RAM_BASE, RAM_BASE + RAM_SIZE - 1);
  Serial.printf("<FLASH> %08x .. %08x\n", FLASH_BASE, FLASH_BASE + FLASH_SIZE - 1);

  if (external_psram_size > 0) {
    Serial.printf("<PSRAM> %08x .. %08x\n", _extram_start, _extram_start + (external_psram_size << 20) - 1);
  }

  int32_t stackAvailable = sp - _ebss;
  int32_t stackCurrent = _estack - sp;
  int32_t stackSize = _estack - _ebss;
  Serial.printf("STACK available %8d B %5d KiB\n", stackAvailable, stackAvailable >> 10);
  Serial.printf("STACK current   %8d B %5d KiB\n", stackCurrent, stackCurrent >> 10);
  Serial.printf("STACK size      %8d B %5d KiB\n", stackSize, stackSize >> 10);

  int32_t heapAvailable = _heap_end - __brkval;
  int32_t heapCurrent = __brkval - _heap_start;
  int32_t heapSize = _heap_end - _heap_start;
  Serial.printf("HEAP available  %8d B %5d KiB\n", heapAvailable, heapAvailable >> 10);
  Serial.printf("HEAP current    %8d B %5d KiB\n", heapCurrent, heapCurrent >> 10);
  Serial.printf("HEAP size       %8d B %5d KiB\n", heapSize, heapSize >> 10);

  if (external_psram_size > 0) {
    int32_t psram = _extram_start + (external_psram_size << 20) - _extram_end;
    Serial.printf("PSRAM available %8d B %5d KiB\n\n", psram, psram >> 10);
  }

  int32_t sizeUnusedITCM = (32 * 1024) - (((unsigned long int)&_etext - (unsigned long int)&_stext) % (32 * 1024));
  Serial.printf("ITCM unused     %8d B\n", sizeUnusedITCM);
}

int32_t getHeapAvailable() {
  extern char _heap_end[];
  extern char *__brkval;

  int32_t heap = _heap_end - __brkval;
  return heap;
}

int32_t getStackAvailable() {
  extern char _ebss[];

  auto sp = (char *)__builtin_frame_address(0);
  auto stack = sp - _ebss;

  return stack;
}
