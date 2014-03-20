#include "Utils/NetServer.hpp"
#include "Utils/Color.hpp"
#include "Iriss/Orders.hpp"
#include <iostream>
#include <cstring>

class Server : public Utils::NetServer {
public:
    Server(unsigned short port) :
        Utils::NetServer(port)
    {
    }

    int get_num_clients(void)
    {
        std::map<int, std::string> clientele;
        get_client_list(clientele);
        return clientele.size();
    }

protected:
    virtual bool receive(const std::pair<int, std::string>& client, char *buf, unsigned int len)
    {
        char *strBuf = new char[len+1];
        memcpy(strBuf, buf, len);
        strBuf[len] = '\0';
        std::cout << client.second << ": " << strBuf << std::endl;
        return true;
    }
};

int list_clients(Server& serv)
{
    std::map<int, std::string> clients;
    serv.get_client_list(clients);
    int count = 1;
    for(auto client : clients) {
        std::cout << count++ << ": " << client.second << std::endl;
    }
    return count-1;
}

int list_tasks(void) {
    std::cout << "Available tasks. Enter a number or q to send:\n"
              << "1. Take off\n"
              << "2. Follow Line\n"
              << "3. Loiter at altitude\n"
              << "4. Land\n";
    return 4;
}

int main(void)
{
    Server server(9001);
    server.start_server();

    bool noClients = (server.get_num_clients() == 0);
    char check = '\0';
    while(noClients) {
        std::cout << "Waiting for clients to connect\n";
        std::cout << "Press any key to check for new connections.\n";
        std::cin >> check;
        noClients = (server.get_num_clients() == 0);
    }

    int selection = 0;
    int quit = 0;
    while(quit == 0) {
        int numClients = list_clients(server);
        std::cin >> selection;
        if(selection < 0 || selection > numClients) {
            std::cout << "Bad choice. Choose again.\n";
            continue;
        }

        std::map<int, std::string> clients;
        server.get_client_list(clients);
        auto itr = clients.begin();
        for(int i = 1; i < selection; ++i) {
           ++itr; 
        }
        int clientFd = itr->first;

        selection = 0;
        Iriss::Orders orders;
        Utils::Color c(0, 0, 0);
        bool sendNow = false;
        while(!sendNow) {
            list_tasks();
            std::cin >> selection;
            switch(selection) {
            case 1: // take off
                orders.queue_task(Iriss::Orders::TAKE_OFF, 0);
                break;
            case 2: // follow line
                std::cout << "Enter the line color as RGB\n"
                          << "R (0-255): ";
                std::cin >> selection;
                c.r = static_cast<unsigned char>(selection&0xFF);

                std::cout << "G (0-255): ";
                std::cin >> selection;
                c.g = static_cast<unsigned char>(selection&0xFF);

                std::cout << "B (0-255): ";
                std::cin >> selection;
                c.b = static_cast<unsigned char>(selection&0xFF);

                orders.queue_task(Iriss::Orders::FOLLOW_LINE, c.asUint32());
                break;
            case 3: // hold alt for duration
                std::cout << "Enter the duration to loiter at altitude: ";
                std::cin >> selection;
                orders.queue_task(Iriss::Orders::LOITER_ALT, selection);
                break;
            case 4: // land
                orders.queue_task(Iriss::Orders::LAND, 0);
                break;
            case 5:
                sendNow = true;
                break;
            default:
                std::cout << "Bad choice. Try again.\n";
                continue;
            }
        }
        server.send(clientFd, orders);
        orders.clear();
    }
}
