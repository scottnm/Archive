#pragma once

#include <string>
#include <vector>

using std::string;
using std::vector;

typedef unsigned char byte;

namespace twodthreed
{
    class iimgloader
    {
        virtual vector<byte> load_image_as_pixel_array(const string& filename) = 0;
    };
}
