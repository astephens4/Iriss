#ifndef IRISS_MAIN_H
#define IRISS_MAIN_H 1

namespace Iriss {

/**
 * Open an IPv4 TCP stream socket
 * @param [in] addr IPv4 address string to connect to
 * @param [in] portNo Socket port number to connect to
 * @return File descriptor for the socket, or negative value for an error.
 */
int connect_to_command_center(const std::string& addr, unsigned short portNo);

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
 * Performs various checks to see if the given orders are completed
 * @param [in] orders Orders issued to this quadcopter
 * @return True if the orders are complete, false otherwise.
 */
bool orders_complete(const Utils::Orders& orders);

/**
 * Check to see if the given file exists
 * @param [in] fileName File to check, can be relative or absolute
 */
bool is_file(const std::string& fileName);

} // end namespace Iriss

#endif // IRISS_MAIN_H
