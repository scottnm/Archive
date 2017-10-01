#include "stdafx.h"

#include "tui.h"
#include <assert.h>
#include <stdint.h>

#define INPUT_FIELD_RATIO 0.2

namespace tui
{
    static void debug_scbuf_info(void);

    static HANDLE console_handle;
    static CONSOLE_SCREEN_BUFFER_INFO screen_buffer_info;
    static int16_t input_field_y_offset;

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

        SetConsoleActiveScreenBuffer(
                console_handle
                );

        input_field_y_offset = static_cast<int16_t>(screen_buffer_info.dwSize.Y * (1 - INPUT_FIELD_RATIO));

        /*
        char cycle_char = 'a';
        for (SHORT row = screen_buffer_info.dwSize.Y - 1; row >= 0; --row)
        {
            for (SHORT col = screen_buffer_info.dwSize.X - 1; col >= 0; --col)
            {
                BOOL success;
                success = SetConsoleCursorPosition(
                        console_handle,
                        COORD {col, row}
                        );
                assert (success);
                success = WriteFile(
                        console_handle,
                        &cycle_char,
                        1,
                        nullptr,
                        nullptr
                        );
                assert (success);
                cycle_char = ((cycle_char - 'a' + 1) % 26) + 'a';
            }
        }

        Sleep(2000);
        */
    }

    void write_to_input_field(char c)
    {
        (void)c;
    }

    void clear_input_field(void)
    {
        static char flip_char = 'a';
        uint32_t input_field_size = screen_buffer_info.dwSize.X * (screen_buffer_info.dwSize.Y - input_field_y_offset);
        printf("input_field_size = %d\ninput_field_y_offset = %d\n\n", input_field_size, input_field_y_offset);
        SetConsoleCursorPosition(console_handle, COORD {0, input_field_y_offset});
        debug_scbuf_info();
        for (uint32_t cursor = 0; cursor < input_field_size; ++cursor)
        {
            WriteFile(console_handle, &flip_char, 1, nullptr, nullptr);
        }
        flip_char = flip_char == 'a' ? ' ' : 'a';
        SetConsoleWindowInfo(console_handle, TRUE, &screen_buffer_info.srWindow);
        Beep(523,500);
    }

    void write_msg_to_conversation_thread(const char* msg)
    {
        (void)msg;
    }

    void debug_scbuf_info(void)
    {
		auto dwSize = screen_buffer_info.dwSize;
		auto dwCursorPosition = screen_buffer_info.dwCursorPosition;
		auto wAttributes = screen_buffer_info.wAttributes;
		auto srWindow = screen_buffer_info.srWindow;
		auto dwMaximumWindowSize = screen_buffer_info.dwMaximumWindowSize;
		printf(
			"---------DEBUG-----------\n"
			"dwSize: %d, %d\n"
			"dwCursorPosition: %d, %d\n"
			"wAttributes: %d\n"
			"srWindow: L_%d, T_%d, R_%d, B_%d\n"
			"dwMaximumWindowSize: %d, %d\n"
			"input_field_y_offset: %d\n",
			dwSize.X, dwSize.Y,
			dwCursorPosition.X, dwCursorPosition.Y,
			wAttributes,
			srWindow.Left, srWindow.Top, srWindow.Right, srWindow.Bottom,
			dwMaximumWindowSize.X, dwMaximumWindowSize.Y,
            input_field_y_offset
		);
    }
}
