#include "FloatEquals.hpp"
#include <cmath>

namespace Utils {
union Float_t {
    Float_t(float val) : f(val) { }
    bool negative() { return f < 0.0f; }

    int i;
    float f;
};

bool FloatEquals(float arg1, float arg2)
{
    Float_t f1(arg1);
    Float_t f2(arg2);

    if(f1.negative() != f2.negative()) {
        if(std::fabs(f1.f) == std::fabs(f2.f))
            return true;
        return false;
    }

    int ulpDiff = std::abs(f1.i - f2.i);
    return ulpDiff <= MAX_ULP_DIFFERENCE;
}
} // end namespace Utils
