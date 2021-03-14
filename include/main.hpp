#ifndef main_header
#define main_header
#include "constants.h"
#include <ArduinoOTA.h>
#ifndef config
#include "config.h"
#endif // !config
#include "time_t.hpp"
#include "isr.hpp"
#include "eeprom.hpp"
#include "sensors.hpp"
#include "webserver.h"
#include "display.hpp"

// firmware related internal uses
unsigned long ms_begin_loop = 0;
time_struct current_time;
read_values current_values;
#ifdef USE_EEPROM
set_values preset;
#endif
#ifdef HAS_BUTTONS
quickcall quickcall_buttons;
#endif

#endif