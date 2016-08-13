#pragma once

#include "iimgloader.h"

class bmploader : public iimgloader
{
    vector<byte> load_image_as_pixel_array(const string& filename) override;
};
