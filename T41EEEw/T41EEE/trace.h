#ifndef T41_TRACE_H
#define T41_TRACE_H

#include <cstdint>

#define TR_L_NO 0
#define TR_L_FATAL 1
#define TR_L_ERROR 2
#define TR_L_WARN 3
#define TR_L_INFO 4
#define TR_L_DEBUG 5
#define TR_L_TRACE 6
#define TR_L_ALL 7

#define TRACE_GLOBAL_LEVEL TR_L_ALL
#define TRACE_METHOD T41_USB_SERIAL

extern uint8_t globalTraceLevel;

#define TRACE_GLOBAL_CONDITION(level) ((level) <= globalTraceLevel)

#if !defined(TRACE_GLOBAL_LEVEL)
#define TRACE_GLOBAL_LEVEL TR_L_NO
#endif // if !defined ( TRACE_GLOBAL_LEVEL )

#if !defined(TRACE_MODULE_LEVEL)
#define TRACE_MODULE_LEVEL TR_L_NO
#endif // if !defined ( TRACE_MODULE_LEVEL )

#if defined(TRACE_GLOBAL_LEVEL) && defined(TRACE_MODULE_LEVEL)
#if TRACE_MODULE_LEVEL > TRACE_GLOBAL_LEVEL
#undef TRACE_MODULE_LEVEL
#define TRACE_MODULE_LEVEL TRACE_GLOBAL_LEVEL
#endif // if TRACE_MODULE_LEVEL > TRACE_GLOBAL_LEVEL
#endif // if defined ( TRACE_GLOBAL_LEVEL ) && defined ( TRACE_MODULE_LEVEL )

#define TRACE_PARAMS __FILE__, __LINE__, __PRETTY_FUNCTION__
#define TRACE_CONDITION(l) (TRACE_VAR >= (l) && (l) <= TRACE_MODULE_LEVEL)

#define MAKE_STR_(par) #par
#define MAKE_STR(par) MAKE_STR_(par)

#define TRACE_PREFIX(l) "(" MAKE_STR(TRACE_MODULE_NAME) ") "

#define TRACE_METHOD_NAME_M(suffix) TRACE_TO_##suffix
#define TRACE_METHOD_NAME_(suffix) TRACE_METHOD_NAME_M(suffix)
#define TRACE_METHOD_NAME TRACE_METHOD_NAME_(TRACE_METHOD)

#undef TRACE_T41
#if TRACE_MODULE_LEVEL > 0
#define TRACE_LEVEL(val) const uint8_t TRACE_VAR = val
#define TRACE_T41(l, ...)                                                      \
  do {                                                                         \
    if (TRACE_CONDITION(l) && (TRACE_GLOBAL_CONDITION(l))) {                   \
      TRACE_METHOD_NAME(l, TRACE_PARAMS, TRACE_PREFIX(l) __VA_ARGS__);         \
    }                                                                          \
  } while (false)
#else // if TRACE_MODULE_LEVEL > 0
#define TRACE_LEVEL(val)
#define TRACE_T41(l, ...)                                                      \
  do {                                                                         \
  } while (false)
#endif // if TRACE_MODULE_LEVEL > 0

void TRACE_TO_T41_USB_SERIAL(uint8_t level, const char *file, int32_t line,
                             const char *func, const char *format, ...);

#endif // T41_TRACE_H

/* *
 * Before #include "trace.h"
 *
 * #define TRACE_MODULE_NAME <name>           set prefix of message in the
 *source file #define TRACE_MODULE_LEVEL [<level>]       set trace level for
 *whole file
 *
 * level:
 *
 * #define TR_L_NO  0
 * #define TR_L_FATAL    1
 * #define TR_L_ERROR    2
 * #define TR_L_WARN     3
 * #define TR_L_INFO     4
 * #define TR_L_DEBUG    5
 * #define TR_L_TRACE    6
 *
 * TRACE_LEVEL()      set trace level for compile time
 * globalTraceLevel   set trace level in runtime
 * TRACE_T41(level,msg_printf_format)   macro for message
 **/

/* Example - only TR_L_INFO is displayed

#include <iostream>

using namespace std;

#define TRACE_MODULE_LEVEL TR_L_ALL
#define TRACE_MODULE_NAME main_module

#include "trace.h"

int main()
{
    // TR_L_TRACE, TR_L_DEBUG is removed during compile time
    // TR_L_INFO, TR_L_WARN, TR_L_ERROR, TR_L_FATAL is not removed during
compile time
    // TR_L_INFO and up ...
    TRACE_LEVEL( TR_L_INFO );

    TRACE_T41( TR_L_TRACE, "level trace log message value:%d",100 );
    TRACE_T41( TR_L_INFO, "level info log message value:%d",100 );

    // TR_L_FATAL,TR_L_ERROR, TR_L_WARN is enabled in run time
    // TR_L_WARN and up ...
    globalTraceLevel=TR_L_WARN;

    TRACE_T41( TR_L_FATAL, "level fatal log message value:%d",100 );
    TRACE_T41( TR_L_TRACE, "level trace log message value:%d",100 );
    TRACE_T41( TR_L_INFO, "level info log message value:%d",100 );

    cout << "Hello World!" << endl;
    return 0;
}

expected output:
INF (main_module) level info log message value:100
FAT (main_module) level fatal log message value:100
Hello World!
 */
