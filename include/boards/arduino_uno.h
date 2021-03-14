#ifndef arduino_uno_board_pinconfig
#define arduino_uno_board_pinconfig

#define HEATBED // pwm possible
#define DHT_PIN 4
#define FAN              // pwm possible
#define BUTTON_UP 2      // interrupt
#define BUTTON_DOWN 3    // interrupt
#define BUTTON_FASTCALL1 // io
#define BUTTON_FASTCALL2 // io
#define BUTTON_FASTCALL3 // io

#define LED_HEATER_ON // io
#define LED_FAN_ON    // io
#define LED_FASTCALL1 // io
#define LED_FASTCALL2 // io
#define LED_FASTCALL3 // io

#define LCD_DATA A4 // i2c data
#define LCD_CLK A5  // i2c clock

#endif