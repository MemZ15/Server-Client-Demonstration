#include "Client.h"

int main() {

    /* create client instance */ 
    Client client{ };

    /* connect to server */
    if ( !client.Connect( "127.0.0.1", 8080 ) )
        return 1;

    /* run client loop */ 
    client.Run( );

    system( "pause" );

    return 0;
}