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

	template<typename T> std::vector<T> receive();
	template<typename T> void send(const std::vector<T>& data);

private:
    udp::endpoint _endpoint;
	udp::socket _socket;
};

template<typename T>
std::vector<T> Server::receive()
{
	std::vector<T> buffer(BUFFER_SIZE);

	_socket.receive_from(
		boost::asio::buffer(buffer),
		_endpoint);

	return buffer;
}

template<typename T>
void Server::send(const std::vector<T>& data)
{
	_socket.send_to(boost::asio::buffer(data, data.size() * sizeof(T)), _endpoint);
}

#endif // SERVER_H
