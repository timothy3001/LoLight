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

    static float CubicOutEase(float progress)
    {
        progress -= 1.0f;
        progress = progress * progress * progress;
        progress += 1.0f;

        return progress;
    }
};

#endif