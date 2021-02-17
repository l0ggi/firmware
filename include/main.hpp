#ifndef main_header
#define main_header
#include "config.h"
#include "constants.h"
#include <SimpleDHT.h>
#include "display.hpp"
// firmware related internal uses
unsigned long ms_begin_loop = 0;
time_struct current_time;
read_values current_values;
set_values preset;
quickcall quickcall_buttons;

#endif