#ifndef SERVER_H
#define SERVER_H

#include <iostream>

#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/enable_shared_from_this.hpp>

class Server
{
public:
    Server(boost::asio::io_service& ioService)
        : _socket(ioService, boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), 1569))
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
    // void receiveCallback(const boost::system::error_code& error, size_t nSentBytes);
    // void sendCallback(
    //     boost::shared_ptr<std::string> message,
    //     const boost::system::error_code& error,
    //     size_t nSentBytes
    // );

    std::vector<unsigned char> _buffer;
    boost::asio::ip::udp::endpoint _endpoint;
    boost::asio::ip::udp::socket _socket;
};

#endif // SERVER_H
