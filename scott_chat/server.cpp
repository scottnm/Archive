#include "stdafx.h"

#include "server.h"

#define CLEANUP_EXIT() do{WSACleanup(); exit(1);}while(0);

SOCKET server::create_socket(const char* hostname)
{
    struct addrinfo hints = {0};
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    struct addrinfo* serv_addr;
    auto addr_res = getaddrinfo(nullptr, c_default_port, &hints, &serv_addr);
    if (addr_res != 0)
    {
        printf("ERR: Getting server address info failed <%d>\n", addr_res);
        WSACleanup();
        exit(1);
    }
    printf("....got addr info");

    auto listening_socket = socket(serv_addr->ai_family, serv_addr->ai_socktype, serv_addr->ai_protocol);
    if (listening_socket == INVALID_SOCKET)
    {
        printf("ERR: Cannot create socket for server to listen <%ld>\n", WSAGetLastError());
        freeaddrinfo(serv_addr);
        CLEANUP_EXIT();
    }
    printf("....created listening socket");

    auto bind_result = bind(listening_socket, serv_addr->ai_addr, static_cast<int>(serv_addr->ai_addrlen));
    freeaddrinfo(serv_addr);
    if (bind_result == SOCKET_ERROR)
    {
        printf("ERR: Server failed to bind socket <%d>\n", bind_result);
        closesocket(listening_socket);
        CLEANUP_EXIT();
    }
    printf("....bound to port");

    if (listen(listening_socket, SOMAXCONN) == SOCKET_ERROR)
    {
        printf("ERR: Server failed to listen on socket <%d>\n", WSAGetLastError());
        closesocket(listening_socket);
        CLEANUP_EXIT();
    }
    printf("....server listening on socket");

    auto client_socket = accept(listening_socket, nullptr, nullptr);
    closesocket(listening_socket);
    if (client_socket == INVALID_SOCKET)
    {
        printf("ERR: Server failed to accept <%d>\n", WSAGetLastError());
        CLEANUP_EXIT();
    }
    printf("....FOUND CLIENT\n");

    return client_socket;
}
