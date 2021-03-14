#ifndef l0ggi_config
#define l0ggi_config

#define DEBUG_VIA_SERIAL
#define l0ggi_v1 // define board
#define USE_WEB_SERVER true

//#define HAS_BUTTONS
//#define USE_EEPROM

#define HAS_TEMP_SENSOR
//#define HAS_LIGHT_SENSOR
//#define HAS_NOX_SENSOR
//#define HAS_CO2_SENSOR

// find webserver related config in webserver_config.h

// PINS
// -----------------------------------------------------------------------------------------
#ifdef board_ARDUINO_UNO
#include "boards/arduino_uno.h"
#endif

#ifdef l0ggi_v1
#include "boards/l0ggi_v1.h"
#endif
// -----------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------

// Software Constants
// -----------------------------------------------------------------------------------------
#define loops_to_store 50 // loops until the current preset value is stored to a preset button
#define Kp
#define Ki
#define Kd

#ifdef HAS_BUTTONS
// type of button interrupt
#define INTERRUPT_TYPE FALLING
#endif

// fonts:
#define FONT_NUMBERS 0 //(u8g2_font_freedoomr10_mu)
#define FONT_TEXT 1    //(u8g2_font_VCR_OSD_tu)
#define FONT_ICONS 2   //(u8g2_font_open_iconic_gui_2x)


#ifdef USE_EEPROM
// uno full size: 1024 bytes
// EEPROM Address in bytes
#define EEPROM_ADDRESS_PRESET 0            // 2* 32bits (2*float ^= 2* 4byte)
#define EEPROM_ADDRESS_QUICKCALL 0 + 4 + 4 // beginn, 8 bytes for preset, quickcall is 24 bytes large
#ifdef USE_WEB_SERVER
// esp has its own macro to save wifi information
//#define EEPROM_ADDRESS_WIFI_SETTINGS 0 + EEPROM_ADDRESS_QUICKCALL + 24 + 1 // add one byte offset, just in case..
#endif
#endif
#endif // !config