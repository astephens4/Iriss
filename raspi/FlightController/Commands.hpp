#ifndef IRISS_COMMAND_H
#define IRISS_COMMAND_H 1

#ifdef __cplusplus
namespace Iriss {
#else
extern "C" {
#endif

enum Command {
    ECHO = (1<<1),
    GET_ORIENTATION = (1<<2),
    NUDGE_LEFT = (1<<3),
    NUDGE_RIGHT = (1<<4),
    NUDGE_UP = (1<<5),
    NUDGE_DOWN = (1<<6),
    TURN_RIGHT = (1<<7),
    TURN_LEFT = (1<<8),
};

} // end namespace/extern "C"

#endif // IRISS_COMMAND_H
