#include "stdafx.h"

#include "tui.h"
#include <assert.h>
#include <stdint.h>

#define INPUT_FIELD_RATIO 0.2

namespace tui
{
    static HANDLE console_handle;
    static CONSOLE_SCREEN_BUFFER_INFO screen_buffer_info;

    static std::vector<char> conversation_buffer;
    static uint32_t conversation_offset;

    static std::vector<char> input_buffer;
    static uint32_t input_offset;

    static uint16_t conversation_window_height;
    static uint16_t window_line_count;

    void init()
    {
        console_handle = CreateConsoleScreenBuffer(
                GENERIC_WRITE | GENERIC_READ,
                FILE_SHARE_WRITE | FILE_SHARE_READ,
                nullptr,
                CONSOLE_TEXTMODE_BUFFER,
                nullptr
                );
        assert (console_handle != INVALID_HANDLE_VALUE);

        auto got_buffer_info = GetConsoleScreenBufferInfo(
                console_handle,
                &screen_buffer_info
                );

        assert (got_buffer_info);

        SetConsoleActiveScreenBuffer(console_handle);

        conversation_window_height = static_cast<uint16_t>(screen_buffer_info.dwSize.Y * (1 - INPUT_FIELD_RATIO));
        window_line_count = (screen_buffer_info.dwSize.Y - conversation_window_height) - 1;
    }

    void write_to_input_field(char c)
    {
        (void)c;
    }

    void clear_input_field(void)
    {
        static char flip_char = 'a';
        SetConsoleCursorPosition(console_handle, COORD {0, static_cast<int16_t>(conversation_window_height)});
        for (uint32_t cursor = 0; cursor < window_line_count * screen_buffer_info.dwSize.X; ++cursor)
        {
            WriteFile(console_handle, &flip_char, 1, nullptr, nullptr);
        }
        flip_char = 'a' + ((flip_char + 1 - 'a') % 26);
        SetConsoleWindowInfo(console_handle, TRUE, &screen_buffer_info.srWindow);
    }

    void write_msg_to_conversation_thread(const char* msg)
    {
        (void)msg;
    }
}
