#ifndef dht_header
#define dht_header
#include <Arduino.h>
#include "config.h"
#include "constants.h"

void temp_sensor_setup(void);
void temp_sensor_read(read_values *storage, time_struct timestamp);

#endif // !dht_header
