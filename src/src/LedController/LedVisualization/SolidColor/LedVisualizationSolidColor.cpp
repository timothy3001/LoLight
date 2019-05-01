#include "LedVisualizationSolidColor.h"

LedVisualizationSolidColor::LedVisualizationSolidColor(uint8_t red, uint8_t green, uint8_t blue)
{
}

uint LedVisualizationSolidColor::handle(LedValue leds[], int ledsSize)
{
    for (int i = 0; i < ledsSize; i++)
    {
        leds[i].red = red;
        leds[i].green = green;
        leds[i].blue = blue;
    }
    return 5000;
}
