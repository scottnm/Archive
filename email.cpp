#include "email.h"
#include "..\scott_chat\client.h"

#include <iostream>
#include <cstdlib>
#include <fstream>
#include <iostream>

#define CLEANUP_EXIT() do{WSACleanup(); exit(1);}while(0);

static const std::string crlf = "\r\n";
static const std::string smtp_server = "smtp.gmail.com";
static const std::string smtp_port = "587";

namespace email
{
    static
    void
    ensure_success(int socket_status, std::string call_id)
    {
        if (socket_status == SOCKET_ERROR || !socket_status)
        {
            std::cerr << "Error: " << call_id << " failed with: " << socket_status << " - " << WSAGetLastError() << std::endl;
            exit(1);
        }
    }

    SOCKET
    init(
        )
    {
        WSADATA wsa_data;
        auto wsa_startup_res = WSAStartup(MAKEWORD(2, 2), &wsa_data);
        if (wsa_startup_res != 0)
        {
            std::cerr << "Error: WSAStartup failed " << wsa_startup_res << std::endl;
            exit(1);
        }

        auto serv_entry = getservbyname("mail", 0);
        return client::create_socket(smtp_server.c_str(), smtp_port.c_str());
    }

    void
    send_mail(
        SOCKET email_socket,
        std::string to_addr,
        std::string from_addr,
        std::string subject,
        std::string msg_body
        )
    {
        constexpr uint16_t cch_recv_buffer = 1024;
        char recv_buffer[cch_recv_buffer] = {0};

        constexpr uint16_t cch_send_buffer = 256;;
        char send_buffer[cch_send_buffer] = {0};

        (void)subject;

        // recv initial message
        ensure_success(
            recv(email_socket, recv_buffer, cch_recv_buffer, 0),
            "recv initial message");

        // send hello
        sprintf(send_buffer, "HELO %s%s", smtp_server.c_str(), crlf.c_str());
        ensure_success(
            send(email_socket, send_buffer, strlen(send_buffer), 0),
            "send hello message");
        ensure_success(
            recv(email_socket, recv_buffer, cch_recv_buffer, 0),
            "recv hello message");
        printf("recv<hello> :: %s", recv_buffer);

        // send mail from
        sprintf(send_buffer, "MAIL FROM: <%s>%s", from_addr.c_str(), crlf.c_str());
        ensure_success(
            send(email_socket, send_buffer, strlen(send_buffer), 0),
            "send from message");
        ensure_success(
            recv(email_socket, recv_buffer, cch_recv_buffer, 0),
            "recv from message");
        printf("recv<from> :: %s", recv_buffer);

        // send recipient
        sprintf(send_buffer, "RCPT TO:<%s>%s", to_addr.c_str(), crlf.c_str());
        ensure_success(
            send(email_socket, send_buffer, strlen(send_buffer), 0),
            "send() RCPT TO");
        ensure_success(
            recv(email_socket, recv_buffer, cch_recv_buffer, 0),
            "recv() RCPT TO");
        printf("recv<rcpt> :: %s", recv_buffer);

        // send DATA header
        sprintf(send_buffer, "DATA%s", crlf.c_str());
        ensure_success(
            send(email_socket, send_buffer, strlen(send_buffer), 0),
            "send() DATA");
        ensure_success(
            recv(email_socket, recv_buffer, cch_recv_buffer, 0),
            "recv() DATA");
        printf("recv<data header> :: %s", recv_buffer);

        // send msg body
        ensure_success(
            send(email_socket, msg_body.c_str(), msg_body.size(), 0),
            "send() message body");

        // end message with crlf . crlf
        sprintf(send_buffer, "%s.%s", crlf.c_str(), crlf.c_str());
        ensure_success(
            send(email_socket, send_buffer, strlen(send_buffer), 0),
            "send() end-message");
        ensure_success(
            recv(email_socket, recv_buffer, cch_recv_buffer, 0),
            "recv() end-message");
        printf("recv<end-message> :: %s", recv_buffer);

        // send QUIT
        sprintf(send_buffer, "QUIT%s", crlf.c_str());
        ensure_success(
            send(email_socket, send_buffer, strlen(send_buffer), 0),
            "send() QUIT");
        ensure_success(
            recv(email_socket, recv_buffer, cch_recv_buffer, 0),
            "recv() QUIT");
        printf("recv<quit> :: %s", recv_buffer);

        std::cout << "email success" << std::endl;
    }

    void
    cleanup(
        SOCKET socket
        )
    {
        closesocket(socket);
        WSACleanup();
    }
}
