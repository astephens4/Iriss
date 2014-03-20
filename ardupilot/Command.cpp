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

void Command::pack(uint8_t *bytes, uint16_t bufLen) const
{
    assert(bufLen >= sizeof(m_directive) + 1);
    bytes[0] = CMDMSG;
    pack_uint32(m_directive, &(bytes[1]));
    return;
}

void Command::unpack(const uint8_t *bytes, uint16_t bufLen)
{
    assert(bufLen >= sizeof(m_directive) + 1);
    assert(bytes[0] == CMDMSG);
    uint32_t dir;
    unpack_uint32(&(bytes[1]), dir);
    m_directive = static_cast<Directive>(dir);
    return;
}

} // end namespace Iriss
