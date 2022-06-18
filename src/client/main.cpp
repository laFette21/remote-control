#include <iostream>

#include "Client.h"

int main(int argc, char* argv[])
{
	boost::asio::io_service service;
	Client client(service, "localhost", "1569");

    auto start = std::chrono::system_clock::now();

	std::vector<unsigned char> data(9216, 'a');

	client.send(data);

    auto wrote = std::chrono::system_clock::now();

    std::string response = client.receive();

    auto received = std::chrono::system_clock::now();

    std::cout << response << std::endl;

    auto end = std::chrono::system_clock::now();

    std::cout << "Time to write: " << std::chrono::duration_cast<std::chrono::milliseconds>(wrote - start).count() << " ms" << std::endl;
    std::cout << "Time to receive: " << std::chrono::duration_cast<std::chrono::milliseconds>(received - wrote).count() << " ms" << std::endl;
    std::cout << "Time to complete: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms" << std::endl;
}
