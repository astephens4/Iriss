#ifndef IRISS_TEST_H
#define IRISS_TEST_H 1

#include <iostream>
#include <cinttypes>
#include <cmath>

#define MAX_ULP_DIFFERENCE 5

#include <cassert>
#define MAKE_ASSERTION(op1, operation, op2) \
    assert(((op1) operation (op2)) \
            || !(std::cout << "Assertion Failed @ " << __FILE__ << ":" << __LINE__ << " - " << #op1 << " " << #operation << " " << #op2 \
                           << "\n(" << op1 << #operation << op2 << ")\n"))

union Float_t {
    Float_t(float val) : f(val) { }
    bool negative() { return f < 0.0f; }

    int32_t i;
    float f;
};

void AssertEquals(float arg1, float arg2)
{
    Float_t f1(arg1);
    Float_t f2(arg2);

    if(f1.negative() != f2.negative()) {
        if(std::fabs(f1.f) == std::fabs(f2.f))
            return;
        MAKE_ASSERTION(f1.f, ==, f2.f);
    }

    int ulpDiff = std::abs(f1.i - f2.i);
    MAKE_ASSERTION(ulpDiff, <=, MAX_ULP_DIFFERENCE);
}

void AssertEquals(int32_t arg1, int32_t arg2)
{
    MAKE_ASSERTION(arg1, ==, arg2);
}

void AssertEquals(uint32_t arg1, uint32_t arg2)
{
    MAKE_ASSERTION(arg1, ==, arg2);
}

void AssertEquals(int16_t arg1, int16_t arg2)
{
    MAKE_ASSERTION(arg1, ==, arg2);
}

void AssertEquals(uint16_t arg1, uint16_t arg2)
{
    MAKE_ASSERTION(arg1, ==, arg2);
}

void AssertEquals(uint8_t arg1, uint8_t arg2)
{
    MAKE_ASSERTION(arg1, ==, arg2);
}

void AssertEquals(int8_t arg1, int8_t arg2)
{
    MAKE_ASSERTION(arg1, ==, arg2);
}


#endif // IRISS_TEST_H
