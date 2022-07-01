#include "Server.h"

void Server::send(size_t* data)
{
	_socket.send_to(boost::asio::buffer(data, sizeof(int)), _endpoint);
}

void Server::send(const std::vector<unsigned char>& data)
{
	_socket.send_to(boost::asio::buffer(data, data.size()), _endpoint);
}

std::vector<unsigned char> Server::receive()
{
	std::vector<unsigned char> buffer(BUFFER_SIZE);

	_socket.receive_from(
		boost::asio::buffer(buffer),
		_endpoint);

	return buffer;
}
