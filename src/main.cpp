/**
 * l0ggi wifi data logger
 */
#include <Arduino.h>
#include "main.hpp"

void setup()
{

#ifdef DEBUG_VIA_SERIAL
  // as we are still debugging this thing - start the serial connection at 115200 baud
  Serial.begin(115200);

  Serial.println(("DEBUG | SETUP | BOOTING"));
// start display
#endif
  display_setup();

  // initialize the time calculator
  current_time.hours = 0;
  current_time.millis = 0;
  current_time.minutes = 0;
  current_time.seconds = 0;

  // initialize the read values structure
  current_values.values.humidity = 0;
  current_values.values.temperature = 0;
  current_values.timestamp = current_time;

#ifdef DEBUG_VIA_SERIAL
  Serial.println(F("DEBUG | SETUP | Setting up ISR for Button 1"));
#endif
  // button up isr
  pinMode(BUTTON_UP, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(BUTTON_UP), button_up_isr, INTERRUPT_TYPE);

#ifdef DEBUG_VIA_SERIAL
  Serial.println(F("DEBUG | SETUP | Setting up ISR for Button 2"));
#endif
  // button down isr
  pinMode(BUTTON_DOWN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(BUTTON_DOWN), button_down_isr, INTERRUPT_TYPE);

  // check if its the first ever start and if write the eeprom initially
  write_eeprom_initially(&preset, &quickcall_buttons);
  // read last stored values from EEPROM
  read_preset_from_eeprom(&preset);
  read_fastcalls_from_eeprom(&quickcall_buttons);

#ifdef USE_WEB_SERVER
  webserver_setup();
  ArduinoOTA.onStart([]() {});
  ArduinoOTA.begin();
#endif
}

/**
 * interrupt up:
 * - increase the temp value by 1 digit
 * 
 * interrupt down:
 * - decrease the temp value by 1 digit
 * 
 *  single loop:
 *  - read the quick-buttons, if one is pressed increase the quick-array entry, if value is above the store-val store the current set_temp in struct and in eeprom
 *  - measure the last loop time and add it to the time_t struct
 * 
 *  - if measure cycle this time, measure temperature and humidity 
 *  -- update display that menu is blocked (tiny icon or something)
 *  -- pull values for humidity and temperature and store in current_values struct
 *  -- update display that menu is available
 *  -- update display to current values
 *  - call pid-loop for possible regulation
 * 
 */

int lastSecs = 0;

void loop()
{
  ms_begin_loop = millis();

  // run only every second
  if (lastSecs != current_time.seconds)
  {
    // read temp sensor (sampling rate is 0.5Hz, but don't care about the dropped sample)
    temp_sensor_read(&current_values, current_time);
    display_clear();

#ifdef USE_WEB_SERVER
    display_print_ip(webserver_local_ip());
#else
    display_print_time(current_time);
#endif

    display_print_temperature(current_values);
    display_push();
    lastSecs = current_time.seconds;
  }

  if (millis() - ms_begin_loop < 100)
  {
    delay(100 - (millis() - ms_begin_loop));
  }
  add_to_time_struct(millis() - ms_begin_loop, &current_time);
  webserver_loop(current_values);
  ArduinoOTA.handle();
}
