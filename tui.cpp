#include "stdafx.h"

#include "tui.h"
#include <cassert>
#include <cstdint>
#include <chrono>
#include <thread>

constexpr float c_proportional_size_of_conversation_window = 0.8f;
constexpr std::chrono::milliseconds c_sleep_time_between_draw_calls = std::chrono::milliseconds(1000/60);

namespace tui
{
    static void draw_proc();

    static HANDLE console_handle;
    static CONSOLE_SCREEN_BUFFER_INFO screen_buffer_info;

    static std::vector<char> conversation_buffer;
    static uint32_t conversation_offset;

    static std::vector<char> input_buffer;
    static uint32_t input_offset;

    static uint16_t conversation_window_height;
    static uint16_t max_input_lines;

    std::thread init()
    {
        console_handle = CreateConsoleScreenBuffer(
                GENERIC_WRITE | GENERIC_READ,
                FILE_SHARE_WRITE | FILE_SHARE_READ,
                nullptr,
                CONSOLE_TEXTMODE_BUFFER,
                nullptr
                );
        assert (console_handle != INVALID_HANDLE_VALUE);
        SetConsoleActiveScreenBuffer(console_handle);

        auto got_buffer_info = GetConsoleScreenBufferInfo(
                console_handle,
                &screen_buffer_info
                );
        assert (got_buffer_info);

        auto screen_height = screen_buffer_info.dwSize.Y;
        conversation_window_height = static_cast<uint16_t>(screen_height * c_proportional_size_of_conversation_window);
        max_input_lines = (screen_height - conversation_window_height) - 1;

        return std::thread(draw_proc);
    }

    void write_to_input_field(char c)
    {
        (void)c;
    }

    void clear_input_field(void)
    {
        static char flip_char = 'a';
        SetConsoleCursorPosition(console_handle, COORD {0, static_cast<int16_t>(conversation_window_height)});
        for (uint32_t cursor = 0; cursor < max_input_lines * screen_buffer_info.dwSize.X; ++cursor)
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

    void draw_proc()
    {
        while (globals::application_running)
        {
            printf("draw call\n");
            std::this_thread::sleep_for(c_sleep_time_between_draw_calls);
        }
    }
}
