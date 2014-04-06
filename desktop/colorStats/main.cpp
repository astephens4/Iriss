#include <iostream>
#include "Iriss/Command.hpp"

int main(void)
{
    Iriss::Command cmd;
    cmd.include(Iriss::Command::NUDGE_ROLL_LEFT);
    cmd.include(Iriss::Command::NUDGE_UP);
    cmd.include(Iriss::Command::NUDGE_YAW_CW);
    std::cout << cmd;
}
