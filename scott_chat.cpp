#include "stdafx.h"

#include "client.h"
#include "input.h"
#include "server.h"
#include "tui.h"

#include <string.h>
#include <thread>

constexpr uint16_t c_connection_aborted = 10053;

bool globals::application_running = true;

struct app_args
{
    app_args() : run_as_server(false), user_name(nullptr) {}

    app_args(int argc, char** argv)
    {
        if (argc < 3)
        {
            printf("Usage: ./scott_chat [server | client] [user_name]\n");
            exit(1);
        }

        run_as_server = _stricmp(argv[1], "server") == 0;
        user_name = argv[2];
        user_name_len = strlen(user_name);
    }

    bool run_as_server;
    const char* user_name;
    uint8_t user_name_len;
};
static struct app_args args;

static void initialize_winsock();
static bool is_exit_msg(const char* msg);

static SOCKET chat_socket;
static void recv_proc(void);
static void send_proc(void);

int main(int argc, char** argv)
{
    args = struct app_args(argc, argv);
    initialize_winsock();
    auto tui_thread = tui::init();

    /*
    chat_socket = args.run_as_server ? server::create_socket("localhost") : client::create_socket("localhost");
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

void recv_proc(void)
{
    while (globals::application_running)
    {
        auto recv_buf = std::array<char, c_network_msg_cch>();
        auto bytes_recv = recv(chat_socket, recv_buf.data(), recv_buf.max_size(), 0);
        if (bytes_recv <= 0)
        {
            auto error = WSAGetLastError();
            if (bytes_recv == 0 || error == c_connection_aborted)
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
        // TODO: replace printf with place to convo thread
        printf("> %s\n", recv_buf.data());
    }
    shutdown(chat_socket, SD_BOTH);
}

void send_proc(void)
{
    auto msg_buf = std::array<char, c_network_msg_cch>();
    while (globals::application_running)
    {
        auto input_not_ready = !input::process_input(500);
        if (input_not_ready)
        {
            continue;
        }

        input::read(msg_buf.data(), msg_buf.max_size());
        if (strncmp("exit", msg_buf.data(), 4) == 0)
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
        tui::write_msg_to_conversation_thread(msg_buf.data());
        tui::clear_input();
    }
    shutdown(chat_socket, SD_BOTH);
}
