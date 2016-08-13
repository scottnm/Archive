#include "byte_buffer.h"
#include <iostream>

using twodthreed::utility::byte_buffer;
using std::cout;
using std::endl;

int main()
{
    byte_buffer buf(10);
    for (int i = 0; i < 10; ++i)
    {
        buf[i] = i + 'a';
    }

    for (auto c : buf)
    {
        cout << c << endl;
    }
}
