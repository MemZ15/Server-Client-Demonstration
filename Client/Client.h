#pragma once
#include "Includes.h"
using boost::asio::ip::tcp;

class Client
{
public:
    bool Initialize();

    bool Connect(
        std::string ip,
        uint16_t port
    );

    bool Send(
        std::string message
    );

    void RunClient();

    std::string Receive();

    void Disconnect();

private:
    boost::asio::io_context ioContext_{ 0 };

    std::unique_ptr<tcp::socket> socket_ = { nullptr };
};