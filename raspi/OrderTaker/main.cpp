#include "Utils/NetClient.hpp"
#include "Iriss/Orders.hpp"
#include <iostream>

int main(void)
{
    Utils::NetClient client;
    client.connect_to_server("192.168.10.81", 9001);

    Iriss::Orders orders;
    if(client.is_valid()) {
        while(1) {
            bool ret = client.receive(orders);
            if(!ret) {
                std::cout << "Server disconnected\n";
                return 0;
            }
            std::cout << "My Orders: " << orders.to_string() << std::endl;
        }
    }
    else {
        std::cout << "Could not connect to server!\n";
        return -1;
    }
}
