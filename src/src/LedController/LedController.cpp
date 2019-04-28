#include "LedController.h"

LedController::LedController(int dataPin, int numLeds)
{
    this->dataPin = dataPin;
    this->numLeds = numLeds;

    this->strip = new Adafruit_NeoPixel(this->numLeds, this->dataPin, NEO_RGBW + NEO_KHZ800);
    strip->begin();
    strip->show();
}

void LedController::setSolidColor(int r, int g, int b, int w)
{
    for (int i = 0; i < this->numLeds; i++)
    {
        strip->setPixelColor(i, r, g, b, w);
    }
    strip->show();
}

LedController::~LedController()
{
    delete this->strip;
}