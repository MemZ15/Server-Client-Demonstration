#include "Includes.h"

bool Client::Initialize() {

    /* allocate TCP socket object */
    socket_ = std::make_unique<tcp::socket>(
        ioContext_
    );

    return socket_ != nullptr;
}

bool Client::Connect(std::string ip, uint16_t port) {

    if (!socket_) 
        return false;
  
    boost::system::error_code ec{ };

    /* establish connection to remote endpoint */
    socket_->connect(
        tcp::endpoint(
            boost::asio::ip::make_address(ip, ec),
            port
        ),
        ec
    );

    return !ec;
}

bool Client::Send(std::string message) {

    if (!socket_)
        return false;

    boost::system::error_code ec{ };

    /* transmit outgoing message */
    boost::asio::write(
        *socket_,
        boost::asio::buffer(message),
        ec
    );

    return !ec;
}

std::string Client::Receive() {

    if (!socket_)
        return { };

    std::array<char, 1024> buffer{ };

    boost::system::error_code ec{ };

    /* receive incoming data */
    auto length =
        socket_->read_some(
            boost::asio::buffer(buffer),
            ec
        );

    /* return empty string on failure */
    if (ec) 
        return { "[DEBUG] Error receiving data" };

    /* construct message from buffer */
    return std::string( buffer.data(), length );
}

void Client::RunClient() {

    /* main client loop */
    while (true) {

        std::string input{ };

        std::cout << "> ";
        std::getline(std::cin, input);

        if (input == "q")
            break;

        /* transmit user message */
        if (!Send(input)) 
            break;

        /* wait for server response */
        auto response = Receive();

        if (response.empty()) 
            break;

        std::cout << "[Server]: " << response << '\n';
    }
}

void Client::Disconnect() {

    /* ignore disconnect requests on invalid socket */
    if (!socket_)
        return;

    boost::system::error_code ec{ };

    /* end connection */
    socket_->close(ec);

    /* release socket */
    socket_.reset();
}