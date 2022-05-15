#include "load_image.h"
#include <iostream>

using twodthreed::utility::byte_buffer;
using twodthreed::load_bitmap_into_buffer;
using std::cout;
using std::endl;

int main()
{
    load_bitmap_into_buffer("../assets/2d/skeletonghost.bmp");
}
