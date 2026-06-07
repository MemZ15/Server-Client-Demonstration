#pragma once
#include "Includes.h"

using boost::asio::ip::tcp;

class ServerHandler
{
public:

    void Init(boost::asio::io_context& io, uint16_t port)
    {
        /* create acceptor bound to IPv4 endpoint and specified port */
        acceptor_ = std::make_unique<tcp::acceptor>(
            io,
            tcp::endpoint(tcp::v4(), port)
        );

        StartAccept();

        std::cout << "Server Started\n";
        std::cout << "Listening on port " << port << "\n";
    }

    void Stop()
    {
        boost::system::error_code ec{};

        /* safely close acceptor if it is active */
        if (acceptor_ && acceptor_->is_open())
            acceptor_->close(ec);

        std::cout << "Server Stopped\n";
    }

    void SetCommandHandler(CommandHandler* handler)
    {
        commandHandler_ = handler;
    }

private:
    void StartAccept()
    {
        /* wait for incoming client connection */
        acceptor_->async_accept(
            [this](boost::system::error_code ec, boost::asio::ip::tcp::socket socket)
        {
            /* on successful accept, create a session */
            if (!ec)
            {
                CreateSession(std::move(socket));
            }
            else
                StartAccept();
        }
        );
    }

    void CreateSession(tcp::socket socket)
    {
        /* allocate new session for connected client */
        auto session = std::make_shared<Session>(std::move(socket));

        /* attach command handler to session */
        session->SetCommandHandler(commandHandler_);

        /* start session processing */
        session->Start();
    }

private:
    std::unique_ptr<tcp::acceptor> acceptor_;
    CommandHandler* commandHandler_ = nullptr;
};