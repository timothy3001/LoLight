#ifndef LedController_h_
#define LedController_h_

#include <Arduino.h>
#include <NeoPixelBus.h>

#include "LedVisualization/LedVisualizationBase.h"
#include "LedVisualization/SolidColor/LedVisualizationSolidColor.h"
#include "LedVisualization/LedVisualizationTwoColorBlending/LedVisualizationTwoColorBlending.h"

class LedController
{
public:
    LedController(int dataPin, int numLeds);
    ~LedController();
    void handle();

    void setSolidColor(uint8_t r, uint8_t g, uint8_t b);
    void setTwoColorBlending(uint8_t r1, uint8_t g1, uint8_t b1, uint8_t r2, uint8_t g2, uint8_t b2);

private:
    static const uint8_t gamma8Correction[];

    int numLeds;
    int dataPin;

    LedVisualizationBase *currentVisualization = NULL;
    ulong nextCheck = 0;

    // This is not ideal. We got two representations of the LEDs. But this way it's easiest to have a seperation between LED library and animation logic.
    // The arrays might become quite large, for 150 pixels it is 150 (Pixels) * 3 (RGB) * 2 (currentedValues and strip). That is 900 bytes of memory. But
    // since the ESP32 has 520kb of RAM, we should be fine here.
    LedValue *currentLedValues;
    NeoPixelBus<NeoGrbwFeature, Neo800KbpsMethod> *strip;

    void logDebug(String message);
    void calculateRgbToRgbw(uint8_t *r, uint8_t *g, uint8_t *b, uint8_t *w);
    void setPixels();
    void setPixelsFullSpectrum();
};

#endif