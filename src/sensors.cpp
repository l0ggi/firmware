#include "sensors.hpp"
#include <SimpleDHT.h>

SimpleDHT22 temp_sensor(DHT_PIN);

void temp_sensor_setup(void)
{
    // currently no boot needed
}

void temp_sensor_read(read_values *storage, time_struct timestamp)
{
// start working...
#ifdef DEBUG_VIA_SERIAL
    Serial.println(F("DEBUG | TEMPSENSOR | Start reading.."));
#endif

    float temperature = 0;
    float humidity = 0;
    int err = SimpleDHTErrSuccess;
    if ((err = temp_sensor.read2(&temperature, &humidity, NULL)) != SimpleDHTErrSuccess)
    {
#ifdef DEBUG_VIA_SERIAL
        Serial.print(F("DEBUG | TEMPSENSOR | Read DHT22 failed, err="));
        Serial.println(err);
#endif
        return;
    }
#ifdef DEBUG_VIA_SERIAL
    Serial.print(F("DEBUG | TEMPSENSOR | Read values: "));
    Serial.print(temperature);
    Serial.print(F("°C "));
    Serial.print(humidity);
    Serial.println(F("%"));
#endif

    set_values temp_struct;
    temp_struct.humidity = humidity;
    temp_struct.temperature = temperature;

    storage->values = temp_struct;
    storage->timestamp = timestamp;
}