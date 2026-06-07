#pragma once
#include "Includes.h"

using boost::asio::ip::tcp;

class ServerHandler
{
public:
    void Init( boost::asio::io_context& IO, uint16_t Port ) {
        /* create acceptor bound to IPv4 endpoint and specified port */
        Acceptor = std::make_unique<tcp::acceptor>(
            IO,
            tcp::endpoint(tcp::v4( ), Port)
        );

        AwaitAccept( );

        std::cout << "Server Started\n";
        std::cout << "Listening on port " << Port << "\n";
    }

    void Stop() {
        boost::system::error_code ec{ };

        /* safely close acceptor if it is active */
        if ( Acceptor && Acceptor->is_open( ) )
            Acceptor->close( ec );

        std::cout << "Server Stopped\n";
    }

    void SetCommandHandler( CommandHandler* handler) { commandHandler_ = handler; }

private:
    void AwaitAccept() {
        /* wait for incoming client connection */
        Acceptor->async_accept(
            [this]( boost::system::error_code ec, boost::asio::ip::tcp::socket socket )
            {
                /* on successful accept, create a session */
                if ( !ec ) {
                    CreateSession( std::move(socket) );

                    std::cout << "New client connected\n";
                }
                else
                    AwaitAccept();
            }
        );
    }

    void CreateSession(tcp::socket socket) {
        /* allocate new session for connected client */
        auto session = std::make_shared<Session>(std::move(socket));

        /* attach command handler to session */
        session->SetCommandHandler(commandHandler_);

        /* start session processing */
        session->Start();
    }

private:
    std::unique_ptr<tcp::acceptor> Acceptor{ nullptr };
    CommandHandler* commandHandler_ { nullptr };
};