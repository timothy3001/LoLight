#ifndef LedVisualizationTwoColorBlendingAnimated_h_
#define LedVisualizationTwoColorBlendingAnimated_h_

#include "../LedVisualizationBase.h"
#include "../../../Other/MathFunctions.h"

class LedVisualizationTwoColorBlendingAnimated : LedVisualizationBase
{
private:
    int ledsSize;
    int cycleDurationMs;
    bool useLinearEase;

    uint8_t r1, g1, b1;
    uint8_t r2, g2, b2;

    float *progressShifts;

    void createRandomStartOrder();
    void createSmoothStartOrder();
    void logDebug(String message);

public:
    uint handle(LedValue leds[]);
    LedVisualizationTwoColorBlendingAnimated(int ledsSize, int cycleDurationMs, bool randomStartOrder, bool useLinearEase, uint8_t r1, uint8_t g1, uint8_t b1, uint8_t r2, uint8_t g2, uint8_t b2);
    ~LedVisualizationTwoColorBlendingAnimated();
    enum LedVisualizationType getVisualizationType();
};

#endif