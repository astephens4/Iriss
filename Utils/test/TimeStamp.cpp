#include "Utils/TimeStamp.hpp"
#include "test/Testing.hpp"

int main(void)
{
    Utils::TimeStamp t1;

    sleep(5);

    Utils::TimeStamp t2;

    AssertEquals(static_cast<int32_t>(t2.get_sec() - t1.get_sec()), static_cast<int32_t>(5));

    return 0;
}
