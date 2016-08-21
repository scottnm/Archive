#include "byte_buffer.h"
#include "gtest/gtest.h"

using twodthreed::utility::byte_buffer;

TEST(byte_buffer_test, size)
{
    byte_buffer buf(10);
    ASSERT_EQ(10, buf.size());
    buf.destroy();
}

TEST(byte_buffer_test, bracket_access)
{
    byte_buffer buf(2);
    buf[0] = 'a';
    buf[1] = 'b';
    ASSERT_EQ(buf[0], 'a');
    ASSERT_EQ(buf[1], 'b');

    buf[0] = 'c';
    buf[1] = 'd';
    ASSERT_NE(buf[0], 'a');
    ASSERT_NE(buf[1], 'b');
    ASSERT_EQ(buf[0], 'c');
    ASSERT_EQ(buf[1], 'd');
    buf.destroy();
}

TEST(byte_buffer_test, data_access)
{
    byte_buffer buf(2);
    byte* raw_buf = buf.data();
    raw_buf[0] = 'a';
    raw_buf[1] = 'b';
    ASSERT_EQ(buf[0], 'a');
    ASSERT_EQ(buf[1], 'b');

    raw_buf[0] = 'c';
    raw_buf[1] = 'd';
    ASSERT_NE(buf[0], 'a');
    ASSERT_NE(buf[1], 'b');
    ASSERT_EQ(buf[0], 'c');
    ASSERT_EQ(buf[1], 'd');
    buf.destroy();
}

TEST(byte_buffer_test, iteration)
{
    byte_buffer buf(10);
    byte* raw_buf = buf.data();
    for (unsigned int i = 0; i < buf.size() - 1; ++i)
    {
        raw_buf[i] = 'a' + i;
    }
    buf[9] = '\0';

    char* alphabet_string = static_cast<char*>(malloc(10));
    int i = 0;
    for (auto _byte : buf)
    {
        alphabet_string[i++] = _byte; 
    }

    ASSERT_STREQ("abcdefghi", alphabet_string);
    buf.destroy();
}
