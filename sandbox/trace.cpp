#include "SDT.h"

#include "Arduino.h"

#include <cstdarg>
#include <cstdio>
#include <stdlib.h>
#include <string.h>

#define TRACE_MODULE_NAME trace

#include "trace.h"

uint8_t globalTraceLevel = TRACE_GLOBAL_LEVEL;

#if TRACE_GLOBAL_LEVEL > TR_L_NO
static char traceMessage[512];

static const char *traceLevelText(uint8_t level) {
  switch (level) {
  case TR_L_FATAL:
    return "FAT";
    break;
  case TR_L_ERROR:
    return "ERR";
    break;
  case TR_L_WARN:
    return "WAR";
    break;
  case TR_L_INFO:
    return "INF";
    break;
  case TR_L_DEBUG:
    return "DBG";
    break;
  case TR_L_TRACE:
    return "TRC";
    break;
  default:
    return "---";
    break;
  }
}

static const char *traceLevelTextColor(uint8_t level) {
  switch (level) {
  case TR_L_FATAL:
    return "\033[95mFAT\033[0m";
    break;
  case TR_L_ERROR:
    return "\033[31mERR\033[0m";
    break;
  case TR_L_WARN:
    return "\033[93mWAR\033[0m";
    break;
  case TR_L_INFO:
    return "\033[94mINF\033[0m";
    break;
  case TR_L_DEBUG:
    return "\033[37mDBG\033[0m";
    break;
  case TR_L_TRACE:
    return "\033[90mTRC\033[0m";
    break;
  default:
    return "---";
    break;
  }
}

extern class usb_serial_class Serial;
void TRACE_TO_T41_USB_SERIAL(uint8_t level, const char *file, int32_t line,
                             const char *func, const char *format, ...) {
  va_list arglist;

  snprintf(traceMessage, 255, "%08lu %s ", millis(),
           traceLevelTextColor(level));

  size_t len = strlen(traceMessage);

  va_start(arglist, format);
  vsnprintf(traceMessage + len, (sizeof(traceMessage) - len), format, arglist);
  len = strlen(traceMessage);
  snprintf(traceMessage + len, (sizeof(traceMessage) - len), " %s %s:%ld\n",
           func, file, line);
  va_end(arglist);
  Serial.printf("%s", traceMessage);
}

#else  // if TRACE_GLOBAL_LEVEL > TR_L_NO
void TRACE_TO_T41_USB_SERIAL(int16_t /*level*/, const char * /*file*/,
                             int32_t /*line*/, const char * /*func*/,
                             const char * /*format*/, ...) {}
#endif // if TRACE_GLOBAL_LEVEL > TR_L_NO
