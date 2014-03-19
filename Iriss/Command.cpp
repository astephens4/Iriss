#include "Iriss/Command.hpp"
#include <cassert>

namespace Iriss {

Command::Command(void) :
    m_directive(NO_COMMAND)
{

}

Command::Command(Directive d) :
    m_directive(d)
{

}

void Command::pack(std::vector<uint8_t>& bytes) const
{
    bytes.clear();
    bytes.resize(sizeof(m_directive)+1);
    bytes[0] = CMDMSG;
    pack_uint32(m_directive, &(bytes[1]));
    return;
}

void Command::unpack(const std::vector<uint8_t>& bytes)
{
    assert(bytes.size() >= 1+sizeof(m_directive));
    assert(bytes[0] == CMDMSG);
    uint32_t dir;
    unpack_uint32(&(bytes[1]), dir);
    m_directive = static_cast<Directive>(dir);
    return;
}

} // end namespace Iriss
