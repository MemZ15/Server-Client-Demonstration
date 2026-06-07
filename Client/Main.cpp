#include "Client.h"

int main() {

    Client ClientCtx{};

    try {
        if (!ClientCtx.Initialize())
            return -1;

        if (!ClientCtx.Connect("127.0.0.1", 8080))
            return -1;

        ClientCtx.RunClient();
    }
    catch ( std::exception& ex ) {
        std::cerr << "Exception: " << ex.what() << "\n";
    }

    return 0;
}