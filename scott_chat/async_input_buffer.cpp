#include "async_input_buffer.h"
#include <cassert>
#include <chrono>
#include <cstring>

using std::mutex;
using std::unique_lock;

const std::chrono::milliseconds TIMEOUT(2500);

async_input_buffer::async_input_buffer()
    : m_reader(nullptr),
      m_writer_start(&m_buf[0]),
      m_writer_mark(&m_buf[0])
{
}

void async_input_buffer::write_char(char c)
{
}

void async_input_buffer::end_write(void)
{
}

bool async_input_buffer::read_string_blk(char* outbuf)
{
    unique_lock<mutex> lock(m_read_mutex);
    auto wait_res = m_buf_ready_or_timeout.wait_for(lock, TIMEOUT);
    if (wait_res == std::cv_status::timeout)
    {
        strncpy(outbuf, "TIMEOUT", 7);
    }
    else
    {
        assert(wait_res == std::cv_status::no_timeout);
        assert(false);
    }
    return true;
}
