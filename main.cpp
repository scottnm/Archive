#include <cstdio>
#include <cstdlib>
#include <string>
#include <iostream>
#include <fstream>
#include <streambuf>
#include <windows.h>
#pragma comment (lib, "kernel32.lib")

static const std::string editor_path = "C:\\Program Files (x86)\\Vim\\vim80\\vim.exe";

static
void
append_data_to_log(std::string data)
{
    static const std::string home_dir = getenv("HOME");
    static const std::string log_filename = "\\Documents\\time_pest_log.txt";

    auto log_filepath_length = home_dir.size() + log_filename.size() + 1;
    char* log_file = new char[log_filepath_length];
    strncat(log_file, home_dir.c_str(), home_dir.size());
    strncat(log_file, log_filename.c_str(), log_filename.size());

    std::ofstream outfile;
    outfile.open(log_file, std::ios_base::app);
    outfile << data.c_str();

    delete[] log_file;
}

static
char*
get_tmp_filepath()
{
    static const std::string temp_dir = getenv("TEMP");
    static const std::string temp_filename = "\\time_pest_temp.txt";

    char* path = new char[temp_dir.size() + temp_filename.size()+ 1];
    strncat(path, temp_dir.c_str(), temp_dir.size());
    strncat(path, temp_filename.c_str(), temp_filename.size());

    return path;
}

static
char*
get_editor_cmd()
{
    auto fp = get_tmp_filepath();
    std::string filepath = fp;
    delete[] fp;
    char* cmd = new char[editor_path.size() + 1 + filepath.size() + 1];
    strncat(cmd, editor_path.c_str(), editor_path.size());
    strncat(cmd, " ", 1);
    strncat(cmd, filepath.c_str(), filepath.size());

    return cmd;
}

static
std::string
get_tmp_file_contents(std::string tmp_file)
{
    std::ifstream t(tmp_file.c_str());
    std::string contents((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
    return contents;
}

static
void
clear_tmp_file_contents(std::string tmp_file)
{
    static const std::string clear_cmd = "type nul > ";
    char* cmd = new char[clear_cmd.size() + tmp_file.size() + 1];
    strncat(cmd, clear_cmd.c_str(), clear_cmd.size());
    strncat(cmd, tmp_file.c_str(), tmp_file.size());
    std::system(cmd);
    delete[] cmd;
}

int
main()
{
    // open up notepad.exe with a tmp file name
    STARTUPINFO startup_info = {0};
    PROCESS_INFORMATION proc_info = {0};

    auto cmd = get_editor_cmd();
    auto proc_ret = CreateProcess(
        nullptr,
        cmd,
		nullptr,
		nullptr,
		static_cast<BOOL>(false),
		0,
		nullptr,
        nullptr,
        &startup_info,
        &proc_info);
    (void)proc_ret;

    // wait for notepad.exe to quit
    auto ret = WaitForSingleObject(proc_info.hProcess, INFINITE /* swap out for timeout? */);
    (void)ret;

    // take contents of tmp file and write it to master log file
    auto contents = get_tmp_file_contents(get_tmp_filepath());
    clear_tmp_file_contents(get_tmp_filepath());
    append_data_to_log(contents);

    // free resources
    CloseHandle(proc_info.hProcess);
    CloseHandle(proc_info.hThread);
    delete[] cmd;

    // quit
    return 0;
}
