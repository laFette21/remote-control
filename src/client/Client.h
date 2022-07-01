#ifndef CLIENT_H
#define CLIENT_H

#include <chrono>
#include <iostream>

#include <boost/array.hpp>
#include <boost/asio.hpp>

using boost::asio::ip::udp;

static constexpr size_t BUFFER_SIZE = 65507;

class Client
{
public:
	Client(
		boost::asio::io_service& service, 
		const std::string& host, 
		unsigned short port
	) : _service(service), _socket(service, udp::endpoint(udp::v4(), 0))
    {
		udp::resolver resolver(_service);
		udp::resolver::query query(udp::v4(), host, std::to_string(port));
		_endpoint = *resolver.resolve(query);
		_buffer = std::vector<unsigned char>(BUFFER_SIZE);
	}

    ~Client()
	{
        _socket.close();
    }

	std::vector<unsigned char> getBuffer() const { return _buffer; }
    size_t receive();
	void send(const std::string& data);

private:
    std::vector<unsigned char> _buffer;

	boost::asio::io_service& _service;
    udp::endpoint _endpoint;
    udp::socket _socket;
};

#endif // CLIENT_H
