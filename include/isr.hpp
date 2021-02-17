#ifndef isr_header
#define isr_header
#include <Arduino.h> // needed for easier string access
#include "config.h"

#ifdef board_ESP8266
ICACHE_RAM_ATTR void button_down_isr(void);
#else
void button_down_isr(void);
#endif

#ifdef board_ESP8266
ICACHE_RAM_ATTR void button_up_isr(void);
#else
void button_up_isr(void);
#endif

#endif // !isr_header