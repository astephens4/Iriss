#ifndef AVR_COMMAND_H
#define AVR_COMMAND_H 1

#include "Packable.hpp"

namespace Iriss {

const static uint8_t BEGIN_UART_MESSAGE = 128;

class Command : public Utils::Packable {
public:
    static const uint8_t CMDMSG;
    static const uint32_t PACKED_SIZE; ///< uint32t(directive) + uint8_t(CMDMSG)
    enum Directive {
        TX_ERR = 0,
        SEND_AGAIN = (1<<1),
        ACK = (1<<2),
        GET_ORIENTATION = (1<<3),
        NUDGE_ROLL_LEFT = (1<<4),
        NUDGE_ROLL_RIGHT = (1<<5),
        NUDGE_UP = (1<<6),
        NUDGE_DOWN = (1<<7),
        NUDGE_YAW_CCW = (1<<8),
        NUDGE_YAW_CW = (1<<9),
        NUDGE_PITCH_DOWN = (1<<10),
        NUDGE_PITCH_UP = (1<<11)
    };

    Command(void);
    Command(Directive d);

    uint32_t get(void) const { return m_directives; };
    void set(uint32_t directive) { m_directives = directive; };
    void include(Directive directive) { m_directives |= directive; };

    virtual void pack(uint8_t* bytes, uint16_t bufLen) const;
    virtual void unpack(const uint8_t* bytes, uint16_t bufLen);
private:
    uint32_t m_directives;
};

} // end namespace/extern "C"

#endif // AVR_COMMAND_H
