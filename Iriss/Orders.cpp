#include "Orders.hpp"
#include "LineAnalysis/Line.hpp"
#include "LineAnalysis/LineDetector.hpp"
#include "test/Testing.hpp"

namespace Iriss {

Orders::Orders(void) :
    m_taskList()
{
}

void Orders::pack(std::vector<uint8_t>& bytes) const
{
    bytes.clear();
    bytes.resize(taskpairSize*m_taskList.size() + 2, 0);

    bytes[0] = ORDERS_MSG;
    int bytePos = 1;
    for(auto task : m_taskList) {
        bytes[bytePos++] = task.first;
        pack_int32(task.second, &(bytes[bytePos]));
        bytePos += 4;
    }
    return;
}

void Orders::unpack(const std::vector<uint8_t>& bytes)
{
    AssertEquals(ORDERS_MSG, bytes[0]);

    m_taskList.clear();
    for(unsigned int bytePos = 1; bytePos < bytes.size(); bytePos += taskpairSize) {
        std::pair<Task, int> taskPair;
        taskPair.first = static_cast<Task>(bytes[bytePos]);
        unpack_int32(&(bytes[bytePos+1]), taskPair.second);
        m_taskList.insert(taskPair);
    }
    return;
}

Iriss::Command Orders::apply(const LineAnalysis::Line& /*line*/, const Iriss::Orientation& /*orientation*/)
{
    // check the status of the current task, if it is complete move on to the next
    // task

    // Calculate the corrections needed

    // Return the bast command for the correction
    Command cmd(Command::ACK);
    return cmd;
}

bool Orders::has_tasks(void)
{
    return !m_taskList.empty();
}


} // end namespace Iriss
