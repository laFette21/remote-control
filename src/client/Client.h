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
		_sizeBuffer = std::vector<int>(BUFFER_SIZE);
	}

    ~Client()
	{
        _socket.close();
    }

	std::vector<int> getSizeBuffer() const { return _sizeBuffer; }
	template<typename T> std::pair<size_t, std::vector<T>> receive();
    size_t receive();
	void send(const std::string& data);

private:
    std::vector<int> _sizeBuffer;

	boost::asio::io_service& _service;
    udp::endpoint _endpoint;
    udp::socket _socket;
};

template<typename T>
std::pair<size_t, std::vector<T>> Client::receive()
{
	std::vector<T> buffer(BUFFER_SIZE);

	size_t size = _socket.receive_from(
		boost::asio::buffer(buffer),
		_endpoint);

	return std::pair<size_t, std::vector<T>>(size, buffer);
}

size_t Client::receive()
{
	return _socket.receive_from(boost::asio::buffer(_sizeBuffer), _endpoint);
}

void Client::send(const std::string& data)
{
	_socket.send_to(boost::asio::buffer(data, data.size()), _endpoint);
}

#endif // CLIENT_H
