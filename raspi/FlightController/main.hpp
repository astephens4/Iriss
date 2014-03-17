#ifndef IRISS_MAIN_H
#define IRISS_MAIN_H 1

namespace Iriss {

/**
 * Check to see if the named process is running
 * @param [in] process name
 * @return True if the given process name is running, false otherwise
 */
bool is_process_running(const std::string& execName);

/**
 * Wait to receive a set of orders from the command center on socket
 * @param [in] socket Open connection to the command center
 * @param [out] ccOrders Orders that are received from the command center
 * @return True if action orders are given, false if disconnected
 */
bool wait_on_orders(int socket, Utils::Orders& ccOrders);

/**
 * Check to see if the given file exists
 * @param [in] fileName File to check, can be relative or absolute
 */
bool is_file(const std::string& fileName);

} // end namespace Iriss

#endif // IRISS_MAIN_H
