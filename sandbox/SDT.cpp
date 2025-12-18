#include "SDT.h"

#if 0
#include <chrono>
#include <cstdio>

uint64_t millis() {
  const auto now = std::chrono::steady_clock::now().time_since_epoch();
  const auto m =
      std::chrono::duration_cast<std::chrono::milliseconds>(now).count();
  return m;
}

int usb_serial_class::printf(const char *format, const char *msg) {
  std::printf("%s", msg);
  return 0;
}
#endif
