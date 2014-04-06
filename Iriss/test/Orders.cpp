#include "test/Testing.hpp"
#include "Iriss/Orders.hpp"
#include "Iriss/Orientation.hpp"

int main(void)
{
    Iriss::Orders o;
    o.queue_task(Iriss::Orders::TAKE_OFF, 60);
    std::string imgFile = "/dev/shm/photocache/lineFollow.png";
    Iriss::Orientation orientation(0, 0, 0);
    o.apply(imgFile, orientation);
    return 0;
}
