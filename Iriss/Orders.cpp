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
    if(t == TAKE_OFF || t == LOITER_ALT) {
        commandedHeight = Utils::Inches(param);
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

        Math::quatf orientation(Utils::Degrees(orientation.roll), Math::vec3f(1, 0, 0),
                              Utils::Degrees(orientation.pitch), Math::vec3f(0, 1, 0),
                              Utils::Degrees(orientation.yaw), Math::vec3f(0, 0, 1));
        // start lookat with the center of the line in the image
        Math::vec3f lookatCenter(0.0f, 0.0f, -line.get_distance().asInches());
        lookatCenter = lookatCenter * orientation;
     
        Math::vec3f lookatLine(line.get_center_x().asInches(),
                               line.get_center_y().asInches(),
                               -line.get_distance().asInches());

        // Ideally, lookatCenter = lookatLine = (0, 0, -1)
        // we want to have flat roll and 0 yaw

        // Try to maintain straight and level
        if(lookatCenter.x() < -0.01f) {
            cmd.include(Iriss::Command::NUDGE_ROLL_LEFT);
        }
        else if(lookatCenter.x() > 0.01f) {
            cmd.include(Iriss::Command::NUDGE_ROLL_RIGHT);
        }

        if(lookatCenter.y() < -0.01f) {
            cmd.include(Iriss::Command::NUDGE_PITCH_UP);
        }
        else if(lookatCenter.y() > 0.01f) {
            cmd.include(Iriss::Command::NUDGE_PITCH_DOWN);
        }

        if(line.get_angle().asDegrees() > 0.001f) {
            cmd.include(Iriss::Command::NUDGE_YAW_CCW);
        }
        else if(line.get_angle().asDegrees() < -0.001f) {
            cmd.include(Iriss::Command::NUDGE_YAW_CW);
        }

        // Maintain a certain height above the line
        if(-lookatLine.z() > m_commandedHeight.asInches()) {
            cmd.include(Iriss::Command::NUDGE_DOWN);
        }
        else if(-lookatLine.z() < m_commandedHeight.asInches()) {
            cmd.include(Iriss::Command::NUDGE_UP);
        }


        // accomplish task
        switch(t) {
        case TAKE_OFF:
            // Move towards the line
            if(lookatLine.x() < -0.01f) {
                cmd.include(Iriss::Command::NUDGE_ROLL_LEFT);
            }
            else if(lookatLine.x > 0.01f) {
                cmd.include(Iriss::Command::NUDGE_ROLL_RIGHT);
            }

            // check for completion
            if(std::fabs(line.get_distance().asInches() - m_commandedHeight.asInches()) < 4.0f) {
                m_taskList.erase(m_taskList.begin());
            }
            break;
        case FOLLOW_LINE:
            // Move towards the line
            if(lookatLine.x() < -0.01f) {
                cmd.include(Iriss::Command::NUDGE_ROLL_LEFT);
            }
            else if(lookatLine.x > 0.01f) {
                cmd.include(Iriss::Command::NUDGE_ROLL_RIGHT);
            }

            // move forward
            float targetPitch = -1.0f;
            if(orientation.pitch < targetPitch) {
                cmd.exclude(Iriss::Command::NUDGE_PITCH_DOWN);
                cmd.include(Iriss::Command::NUDGE_PITCH_UP);
            }
            else if(orientation.pitch > targetPitch) {
                cmd.exclude(Iriss::Command::NUDGE_PITCH_UP);
                cmd.include(Iriss::Command::NUDGE_PITCH_DOWN);
            }
            
            // check for completion
            // we stop when we find a 12-inch long line, which should be around 90 degrees
            if((std::fabs(line.get_length().asInches() - 12.0f) < 2.0f) &&
               (std::fabs(line.get_angle().asDegrees()-90.0f) < 5.0f)) {
                m_taskList.erase(m_taskList.begin());
            }
            break;
        case LOITER_ALT:
            // Move towards the line
            if(lookatLine.x() < -0.01f) {
                cmd.include(Iriss::Command::NUDGE_ROLL_LEFT);
            }
            else if(lookatLine.x > 0.01f) {
                cmd.include(Iriss::Command::NUDGE_ROLL_RIGHT);
            }

            // we are done after the specified number of seconds
            if(std::fabs(param) < 4.0f) {
                m_taskList.erase(m_taskList.begin());
            }
            break;
        case LAND:
            // Move towards the line
            if(lookatLine.x() < -0.01f) {
                cmd.include(Iriss::Command::NUDGE_ROLL_LEFT);
            }
            else if(lookatLine.x > 0.01f) {
                cmd.include(Iriss::Command::NUDGE_ROLL_RIGHT);
            }

            // check for completion
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
