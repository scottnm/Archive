#ifndef __CLIENT_H__
#define __CLIENT_H__

#include "stdafx.h"

namespace client
{
    SOCKET create_socket(const char* hostname);
}

#endif //__CLIENT_H__
