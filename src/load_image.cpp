#include "load_image.h"
#include "bmp_constants.h"

#include <cassert>
#include <cstdio>
#include <cstdint>
#include <fstream>

using std::ifstream;

namespace twodthreed
{
    static uint32_t read_uint32(ifstream& file, int pos)
    {
        file.seekg(pos);
        uint32_t val;
        file.read(reinterpret_cast<char*>(&val), sizeof(uint32_t));
        assert(!file.fail());
        return val;
    }

    bitmap load_bitmap_into_buffer(const string& filepath)
    {
        ifstream bmp_file(filepath);

        //
        // read the specific type of bitmap
        // ensure this is the right kind of file
        // 
        char header_id[bmp_constants::header::size];
        bmp_file.read(header_id, bmp_constants::header::size); 
        assert(header_id[0] == bmp_constants::header::id[0] &&
               header_id[1] == bmp_constants::header::id[1] &&
               "File does not have Windows bitmap header");


        //
        // read the size of the file
        //
        uint32_t size_of_file = read_uint32(bmp_file, 2);

        //
        // read the size of the dibs header
        //
        uint32_t size_of_dibs_header = read_uint32(bmp_file, 14);
        assert(size_of_dibs_header == bmp_constants::dibs_info_header::size &&
                "File does not have Windows bitmap dibs header");
        uint32_t img_width = read_uint32(bmp_file, 18);
        uint32_t img_height = read_uint32(bmp_file, 22);

        //
        //
        // PLACEHOLDER
        //
        //
        printf("bitmap\n"
                "id: %c%c\n"
                "size: %d\n"
                "dibs size: %d\n"
                "width: %d\n"
                "height: %d\n",
                header_id[0], header_id[1],
                size_of_file,
                size_of_dibs_header,
                img_width,
                img_height);

        bitmap b;
        return b;
    }
}
