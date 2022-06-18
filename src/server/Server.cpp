#include "Server.h"

size_t Server::receive()
{
    return _socket.receive_from(boost::asio::buffer(_buffer), _endpoint);
}
