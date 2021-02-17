#include "display.hpp"
#include <stdio.h>

// Rotation:
// R0 = default
// R1 = 90°
// R2 = 180°
// R3 = 270°
U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C display(U8G2_R0, /* reset=*/U8X8_PIN_NONE, LCD_CLK, LCD_DATA);

void display_apply_font()
{
    display.setFont(u8g2_font_ncenB14_tr);
}

void display_setup(void)
{
#ifdef DEBUG_VIA_SERIAL
    Serial.println(F("DEBUG | DISPLAY | Booting Display.."));
#endif
    display.begin();
    display_apply_font();
    display.sendBuffer();
}

void display_print(int x, int y, char *message)
{
#ifdef DEBUG_VIA_SERIAL
    Serial.print(F("DEBUG | DISPLAY | Writing message to display: "));
    Serial.println(message);
#endif
    display.drawStr(x, y, message);
}

void display_push(void)
{
    display.sendBuffer();
}

void display_print_time(time_struct timestruct)
{
    char current;
    sprintf(&current, "%d:%d:%d", timestruct.hours, timestruct.minutes, timestruct.seconds);
    display_print(0, 14, &current);
}

void display_print_ip(String ip)
{
    char current;
    ip.toCharArray(&current, 16);
    display_print(0, 14, &current);
}

void display_print_temperature(read_values tempstruct)
{
#ifdef DEBUG_VIA_SERIAL
    Serial.print(F("DEBUG | DISPLAY | Writing temperature: "));
    Serial.print(tempstruct.values.temperature);
    Serial.print(F("°C and "));
    Serial.print(tempstruct.values.humidity);
    Serial.println(F("% humidity to display"));
#endif

    /**
     * Arduino has no float-functions in sprintf.. we need do build something on our own:
     * */

    String temp = String(tempstruct.values.temperature, '\002');
    String humid = String(tempstruct.values.humidity, '\000');

    char curTemp[sizeof("00.0")];
    char curHumid[sizeof("00")];
    temp.toCharArray(curTemp, sizeof("00.0"));
    humid.toCharArray(curHumid, sizeof("00"));

    char current;
    sprintf(&current, "%s °C %s %%", curTemp, curHumid);
    display_print(0, 32, &current);
}

void display_clear(void)
{
    display.clearBuffer();
}