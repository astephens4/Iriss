#ifndef IRISS_NET_H 
#define IRISS_NET_H 1

#include <string>
#include <vector>

namespace Utils {

class Packable;

class NetClient {
public:

    NetClient(void);
    ~NetClient(void);

    /**
     * Open an IPv4 TCP stream socket
     * @param [in] addr IPv4 address string to connect to
     * @param [in] portNo Socket port number to connect to
     * @return True if the connection was succeffully made
     */
    bool connect_to_server(const std::string& addr, unsigned short portNo);

    /**
     * Check to see if the connection is valid
     * @retun True for a valid connection. False otherwise
     */
    bool is_valid(void) { return m_isValid; };

    /**
     * Get the destination IP address and socket
     * @retun a string containing the ip address and port formatted as
     *        <ip-address-dotted-decimal>:<port number>
     */
    std::string get_destination(void);

    /**
     * Get just the destination address
     * @return just the destination address
     */
    std::string get_destination_addr(void) { return m_targetIP; };

    /**
     * Get just the destination port number
     * @retun the destination port number
     */
    unsigned short get_destination_port(void) { return m_portNo; };

    /**
     * Send the packable class out the connection
     * @param [in] Class to transmit
     * @return True if the entire class was sent
     */
    bool send(const Utils::Packable& data);

    /**
     * Send a plain old null-termintated string of data out
     * the connection
     * @param [in] Null-terminated data to send
     * @return True if the entire string was sent
     */
    bool send(const char *str);

    /**
     * Receive data from a connection into a Packable
     * @param [in] The packable to place the received dta into
     * @return True if the entire packable was received 
     */
    bool receive(Utils::Packable& data);

    /**
     * Receive data from a connection into a basic character buffer
     * @param [in] The buffer to place the data into
     * @return True if at least the entire buffer was filled
     */
    bool receive(std::vector<char>& buf);


private:
    bool m_isValid;
    std::string m_targetIP;
    unsigned short m_portNo;
    int m_fd;
};

} // end namespace Utils



#endif // IRISS_NET_H
