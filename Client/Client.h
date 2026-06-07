#pragma once
#include <boost/asio.hpp>
#include <iostream>

using boost::asio::ip::tcp;

class Client
{
public:
    Client()
        : socket_(io_)
    {}

    bool Connect(const std::string& ip, uint16_t port)
    {
        try
        {
            socket_.connect(
                tcp::endpoint(
                    boost::asio::ip::make_address(ip),
                    port
                )
            );

            std::cout << "Connected to server.\n";
            return true;
        }
        catch (const std::exception& e)
        {
            std::cout << "Connection failed: " << e.what() << "\n";
            return false;
        }
    }

    void Run()
    {
        std::string msg;

        while (true)
        {
            msg = GetUserInput();

            if (msg == "exit")
                break;

            if (!Send(msg))
                break;

            if (!Receive())
                break;
        }

        Close();
    }

private:

    std::string GetUserInput()
    {
        std::string msg;
        std::cout << "\n> ";
        std::getline(std::cin, msg);
        return msg;
    }

    bool Send(const std::string& msg)
    {
        try
        {
            boost::asio::write(socket_, boost::asio::buffer(msg));
            return true;
        }
        catch (const std::exception& e)
        {
            std::cout << "Send failed: " << e.what() << "\n";
            return false;
        }
    }

    bool Receive()
    {
        try
        {
            char reply[1024]{};

            boost::system::error_code ec;
            size_t len = socket_.read_some(boost::asio::buffer(reply), ec);

            if (ec)
            {
                std::cout << "Disconnected from server.\n";
                return false;
            }

            PrintServerResponse(reply, len);
            return true;
        }
        catch (const std::exception& e)
        {
            std::cout << "Receive failed: " << e.what() << "\n";
            return false;
        }
    }

    void PrintServerResponse(const char* data, size_t len)
    {
        std::cout << "server: ";
        std::cout.write(data, len);
        std::cout << "\n";
    }

    void Close()
    {
        boost::system::error_code ec;
        socket_.close(ec);
    }

private:
    boost::asio::io_context io_;
    tcp::socket socket_;
};