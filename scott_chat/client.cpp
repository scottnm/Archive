/*
 * Initialize Winsock.
 * Create a socket.
 * Connect to the server.
 * Send and receive data.
 * Disconnect.
 */

#include "client.h"
#define CLEANUP_EXIT() do{WSACleanup(); exit(1);}while(0);

#define DEFAULT_PORT "50000" // DEFAULT CLIENT PORT

SOCKET client::create_socket(const char* hostname)
{
    struct addrinfo hints = {0};
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    // get server info
    struct addrinfo* serv_addr;
    auto addr_res = getaddrinfo(hostname, DEFAULT_PORT, &hints, &serv_addr);
    if (addr_res != 0)
    {
        printf("ERR: Getting server address failed <%d>\n", addr_res);
        CLEANUP_EXIT();
    }
    printf("...resolved server info");


    auto client_socket = socket(serv_addr->ai_family, serv_addr->ai_socktype, serv_addr->ai_protocol);
    if (client_socket == INVALID_SOCKET)
    {
        printf("ERR: Couldn't create client socket <%ld>\n", WSAGetLastError());
        freeaddrinfo(serv_addr);
        CLEANUP_EXIT();
    }
    printf("...generated socket to connect to");

    if (connect(client_socket, serv_addr->ai_addr, static_cast<int>(serv_addr->ai_addrlen)) == SOCKET_ERROR)
    {
        freeaddrinfo(serv_addr);
        closesocket(client_socket);
        printf("ERR: Unable to connect to server!\n");
        CLEANUP_EXIT();
    }
    freeaddrinfo(serv_addr);
    printf("...Connected to server!\n");

    return client_socket;
}
