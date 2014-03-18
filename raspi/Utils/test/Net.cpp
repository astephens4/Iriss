#include "Utils/NetServer.cpp"
#include "Utils/NetClient.cpp"
#include "test/Testing.hpp"
#include <cstring>

class PackableImpl : public Utils::Packable {
public:
    PackableImpl(void) :
        testInt(-42),
        testUint(87252),
        testFloat(-0.3245f)
    {
    }

    virtual void pack(std::vector<uint8_t>& bytes) const
    {
        bytes.clear();
        bytes.resize(12);
        pack_int32(testInt, &(bytes[0]));
        pack_uint32(testUint, &(bytes[4]));
        pack_float32(testFloat, &(bytes[8]));
    }

    virtual void unpack(const std::vector<uint8_t>& bytes)
    {
        int32_t unpackedInt;
        uint32_t unpackedUint;
        float unpackedFloat;

        unpack_int32(&(bytes[0]), unpackedInt);
        unpack_uint32(&(bytes[4]), unpackedUint);
        unpack_float32(&(bytes[8]), unpackedFloat);

        AssertEquals(testInt, unpackedInt);
        AssertEquals(testUint, unpackedUint);
        AssertEquals(testFloat, unpackedFloat);
    }

    friend std::ostream& operator<<(std::ostream& stream, const PackableImpl& p);
private:
    int32_t testInt;
    uint32_t testUint;
    float testFloat;
};


std::ostream& operator<<(std::ostream& stream, const PackableImpl& p)
{
    stream << "testInt{" << p.testInt << "} testUint{" << p.testUint << "} testFloat{" << p.testFloat << "}";
    return stream;
}


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

    sleep(1);

    std::map<int, std::string> list;
    server.get_client_list(list);
    for(auto client : list) {
        PackableImpl packer;
        server.send(client.first, packer);
    }

    PackableImpl pack1;
    client1.receive(pack1);
    std::cout << "Client 1 received:\n"
              << pack1 << std::endl;

    
    PackableImpl pack2;
    client2.receive(pack2);
    std::cout << "Client 2 received:\n"
              << pack2 << std::endl;

    while(server.num_received() < 2) {}

    return 0;
}
