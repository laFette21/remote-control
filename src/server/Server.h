#ifndef SERVER_H
#define SERVER_H

#include <chrono>
#include <iostream>

#include <boost/array.hpp>
#include <boost/asio.hpp>

using boost::asio::ip::udp;

static constexpr size_t BUFFER_SIZE = 65507;

class Server
{
public:
	Server(boost::asio::io_service& ioService, unsigned short port)
        : _socket(ioService, udp::endpoint(udp::v4(), port))
    {}

	~Server()
	{
		_socket.close();
	}

    std::vector<unsigned char> receive();
    void send(size_t* data);
    void send(const std::vector<unsigned char>& data);

private:
    udp::endpoint _endpoint;
	udp::socket _socket;
};

#endif // SERVER_H
