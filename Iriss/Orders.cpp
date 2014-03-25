#include "Iriss/Orders.hpp"
#include "Math/quatf.hpp"
#include "Math/vec3.hpp"
#include "Math/operations.hpp"
#include "LineAnalysis/Line.hpp"
#include "LineAnalysis/LineDetector.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
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

Iriss::Command Orders::apply(const std::string& imgFile, const Iriss::Orientation& orientation)
{
    Task t = (m_taskList.begin())->first;
    uint32_t param = (m_taskList.begin())->second;

    cv::Mat image = cv::imread(imgFile);
    if(image.empty()) {
        std::cerr << "Bad image!\n";
        return Iriss::Command(Iriss::Command::TX_ERR);
    }

    LineAnalysis::LineDetector detector;
    detector.set_image(image);
    if(t == TAKE_OFF) {
        // the color should be in the middle of the image
        cv::Vec3b lineColor = image.at<cv::Vec3b>(image.size().width/2, image.size().height/2);
        m_detector.add_color(Utils::Color(lineColor[2], lineColor[1], lineColor[0]));
    }
    else if(t == FOLLOW_LINE) {
        m_detector.add_color(Utils::Color(param));
    }
    // otherwise use the current colors

    // check the status of the current task, if it is complete move on to the next
    // task
    std::vector<LineAnalysis::Line> detectedLines;
    m_detector.get_lines(detectedLines);

    
    Iriss::Command cmd;
    for(LineAnalysis::Line line : detectedLines) {
        // Determining corrections takes place in two stages:
        //  1. Corrections to get the quad straight and level
        //  2. Corrections to accomplish the task

        Math::quatf quaternion(Utils::Degrees(orientation.roll), Math::vec3f(1, 0, 0),
                              Utils::Degrees(orientation.pitch), Math::vec3f(0, 1, 0),
                              Utils::Degrees(orientation.yaw), Math::vec3f(0, 0, 1));
        // start lookat with the center of the line in the image
        Math::vec3f lookat(0.0f, 0.0f, -line.get_distance().asInches());
        lookat = lookat * quaternion;
     
        // lookat tells us the displacement vector from the camera's center

        // we want to have flat roll and 0 yaw
        

        switch(t) {
        case TAKE_OFF:

            break;
        case FOLLOW_LINE:

            break;
        case LOITER_ALT:

            break;
        case LAND:

            break;
        default:
            std::cerr << "Unknown task, moving on to the next task\n";
            m_taskList.erase(m_taskList.begin());
            cmd.set(Iriss::Command::TX_ERR);
        }
    }

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
