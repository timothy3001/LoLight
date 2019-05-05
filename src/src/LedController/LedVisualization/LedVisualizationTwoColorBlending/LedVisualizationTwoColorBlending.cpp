#include "LedVisualizationTwoColorBlending.h"

LedVisualizationTwoColorBlending::LedVisualizationTwoColorBlending(int ledsSize, int cycleDurationMs, uint8_t r1, uint8_t g1, uint8_t b1, uint8_t r2, uint8_t g2, uint8_t b2)
{
    this->ledsSize = ledsSize;
    this->cycleDurationMs = cycleDurationMs;

    this->r1 = r1;
    this->g1 = g1;
    this->b1 = b1;

    this->r2 = r2;
    this->g2 = g2;
    this->b2 = b2;

    this->progressShifts = new float[ledsSize];
    for (int i = 0; i < ledsSize; i++)
    {
        this->progressShifts[i] = random(100) / (float)100;
    }
}

LedVisualizationTwoColorBlending::~LedVisualizationTwoColorBlending()
{
}

uint LedVisualizationTwoColorBlending::handle(LedValue leds[])
{
    float progressStep = (millis() % this->cycleDurationMs) / (float)cycleDurationMs;

    for (int i = 0; i < ledsSize; i++)
    {
        float progress = MathFunctions::handleOverflowingFloat(progressShifts[i] + progressStep);
        progress = MathFunctions::CubicInEase(progress);
        progress = MathFunctions::reverseOverPoint5(progress);

        leds[i].red = (2 * (r2 - r1) * progress) + r1;
        leds[i].green = (2 * (g2 - g1) * progress) + g1;
        leds[i].blue = (2 * (b2 - b1) * progress) + b1;
    }

    return 20;
}