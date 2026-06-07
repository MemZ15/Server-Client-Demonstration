#include "Includes.h"


void Session::Send(std::string msg) {
    auto self = shared_from_this();

    /* copy outgoing message into current session buffer */
    std::memcpy(buffer_, msg.data(), msg.size());

    /* queue socket write */
    socket_.async_write_some(
        boost::asio::buffer(
            buffer_,
            msg.size()
        ),
        [self](
            const boost::system::error_code& ec,
            std::size_t len)
    {
        /* terminate session on write failure */
        if (ec) {
            self->socket_.close();
            return;
        }

        std::cout << "[BYTES SENT] " << len << '\n';

        /* resume receive loop */
        self->Read();
    }
    );
}

void Session::Read() {
    auto self = shared_from_this();

    /* queue socket read */
    socket_.async_read_some(
        boost::asio::buffer(buffer_),
        [self](
            const boost::system::error_code& ec,
            std::size_t len)
    {
            /* terminate session on read failure */
            if (ec) {
                std::cout << "[DEBUG] Read error: " << ec.message() << '\n';
                self->socket_.close();
                return;
            }

            std::string msg(self->buffer_, len);
            std::cout << "[BYTES RECEIVED] " << len << " bytes: ";

            std::cout.write(self->buffer_, len);

            std::cout << '\n';

            /* process request and generate response */
            if (self->commandHandler_)
                self->Send(self->commandHandler_->Execute(msg));

        }
    );
}

void Session::Write(std::size_t len) {
    auto self = shared_from_this();

    /* write using current session buffer */
    socket_.async_write_some(
        boost::asio::buffer(
            buffer_,
            len
        ),
        [self](
            const boost::system::error_code& ec,
            std::size_t bytesTransferred)
        {
            /* terminate session on write failure */
            if (ec)
            {
                self->socket_.close();
                return;
            }

            std::cout << "[BYTES SENT] " << bytesTransferred << " bytes: ";

            std::cout.write(self->buffer_, bytesTransferred);

            std::cout << '\n';

            /* reset state */
            self->Read();
        }
    );
}