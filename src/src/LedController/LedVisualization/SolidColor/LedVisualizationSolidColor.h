#ifndef LedVisualizationSolidColor_h_
#define LedVisualizationSolidColor_h_

#include <Arduino.h>

#include "../LedVisualizationBase.h"

class LedVisualizationSolidColor : LedVisualizationBase
{
private:
    int ledsSize;

    uint8_t red;
    uint8_t green;
    uint8_t blue;

public:
    uint handle(LedValue leds[]);
    LedVisualizationSolidColor(int ledsSize, uint8_t red, uint8_t green, uint8_t blue);
    ~LedVisualizationSolidColor();
    enum LedVisualizationType getVisualizationType();
};

#endif