#ifndef LedController_h_
#define LedController_h_

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

class LedController
{
public:
    LedController(int dataPin, int numLeds);

    void setSolidColor(uint8_t r, uint8_t g, uint8_t b);

private:
    ~LedController();
    int numLeds;
    int dataPin;

    static const uint8_t gamma8Correction[];
    Adafruit_NeoPixel *strip;

    void calculateRgbToRgbw(uint8_t *r, uint8_t *g, uint8_t *b, uint8_t *w);
};

#endif