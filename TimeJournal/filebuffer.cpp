#include "filebuffer.h"

#include <assert.h>
#include <stdarg.h>
#include <stdlib.h>

#include "utils.h"

FileBuffer::FileBuffer(
    const std::string& fileName
    ) :
    m_fileName(fileName)
{
}

FileBuffer::~FileBuffer()
{
    Flush();
}

void
FileBuffer::WriteLine(
    const char* line
    )
{
    std::lock_guard<std::mutex> m_bufferGuard(m_bufferMutex);
    m_buffer.push_back(line);
}

void
FileBuffer::WriteLineF(
    const char* lineFmt,
    ...
    )
{
    // TODO: return errors via some result helper
    char lineBuffer[2048];
    {
        va_list lineArgs;
        va_start(lineArgs, lineFmt);
        int charactersWritten = vsprintf_s(
            lineBuffer,
            CountOf(lineBuffer),
            lineFmt,
            lineArgs);
        va_end(lineArgs);

        assert(charactersWritten > 0);
    }
    WriteLine(lineBuffer);
}

void
FileBuffer::Flush()
{
    FILE* file;
    errno_t openError = fopen_s(&file, m_fileName.c_str(), "a");
    assert(openError == 0);

    {
        std::lock_guard<std::mutex> m_bufferGuard(m_bufferMutex);
        for (const std::string& line : m_buffer)
        {
            int charactersWritten = fprintf_s(file, "%s\n", line.c_str());
            assert(charactersWritten == line.size() + 1); // +1 for newline
        }
        m_buffer.clear();
    }

    // NOTE: explicitly calling fclose is pretty lame. Consider pulling in WIL for helpful RAII wrappers
    fclose(file);
}
