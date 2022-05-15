#pragma once

#include "bitmap.h"

#include <string>
using std::string;

namespace twodthreed
{
    bitmap load_bitmap_into_buffer(const string& filepath);
}
