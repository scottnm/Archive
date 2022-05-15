/*
 * A simple wrapper around an array of byte
 * and the length of that array
 */

#pragma once

#include <cstdint>
#include <cstdlib>

typedef unsigned char byte;
typedef uint32_t uint32;

namespace twodthreed
{
    namespace utility
    {
        class byte_buffer
        {
        public:
            // need to create the buffer
            byte_buffer(uint32 size = 0);
            byte_buffer(const byte_buffer&) = default;
            byte_buffer(byte_buffer&&) = default;
            byte_buffer& operator=(const byte_buffer&) & = default;
            byte_buffer& operator=(byte_buffer&&) & = default;

            void destroy(void);

            byte& operator[](int i);
            byte operator[](int i) const;

            byte* data(void);
            uint32 size(void) const;

            byte* begin(void);
            const byte* cbegin(void) const;
            byte* end(void);
            const byte* cend(void) const;

        private:
            uint32 _size;
            byte* _buffer;
        };
    }
}
