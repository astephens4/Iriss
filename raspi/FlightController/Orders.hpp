#ifndef IRISS_ORDERS_H
#define IRISS_ORDERS_H 1

#include "Utils/Packable.hpp"
#include "Command.hpp"
#include <vector>
#include <map>

namespace LineAnalysis {
class Line;
}

namespace Iriss {

struct Orientation;

class Orders : public Utils::Packable {
public:
    static const unsigned int IRISS_MAX_TASKS = 32;
    static const unsigned char ORDERS_MSG = 1;
    static const unsigned int taskpairSize = 5; // Task enum + int param
    enum Task : unsigned char {
        TAKE_OFF = 0,
        LAND,
        MAINTAIN_ALT,
        FOLLOW_COLOR
    };

    Orders(void);

    /**
     * Apply the current task given the tracking line and orientation
     * @param [in] line Detected tracking line
     * @param [in] orientation Vehicle roll, pitch, yaw, and barometer data
     * @return Command for the ArduPilot
     */
    Iriss::Command apply(const LineAnalysis::Line& line, const Iriss::Orientation& orientation);

    /**
     * Check to see if this set of orders has any more tasks to complete
     * @return True if there are still more tasks to complete, false otherwise
     */
    bool has_tasks(void);

    /**
     * Convert and pack this class into a stream of bytes.
     * @param [out] bytes The packed class contents are placed in here
     */
    virtual void pack(std::vector<uint8_t>& bytes) const;

    /**
     * Convert the received stream of bytes into this class' contents
     * @param [out] bytes The packed class contents are placed in here
     */
    virtual void unpack(const std::vector<uint8_t>& bytes);



private:
    std::map<Task, int> m_taskList;
};

} // end namespace Iriss

#endif // IRISS_ORDERS_H
