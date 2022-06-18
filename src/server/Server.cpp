#include "Server.h"

void Server::receive()
{
    _socket.async_receive_from(
        boost::asio::buffer(_buffer),
        _endpoint,
        boost::bind(
            &Server::receiveCallback,
            this,
            boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred
        )
    );
}

void Server::receiveCallback(const boost::system::error_code& error, size_t nSentBytes)
{
    if (!error || error == boost::asio::error::message_size)
    {
        std::cout << std::string(reinterpret_cast<const char*>(_buffer.data()), nSentBytes) << std::endl;

        boost::shared_ptr<std::string> message(new std::string("Hello from the Server!"));

        _socket.async_send_to(
            boost::asio::buffer(*message),
            _endpoint,
            boost::bind(&Server::sendCallback,
                this,
                message,
                boost::asio::placeholders::error,
                boost::asio::placeholders::bytes_transferred
            )
        );

        receive();
    }
}

void Server::sendCallback(
    boost::shared_ptr<std::string> message,
    const boost::system::error_code& error,
    size_t nSentBytes
)
{
    std::cerr << "Sent " << nSentBytes << " bytes to " << _endpoint << std::endl;
}
