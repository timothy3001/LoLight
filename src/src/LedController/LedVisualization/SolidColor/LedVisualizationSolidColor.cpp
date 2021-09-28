#include "LedVisualizationSolidColor.h"

LedVisualizationSolidColor::LedVisualizationSolidColor(int ledsSize, uint8_t red, uint8_t green, uint8_t blue)
{
    this->ledsSize = ledsSize;

    this->red = red;
    this->green = green;
    this->blue = blue;
}

LedVisualizationSolidColor::~LedVisualizationSolidColor()
{
}

uint LedVisualizationSolidColor::handle(LedValue leds[])
{
    for (int i = 0; i < ledsSize; i++)
    {
        leds[i].red = red;
        leds[i].green = green;
        leds[i].blue = blue;

        yield();
    }
    return 5000;
}

enum LedVisualizationType LedVisualizationSolidColor::getVisualizationType()
{
    return SolidColor;
}
