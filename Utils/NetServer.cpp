#include "Utils/NetServer.hpp"
#include "Utils/Packable.hpp"

// TCP/IP sockets
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/poll.h>

// allow printing of system errors
#include <cstdio>
#include <errno.h>
#include <unistd.h>

// Some I/O
#include <iostream>
#include <sstream>
#include <cstring>

namespace Utils {

NetServer::NetServer(unsigned short portNo) :
    m_isValid(false),
    m_portNo(portNo),
    m_fd(-1),
    m_acceptThread(),
    m_clientList(),
    m_quitting(false),
    m_clientThreads()
{
}


NetServer::~NetServer(void)
{
    stop_server();
}

bool NetServer::start_server(void)
{
    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    struct addrinfo *results = nullptr;
    std::ostringstream port;
    port << m_portNo;
    int status = getaddrinfo(NULL, (port.str()).c_str(), &hints, &results);
    if(status != 0) {
        std::cerr << "Error getting server info on port " << m_portNo << " - " << gai_strerror(status) << std::endl;
        std::cerr << "Fix it\n";
        m_isValid = false;
        return m_isValid;
    }

    // get the IPv4 address from the result
    struct addrinfo *v4Addr = nullptr;
    for(struct addrinfo *curInfo = results; curInfo != nullptr; curInfo = curInfo->ai_next) {
        if (curInfo->ai_family == AF_INET) { // IPv4
            v4Addr = curInfo;
            break;
        }
    }

    if(v4Addr == nullptr) {
        std::cerr << "Couldn't find IPv4 address!\n" << std::endl;
        freeaddrinfo(results);
        m_isValid = false;
        return m_isValid;
    }

    // open the socket!
    m_fd = socket(v4Addr->ai_family, v4Addr->ai_socktype, v4Addr->ai_protocol);
    if(m_fd < 0) {
        perror("Unable to open socket!");
        freeaddrinfo(results);
        m_isValid = false;
        return m_isValid;
    }

    // Take care of issue with binding to a recently used port
    int yes = 1;
    status = setsockopt(m_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
    if(status == -1) {
        perror("setsockopt");
        m_isValid = false;
        return m_isValid;
    }

    // bind it to allow for listening
    status = bind(m_fd, v4Addr->ai_addr, v4Addr->ai_addrlen);
    if(status < 0) {
        perror("Unable to bind!");
        m_isValid = false;
    }
    else {
        m_isValid = true;
        // start listening for connections!
        status = listen(m_fd, 10);
        if(status < 0) {
            perror("I can't listen...");
            m_isValid = false;
        }
        else {
            // kick off the thread which accepts new connections!
            m_acceptThread = std::thread(&NetServer::acceptor, this);
        }
    }
    freeaddrinfo(results);
    return m_isValid;

}

void NetServer::stop_server(void)
{
    // the accepting thread is probably blocking on accept(), set quitting now
    m_isValid = false;
    m_quitting.store(true);

    // close the listening fd to stop the accept() call in m_acceptThread
    close(m_fd);
    // close all of the connections
    for(std::pair<int, std::string> client : m_clientList) {
        close(client.first);
    }

    // wait for it to finish
    m_acceptThread.join();
    for(std::vector<std::thread>::iterator itr = m_clientThreads.begin(); itr != m_clientThreads.end(); ++itr) {
        itr->join();
    }
}

bool NetServer::send(int fd, const Utils::Packable& data)
{
    if(!m_isValid || fd < 0) {
        return false;
    }

    std::vector<uint8_t> bytes;
    data.pack(bytes);

    int bytesToSend = bytes.size();
    int btsNet = htonl(bytesToSend);
    int numBytes = ::send(fd, &btsNet, sizeof(int), 0);
    if(numBytes <= 0) {
        m_isValid = false;
        return m_isValid;
    }

    int bytePos = 0;
    while(bytesToSend > 0) {
        int numBytes = ::send(fd, reinterpret_cast<const void*>(&(bytes[bytePos])), bytesToSend, 0);
        if(numBytes == 0) {
            std::cerr << "Lost connection receiver!!!!\n";
            m_isValid = false;
            return false;
        }
        else if(numBytes < 0) {
            perror("Oh Noes! (NetServer::send)");
            m_isValid = false;
            return false;
        }
        bytesToSend -= numBytes;
        bytePos += numBytes;
    }

    if(bytesToSend == 0) {
        return true;
    }
    std::cerr << "Sent more bytes than I had to send... WTF?\n";
    return false;

}

bool NetServer::send(int fd, const char *str)
{
    if(!m_isValid || fd < 0 || str == nullptr) {
        return false;
    }

    int bytesToSend = strlen(str);
    int btsNet = htonl(bytesToSend);
    int numBytes = ::send(fd, &btsNet, sizeof(int), 0);
    int bytePos = 0;
    while(bytesToSend > 0) {
        numBytes = ::send(fd, reinterpret_cast<const void*>(&(str[bytePos])), bytesToSend, 0);
        if(numBytes == 0) {
            std::cerr << "Lost connection receiver!!!!\n";
            m_isValid = false;
            return false;
        }
        else if(numBytes < 0) {
            perror("Oh Noes! (NetServer::send)");
            m_isValid = false;
            return false;
        }
        bytesToSend -= numBytes;
        bytePos += numBytes;
    }

    if(bytesToSend == 0) {
        m_isValid = true;
        return m_isValid;
    }
    std::cerr << "Sent more bytes than I had to send... WTF?\n";
    m_isValid = false;
    return m_isValid;

}

bool NetServer::do_recv(int fd)
{
    if(!m_isValid || fd < 0) {
        return false;
    }

    /**
     * Get the message size
     */
    int msgSizeNet;
    int bytesRec = ::recv(fd, &msgSizeNet, sizeof(int), 0);
    if(bytesRec <= 0) {
        m_isValid = false;
        return m_isValid;
    }

    int bufLen = ntohl(msgSizeNet);
    char *buf = new char[bufLen];
    int pos = 0;
    ssize_t numBytes = 0;
    while(pos < bufLen) {
        numBytes = ::recv(fd, reinterpret_cast<void*>(&(buf[pos])), bufLen-pos, 0);
        if(numBytes == 0) {
            std::cerr << "Lost connection to sender!!!!\n";
            m_isValid = false;
            return false;
        }
        else if(numBytes < 0) {
            perror("Oh Noes! (NetServer::do_recv)");
            m_isValid = false;
            return false;
        }
        pos += numBytes;
    }
    m_isValid = true;

    bool ret;
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        std::pair<int, std::string> client;
        client.first = fd;
        client.second = m_clientList[fd];
        ret = receive(client, buf, pos);
    }
    delete [] buf;
    return ret;
}


void NetServer::acceptor(void)
{
    struct pollfd ufds[1];
    ufds[0].fd = m_fd;
    ufds[0].events = POLLIN;

    struct sockaddr_storage addr;
    socklen_t addrSize = sizeof(addr);
    while(!m_quitting.load()) {
        int rv = poll(ufds, 1, 50);
        if(rv < 0) {
            perror("accept_poll");
            m_isValid = false;
            return;
        }
        else if(rv > 0 && (ufds[0].revents&POLLHUP || ufds[0].revents&POLLERR || ufds[0].revents&POLLNVAL)) {
            return;
        }
        else if(rv == 0) {
            continue;
        }
        
        int fd = accept(m_fd, reinterpret_cast<struct sockaddr*>(&addr), &addrSize);
        if(fd < 0) {
            perror("acceptor thread should exit now");
            continue;
        }
        // get the connection information
        {
        std::lock_guard<std::mutex> lock(m_mutex);
        char name[16];
        void *ipv4 = reinterpret_cast<void*>(&(reinterpret_cast<struct sockaddr_in*>(&addr)->sin_addr));
        if(inet_ntop(AF_INET, ipv4, name, sizeof(name))) {
            m_clientList.insert(std::pair<int, std::string>(fd, name));
        }
        else {
            m_clientList.insert(std::pair<int, std::string>(fd, "Unknown Client"));
        }
        }
        // kick off a thread to handle the client's connection
        m_clientThreads.push_back(std::thread(&NetServer::client_listener, this, fd));

    }
}

void NetServer::client_listener(int fd)
{
    struct pollfd ufds[1];
    ufds[0].fd = fd;
    ufds[0].events = POLLIN;
    while(!m_quitting.load()) {
        int rv = poll(ufds, 1, 50);
        if(rv > 0 && (ufds[0].revents&POLLHUP || ufds[0].revents&POLLERR || ufds[0].revents&POLLNVAL)) {
            return;
        }
        else if(rv > 0 && ufds[0].revents&POLLIN) {
            bool isConnected = do_recv(fd);
            if(!isConnected) {
                break;
            }
        }
        else if(rv == -1) {
            perror("BAAAD things happened");
            return;
        }
    }
}

void NetServer::get_client_list(std::map<int, std::string>& clients)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    clients = m_clientList;
    return;
}

} // end namespace Utils
