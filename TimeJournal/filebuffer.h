#pragma once

#include <mutex>
#include <string>
#include <vector>

class FileBuffer
{
public:
    FileBuffer(
        const std::string& fileName
        );

    ~FileBuffer();

    // Not safe to copy
    FileBuffer(const FileBuffer&) = delete;
    FileBuffer& operator=(const FileBuffer&) = delete;

    void
    WriteLine(
        const char* line
        );

    void
    WriteLineF(
        const char* lineFmt,
        ...
        );

private:
    void
    Flush();

    const std::string m_fileName;
    // NOTE: std::mutex is a heavy synchronization primitive, but until tracing becomes a meaningful perf bottleneck, I won't bother with something more complicated.
    // SRWLocks could be a good idea
    std::mutex m_bufferMutex;
    std::vector<std::string> m_buffer;
};
