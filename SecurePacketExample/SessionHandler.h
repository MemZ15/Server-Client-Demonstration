#pragma once
#include "Includes.h"
#include <functional>

using boost::asio::ip::tcp;

class Session : public std::enable_shared_from_this<Session>
{
public:
    Session(tcp::socket socket)
        : socket_(std::move(socket))
    {}

    /* begin asynchronous receive loop */
    void Start() { Read( ); }

    /* attach command processor for incoming messages */
    void SetCommandHandler(CommandHandler* handler) { commandHandler_ = handler; }

    void Send(std::string msg);
    void Read();

    void Write(std::size_t len);

private:
    /* TCP Socket */
    tcp::socket socket_;

    /* Command handler */
    CommandHandler* commandHandler_ = nullptr;

    /* Current session shared I/O buffer */
    char buffer_[1024]{};
    
};