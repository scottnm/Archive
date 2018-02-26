#include "pch.h"
#include "email.h"

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <streambuf>

static const std::string s_editorPath = "C:\\Program Files (x86)\\Vim\\vim80\\vim.exe";
static const std::string s_tmpFilepath = std::string(getenv("TEMP")) + std::string("\\time_pest_temp.txt");

static
char*
GenerateEditorCmd()
{
    static const std::string s_editorCmdString = s_tmpFilepath + std::string(" ") + s_editorPath;
    static const uint32_t s_editorCmdBufferSize = s_editorCmdString.size() + 1;
    char* editorCmd = new char[s_editorCmdBufferSize];
    ZeroMemory(editorCmd, sizeof(*editorCmd) * s_editorCmdBufferSize);
    CopyMemory(editorCmd, s_editorCmdString.data(), s_editorCmdString.size());

    return editorCmd;
}

static char* s_editorCmd = GenerateEditorCmd();


static
void
AppendDataToLog(std::string data)
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
std::string
GetTmpFileContents(std::string tmpFile)
{
    std::ifstream tmpFileStream(tmpFile.c_str());
    std::string contents((std::istreambuf_iterator<char>(tmpFileStream)), std::istreambuf_iterator<char>());
    return contents;
}

static
void
ClearTmpFileContents(std::string tmpFile)
{
    static const std::string s_clearCmd("type nul > ");

    std::string cmd = s_clearCmd + tmpFile;
    std::system(cmd.c_str());
}

int
main()
{
    email::init();

    // open up text editor with a tmp file name
    startup_info_t startupInfo{0};
    process_information_t processInfo{0};
    auto createProcessResult = CreateProcess(
        nullptr,
        s_editorCmd,
		nullptr,
		nullptr,
		static_cast<BOOL>(false),
		0,
		nullptr,
        nullptr,
        &startupInfo,
        &processInfo);
    (void)createProcessResult;

    // wait for text editor to quit
    auto waitResult = WaitForSingleObject(processInfo.hProcess, INFINITE);
    (void)waitResult;

    // take contents of tmp file and write it to master log file
    auto contents = GetTmpFileContents(s_tmpFilepath);
    AppendDataToLog(contents);

    // free resources
    ClearTmpFileContents(s_tmpFilepath);
    CloseHandle(processInfo.hProcess);
    CloseHandle(processInfo.hThread);

    email::send_mail();
    email::cleanup();

    // quit
    return 0;
}
