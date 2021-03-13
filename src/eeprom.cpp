#include "config.h"
#include "eeprom.hpp"
#include <Arduino.h>
#include <EEPROM.h>

// perform a first write to eeprom if the eeprom is empty. last eeprom is used to check if arduino hasn't been used before
// note: ATMEGA 328p with 1024bytes of eeprom is used
void write_eeprom_initially(set_values *preset, quickcall *quickcall_buttons)
{
    byte last_byte_in_eeprom;
    EEPROM.get(1023, last_byte_in_eeprom);
#ifdef DEBUG_VIA_SERIAL
    Serial.print(F("DEBUG | EEPROM | Reading last byte from EEPROM: "));
    Serial.println(last_byte_in_eeprom);
#endif
    if (last_byte_in_eeprom != 65)
    {
#ifdef DEBUG_VIA_SERIAL
        Serial.print(F("DEBUG | EEPROM | Write Initial Config to EEPROM"));
#endif
        EEPROM.put(1023, 65);
        preset->humidity = 60.0;
        preset->temperature = 28.0;
        write_preset_to_eeprom(preset);
        quickcall_buttons->button_1 = (*preset);
        quickcall_buttons->button_2 = (*preset);
        quickcall_buttons->button_3 = (*preset);
        write_fastcalls_to_eeprom(quickcall_buttons);

#ifdef USE_WEB_SERVER
      /**  wifi_settings init_wifi_settings;
        init_wifi_settings.configured = false;
        init_wifi_settings.ssid = "";
        init_wifi_settings.password = "";
        write_wifi_settings_to_eeprom(&init_wifi_settings);
        */
#endif
    }
}

void write_preset_to_eeprom(set_values *preset)
{
#ifdef DEBUG_VIA_SERIAL
    Serial.println(F("DEBUG | EEPROM | Storing preset in EEPROM"));
#endif
    EEPROM.put(EEPROM_ADDRESS_PRESET, preset);
}

void read_preset_from_eeprom(set_values *preset)
{
#ifdef DEBUG_VIA_SERIAL
    Serial.println(F("DEBUG | EEPROM | Reading preset from EEPROM"));
#endif
    EEPROM.get(EEPROM_ADDRESS_PRESET, preset);
}

void write_fastcalls_to_eeprom(quickcall *quickcall_buttons)
{
#ifdef DEBUG_VIA_SERIAL
    Serial.println(F("DEBUG | EEPROM | Storing Fastcalls to EEPROM"));
#endif
    EEPROM.put(EEPROM_ADDRESS_QUICKCALL, quickcall_buttons);
}

void read_fastcalls_from_eeprom(quickcall *quickcall_buttons)
{
#ifdef DEBUG_VIA_SERIAL
    Serial.println(F("DEBUG | EEPROM | Reading fastcalls from EEPROM"));
#endif
    EEPROM.get(EEPROM_ADDRESS_QUICKCALL, quickcall_buttons);
}

#ifdef USE_WEB_SERVER
 // No need to write them in the eeprom. we can use a macro for this
/**
void write_wifi_settings_to_eeprom(wifi_settings *settings)
{
    Serial.println("DEBUG | EEPROM | Writing wifi settings to eeprom");
    EEPROM.put(EEPROM_ADDRESS_WIFI_SETTINGS, settings);
}

void read_wifi_settings_from_eeprom(wifi_settings *settings)
{
    Serial.println("DEBUG | EEPROM | Reading wifi settings from eeprom");
    EEPROM.get(EEPROM_ADDRESS_WIFI_SETTINGS, settings);
}

*/
#endif