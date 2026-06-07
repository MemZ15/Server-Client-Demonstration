#include "Includes.h"

int main() {

    // Initialize Application */
    try {
        /* Create IO Context */
        boost::asio::io_context IO{ };

        ServerHandler server{ };
        CommandHandler commands{ };

        /* Register commands and set command handler for server */
        commands.RegisterCommands( commands );
        server.SetCommandHandler( &commands );

        /* Initialize server and run IO Context */
        server.Init( IO, 8080 );
        IO.run();
    }

    catch ( const boost::system::system_error& e ) {
        std::cout << "Fatal error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}