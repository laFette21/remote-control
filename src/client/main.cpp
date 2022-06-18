#include <iostream>

#include "Client.h"

int main(int argc, char* argv[])
{
    try
    {
        boost::asio::io_service service;
        Client client(service, "localhost", "1569");

        auto start = std::chrono::high_resolution_clock::now();

        std::vector<unsigned char> data(9216, 'a');

        client.send(data);

        auto sent = std::chrono::high_resolution_clock::now();

        std::vector<unsigned char> response = client.receive();

        auto received = std::chrono::high_resolution_clock::now();

        std::cout << std::string(response.begin(), response.end()) << std::endl;

        std::cout << "Time to send: " << std::chrono::duration_cast<std::chrono::milliseconds>(sent - start).count() << " ms" << std::endl;
        std::cout << "Time to receive: " << std::chrono::duration_cast<std::chrono::milliseconds>(received - sent).count() << " ms" << std::endl;
    }
    catch(std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
