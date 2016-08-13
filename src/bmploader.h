#pragma once

#include "iimgloader.h"

namespace twodthreed
{
    class bmploader : public iimgloader
    {
        vector<byte> load_image_as_pixel_array(const string& filename) override;
    };
}
