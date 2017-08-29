#ifndef __SERVER_H__
#define __SERVER_H__

#include "stdafx.h"

namespace server
{
    SOCKET create_socket(const char* hostname);
}

#endif //__SERVER_H__
