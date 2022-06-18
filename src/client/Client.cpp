#include "Client.h"

void Client::send(const std::vector<unsigned char>& data)
{
	_socket.send_to(boost::asio::buffer(data, data.size()), _endpoint);
}

std::string Client::receive()
{
	unsigned char buffer[65507];

	_socket.receive_from(
		boost::asio::buffer(buffer),
		_endpoint);

	return reinterpret_cast<char*>(buffer);
}
