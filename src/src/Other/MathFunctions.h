#ifndef MathFunctions_h_
#define MathFunctions_h_

class MathFunctions
{
public:
    static float LinearEase(float progress)
    {
        return progress;
    }

    static float QuadraticEase(float progress)
    {
        return progress * progress;
    }

    static float CompleteQuadraticEase(float progress)
    {
        return -pow((2 * progress) - 1, 2.0F) + 1;
    }

    static float CubicOutEase(float progress)
    {
        progress -= 1.0f;
        progress = progress * progress * progress;
        progress += 1.0f;

        return progress;
    }

    static float CubicInEase(float progress)
    {
        return progress * progress * progress;
    }

    static float handleOverflowingFloat(float f)
    {
        if (f > 1.0F)
            return f - floorf(f);
        return f;
    }

    static float reverseOverPoint5(float f)
    {
        if (f <= 0.5F)
        {
            return f;
        }
        else
        {
            return 1 - f;
        }
    }
};

#endif