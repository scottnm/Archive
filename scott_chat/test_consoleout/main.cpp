#define WIN32_LEAN_AND_MEAN
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "kernel32.lib")
#include <windows.h>
#include <stdio.h>

#include <assert.h>

int main()
{
    // auto console_handle = CreateFile(
            // CONOUT$, /* lpFileName */
            // GENERIC_WRITE, /* dwDesiredAccess */
            // FILE_SHARE_WRITE, /* dwShareMode */
            // nullptr, /* lpSecurityAttributes: opt and I wasn't doing anything but defaults */
            // OPEN_EXISTING, /* dwCreationDisposition */
            // FILE_ATTRIBUTE_HIDDEN, /* dwFlagsAndAttributes */
            // nullptr /* hTemplateFile: ignored for existing files */
            // );
    auto console_handle = CreateConsoleScreenBuffer(
            GENERIC_WRITE,
            FILE_SHARE_WRITE,
            nullptr,
            CONSOLE_TEXTMODE_BUFFER,
            nullptr
            );

    assert (console_handle != INVALID_HANDLE_VALUE);
}
