#ifndef LedVisualizationBase_h_
#define LedVisualizationBase_h_

#include <Arduino.h>

struct LedValue
{
    uint8_t red;
    uint8_t green;
    uint8_t blue;
};

class LedVisualizationBase
{
private:
public:
    // Updates the LED array and returns a wait time, when it should be called next.
    virtual uint handle(LedValue leds[], int ledsSize);
};

#endif