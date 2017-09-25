#define WIN32_LEAN_AND_MEAN
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "kernel32.lib")
#include <windows.h>
#include <stdio.h>

#include <assert.h>

int main()
{
    auto console_handle = CreateConsoleScreenBuffer(
            GENERIC_WRITE,
            FILE_SHARE_WRITE,
            nullptr,
            CONSOLE_TEXTMODE_BUFFER,
            nullptr
            );
    assert (console_handle != INVALID_HANDLE_VALUE);

    SetConsoleActiveScreenBuffer(
            console_handle
            );

    CONSOLE_SCREEN_BUFFER_INFO screen_buffer_info;
    GetConsoleScreenBufferInfo(
            console_handle,
            &screen_buffer_info
            );

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
}
