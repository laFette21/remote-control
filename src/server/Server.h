#ifndef SERVER_H
#define SERVER_H

#include <iostream>

#include <boost/array.hpp>
#include <boost/asio.hpp>

class Server
{
public:
    Server(boost::asio::io_service& ioService, unsigned short port)
        : _socket(ioService, boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), port))
    {
        _buffer = std::vector<unsigned char>(65507);
    }

    ~Server()
	{
        _socket.close();
    }

    size_t receive();
    std::vector<unsigned char> getBuffer() const { return _buffer; }

private:
    std::vector<unsigned char> _buffer;
    boost::asio::ip::udp::endpoint _endpoint;
    boost::asio::ip::udp::socket _socket;
};

#endif // SERVER_H
