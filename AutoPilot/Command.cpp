#include "Command.hpp"

namespace Iriss {

const uint8_t Command::CMDMSG = 2;
const uint32_t Command::PACKED_SIZE = 5;

Command::Command(void) :
    m_directives(TX_ERR)
{

}

Command::Command(Directive d) :
    m_directives(d)
{

}

void Command::pack(uint8_t *bytes, uint16_t bufLen) const
{
    bytes[0] = CMDMSG;
    pack_uint32(m_directives, &(bytes[1]));
    return;
}

void Command::unpack(const uint8_t *bytes, uint16_t bufLen)
{
    unpack_uint32(&(bytes[1]), m_directives);
    return;
}

} // end namespace Iriss
