#pragma once

// Standard C++ includes
// std::move is noexcept. Don't generate compiler warings that I'm not compiling with exceptions enabled
#pragma warning(push)
#pragma warning(disable: 4577)
#include <utility> // std::move
#pragma warning(pop)

// Standard C includes
#include <assert.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#pragma comment (lib, "kernel32.lib")

#include "defines.h"

#include "memutils.h"

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;
typedef const char* cstr;
