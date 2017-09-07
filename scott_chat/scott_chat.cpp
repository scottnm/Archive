// scott_chat.cpp : Defines the entry point for the console application.

#include "stdafx.h"
#include "client.h"
#include "server.h"
#include <cstring>
#include <cassert>
#include <iostream>
#include <string>

#define CONNECTION_ABORTED 10053

static void initialize_winsock();
static bool is_server(int argc, char** argv);
static bool is_exit_msg(const char* msg);

int main(int argc, char** argv)
{
    initialize_winsock();

    SOCKET socket;
    const auto msg_len = 14;
    char msg_buf[msg_len] = {0}; // +3 = for a space, extra char, and the null delim
    int messages_to_send;

    if (is_server(argc, argv))
    {
        socket = server::create_socket("localhost");
        strncpy_s(msg_buf, msg_len, "from server  ", 13);
        messages_to_send = 10;
    }
    else
    {
        socket = client::create_socket("localhost");
        strncpy_s(msg_buf, msg_len, "from client  ", 13);
        messages_to_send = 11;
    }

    while (true)
    {
        std::string msg;
        std::cin >> msg;

        if (is_exit_msg(msg.c_str()))
        {
            break;
        }

        auto bytes_sent = send(socket, msg.data(), msg.size(), 0);
        if (bytes_sent <= 0)
        {
            printf("Failed to send\n");
            exit(1);
        }

        char recv_buf[250];
        auto bytes_recv = recv(socket, recv_buf, 250, 0);
        if (bytes_recv <= 0)
        {
            auto error = WSAGetLastError();
            if (bytes_recv == 0 || error == CONNECTION_ABORTED)
            {
                printf("Connection closed\n");
                break;
            }

            assert(bytes_recv == SOCKET_ERROR);
            printf("Failed to recv <%d>\n", error);
            exit(1);
        }
        recv_buf[bytes_recv] = '\0';
        printf("> %s\n", recv_buf);
    }

    shutdown(socket, SD_BOTH);
    closesocket(socket);
    WSACleanup();
    return 0;
}

void initialize_winsock()
{
    WSADATA wsa_data;
    int wsa_startup_res = WSAStartup(MAKEWORD(2, 2), &wsa_data);
    if (wsa_startup_res != 0)
    {
        printf("ERR: WSAStartup failed <%d>\n", wsa_startup_res);
        exit(1);
    }
}

bool is_server(int argc, char** argv)
{
    if (argc < 2)
    {
        printf("Usage: ./scott_chat [server | client]\n");
        exit(1);
    }

    return _stricmp(argv[1], "server") == 0;
}

bool is_exit_msg(const char* msg)
{
    return strlen(msg) >= 4 &&
           tolower(msg[0]) == 'e' &&
           tolower(msg[1]) == 'x' &&
           tolower(msg[2]) == 'i' &&
           tolower(msg[3]) == 't' &&
           tolower(msg[4]) == '\0';
}
