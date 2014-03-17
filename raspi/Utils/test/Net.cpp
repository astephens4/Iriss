#include "Utils/NetServer.cpp"
#include "Utils/NetClient.cpp"
#include "test/Testing.hpp"
#include <cstring>

class Server : public Utils::NetServer {
public:
    Server(unsigned short port) :
        NetServer(port),
        m_received(0)
    {
    }

    int num_received(void) { return m_received; };
protected:
    virtual bool receive(const std::pair<int, std::string>& client, char *buf, unsigned int bufLen)
    {
        char *message = new char[bufLen+1];
        memcpy(message, buf, bufLen);
        message[bufLen] = '\0';
        std::cout << "Message from " << client.second << ":\n"
                  << message << std::endl;
        ++m_received;
        return 0;
    }
private:
    int m_received;
};

int main(void)
{
    Server server(8008);
    server.start_server();

    Utils::NetClient client1;
    Utils::NetClient client2;

    client1.connect_to_server("127.0.0.1", 8008);
    client2.connect_to_server("127.0.0.1", 8008);

    char sendMsg[128];
    
    strncpy(sendMsg, "Hello from Client 1!", 128);
    client1.send(sendMsg);
   
    strncpy(sendMsg, "Hello from Client 2!", 128);
    client2.send(sendMsg);

    while(server.num_received() < 2) {}

    return 0;
}
