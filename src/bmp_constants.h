#pragma once

// Bitmap Header Constants
namespace bmp_constants 
{
    namespace header
    {
        constexpr int size = 14;
        constexpr const char* id = "BM";
        constexpr int id_ofs = 0;
        constexpr int file_size_ofs = 2;
        constexpr int data_ofs_ofs = 10;
    }

    namespace dibs_info_header
    {
        constexpr int size = 40;
    }
}
