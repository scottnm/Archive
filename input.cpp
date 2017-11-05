#include "stdafx.h"

#include "input.h"
#include "tui.h"

#include <assert.h>
#include <string.h>

#define BUFFER_SIZE 1024
namespace input
{
    static char buffer[BUFFER_SIZE];
    static uint16_t size = 0;
    static uint16_t size_final = 0;

    static void pushchar(char c)
    {
        buffer[size++] = c;
        assert( 0 < size && size < BUFFER_SIZE );
    }

    static void popchar()
    {
        if (size > 0)
        {
            --size;
        }
        assert(0 <= size && size < BUFFER_SIZE - 1);
    }

    static void finalize_string(void)
    {
        printf("\n");
        size_final = size;
        size = 0;
        buffer[size_final++] = '\0';
        assert(size_final <= BUFFER_SIZE);
    }

    bool process_input(uint32_t timeout_ms)
    {
        auto input_res = WaitForSingleObject(GetStdHandle(STD_INPUT_HANDLE), timeout_ms);

        if (input_res == WAIT_TIMEOUT)
        {
            return false;
        }
        assert(input_res == WAIT_OBJECT_0);

        INPUT_RECORD record;
        DWORD numRead;
        auto read_res = ReadConsoleInput(GetStdHandle(STD_INPUT_HANDLE), &record, 1, &numRead);
        assert(read_res);

        if(record.EventType != KEY_EVENT)
        {
            // don't care about other console events
            // TODO(scottnm): fix so that moving mouse/other key events don't reset timeout, perform a retry with less time
            return false;
        }

        if(!record.Event.KeyEvent.bKeyDown)
        {
            // really only care about keydown
            return false;
        }

        char c = record.Event.KeyEvent.uChar.AsciiChar;
        if (c == '\n' || c == '\r')
        {
            finalize_string();
            return size_final > 1; // only return true if we have a non empty buffer
        }
        else
        {
            if (c == '\b')
            {
                popchar();
                tui::pop_from_input_field();
            }
            else
            {
                pushchar(c);
                tui::push_to_input_field(c);
            }
            return false;
        }
    }

    void read(char* outbuf, uint16_t outbuf_capacity)
    {
        assert( outbuf_capacity >= size_final );
        strncpy_s(outbuf, outbuf_capacity, buffer, size_final);
        size_final = 0;
        // ensure that the string is null-terminated since strncpy_s does not if the capacity is reached
        outbuf[outbuf_capacity - 1] = '\0';
    }
}
