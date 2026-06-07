#pragma once
#include "Includes.h"
#include <functional>

using boost::asio::ip::tcp;

class Session : public std::enable_shared_from_this<Session>
{
public:
    explicit Session(tcp::socket socket)
        : socket_(std::move(socket))
    {}

    void Start()
    {
        /* begin read loop for this session */
        DoRead();
    }

    void SetCommandHandler(CommandHandler* handler)
    {
        /* attach external command handler used for processing incoming messages */
        commandHandler_ = handler;
    }

private:

    void Send(const std::string& msg)
    {
        auto self = shared_from_this();

        /* copy outgoing message into internal fixed buffer */
        std::memcpy(buffer_, msg.data(), msg.size());

        /* write data to socket */
        socket_.async_write_some(
            boost::asio::buffer(buffer_, msg.size()),
            std::bind(&Session::OnWrite,
                      self,
                      std::placeholders::_1,
                      std::placeholders::_2)
        );
    }

    // READ DISPATCH
    void DoRead()
    {
        /* read incoming data into buffer */
        socket_.async_read_some(
            boost::asio::buffer(buffer_),
            std::bind(&Session::OnRead,
                      shared_from_this(),
                      std::placeholders::_1,
                      std::placeholders::_2)
        );
    }

    // WRITE DISPATCH
    void DoWrite(std::size_t len)
    {
        /* write len bytes from buffer to socket */
        socket_.async_write_some(
            boost::asio::buffer(buffer_, len),
            std::bind(&Session::OnWrite,
                      shared_from_this(),
                      std::placeholders::_1,
                      std::placeholders::_2)
        );
    }

    void OnRead(const boost::system::error_code& ec, std::size_t len)
    {
        /* handle read error and terminate session if needed */
        if (ec)
        {
            socket_.close();
            return;
        }

        std::string msg(buffer_, len);

        std::cout << "[BYTES RECEIVED] " << len << " bytes: ";
        std::cout.write(buffer_, len);
        std::cout << "\n";

        /* process message via command handler if available */
        if (commandHandler_)
        {
            std::string response = commandHandler_->Execute(msg);

            /* send generated response back to client */
            Send(response);
            return;
        }
    }

    // WRITE HANDLER
    void OnWrite(const boost::system::error_code& ec, std::size_t len)
    {
        /* handle write error and terminate session if needed */
        if (ec)
        {
            socket_.close();
            return;
        }

        std::cout << "[BYTES SENT] " << len << " bytes: ";
        std::cout.write(buffer_, len);
        std::cout << "\n";

        DoRead();
    }

private:
    tcp::socket socket_;
    CommandHandler* commandHandler_ = nullptr;
    char buffer_[1024]{};
};