#include <boost/asio.hpp>
#include <iostream>

using boost::asio::ip::tcp;

int main()
{
    boost::asio::io_context io;

    tcp::socket socket(io);

    try
    {
        socket.connect(
            tcp::endpoint(
                boost::asio::ip::make_address("127.0.0.1"),
                8080
            )
        );

        std::cout << "Connected to server.\n";
    }
    catch (std::exception& e)
    {
        std::cout << "Connection failed: " << e.what() << "\n";
        return 1;
    }

    while (true)
    {
        std::string msg;

        std::cout << "\n> ";
        std::getline(std::cin, msg);

        if (msg == "exit")
            break;

        // ---------------- SEND ----------------
        boost::asio::write(socket, boost::asio::buffer(msg));

        // ---------------- RECEIVE ----------------
        char reply[1024]{};

        boost::system::error_code ec;
        size_t len = socket.read_some(boost::asio::buffer(reply), ec);

        if (ec)
        {
            std::cout << "Disconnected from server.\n";
            break;
        }

        std::cout << "server: ";
        std::cout.write(reply, len);
        std::cout << "\n";
    }

    socket.close();

    system("pause");
    return 0;
}