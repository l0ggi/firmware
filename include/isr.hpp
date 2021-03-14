#ifndef isr_header
#define isr_header
#include <Arduino.h> // needed for easier string access
#include "config.h"

#ifdef HAS_BUTTONS
#if uController == ESP8266
ICACHE_RAM_ATTR void button_down_isr(void);
#else
void button_down_isr(void);
#endif

#if uController == ESP8266
ICACHE_RAM_ATTR void button_up_isr(void);
#else
void button_up_isr(void);
#endif

#endif
#endif // !isr_header