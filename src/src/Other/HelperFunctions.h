#ifndef HelperFunctions_h_
#define HelperFunctions_h_

#include <Arduino.h>

class HelperFunctions
{
public:
    static void extractColorFromString(const String &str, uint8_t *r, uint8_t *g, uint8_t *b)
    {
        char color[8];
        str.toCharArray(color, 8);

        char redPart[3];
        char greenPart[3];
        char bluePart[3];

        strncpy(redPart, color + 1, 2);
        redPart[2] = 0;
        strncpy(greenPart, color + 3, 2);
        greenPart[2] = 0;
        strncpy(bluePart, color + 5, 2);
        bluePart[2] = 0;

        *r = strtol(redPart, NULL, 16);
        *g = strtol(greenPart, NULL, 16);
        *b = strtol(bluePart, NULL, 16);
    }
};

#endif