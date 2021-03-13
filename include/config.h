#ifndef l0ggi_config
#define l0ggi_config

#define DEBUG_VIA_SERIAL
#define board_ESP8266 // define board
#define USE_WEB_SERVER true

// find webserver related config in webserver_config.h

// PINS
// -----------------------------------------------------------------------------------------
#ifdef board_ARDUINO_UNO
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

#ifdef board_ESP8266
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
// -----------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------

// Software Constants
// -----------------------------------------------------------------------------------------
#define loops_to_store 50 // loops until the current preset value is stored to a preset button
#define Kp
#define Ki
#define Kd

// type of button interrupt
#define INTERRUPT_TYPE FALLING

// fonts:
#define FONT_NUMBERS 0 //(u8g2_font_freedoomr10_mu)
#define FONT_TEXT 1    //(u8g2_font_VCR_OSD_tu)
#define FONT_ICONS 2   //(u8g2_font_open_iconic_gui_2x)

// uno full size: 1024 bytes
// EEPROM Address in bytes
#define EEPROM_ADDRESS_PRESET 0            // 2* 32bits (2*float ^= 2* 4byte)
#define EEPROM_ADDRESS_QUICKCALL 0 + 4 + 4 // beginn, 8 bytes for preset, quickcall is 24 bytes large
#ifdef USE_WEB_SERVER
// esp has its own macro to save wifi information
//#define EEPROM_ADDRESS_WIFI_SETTINGS 0 + EEPROM_ADDRESS_QUICKCALL + 24 + 1 // add one byte offset, just in case..
#endif
#endif // !config