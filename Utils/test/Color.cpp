#include "test/Testing.hpp"
#include "Utils/Color.hpp"

int main(void)
{
    Utils::Color black(0, 0, 0);
    Utils::Color white(255, 255, 255);
    Utils::Color yellow(255, 255, 0);
    Utils::Color purple(255, 0, 255);

    AssertEquals(black.asUint32(), 0x00000000u);
    AssertEquals(white.asUint32(), 0x00FFFFFFu);
    AssertEquals(yellow.asUint32(), 0x00FFFF00u);
    AssertEquals(purple.asUint32(), 0x00FF00FFu);

    return 0;
}
