#ifndef TwoColorBlendingAnimated_h_
#define TwoColorBlendingAnimated_h_

#include "../LedVisualizationBase.h"
#include "../../../Other/MathFunctions.h"

class TwoColorBlendingAnimated : LedVisualizationBase
{
private:
    int ledsSize;
    int cycleDurationMs;

    uint8_t r1, g1, b1;
    uint8_t r2, g2, b2;

    float *progressShifts;

public:
    uint handle(LedValue leds[]);
    TwoColorBlendingAnimated(int ledsSize, int cycleDurationMs, uint8_t r1, uint8_t g1, uint8_t b1, uint8_t r2, uint8_t g2, uint8_t b2);
    ~TwoColorBlendingAnimated();
};

#endif