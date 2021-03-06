#ifndef CLIENT_H
#define CLIENT_H

#include <iostream>
#include <vector>

#include <boost/array.hpp>
#include <boost/asio.hpp>

using boost::asio::ip::udp;

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
	}

	~Client()
	{
		_socket.close();
	}

    std::vector<unsigned char> receive();
    void send(int* data);
    void send(const std::vector<unsigned char>& data);

private:
	boost::asio::io_service& _service;
	udp::socket _socket;
	udp::endpoint _endpoint;
};

#endif // CLIENT_H
