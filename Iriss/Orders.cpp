#include "Orders.hpp"
#include "LineAnalysis/Line.hpp"
#include "LineAnalysis/LineDetector.hpp"
#include "test/Testing.hpp"
#include <sstream>


namespace Iriss {

Orders::Orders(void) :
    m_taskList()
{
}

void Orders::pack(std::vector<uint8_t>& bytes) const
{
    bytes.clear();
    bytes.resize(taskpairSize*m_taskList.size() + 1, 0);

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
        m_taskList.push_back(taskPair);
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

void Orders::queue_task(Task t, unsigned int val)
{
    if(m_taskList.size() == IRISS_MAX_TASKS) {
        return;
    }
    m_taskList.push_back(std::pair<Task, unsigned int>(t, val));  
}

void Orders::clear()
{
    m_taskList.clear();
}

std::string Orders::to_string(void)
{
    std::ostringstream oss;
    for(auto taskPair : m_taskList) {
        switch(taskPair.first) {
            case TAKE_OFF:
                oss << "TakeOff{";
                break;
            case LAND:
                oss << "Land{";
                break;
            case LOITER_ALT:
                oss << "LoiterForDuration{";
                break;
            case FOLLOW_LINE:
                oss << "FollowLineColor{";
                break;
            default:
                oss << taskPair.first << "{";
        }
        oss << taskPair.second << "}\n";
    }
    return oss.str();
}

} // end namespace Iriss
