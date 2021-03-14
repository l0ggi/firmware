#include "isr.hpp"

#ifdef HAS_BUTTONS
#if uController == ESP8266
ICACHE_RAM_ATTR void button_up_isr(void)
#else
void button_up_isr(void)
#endif
{
    Serial.println("button_up_isr() triggered");
}

#if uController == ESP8266
ICACHE_RAM_ATTR void button_down_isr(void)
#else
void button_down_isr(void)
#endif
{
    Serial.println("button_down_isr() triggered");
}
#endif