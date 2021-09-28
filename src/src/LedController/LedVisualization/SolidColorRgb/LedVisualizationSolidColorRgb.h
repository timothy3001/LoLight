#ifndef LedVisualizationSolidColorRgb_h_
#define LedVisualizationSolidColorRgb_h_

#include <Arduino.h>
#include "../SolidColor/LedVisualizationSolidColor.h"

class LedVisualizationSolidColorRgb : LedVisualizationSolidColor
{
    enum LedVisualizationType getVisualizationType();
};

#endif