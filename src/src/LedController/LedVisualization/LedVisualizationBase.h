#ifndef LedVisualizationBase_h_
#define LedVisualizationBase_h_

#include <Arduino.h>

enum LedVisualizationType {
    SolidColor,
    SolidColorRgb,
    Stroboscope,
    TwoColorBlendingAnimated
};

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
    virtual ~LedVisualizationBase(){};

    // Updates the LED array and returns a wait time, when it should be called next.
    // ATTENTION: Don't forget to yield() sometimes so that WiFi and so on won't crash!
    virtual uint handle(LedValue leds[]);
    virtual enum LedVisualizationType getVisualizationType();
};

#endif