/*
 * Initialize Winsock.
 * Create a socket.
 * Connect to the server.
 * Send and receive data.
 * Disconnect.
 */

#include "client.h"

#define DEFAULT_PORT "27015" // DEFAULT CLIENT PORT

SOCKET client::create_socket(const char* hostname)
{
    struct addrinfo hints = {0};
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    // get server info
    struct addrinfo* server_addr;
    int addr_res = getaddrinfo(hostname, DEFAULT_PORT, &hints, &server_addr);
    if (addr_res != 0)
    {
        printf("ERR: Getting server address failed <%d>\n", addr_res);
        WSACleanup();
        exit(1);
    }


    SOCKET client_socket = socket(server_addr->ai_family, server_addr->ai_socktype, server_addr->ai_protocol);
    if (client_socket == INVALID_SOCKET)
    {
        printf("ERR: Couldn't create client socket <%ld>\n", WSAGetLastError());
        freeaddrinfo(server_addr);
        WSACleanup();
        exit(1);
    }

    int connect_res = connect(client_socket, server_addr->ai_addr, static_cast<int>(server_addr->ai_addrlen));
    freeaddrinfo(server_addr);
    if (connect_res == SOCKET_ERROR)
    {
        closesocket(client_socket);
        printf("ERR: Unable to connect to server!\n");
        WSACleanup();
        exit(1);
    }

    return client_socket;
}
