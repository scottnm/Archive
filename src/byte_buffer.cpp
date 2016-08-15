#include "byte_buffer.h"

using twodthreed::utility::byte_buffer;

byte_buffer::byte_buffer(uint32 size) :
    _size(size),
{
    _buffer = size == 0 ? nullptr : static_cast<byte*>(malloc(size));
}

byte_buffer::~byte_buffer()
{
    delete _buffer;
}

byte& byte_buffer::operator[](int i)
{
    return _buffer[i];
}

byte byte_buffer::operator[](int i) const
{
    return _buffer[i];
}

uint32 byte_buffer::size() const
{
    return _size;
}

byte* byte_buffer::begin(void)
{
    return _buffer;
}

const byte* byte_buffer::cbegin(void) const
{
    return _buffer;
}

byte* byte_buffer::end(void)
{
    return _buffer + _size;
}

const byte* byte_buffer::cend(void) const
{
    return _buffer + _size;
}

