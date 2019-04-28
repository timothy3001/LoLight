#ifndef LedController_h_
#define LedController_h_

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

class LedController
{
public:
    LedController(int dataPin, int numLeds);

    void setSolidColor(int r, int g, int b, int w);

private:
    ~LedController();
    int numLeds;
    int dataPin;

    Adafruit_NeoPixel *strip;
};

#endif