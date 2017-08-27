// scottchat.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "client.h"

static void initialize_winsock()
{
    WSADATA wsa_data;
    CHECK_ERR( WSAStartup(MAKEWORD(2, 2), &wsa_data), 0, "WSAStartup" );
}

int main()
{
    initialize_winsock();
    return 0;
}

