#ifndef l0ggi_v1_pinconfig
#define l0ggi_v1_pinconfig

#define uController ESP8266

#define HEATBED 0 // pwm possible
#define DHT_PIN 16
#define FAN 1            // pwm possible
#define BUTTON_UP 12     // interrupt
#define BUTTON_DOWN 13   // interrupt
#define BUTTON_FASTCALL1 // io
#define BUTTON_FASTCALL2 // io
#define BUTTON_FASTCALL3 // io

#define LED_HEATER_ON // io
#define LED_FAN_ON    // io
#define LED_FASTCALL1 // io
#define LED_FASTCALL2 // io
#define LED_FASTCALL3 // io

#define LCD_DATA 4 // i2c data
#define LCD_CLK 5  // i2c clock

#endif