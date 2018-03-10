#include "pch.h"

// C++ includes
#include <iostream>
#include <fstream>
#include <streambuf>

static const std::string s_tmpFilepath = std::string(getenv("TEMP")) + std::string("\\time_pest_temp.txt");

static
PROCESS_INFORMATION
CreateEditorProcess(
    _In_ const std::string& fileToEdit
    )
{
    // Need a non-const version of the editor string because CreateProcess takes a non-const char*
    static const std::string s_editorCmdString = std::string("C:\\Program Files (x86)\\Vim\\vim80\\vim.exe ") + fileToEdit;
    char* editorCmd = new char[s_editorCmdString.size() + 1];
    strcpy(editorCmd, s_editorCmdString.c_str());

    STARTUPINFO startupInfo = {0};
    PROCESS_INFORMATION processInfo = {0};
    auto createProcessResult = CreateProcess(
        nullptr,
        editorCmd,
		nullptr,
		nullptr,
		static_cast<BOOL>(false),
		0,
		nullptr,
        nullptr,
        &startupInfo,
        &processInfo);
    assert(createProcessResult != 0);

    delete[] editorCmd;

    return processInfo;
}

static
void
AppendDataToLog(std::string data)
{
    static const std::string homeDir = getenv("HOME");
    static const std::string logFilename = "\\Documents\\time_pest_log.txt";
    static const std::string logFilepath = homeDir + logFilename;

    std::ofstream outFile;
    outFile.open(logFilepath.c_str(), std::ios_base::app);
    outFile << data.c_str();
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
    // open up text editor with a tmp file name
    auto processInfo = CreateEditorProcess(s_tmpFilepath);

    // wait for text editor to quit
    auto waitResult = WaitForSingleObject(processInfo.hProcess, INFINITE);
    assert(waitResult == WAIT_OBJECT_0);

    // take contents of tmp file and write it to master log file
    auto contents = GetTmpFileContents(s_tmpFilepath);
    AppendDataToLog(contents);

    // free resources
    ClearTmpFileContents(s_tmpFilepath);
    CloseHandle(processInfo.hProcess);
    CloseHandle(processInfo.hThread);

    // quit
    return 0;
}
