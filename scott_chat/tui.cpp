#include "stdafx.h"

#include "tui.h"

#include <chrono>
#include <mutex>
#include <thread>

constexpr COORD c_console_origin {0, 0};
constexpr std::chrono::milliseconds c_sleep_time_between_draw_calls = std::chrono::milliseconds(1000/30);

namespace tui
{
    static void draw_proc();
    static void reserve_buffers();
    static void update_divider(uint32_t line_width);
    static void create_screen_buffers();
    static void update_screen_buffer_info_if_necessary();
    static void clear_screen();
    static uint16_t input_window_height();
    static uint16_t conversation_window_height();

    static std::mutex screen_buffer_mutex;

    static HANDLE display_console;
    static HANDLE write_buffer_console;
    static CONSOLE_SCREEN_BUFFER_INFO screen_buffer_info = {0};

    static std::vector<char> conversation_buffer;
    static uint32_t conversation_offset = 0;

    static std::vector<char> input_buffer;
    static uint32_t input_offset = 0;

    static const char* divider = nullptr;

    std::thread init()
    {
        create_screen_buffers();
        reserve_buffers();
        update_divider(screen_buffer_info.dwSize.X);
        SetConsoleActiveScreenBuffer(display_console);
        return std::thread(draw_proc);
    }

    void create_screen_buffers()
    {
        display_console = CreateConsoleScreenBuffer(
                GENERIC_WRITE | GENERIC_READ,
                FILE_SHARE_WRITE | FILE_SHARE_READ,
                nullptr,
                CONSOLE_TEXTMODE_BUFFER,
                nullptr);
        assert (display_console != INVALID_HANDLE_VALUE);

        write_buffer_console = CreateConsoleScreenBuffer(
                GENERIC_WRITE | GENERIC_READ,
                FILE_SHARE_WRITE | FILE_SHARE_READ,
                nullptr,
                CONSOLE_TEXTMODE_BUFFER,
                nullptr);
        assert (write_buffer_console != INVALID_HANDLE_VALUE);
    }

    static bool rect_eq(SMALL_RECT* w1, SMALL_RECT* w2)
    {
        auto wp1 = reinterpret_cast<uint64_t*>(w1);
        auto wp2 = reinterpret_cast<uint64_t*>(w2);
        return *wp1 == *wp2;
    }

    void update_screen_buffer_info_if_necessary()
    {
        CONSOLE_SCREEN_BUFFER_INFO buffer_info;
        auto got_buffer_info = GetConsoleScreenBufferInfo(
                display_console,
                &buffer_info);
        assert (got_buffer_info);

        if (! rect_eq(&buffer_info.srWindow, &screen_buffer_info.srWindow))
        {
            screen_buffer_info = buffer_info;
            CloseHandle(write_buffer_console);
            write_buffer_console = CreateConsoleScreenBuffer(
                    GENERIC_WRITE | GENERIC_READ,
                    FILE_SHARE_WRITE | FILE_SHARE_READ,
                    nullptr,
                    CONSOLE_TEXTMODE_BUFFER,
                    nullptr);
            assert (write_buffer_console != INVALID_HANDLE_VALUE);
            update_divider(screen_buffer_info.dwSize.X);
        }
    }

    void push_to_input_field(char c)
    {
        std::lock_guard<std::mutex> screen_buffer_guard(screen_buffer_mutex);
        input_buffer.push_back(c);
    }

    void pop_from_input_field()
    {
        std::lock_guard<std::mutex> screen_buffer_guard(screen_buffer_mutex);
        input_buffer.pop_back();
    }

    void clear_input()
    {
        std::lock_guard<std::mutex> screen_buffer_guard(screen_buffer_mutex);
        input_buffer.clear();
    }

    void write_msg_to_conversation_thread(const char* msg)
    {
        std::lock_guard<std::mutex> screen_buffer_guard(screen_buffer_mutex);
        while (*msg)
        {
            conversation_buffer.push_back(*msg++);
        }
        conversation_buffer.push_back('\0');
    }

    void draw_proc()
    {
        std::unique_lock<std::mutex> screen_buffer_guard(screen_buffer_mutex, std::defer_lock);
        while (globals::application_running)
        {
            update_screen_buffer_info_if_necessary();
            screen_buffer_guard.lock();

            auto cursor_pos = c_console_origin;
            auto screen_width = screen_buffer_info.dwSize.X;
            uint32_t conversation_chars_left = conversation_window_height() * screen_width;
            char* msg_ptr = conversation_buffer.data();
            char* msgs_end = msg_ptr + conversation_buffer.size();
            while (msg_ptr < msgs_end && conversation_chars_left > 0)
            {
                SetConsoleCursorPosition(write_buffer_console, cursor_pos);

                uint32_t next_string_size = strlen(msg_ptr);
                uint32_t chars_to_write = next_string_size < conversation_chars_left ? next_string_size : conversation_chars_left;
                uint32_t lines_to_write = (chars_to_write / screen_width) + 1;

                WriteFile(write_buffer_console, msg_ptr, chars_to_write, nullptr, nullptr);

                conversation_chars_left -= chars_to_write;
                msg_ptr += next_string_size + 1;
                cursor_pos.Y += lines_to_write;
            }
            cursor_pos.Y = conversation_window_height();
            SetConsoleCursorPosition(write_buffer_console, cursor_pos);
            WriteFile(write_buffer_console, divider, screen_buffer_info.dwSize.X, nullptr, nullptr);
            WriteFile(write_buffer_console, input_buffer.data(), input_buffer.size(), nullptr, nullptr);

            screen_buffer_guard.unlock();

            std::swap(display_console, write_buffer_console);
            SetConsoleActiveScreenBuffer(display_console);
            clear_screen();

            std::this_thread::sleep_for(c_sleep_time_between_draw_calls);
        }
    }

    void update_divider(uint32_t line_width)
    {
        // +1 to account for null term
        auto divider_buffer = new char[line_width + 1];
        for(int i = line_width - 1; i >= 0; --i)
        {
            divider_buffer[i] = '-';
        }
        divider_buffer[line_width] = '\0';
        if (divider)
        {
            delete[] divider;
        }
        divider = divider_buffer;
    }

    void reserve_buffers()
    {
        auto screen_width = screen_buffer_info.dwSize.X;
        conversation_buffer.reserve(conversation_window_height() * screen_width);
        input_buffer.reserve(input_window_height() * screen_width);
    }

    void clear_screen()
    {
        auto screen_size = screen_buffer_info.dwSize.X * screen_buffer_info.dwSize.Y;
        DWORD chars_read;
        FillConsoleOutputCharacter(write_buffer_console, ' ', screen_size, c_console_origin, &chars_read);
    }

    uint16_t input_window_height()
    {
        return 2;
    }

    uint16_t conversation_window_height()
    {
        // 1 line for the scroll buffer, 1 line for the divider, 2 line for the input field
        return screen_buffer_info.dwSize.Y - 2 - input_window_height();
    }

}
