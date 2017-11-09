#include <cstdio>
#include <cstdlib>
#include <windows.h>
#pragma comment (lib, "kernel32.lib")

static
const char*
get_temp_filepath()
{
    static auto temp_filename = "\\time_pest_temp.txt";
    static auto temp_filename_len = strlen(temp_filename);
    auto temp_dir = getenv("TEMP");
    auto temp_dir_len = strlen(temp_dir);

    char* path = new char[temp_dir_len + temp_filename_len + 1];
    memcpy_s(path, temp_dir_len + temp_filename_len, temp_dir, temp_dir_len);
    memcpy_s(path + temp_dir_len, temp_filename_len, temp_filename, temp_filename_len);
    path[temp_dir_len + temp_filename_len] = '\0';

    return path;
}

int
main()
{
    // open up notepad.exe with a tmp file name
    STARTUPINFO startup_info = {0};
    PROCESS_INFORMATION proc_info = {0};
    CreateProcess(
		"c:\\windows\\system32\\notepad.exe",
        nullptr,
		nullptr,
		nullptr,
		static_cast<BOOL>(false),
		0,
		nullptr,
        nullptr,
        &startup_info,
        &proc_info);

    auto ret = WaitForSingleObject(proc_info.hProcess, INFINITE /* swap out for timeout? */);
    (void)ret;

    auto temp_path = get_temp_filepath();
    printf("%s\n", temp_path);
/*
BOOL WINAPI CreateProcess(
  _In_opt_    LPCTSTR               lpApplicationName,
  _Inout_opt_ LPTSTR                lpCommandLine,
  _In_opt_    LPSECURITY_ATTRIBUTES lpProcessAttributes,
  _In_opt_    LPSECURITY_ATTRIBUTES lpThreadAttributes,
  _In_        BOOL                  bInheritHandles,
  _In_        DWORD                 dwCreationFlags,
  _In_opt_    LPVOID                lpEnvironment,
  _In_opt_    LPCTSTR               lpCurrentDirectory,
  _In_        LPSTARTUPINFO         lpStartupInfo,
  _Out_       LPPROCESS_INFORMATION lpProcessInformation
);
*/

    // wait for notepad.exe to quit
    // take contents of tmp file and write it to master log file

    // free resources
    CloseHandle(proc_info.hProcess);
    CloseHandle(proc_info.hThread);
    delete[] temp_path;

    // quit
    return 0;
}
