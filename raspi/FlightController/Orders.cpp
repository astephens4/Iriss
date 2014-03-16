#include "Orders.hpp"

namespace Iriss {

Orders::Orders(void) :
    taskList()
{
}

void Orders::pack(std::vector<uint8_t>& bytes) const
{
    bytes.clear();
    bytes.reserve(taskpairSize*m_taskList.size() + 2);

    bytes[0] = taskpairSize*m_taskList.size() + 1;
    bytes[1] = ORDERS_MSG;
    int bytePos = 2;
    for( std::pair<Task, int> task : taskList) {
        bytes[bytePos++] = task.first;
        pack_int32(task.second, &(bytes[bytePos]));
        bytePos += 4;
    }
    return;
}

void Orders::unpack(const std::vector<uint8_t>& bytes)
{
    size_t len = bytes.size();
    uint8_t reported = bytes[0];

    AssertEquals(len, reported+1);
    AssertEquals(ORDERS_MSG, bytes[1]);

    taskList.clear();
    for(int bytePos = 2; bytePos < len; bytePos += taskpairSize) {
        std::pair<Task, int> taskPair;
        taskPair.first = bytes[bytePos];
        unpack_int32(&(bytes[bytePos+1]), taskPair.second);
        taskList.push_back(taskPair);
    }
    return;
}

Iriss::Command Orders::apply(const LineAnalysis::Line& line, const Iriss::Orientation& orientation)
{
    // check the status of the current task, if it is complete move on to the next
    // task

    // Calculate the corrections needed

    // Return the bast command for the correction
}

bool Orders::has_tasks(void)
{
    return !m_taskList.empty();
}


} // end namespace Iriss
