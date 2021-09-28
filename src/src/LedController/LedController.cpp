#include "LedController.h"

LedController::LedController(int dataPin, int numLeds, String defaultColor)
{
    this->dataPin = dataPin;
    this->numLeds = numLeds;
    this->currentLedValues = new LedValue[numLeds];

    HelperFunctions::extractColorFromString(defaultColor, &defaultRed, &defaultGreen, &defaultBlue);

    this->strip = new NeoPixelBus<NeoGrbwFeature, Neo800KbpsMethod>(this->numLeds, this->dataPin);
    strip->Begin();

    // Disabled for workaround of wifi restarts
    // strip->Show();
}

LedController::~LedController()
{
    delete this->strip;
    delete this->currentLedValues;
    if (this->currentVisualization)
        delete this->currentVisualization;
}

void LedController::handle()
{
    if (currentVisualization)
    {
        ulong now = millis();
        if (nextCheck <= now)
        {
            uint delay = currentVisualization->handle(currentLedValues);

            bool onlyRgb = this->currentVisualization->getVisualizationType() == SolidColorRgb;

            setPixels(onlyRgb);
            nextCheck = delay == 0 ? 0 : millis() + delay;
        }
    }
}

void LedController::setOnOff(bool on)
{
    turnedOn = on;

    if (!on)
    {
        currentVisualization = (LedVisualizationBase *)new LedVisualizationSolidColor(numLeds, 0, 0, 0);
        nextCheck = 0;
        handle();
    }
    else
        setDefault();
}

bool LedController::isTurnedOn()
{
    return turnedOn;
}

void LedController::setDefault()
{
    turnedOn = true;
    currentVisualization = (LedVisualizationBase *)new LedVisualizationSolidColor(numLeds, defaultRed, defaultGreen, defaultBlue);
    nextCheck = 0;
    handle();
}

void LedController::setBrightness(float brightness)
{
    this->brightnessInverted = 255 - (int)round((brightness * (float)255));
    nextCheck = 0;
    handle();
}

float LedController::getBrightness()
{
    return ((255 - this->brightnessInverted) / (float)255);
}

void LedController::setSolidColor(uint8_t r, uint8_t g, uint8_t b, bool onlyRgb)
{
    turnedOn = true;
    logDebug("Setting solid color r: '" + String(r) + "' g: '" + String(g) + "' b: '" + String(b) + "'...");

    if (currentVisualization)
        delete currentVisualization;
    if (!onlyRgb)
        currentVisualization = (LedVisualizationBase *)new LedVisualizationSolidColor(numLeds, r, g, b);
    else
        currentVisualization = (LedVisualizationBase *)new LedVisualizationSolidColorRgb(numLeds, r, g, b);
    nextCheck = 0;
    handle();
}

void LedController::setTwoColorBlendingAnimated(int cycleDuration, bool randomStartOrder, bool useLinearEase, uint8_t r1, uint8_t g1, uint8_t b1, uint8_t r2, uint8_t g2, uint8_t b2)
{
    turnedOn = true;
    logDebug("Setting two color blending r1: '" + String(r1) + "' g1: '" + String(g1) + "' b1: '" + String(b1) + "' r2: '" + String(r2) + "' g2: '" + String(g2) + "' b2: '" + String(b2) + "'...");
    if (currentVisualization)
        delete currentVisualization;
    currentVisualization = (LedVisualizationBase *)new LedVisualizationTwoColorBlendingAnimated(numLeds, cycleDuration, randomStartOrder, useLinearEase, r1, g1, b1, r2, g2, b2);
    nextCheck = 0;
    handle();
}

void LedController::setStroboscope(int speed, uint8_t r, uint8_t g, uint8_t b)
{
    turnedOn = true;
    logDebug("Setting stroboscope...");
    if (currentVisualization)
        delete currentVisualization;
    currentVisualization = (LedVisualizationBase *)new LedVisualizationStroboscope(numLeds, speed, r, g, b);
    nextCheck = 0;
    handle();
}

void LedController::setPixels(bool onlyRgb)
{
    for (int i = 0; i < numLeds; i++)
    {
        uint8_t w = 0;
        uint8_t r = currentLedValues[i].red;
        uint8_t g = currentLedValues[i].green;
        uint8_t b = currentLedValues[i].blue;

        if(!onlyRgb)
            calculateRgbToRgbw(&r, &g, &b, &w);

        RgbwColor color(r, g, b, w);
        strip->SetPixelColor(i, color);
    }
    strip->Show();
}

void LedController::calculateRgbToRgbw(uint8_t *r, uint8_t *g, uint8_t *b, uint8_t *w)
{
    *r = ((int)*r - this->brightnessInverted) < 0 ? 0 : *r - this->brightnessInverted;
    *g = ((int)*g - this->brightnessInverted) < 0 ? 0 : *g - this->brightnessInverted;
    *b = ((int)*b - this->brightnessInverted) < 0 ? 0 : *b - this->brightnessInverted;

    int whitePart = 255;
    if (*r < whitePart)
        whitePart = *r;
    if (*g < whitePart)
        whitePart = *g;
    if (*b < whitePart)
        whitePart = *b;

    int whitePartSubtrahend = (int)ceil(whitePart / (float)3);

    *r = gamma8Correction[*r - whitePartSubtrahend];
    *g = gamma8Correction[*g - whitePartSubtrahend];
    *b = gamma8Correction[*b - whitePartSubtrahend];
    *w = gamma8Correction[whitePart];
}

void LedController::logDebug(String message)
{
#ifdef DEBUG
    Serial.print("LedController: ");
    Serial.println(message);
#endif
}

const uint8_t LedController::gamma8Correction[] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2,
    2, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 5, 5, 5,
    5, 6, 6, 6, 6, 7, 7, 7, 7, 8, 8, 8, 9, 9, 9, 10,
    10, 10, 11, 11, 11, 12, 12, 13, 13, 13, 14, 14, 15, 15, 16, 16,
    17, 17, 18, 18, 19, 19, 20, 20, 21, 21, 22, 22, 23, 24, 24, 25,
    25, 26, 27, 27, 28, 29, 29, 30, 31, 32, 32, 33, 34, 35, 35, 36,
    37, 38, 39, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 50,
    51, 52, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 66, 67, 68,
    69, 70, 72, 73, 74, 75, 77, 78, 79, 81, 82, 83, 85, 86, 87, 89,
    90, 92, 93, 95, 96, 98, 99, 101, 102, 104, 105, 107, 109, 110, 112, 114,
    115, 117, 119, 120, 122, 124, 126, 127, 129, 131, 133, 135, 137, 138, 140, 142,
    144, 146, 148, 150, 152, 154, 156, 158, 160, 162, 164, 167, 169, 171, 173, 175,
    177, 180, 182, 184, 186, 189, 191, 193, 196, 198, 200, 203, 205, 208, 210, 213,
    215, 218, 220, 223, 225, 228, 231, 233, 236, 239, 241, 244, 247, 249, 252, 255};