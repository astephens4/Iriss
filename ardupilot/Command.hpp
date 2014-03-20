#ifndef IRISS_COMMAND_H
#define IRISS_COMMAND_H 1

#include "Utils/Packable.hpp"

namespace Iriss {

class Command : public Utils::Packable {
public:
    static const uint8_t CMDMSG = 2;
    static const uint8_t PACKED_SIZE = 5; ///< uint32t(directive) + uint8_t(CMDMSG)
    enum Directive {
        NO_COMMAND=0,
        ACK = (1<<1),
        GET_ORIENTATION = (1<<2),
        NUDGE_LEFT = (1<<3),
        NUDGE_RIGHT = (1<<4),
        NUDGE_UP = (1<<5),
        NUDGE_DOWN = (1<<6),
        TURN_RIGHT = (1<<7),
        TURN_LEFT = (1<<8),
        BEGIN_COMMAND_LIST = (1<<9),
        END_COMMAND_LIST = (1<<10)
    };

    Command(void);
    Command(Directive d);

    Directive get(void) { return m_directive; };
    void set(Directive directive) { m_directive = directive; };

    virtual void pack(uint8_t* bytes, uint16_t bufLen) const;
    virtual void unpack(const uint8_t* bytes, uint16_t bufLen);
private:
    Directive m_directive;
};

} // end namespace/extern "C"

#endif // IRISS_COMMAND_H
