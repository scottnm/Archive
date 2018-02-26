#pragma once

#include "pch.h"

namespace email
{
    SOCKET
    init(
        );

    void
    cleanup(
        SOCKET socket
        );

    void
    send_mail(
        SOCKET email_socket,
        std::string to_addr,
        std::string from_addr,
        std::string subject,
        std::string msg_body
        );
}
