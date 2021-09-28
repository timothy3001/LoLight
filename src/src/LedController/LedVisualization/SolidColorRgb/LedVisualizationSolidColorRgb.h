#ifndef LedVisualizationSolidColorRgb_h_
#define LedVisualizationSolidColorRgb_h_

#include <Arduino.h>
#include "../SolidColor/LedVisualizationSolidColor.h"

class LedVisualizationSolidColorRgb : LedVisualizationSolidColor
{
    public:
        LedVisualizationSolidColorRgb(int ledsSize, uint8_t red, uint8_t green, uint8_t blue) : LedVisualizationSolidColor(ledsSize, red, green, blue) {};
        enum LedVisualizationType getVisualizationType();
};

#endif