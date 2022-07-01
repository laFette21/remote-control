#include "Client.h"

size_t Client::receive()
{
    return _socket.receive_from(boost::asio::buffer(_buffer), _endpoint);
}

void Client::send(const std::string& data)
{
	_socket.send_to(boost::asio::buffer(data, data.size()), _endpoint);
}
