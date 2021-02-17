#include "config.h"
#include <Arduino.h>
#ifndef constants
#define constants

// Structs
// -----------------------------------------------------------------------------------------
// uses the time since power on since no rtc is present
struct time_struct
{
    int millis; // not needed for time display but for overflow
    int seconds;
    int minutes;
    int hours;
};

// save to eeprom after 2 minutes without changes
struct set_values
{
    float humidity; // 4 byte
    float temperature; // 4 byte
};

// stores the current values for humidity and temperature with their measured timestamp
struct read_values
{
    set_values values; // 8 byte
    time_struct timestamp;
};

// save to eeprom as soon as set
struct quickcall
{
    set_values button_1; // 8 byte
    set_values button_2; // 8 byte
    set_values button_3; // 8 byte
};

#ifdef USE_WEB_SERVER
//wifi connection settings
struct wifi_settings
{
    String ssid;
    String password;
    boolean configured;
};
#endif
#endif // !constants