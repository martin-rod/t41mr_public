#include "Arduino.h"

#include <malloc.h>

#if 0
extern unsigned long _heap_start;
extern unsigned long _heap_end;
extern char *__brkval;

int freeram() {
  return (char *)&_heap_end - __brkval;
}
#endif

void TraceStack(void) {

  extern char _ebss[];
  char *sp = (char *)__builtin_frame_address(0);
  int32_t stack = (sp - _ebss) ;

  Serial.printf("Stack:%dk\n", stack);
  Serial.printf("Stack:%dk\n", stack >> 10);
}

void TraceHeap(void) {

  struct mallinfo mi = mallinfo();

  Serial.print("arena:");
  Serial.println(mi.arena);

  Serial.print("ordblks:");
  Serial.println(mi.ordblks);

  Serial.print("smblks:");
  Serial.println(mi.smblks);

  Serial.print("hblks:");
  Serial.println(mi.hblks);

  Serial.print("hblkhd:");
  Serial.println(mi.hblkhd);

  Serial.print("usmblks:");
  Serial.println(mi.usmblks);

  Serial.print("fsmblks:");
  Serial.println(mi.fsmblks);

  Serial.print("uordblks:");
  Serial.println(mi.uordblks);

  Serial.print("fordblks:");
  Serial.println(mi.fordblks);

  Serial.print("keepcost:");
  Serial.println(mi.keepcost);

  size_t heapTotalFree = mallinfo().fordblks >> 10;
  size_t heapTotalAlocated = mallinfo().uordblks >> 10;
  Serial.printf( "Heap free:%uk alocated:%uk\n", heapTotalFree, heapTotalAlocated);
}

// ./teensy/hardware/avr/1.59.0/cores/teensy4/imxrt1062_t41.ld
// addr2line -e xxx.elf 0x166

// Feb 23, 2023
// https://forum.pjrc.com/index.php?threads/memory-usage-teensy-4-1.72235/


// https://forum.pjrc.com/threads/33443-How-to-display-free-ram
// full https://forum.pjrc.com/index.php?threads/how-to-display-free-ram.33443/


void memInfo();
void getFreeITCM();//  { // end of CODE ITCM, skip full 32 bits
void setup() {
  // put your setup code here, to run once:
  pinMode( LED_BUILTIN, OUTPUT );
  digitalWrite( LED_BUILTIN, HIGH );
  Serial.begin(115200);
  while (!Serial);
  digitalWrite( LED_BUILTIN, LOW );
  Serial.println("\n" __FILE__ " " __DATE__ " " __TIME__);
  if ( CrashReport ) Serial.print ( CrashReport );

  memInfo();

  // Serial.print("freeram = ");
  // Serial.println(freeram());

  TraceStack();
  TraceHeap();

  volatile char lc[50*1024];
  lc[0]='A';

  volatile char *a= new char[100*1024];
  *a = 0;

  volatile char *p = (char *)malloc(100*1024);
  *p = 0;

  memInfo();

  // Serial.print("freeram = ");
  // Serial.println(freeram());

  TraceStack();
  TraceHeap();

  getFreeITCM();
  Serial.println(lc[0]);
  Serial.println("\n" __FILE__ " " __DATE__ " " __TIME__);
}


void loop() {
  // put your main code here, to run repeatedly:

}


#define printf Serial.printf
#if ARDUINO_TEENSY41
  extern "C" uint8_t external_psram_size;
#endif

void memInfo () {
  constexpr auto RAM_BASE   = 0x2020'0000;
                              constexpr auto RAM_SIZE   = 512 << 10;
                              constexpr auto FLASH_BASE = 0x6000'0000;
#if ARDUINO_TEENSY40
  constexpr auto FLASH_SIZE = 2 << 20;
#elif ARDUINO_TEENSY41
  constexpr auto FLASH_SIZE = 8 << 20;
#endif

  // note: these values are defined by the linker, they are not valid memory
  // locations in all cases - by defining them as arrays, the C++ compiler
  // will use the address of these definitions - it's a big hack, but there's
  // really no clean way to get at linker-defined symbols from the .ld file

  extern char _stext[], _etext[], _sbss[], _ebss[], _sdata[], _edata[],
         _estack[], _heap_start[], _heap_end[], _itcm_block_count[], *__brkval;

  auto sp = (char*) __builtin_frame_address(0);

  printf("_stext        %08x\n",      _stext);
  printf("_etext        %08x +%db\n", _etext, _etext - _stext);
  printf("_sdata        %08x\n",      _sdata);
  printf("_edata        %08x +%db\n", _edata, _edata - _sdata);
  printf("_sbss         %08x\n",      _sbss);
  printf("_ebss         %08x +%db\n", _ebss, _ebss - _sbss);
  printf("curr stack    %08x +%db\n", sp, sp - _ebss);
  printf("_estack       %08x +%db\n", _estack, _estack - sp);
  printf("_heap_start   %08x\n",      _heap_start);
  printf("__brkval      %08x +%db\n", __brkval, __brkval - _heap_start);
  printf("_heap_end     %08x +%db\n", _heap_end, _heap_end - __brkval);
#if ARDUINO_TEENSY41
  extern char _extram_start[], _extram_end[], *__brkval;
  printf("_extram_start %08x\n",      _extram_start);
  printf("_extram_end   %08x +%db\n", _extram_end,
         _extram_end - _extram_start);
#endif
  printf("\n");

  printf("<ITCM>  %08x .. %08x\n",
         _stext, _stext + ((int) _itcm_block_count << 15) - 1);
  printf("<DTCM>  %08x .. %08x\n",
         _sdata, _estack - 1);
  printf("<RAM>   %08x .. %08x\n",
         RAM_BASE, RAM_BASE + RAM_SIZE - 1);
  printf("<FLASH> %08x .. %08x\n",
         FLASH_BASE, FLASH_BASE + FLASH_SIZE - 1);
#if ARDUINO_TEENSY41
  if (external_psram_size > 0)
    printf("<PSRAM> %08x .. %08x\n",
           _extram_start, _extram_start + (external_psram_size << 20) - 1);
#endif
  printf("\n");

  auto stack = sp - _ebss;
  printf("avail STACK %8d b %5d kb\n", stack, stack >> 10);

  auto heap = _heap_end - __brkval;
  printf("avail HEAP  %8d b %5d kb\n", heap, heap >> 10);

#if ARDUINO_TEENSY41
  auto psram = _extram_start + (external_psram_size << 20) - _extram_end;
  printf("avail PSRAM %8d b %5d kb\n", psram, psram >> 10);
#endif
}


uint32_t *ptrFreeITCM;  // Set to Usable ITCM free RAM
uint32_t  sizeofFreeITCM; // sizeof free RAM in uint32_t units.
uint32_t  SizeLeft_etext;
extern char _stext[], _etext[];
FLASHMEM void   getFreeITCM() { // end of CODE ITCM, skip full 32 bits
  Serial.println("\n\n++++++++++++++++++++++");
  SizeLeft_etext = (32 * 1024) - (((uint32_t)&_etext - (uint32_t)&_stext) % (32 * 1024));
  sizeofFreeITCM = SizeLeft_etext - 4;
  sizeofFreeITCM /= sizeof(ptrFreeITCM[0]);
  ptrFreeITCM = (uint32_t *) ( (uint32_t)&_stext + (uint32_t)&_etext + 4 );
  printf( "sizeofFreeITCM = %u\n", sizeofFreeITCM );
  printf( "sizeof(ptrFreeITCM[0]) = %u\n", sizeof(ptrFreeITCM[0]) );
  printf( "Size of Free ITCM in Bytes = %u\n", sizeofFreeITCM * sizeof(ptrFreeITCM[0]) );
  printf( "Start of Free ITCM = %u [%X] \n", ptrFreeITCM, ptrFreeITCM);
  printf( "End of Free ITCM = %u [%X] \n", ptrFreeITCM + sizeofFreeITCM, ptrFreeITCM + sizeofFreeITCM);
  // for ( uint32_t ii = 0; ii < sizeofFreeITCM; ii++) {
  //   printf( "%u ", ii );
  //   ptrFreeITCM[ii] = 1;
  // }
  // uint32_t jj = 0;
  // for ( uint32_t ii = 0; ii < sizeofFreeITCM; ii++) jj += ptrFreeITCM[ii];
  // printf( "ITCM DWORD cnt = %u [#bytes=%u] \n", jj, jj*4);
}

#if 0
C:\T_Drive\tCode\Memory\T4MemInfo\T4MemInfo.ino Feb 23 2023 01:07:54
_stext        00000000
_etext        000084e0 +34016b
_sdata        20000000
_edata        200022c0 +8896b
_sbss         200022c0
_ebss         20002720 +1120b
curr stack    2006ffb0 +448656b
_estack       20070000 +80b
_heap_start   20203060
__brkval      20203060 +0b
_heap_end     20280000 +511904b
_extram_start 70000000
_extram_end   70000000 +0b

<ITCM>  00000000 .. 0000ffff
<DTCM>  20000000 .. 2006ffff
<RAM>   20200000 .. 2027ffff
<FLASH> 60000000 .. 607fffff
<PSRAM> 70000000 .. 707fffff

avail STACK   448656 b   438 kb
avail HEAP    511904 b   499 kb
avail PSRAM  8388608 b  8192 kb


++++++++++++++++++++++
Size of Free ITCM in Bytes = 31516
Start of Free ITCM = 34020 [84E4]
End of Free ITCM = 65536 [10000]
ITCM DWORD cnt = 7879 [#bytes=31516]
#endif
