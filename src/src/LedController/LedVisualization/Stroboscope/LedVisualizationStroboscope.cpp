#include "LedVisualizationStroboscope.h"

LedVisualizationStroboscope::LedVisualizationStroboscope(int ledsSize, int speed, uint8_t red, uint8_t green, uint8_t blue)
{
    this->ledsSize = ledsSize;
    this->speed = speed;

    if (speed < 50)
    {
        this->speed = 50;
    }

    this->red = red;
    this->green = green;
    this->blue = blue;
}

LedVisualizationStroboscope::~LedVisualizationStroboscope()
{
}

uint LedVisualizationStroboscope::handle(LedValue leds[])
{
    uint8_t newRed = 0;
    uint8_t newGreen = 0;
    uint8_t newBlue = 0;

    if (!currentState)
    {
        newRed = red;
        newGreen = green;
        newBlue = blue;
    }

    for (int i = 0; i < ledsSize; i++)
    {
        leds[i].red = newRed;
        leds[i].green = newGreen;
        leds[i].blue = newBlue;

        yield();
    }

    currentState = !currentState;
    return speed;
}
