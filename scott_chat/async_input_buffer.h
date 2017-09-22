#ifndef __ASYNC_INPUT_BUFFER_H__
#define __ASYNC_INPUT_BUFFER_H__

#include <condition_variable>
#include <mutex>
#include <thread>

#define BUFSIZE 1024
class async_input_buffer
{
public:
    async_input_buffer();

    void write_char(char c);
    void end_write(void);
    bool read_string_blk(char* outbuf);

private:
    char m_buf[BUFSIZE];
    char* m_reader;
    char* m_writer_start;
    char* m_writer_mark;

    std::mutex m_read_mutex;
    std::condition_variable m_buf_ready_or_timeout;
};

#endif //__ASYNC_INPUT_BUFFER_H__
