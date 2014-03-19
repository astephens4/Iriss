#ifndef IRISS_COMMAND_H
#define IRISS_COMMAND_H 1

#include "Utils/Packable.hpp"

namespace Iriss {

class Command : public Utils::Packable {
public:
    static const unsigned char CMDMSG = 2;

    enum Directive : uint32_t {
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

    virtual void pack(std::vector<uint8_t>& bytes) const;
    virtual void unpack(const std::vector<uint8_t>& bytes);
private:
    Directive m_directive;
};

} // end namespace/extern "C"

#endif // IRISS_COMMAND_H
