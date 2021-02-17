#include "isr.hpp"

#ifdef board_ESP8266
ICACHE_RAM_ATTR void button_up_isr(void)
#else
void button_up_isr(void)
#endif
{
    Serial.println("button_up_isr() triggered");
}

#ifdef board_ESP8266
ICACHE_RAM_ATTR void button_down_isr(void)
#else
void button_down_isr(void)
#endif
{
    Serial.println("button_down_isr() triggered");
}
