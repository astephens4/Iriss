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
    if(cmd.m_directives & Iriss::Command::NUDGE_LEFT) {
        stream << "NUDGE_LEFT\n";
    }
    if(cmd.m_directives & Iriss::Command::NUDGE_RIGHT) {
        stream << "NUDGE_RIGHT\n";
    }
    if(cmd.m_directives & Iriss::Command::NUDGE_UP) {
        stream << "NUDGE_UP\n";
    }
    if(cmd.m_directives & Iriss::Command::NUDGE_DOWN) {
        stream << "NUDGE_DOWN\n";
    }
    if(cmd.m_directives & Iriss::Command::TURN_RIGHT) {
        stream << "TURN_RIGHT\n";
    }
    if(cmd.m_directives & Iriss::Command::TURN_LEFT) {
        stream << "TURN_LEFT\n";
    }
    if(cmd.m_directives & Iriss::Command::NUDGE_FORWARD) {
        stream << "NUDGE_FORWARD\n";
    }
    if(cmd.m_directives & Iriss::Command::NUDGE_BACKWARD) {
        stream << "NUDGE_BACKWARD\n";
    }
    stream << "}\n";
    return stream;
}

} // end namespace Iriss
