#ifndef eeprom_header
#define eeprom_header
#include "config.h"
#include "constants.h"

void write_eeprom_initially(set_values *preset, quickcall *quickcall_buttons);
void write_preset_to_eeprom(set_values *preset);
void read_preset_from_eeprom(set_values *preset);
void write_fastcalls_to_eeprom(quickcall *quickcall_buttons);
void read_fastcalls_from_eeprom(quickcall *quickcall_buttons);

#ifdef USE_WEB_SERVER
void write_wifi_settings_to_eeprom(wifi_settings *settings);
void read_wifi_settings_from_eeprom(wifi_settings *settings);
#endif
#endif // !eeprom_header
