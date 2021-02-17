#ifndef display_header
#define display_header

#include <U8g2lib.h>
#include <Wire.h>
#include "config.h"
#include "constants.h"

void display_setup(void);
void display_apply_font(void);
void display_push(void);
void display_print(int x, int y, char *message);
void display_print_time(time_struct timestruct);
void display_print_ip(String ip);
void display_print_temperature(read_values tempstruct);
void display_clear(void);
#endif // !display_header
