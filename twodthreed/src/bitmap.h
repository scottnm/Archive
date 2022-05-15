#pragma once

#include "byte_buffer.h"

namespace twodthreed
{
    using utility::byte_buffer;

    struct bitmap
    {
        int width;
        int height;
        int num_color_planes;
        int bits_per_pixel;
        byte_buffer raw_data;
    };
}
