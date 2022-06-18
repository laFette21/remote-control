#include <iostream>

#include "Server.h"

int main(int argc, char *argv[])
{
    try
    {
        boost::asio::io_service service;  
        Server server(service);
        service.run();
    }
    catch(std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
