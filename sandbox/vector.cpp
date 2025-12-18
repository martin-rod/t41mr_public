#include "Arduino.h"

#include <cstring>
#include <iostream>
#include <vector>

#define TRACE_MODULE_LEVEL TR_L_ALL
#define TRACE_MODULE_NAME T41EEE

#include "trace.h"

void trace_test(void) {
  TRACE_LEVEL(TR_L_DEBUG);

  Serial.printf("Test serial printf\n");

  TRACE_T41(TR_L_FATAL, "Test message TR_L_FATAL");
  TRACE_T41(TR_L_ERROR, "Test message TR_L_ERROR");
  TRACE_T41(TR_L_WARN, "Test message TR_L_WARN");
  TRACE_T41(TR_L_INFO, "Test message TR_L_INFO");
  TRACE_T41(TR_L_DEBUG, "Test message TR_L_DEBUG");
  TRACE_T41(TR_L_TRACE, "Test message TR_L_TRACE");
}

std::vector<std::string> IQOptionsGlobal{
    "Freq Cal", "CW PA Cal", "CW Rec Cal", "CW Xmit Cal", "SSB PA Cal", "SSB Rec Cal",
};

void printVectorAsStdString(const std::vector<std::string> &v) {
  TRACE_LEVEL(TR_L_DEBUG);

  TRACE_T41(TR_L_DEBUG, "v: address:0x%X capacity:%u size:%u ", &v, v.capacity(), v.size());
  for (const auto &i : v) {
    TRACE_T41(TR_L_DEBUG, "i: address:0x%X capacity:%u size:%u value:'%s'", &i, i.capacity(), i.size(), i.c_str());
  }
}

const std::vector<const char *> IQOptionsConstVectorConstCharGlobal{
    "Freq Cal", "CW PA Cal", "CW Rec Cal", "CW Xmit Cal", "SSB PA Cal", "SSB Rec Cal",
};

void printVectorAsConstChar(const std::vector<const char *> &v) {
  TRACE_LEVEL(TR_L_DEBUG);

  TRACE_T41(TR_L_DEBUG, "v: address:0x%X capacity:%u size:%u ", &v, v.capacity(), v.size());
  for (const auto &i : v) {
    TRACE_T41(TR_L_DEBUG, "i: address:0x%X address&:0x%X sizeof:%u strlen:%u value:'%s'", i, &i, sizeof(i), strlen(i), i);
  }
}

// OK - in flash
PROGMEM const char hello[] PROGMEM = "Hello, World";
PROGMEM const char flashFreqCal[] = "Freq Cal";
PROGMEM const char flashCwPaCal[] = "CW PA Cal";

const std::vector<const char *> IQOptionsConstVectorConstCharFlashGlobal{
    flashFreqCal, flashCwPaCal
    // , "CW PA Cal", "CW Rec Cal", "CW Xmit Cal", "SSB PA Cal", "SSB Rec Cal",
};

#if 1
void loop() {}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  Serial.begin(115200);
  while (!Serial)
    ;
  digitalWrite(LED_BUILTIN, LOW);
  Serial.println("\n" __FILE__ " " __DATE__ " " __TIME__);
#else
int main() {
#endif

  TRACE_LEVEL(TR_L_DEBUG);

  std::vector<std::string> IQOptionsMain{
      "Freq Cal", "CW PA Cal", "CW Rec Cal", "CW Xmit Cal", "SSB PA Cal", "SSB Rec Cal",
  };

  const std::vector<std::string> IQOptionsConst{
      "Freq Cal", "CW PA Cal", "CW Rec Cal", "CW Xmit Cal", "SSB PA Cal", "SSB Rec Cal",
  };

  TRACE_T41(TR_L_DEBUG, "--------------------");
  TRACE_T41(TR_L_DEBUG, "IQOptionsGlobal");
  printVectorAsStdString(IQOptionsGlobal);

  TRACE_T41(TR_L_DEBUG, "--------------------");
  TRACE_T41(TR_L_DEBUG, "IQOptionsMain");
  printVectorAsStdString(IQOptionsMain);

  TRACE_T41(TR_L_DEBUG, "--------------------");
  TRACE_T41(TR_L_DEBUG, "IQOptionsConst");
  printVectorAsStdString(IQOptionsConst);

  const std::vector<const char *> IQOptionsConstVectorConstChar{
      "Freq Cal", "CW PA Cal", "CW Rec Cal", "CW Xmit Cal", "SSB PA Cal", "SSB Rec Cal",
  };

  TRACE_T41(TR_L_DEBUG, "--------------------");

  TRACE_T41(TR_L_DEBUG, "--------------------");
  TRACE_T41(TR_L_DEBUG, "IQOptionsConstVectorConstCharGlobal");
  printVectorAsConstChar(IQOptionsConstVectorConstCharGlobal);

  TRACE_T41(TR_L_DEBUG, "--------------------");
  TRACE_T41(TR_L_DEBUG, "IQOptionsConstVectorConstChar");
  printVectorAsConstChar(IQOptionsConstVectorConstChar);

  // TRACE_T41(TR_L_DEBUG, F("Freq Cal"));
  // # define PSTR(s) ((const PROGMEM char *)(s))

  Serial.println(hello);
  TRACE_T41(TR_L_DEBUG, "hello: address:0x%X address&:0x%X sizeof:%u strlen:%u value:'%s'", hello, &hello, sizeof(hello),
            strlen((const char *)hello), hello);

  TRACE_T41(TR_L_DEBUG, "--------------------");
  TRACE_T41(TR_L_DEBUG, "IQOptionsConstVectorConstCharFlashGlobal");
  printVectorAsConstChar(IQOptionsConstVectorConstCharFlashGlobal);

  const std::vector<const char *> IQOptionsConstVectorConstCharFlashLocal{
      flashFreqCal, flashCwPaCal
      // , "CW PA Cal", "CW Rec Cal", "CW Xmit Cal", "SSB PA Cal", "SSB Rec Cal",
  };

  TRACE_T41(TR_L_DEBUG, "--------------------");
  TRACE_T41(TR_L_DEBUG, "IQOptionsConstVectorConstCharFlashLocal");
  printVectorAsConstChar(IQOptionsConstVectorConstCharFlashLocal);

  const char *labels[] = {
      "Select", "Menu Up", "Band Up",     "Zoom",        "Menu Dn", "Band Dn",        "Filter",       "DeMod",       "Mode",
      "NR",     "Notch",   "Noise Floor", "Coarse Incr", "Decoder", "Fine Increment", "Reset Tuning", "Frequ Entry", "User 2",
  };

  TRACE_T41(TR_L_DEBUG, "--------------------");
  TRACE_T41(TR_L_DEBUG, "sizeof(labels):%u",sizeof(labels));
  TRACE_T41(TR_L_DEBUG, "size:%u",sizeof(labels)/sizeof(const char *));

  // return 0;
}
