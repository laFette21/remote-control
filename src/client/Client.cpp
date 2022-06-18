#include "Client.h"

void Client::send(int* data)
{
	_socket.send_to(boost::asio::buffer(data, sizeof(int)), _endpoint);
}

void Client::send(const std::vector<unsigned char>& data)
{
	_socket.send_to(boost::asio::buffer(data, data.size()), _endpoint);
}

std::vector<unsigned char> Client::receive()
{
	std::vector<unsigned char> buffer(65507);

	_socket.receive_from(
		boost::asio::buffer(buffer),
		_endpoint);

	return buffer;
}
