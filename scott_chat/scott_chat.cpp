// scott_chat.cpp : Defines the entry point for the console application.

#include "client.h"

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

int main()
{
    initialize_winsock();
    SOCKET client_socket = client::create_socket("localhost");
    return 0;
}
