#include "Iriss/Command.hpp"
#include <cassert>

namespace Iriss {

Command::Command(void) :
    m_directives(TX_ERR)
{

}

Command::Command(Directive d) :
    m_directives(d)
{

}

void Command::pack(std::vector<uint8_t>& bytes) const
{
    bytes.clear();
    bytes.resize(sizeof(m_directives)+1);
    bytes[0] = CMDMSG;
    pack_uint32(m_directives, &(bytes[1]));
    return;
}

void Command::unpack(const std::vector<uint8_t>& bytes)
{
    assert(bytes.size() >= 1+sizeof(m_directives));
    assert(bytes[0] == CMDMSG);
    unpack_uint32(&(bytes[1]), m_directives);
    return;
}

std::ostream& operator<<(std::ostream& stream, const Command& cmd)
{
    stream << "Command{\n";
    if(cmd.m_directives & Iriss::Command::TX_ERR) {
        stream << "TX_ERR\n";
    }
    if(cmd.m_directives & Iriss::Command::SEND_AGAIN) {
        stream << "SEND_AGAIN\n";
    }
    if(cmd.m_directives & Iriss::Command::ACK) {
        stream << "ACK\n";
    }
    if(cmd.m_directives & Iriss::Command::GET_ORIENTATION) {
        stream << "GET_ORIENTATION\n";
    }
    if(cmd.m_directives & Iriss::Command::NUDGE_ROLL_LEFT) {
        stream << "NUDGE_ROLL_LEFT\n";
    }
    if(cmd.m_directives & Iriss::Command::NUDGE_ROLL_RIGHT) {
        stream << "NUDGE_ROLL_RIGHT\n";
    }
    if(cmd.m_directives & Iriss::Command::NUDGE_UP) {
        stream << "NUDGE_UP\n";
    }
    if(cmd.m_directives & Iriss::Command::NUDGE_DOWN) {
        stream << "NUDGE_DOWN\n";
    }
    if(cmd.m_directives & Iriss::Command::NUDGE_YAW_CW) {
        stream << "NUDGE_YAW_CW\n";
    }
    if(cmd.m_directives & Iriss::Command::NUDGE_YAW_CCW) {
        stream << "NUDGE_YAW_CCW\n";
    }
    if(cmd.m_directives & Iriss::Command::NUDGE_PITCH_DOWN) {
        stream << "NUDGE_PITCH_DOWN\n";
    }
    if(cmd.m_directives & Iriss::Command::NUDGE_PITCH_UP) {
        stream << "NUDGE_PITCH_UP\n";
    }
    stream << "}\n";
    return stream;
}

} // end namespace Iriss
