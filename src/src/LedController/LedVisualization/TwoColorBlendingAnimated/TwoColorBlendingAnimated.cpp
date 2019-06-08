#include "TwoColorBlendingAnimated.h"

TwoColorBlendingAnimated::TwoColorBlendingAnimated(int ledsSize, int cycleDurationMs, bool randomStartOrder, bool useLinearEase, uint8_t r1, uint8_t g1, uint8_t b1, uint8_t r2, uint8_t g2, uint8_t b2)
{
    this->ledsSize = ledsSize;
    this->cycleDurationMs = cycleDurationMs;
    this->useLinearEase = useLinearEase;

    this->r1 = r1;
    this->g1 = g1;
    this->b1 = b1;

    this->r2 = r2;
    this->g2 = g2;
    this->b2 = b2;

    this->progressShifts = new float[ledsSize];
    if (randomStartOrder || ledsSize < 20)
    {
        this->createRandomStartOrder();
    }
    else
    {
        this->createSmoothStartOrder();
    }
}

void TwoColorBlendingAnimated::createRandomStartOrder()
{
    for (int i = 0; i < ledsSize; i++)
    {
        this->progressShifts[i] = random(100) / (float)100;
    }
}

void TwoColorBlendingAnimated::createSmoothStartOrder()
{
    int cycleLength = 20;

    for (int i = 0; i < ledsSize; i++)
    {
        float part = (i % cycleLength) / (float)cycleLength;

        this->progressShifts[i] = MathFunctions::CompleteQuadraticEase(part) + ((random(20) - 10) / (float)100);

        if (this->progressShifts[i] > 1.0F)
            this->progressShifts[i] = 1.0F;
        else if (this->progressShifts[i] < 0.0F)
            this->progressShifts[i] = 0.0F;
    }
}

TwoColorBlendingAnimated::~TwoColorBlendingAnimated()
{
    delete[] this->progressShifts;
}

uint TwoColorBlendingAnimated::handle(LedValue leds[])
{
    float progressStep = (millis() % this->cycleDurationMs) / (float)cycleDurationMs;

    for (int i = 0; i < ledsSize; i++)
    {
        float progress = MathFunctions::handleOverflowingFloat(progressShifts[i] + progressStep);
        progress = this->useLinearEase ? MathFunctions::LinearEase(progress) : MathFunctions::CubicInEase(progress);
        progress = MathFunctions::reverseOverPoint5(progress);

        leds[i].red = (2 * (r2 - r1) * progress) + r1;
        leds[i].green = (2 * (g2 - g1) * progress) + g1;
        leds[i].blue = (2 * (b2 - b1) * progress) + b1;
    }

    return 20;
}

void TwoColorBlendingAnimated::logDebug(String message)
{
#ifdef DEBUG
    Serial.print("LedController: ");
    Serial.println(message);
#endif
}