#ifndef LedVisualizationStroboscope_h_
#define LedVisualizationStroboscope_h_

#include <Arduino.h>

#include "../LedVisualizationBase.h"

class LedVisualizationStroboscope : LedVisualizationBase
{
private:
    int ledsSize;

    uint8_t red;
    uint8_t green;
    uint8_t blue;

    int speed;
    bool currentState = false;

public:
    uint handle(LedValue leds[]);
    LedVisualizationStroboscope(int ledsSize, int speed, uint8_t red, uint8_t green, uint8_t blue);
    ~LedVisualizationStroboscope();
    enum LedVisualizationType getVisualizationType();
};

#endif