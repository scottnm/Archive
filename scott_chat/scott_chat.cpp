// scott_chat.cpp : Defines the entry point for the console application.

#include "client.h"
#include "server.h"
#include <string.h>

static void initialize_winsock();
static bool is_server(int argc, char** argv);

int main(int argc, char** argv)
{
    initialize_winsock();
    bool _is_server = is_server(argc, argv);
    auto socket = _is_server ? server::create_socket("localhost") : client::create_socket("localhost");
    auto msg = _is_server ? "Hello from server!" : "Hello from client";
    #define RECV_BUF_SIZE 30
    char recv_buf[RECV_BUF_SIZE] = {0};
    send(socket, msg, strlen(msg), 0);
    recv(socket, recv_buf, RECV_BUF_SIZE, 0);
    printf("> Message recieved!\n> %s\n", recv_buf);
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

    return stricmp(argv[1], "server") == 0;
}
