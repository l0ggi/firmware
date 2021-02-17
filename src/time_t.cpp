/**
 * time relevant functions
 */
#include "config.h"
#include "time_t.hpp"

#ifdef DEBUG_VIA_SERIAL
#define VERBOSE_TIME
#endif

void add_to_time_struct(int ms, time_struct *current_time)
{
#ifdef VERBOSE_TIME_1
    Serial.print(F("DEBUG | TIME | Adding "));
    Serial.print(ms);
    Serial.println(F("ms to time"));
#endif
    // add the ms to the millis
    current_time->millis += ms;
    if (current_time->millis >= 1000)
    {
#ifdef VERBOSE_TIME_1
        Serial.println(F("DEBUG | TIME | millis overflow - adding a second"));
        Serial.print(F("DEBUG | TIME | Current run-time: "));
        Serial.print(current_time->hours);
        Serial.print(":");
        Serial.print(current_time->minutes);
        Serial.print(":");
        Serial.println(current_time->seconds);
#endif
        current_time->seconds++;
        current_time->millis -= 1000;
    }
    if (current_time->seconds >= 60)
    {
#ifdef VERBOSE_TIME
        Serial.println(F("DEBUG | TIME | seconds overflow - adding a minute"));
#endif
        current_time->minutes++;
        current_time->seconds = 0;
    }
    if (current_time->minutes >= 60)
    {
#ifdef VERBOSE_TIME
        Serial.println(F("DEBUG | TIME | minutes overflow - adding a hour"));
#endif
        current_time->hours++;
        current_time->minutes = 0;
    }
}