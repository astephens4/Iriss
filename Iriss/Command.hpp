#ifndef IRISS_COMMAND_H
#define IRISS_COMMAND_H 1

#include "Utils/Packable.hpp"
#include <ostream>

namespace Iriss {

const static uint8_t BEGIN_UART_MESSAGE = 128;

class Command : public Utils::Packable {
public:
    static const unsigned char CMDMSG = 2;
    static const uint32_t PACKED_SIZE = 5; ///< uint32t(directive) + uint8_t(CMDMSG)
    enum Directive : uint32_t {
        TX_ERR = 0,
        SEND_AGAIN = (1<<1),
        ACK = (1<<2),
        GET_ORIENTATION = (1<<3),
        NUDGE_LEFT = (1<<4),
        NUDGE_RIGHT = (1<<5),
        NUDGE_UP = (1<<6),
        NUDGE_DOWN = (1<<7),
        TURN_RIGHT = (1<<8),
        TURN_LEFT = (1<<9),
        NUDGE_FORWARD = (1<<10),
        NUDGE_BACKWARD = (1<<11)
    };

    Command(void);
    Command(Directive d);

    /**
     * Get all of the directives in this command, OR'ed together
     * @return the bitwise or of all of the directives in this command
     */
    uint32_t get(void) { return m_directives; };

    /**
     * Clears all directives under this command and sets it to the provided
     * directive
     * @param [in] directive Set the command to only this directive
     */
    void set(Directive directive) { m_directives = directive; };

    /**
     * Add the given directive to this command's directives
     * @param [in] directive Add the directive to this command
     */
    void include(Directive directive) { m_directives |= directive; };

    virtual void pack(std::vector<uint8_t>& bytes) const;
    virtual void unpack(const std::vector<uint8_t>& bytes);

    friend std::ostream& operator<<(std::ostream& stream, const Command& cmd);
private:
    uint32_t m_directives;
};

} // end namespace/extern "C"

#endif // IRISS_COMMAND_H
