// scott_chat.cpp : Defines the entry point for the console application.

#include "stdafx.h"
#include "client.h"
#include "server.h"
#include <cstring>
#include <cassert>

#include <chrono>
#include <thread>

static void initialize_winsock();
static bool is_server(int argc, char** argv);

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

    char recv_buf[30] = {0};
    for (int i = 0; i < messages_to_send; ++i)
    {
        msg_buf[12] = 'a' + i;
        auto bytes_sent = send(socket, msg_buf, msg_len, 0);
        if (bytes_sent <= 0)
        {
            printf("Failed to send\n");
            exit(1);
        }
        auto bytes_recv = recv(socket, recv_buf, 30, 0);
        if (bytes_recv <= 0)
        {
            if (bytes_recv == 0)
            {
                printf("Connection closed\n");
                break;
            }
            assert(bytes_recv == SOCKET_ERROR);
            printf("Failed to recv <%d>\n", WSAGetLastError());
            exit(1);
        }
        printf("> %s\n", recv_buf);
    }

    shutdown(socket, SD_SEND);
    std::this_thread::sleep_for(std::chrono::milliseconds(16));
    shutdown(socket, SD_RECEIVE);
    closesocket(socket);
    WSACleanup();
    return 0;
}

static void initialize_winsock()
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

