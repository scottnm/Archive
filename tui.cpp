#include "stdafx.h"

#include "tui.h"
#include <cassert>
#include <cstdint>
#include <chrono>
#include <mutex>
#include <thread>

constexpr COORD c_console_origin {0, 0};
constexpr std::chrono::milliseconds c_sleep_time_between_draw_calls = std::chrono::milliseconds(1000/30);

namespace tui
{
    /*
    static int push_cnt = 0;
    static int pop_cnt = 0;
    static int clear_cnt = 0;
    static int write_cnt = 0;
    */




    static void draw_proc();
    static void reserve_buffers();
    static void update_divider(uint32_t line_width);
    static void clear_screen();

    static std::mutex screen_buffer_mutex;

    static HANDLE console_handle;
    static CONSOLE_SCREEN_BUFFER_INFO screen_buffer_info;

    static std::vector<char> conversation_buffer;
    static uint32_t conversation_offset = 0;

    static std::vector<char> input_buffer;
    static uint32_t input_offset = 0;

    static uint16_t conversation_window_height;
    static uint16_t input_window_height;

    static const char* divider = nullptr;

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
        update_divider(screen_buffer_info.dwSize.X);

        auto screen_height = screen_buffer_info.dwSize.Y;
        // 1 line for the scroll buffer, 1 line for the divider, 1 line for the input field
        conversation_window_height = static_cast<uint16_t>(screen_height - 3);
        input_window_height = 1;

        reserve_buffers();

        return std::thread(draw_proc);
    }

    void push_to_input_field(char c)
    {
        std::lock_guard<std::mutex> screen_buffer_guard(screen_buffer_mutex);
        //++push_cnt;
        input_buffer.push_back(c);
    }

    void pop_from_input_field()
    {
        std::lock_guard<std::mutex> screen_buffer_guard(screen_buffer_mutex);
        //++pop_cnt;
        input_buffer.pop_back();
    }

    void clear_input()
    {
        std::lock_guard<std::mutex> screen_buffer_guard(screen_buffer_mutex);
        //++clear_cnt;
        input_buffer.clear();
    }

    void write_msg_to_conversation_thread(const char* msg)
    {
        std::lock_guard<std::mutex> screen_buffer_guard(screen_buffer_mutex);
        //++write_cnt;
        while (*msg)
        {
            conversation_buffer.push_back(*msg++);
        }
        conversation_buffer.push_back('\0');
    }

    void draw_proc()
    {
        uint64_t i = 0;
        std::unique_lock<std::mutex> screen_buffer_guard(screen_buffer_mutex, std::defer_lock);
        while (globals::application_running)
        {
            screen_buffer_guard.lock();
            clear_screen();

            auto cursor_pos = c_console_origin;
            auto screen_width = screen_buffer_info.dwSize.X;
            uint32_t conversation_chars_left = conversation_window_height * screen_width;
            char* msg_ptr = conversation_buffer.data();
            char* msgs_end = msg_ptr + conversation_buffer.size();
            while (msg_ptr < msgs_end && conversation_chars_left > 0 )
            {
                SetConsoleCursorPosition(console_handle, cursor_pos);

                uint32_t next_string_size = strlen(msg_ptr);
                uint32_t chars_to_write = next_string_size < conversation_chars_left ? next_string_size : conversation_chars_left;
                uint32_t lines_to_write = (chars_to_write / screen_width) + 1;

                WriteFile(console_handle, msg_ptr, chars_to_write, nullptr, nullptr);

                conversation_chars_left -= chars_to_write;
                msg_ptr += next_string_size + 1;
                cursor_pos.Y += lines_to_write;
            }
            cursor_pos.Y = conversation_window_height;
            SetConsoleCursorPosition(console_handle, cursor_pos);
            WriteFile(console_handle, divider, screen_buffer_info.dwSize.X, nullptr, nullptr);
            WriteFile(console_handle, input_buffer.data(), input_buffer.size(), nullptr, nullptr);

            screen_buffer_guard.unlock();

            SetConsoleWindowInfo(console_handle, TRUE, &screen_buffer_info.srWindow);
            std::this_thread::sleep_for(c_sleep_time_between_draw_calls);
        }
        /*
        printf("push: %u\tpop: %u\tclear: %u\twrite: %u\n",
                push_cnt, pop_cnt, clear_cnt, write_cnt);
        for (char c : conversation_buffer)
        {
            printf("c: %c\n", c);
        }
        for (char i : input_buffer)
        {
            printf("i: %c\n", i);
        }
        */
    }

    void update_divider(uint32_t line_width)
    {
        // +1 to account for null term
        auto divider_buffer = static_cast<char*>(malloc((line_width + 1) * sizeof(*divider)));
        for(int i = line_width - 1; i >= 0; --i)
        {
            divider_buffer[i] = '-';
        }
        divider_buffer[line_width] = '\0';
        divider = divider_buffer;
    }

    void reserve_buffers()
    {
        auto screen_width = screen_buffer_info.dwSize.X;
        conversation_buffer.reserve(conversation_window_height * screen_width);
        input_buffer.reserve(input_window_height * screen_width);
    }

	void clear_screen()
	{
	   auto screen_size = screen_buffer_info.dwSize.X * screen_buffer_info.dwSize.Y;
       DWORD chars_read;
	   FillConsoleOutputCharacter(console_handle, ' ', screen_size, c_console_origin, &chars_read);
	}
}
