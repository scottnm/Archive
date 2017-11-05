#include "stdafx.h"

#include "client.h"
#include "server.h"
#include "tui.h"
#include <string.h>
#include <assert.h>
#include <thread>

#include "input.h"

#define CONNECTION_ABORTED 10053

bool globals::application_running = true;

static void initialize_winsock();
static bool is_server(int argc, char** argv);
static bool is_exit_msg(const char* msg);

static SOCKET chat_socket;
static void recv_proc(void);
static void send_proc(void);

int main(int argc, char** argv)
{
    initialize_winsock();
    auto tui_thread = tui::init();

    const auto msg_len = 14;
    char msg_buf[msg_len] = {0}; // +3 = for a space, extra char, and the null delim
    int messages_to_send;

    /*
    if (is_server(argc, argv))
    {
        chat_socket = server::create_socket("localhost");
        strncpy_s(msg_buf, msg_len, "from server  ", 13);
        messages_to_send = 10;
    }
    else
    {
        chat_socket = client::create_socket("localhost");
        strncpy_s(msg_buf, msg_len, "from client  ", 13);
        messages_to_send = 11;
    }
    */

    //std::thread recv_thread(recv_proc);
    std::thread send_thread(send_proc);

    //recv_thread.join();
    send_thread.join();
    tui_thread.join();

    closesocket(chat_socket);
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

void recv_proc(void)
{
    while (globals::application_running)
    {
        char recv_buf[250];
        auto bytes_recv = recv(chat_socket, recv_buf, 250, 0);
        if (bytes_recv <= 0)
        {
            auto error = WSAGetLastError();
            if (bytes_recv == 0 || error == CONNECTION_ABORTED)
            {
                printf("Connection closed\n");
            }
            else
            {
                assert(bytes_recv == SOCKET_ERROR);
                printf("Failed to recv <%d>\n", error);
            }
            globals::application_running = false;
            break;
        }
        recv_buf[bytes_recv] = '\0';
        printf("> %s\n", recv_buf);
    }
    shutdown(chat_socket, SD_BOTH);
}

void send_proc(void)
{
    while (globals::application_running)
    {
        bool input_ready = input::process_input(500);
        if ( !input_ready )
        {
            continue;
        }

        char msg_buf[250];
        input::read(msg_buf, 250);
        if (strncmp("exit", msg_buf, 4) == 0)
        {
            globals::application_running = false;
            break;
        }

        /*
        auto bytes_sent = send(chat_socket, msg_buf, strlen(msg_buf), 0);
        if (bytes_sent <= 0)
        {
            printf("Failed to send\n");
            exit(1);
        }
        */
        tui::clear_input_field();
    }
    shutdown(chat_socket, SD_BOTH);
}
