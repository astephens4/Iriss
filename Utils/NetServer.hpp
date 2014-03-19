#ifndef IRISS_NETSERVER_H 
#define IRISS_NETSERVER_H 1

#include <string>
#include <thread>
#include <mutex>
#include <atomic>
#include <map>
#include <queue>

namespace Utils {

class Packable;

class NetServer {
public:
    const static unsigned int MAX_MESSAGE_SIZE = 2048;
    NetServer(unsigned short listPort);

    ~NetServer(void);

    /**
     * Check to see if the connection is valid
     * @retun True for a valid connection. False otherwise
     */
    bool is_valid(void) { return m_isValid; };

    /**
     * Begin the server on the port specified given in the constructor
     * @return True if the server started sucessfully
     */
    bool start_server(void);

    /**
     * Close all connections to the server
     */
    void stop_server(void);

    /**
     * Send the packable class out the connection
     * @param [in] Class to transmit
     * @return True if the entire class was sent
     */
    bool send(int fd, const Utils::Packable& data);

    /**
     * Send a plain old null-termintated string of data out
     * the connection
     * @param [in] Null-terminated data to send
     * @return True if the entire string was sent
     */
    bool send(int fd, const char *str);

    /**
     * Get an immutable list of the connected clients
     * @return a constant reference to the list of clients connected to this server
     */
    void get_client_list(std::map<int, std::string>& clients);

protected:
    /**
     * When a connection in the selector thread has data to receive, do_recv() is called, which
     * in turn calls this function, and implementing classes can handle that data as needed.
     * @param [in] client The client who sent this data
     * @param [in] buf The stream of bytes sent by the client
     * @param [in] len The length of the buffer
     * @return Indicated success or failure of inerpreting the data received
     */
    virtual bool receive(const std::pair<int, std::string>& client, char *buf, unsigned int len) = 0;

private:
    void acceptor(void);

    void client_listener(int fd);

    /**
     * Receive data from a connection into a basic character buffer
     * @param [in] The buffer to place the data into
     * @return True if at least some of the buffer was filled
     */
    bool do_recv(int fd);


    // Connection information
    bool m_isValid;
    unsigned short m_portNo;
    int m_fd;

    // Thread to start accepting new connections
    std::thread m_acceptThread;
    std::map<int, std::string> m_clientList;
    std::atomic<bool> m_quitting;

    // receiving from client stuff
    std::vector<std::thread> m_clientThreads;
    std::mutex m_mutex;
};

} // end namespace Utils



#endif // IRISS_NETSERVER_H
