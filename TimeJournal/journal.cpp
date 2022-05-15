// Platform headers
#define NOMINMAX
#include <windows.h>
#include <Threadpoolapiset.h>

// STL headers
#include <string>
#include <string_view>

// stdlib headers
#include <atomic>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <assert.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>

// external headers
#include <gsl/span>
#include <nlohmann/json.hpp>
#include <pix3.h>

// app headers
#include "utils.h"
#include "filebuffer.h"

static const uint64_t PROFILE_MARKER_COLOR = PIX_COLOR(235, 52, 88);
#define PROFILE_FUNCTION() PIXScopedEvent(PROFILE_MARKER_COLOR, "J: %s", __FUNCTION__)

static
SYSTEMTIME
LocalTime()
{
    PROFILE_FUNCTION();

    SYSTEMTIME localTime;
    GetLocalTime(&localTime);
    return localTime;
}

static
SYSTEMTIME
FileTimeToSystemTime(
    const FILETIME& fileTime
    )
{
    PROFILE_FUNCTION();

    SYSTEMTIME systemTime;
    bool success = FileTimeToSystemTime(&fileTime, &systemTime);
    assert(success);
    return systemTime;
}

static
FILETIME
SystemTimeToFileTime(
    const SYSTEMTIME& systemTime
    )
{
    FILETIME fileTime;
    bool success = SystemTimeToFileTime(&systemTime, &fileTime);
    assert(success);
    return fileTime;
}

static
FILETIME
LocalToUtcFileTime(
    const FILETIME& local
    )
{
    PROFILE_FUNCTION();

    FILETIME utc;
    bool success = LocalFileTimeToFileTime(&local, &utc);
    assert(success);
    return utc;
}

static
FILETIME
OffsetFileTime(
    FILETIME ft,
    uint64_t offset
    )
{
    PROFILE_FUNCTION();

    ULARGE_INTEGER ul;
    ul.u.LowPart = ft.dwLowDateTime;
    ul.u.HighPart = ft.dwHighDateTime;

    ul.QuadPart += offset;

    FILETIME result;
    result.dwLowDateTime = ul.u.LowPart;
    result.dwHighDateTime = ul.u.HighPart;
    return result;
}

static
uint64_t
MillisecondsToFileTimeOffset(
    uint64_t milliseconds
    )
{
    PROFILE_FUNCTION();

    uint64_t microseconds = milliseconds * 1000;
    uint64_t hundredNanoSecondChunks = microseconds * 10;
    return hundredNanoSecondChunks;
}

static
FILETIME
MaxFileTime(
    const FILETIME& ft1,
    const FILETIME& ft2
    )
{
    PROFILE_FUNCTION();

    return CompareFileTime(&ft1, &ft2) > 0 ? ft1 : ft2;
}


template<typename TString>
const char*
ToCStr(
    const TString& s
    )
{
    PROFILE_FUNCTION();

    return s;
}

template<>
const char*
ToCStr<std::string>(
    const std::string& s
    )
{
    PROFILE_FUNCTION();

    return s.c_str();
}

template<typename TString1, typename TString2>
static
bool
strieq(
    const TString1& s1,
    const TString2& s2
    )
{
    PROFILE_FUNCTION();

    return _stricmp(ToCStr(s1), ToCStr(s2)) == 0;
}


static
std::string
GetAppDirectory()
{
    PROFILE_FUNCTION();

    char pathBuffer[FILENAME_MAX];
    size_t bytesWritten;
    errno_t error = getenv_s(&bytesWritten, pathBuffer, "LOCALAPPDATA");
    assert(error == 0);
    strcpy_s(&pathBuffer[bytesWritten - 1], CountOf(pathBuffer) - bytesWritten, "\\TimeJournal\\");
    return pathBuffer;
}

static const std::string APP_DIR = GetAppDirectory();
static const std::string TRACE_FILE = GetAppDirectory() + "trace.txt";

static
std::string
ReadFileToString(
    const char* filePath
    )
{
    PROFILE_FUNCTION();

    // TODO: return errors via some result helper
    FILE* file;
    errno_t openError = fopen_s(&file, filePath, "rb");
    assert(openError == 0);

    fseek(file, 0, SEEK_END);
    size_t fileSize = ftell(file);
    rewind(file);

    std::string contents;
    contents.resize(fileSize);

    fread(&contents[0], 1, contents.size(), file);
    fclose(file);

    return contents;
}

static
std::string
ReadFileToString(
    const std::string& filePath
    )
{
    PROFILE_FUNCTION();

    return ReadFileToString(filePath.c_str());
}

constexpr size_t DATETIME_STRING_BUFSIZE = CountOf("Sat May 02 20:12:43.000 2020");

static
void
GetDateTimeString(
    const SYSTEMTIME& time,
    char (&timeBuffer)[DATETIME_STRING_BUFSIZE]
    )
{
    PROFILE_FUNCTION();

    constexpr const char* months[] =
    {
        "January",
        "February",
        "March",
        "April",
        "May",
        "June",
        "July",
        "August",
        "September",
        "October",
        "November",
        "December",
    };

    constexpr const char* daysOfWeek[] =
    {
        "Monday",
        "Tuesday",
        "Wednesday",
        "Thursday",
        "Friday",
        "Saturday",
        "Sunday",
    };

    int printResult = snprintf(timeBuffer, CountOf(timeBuffer), "%.3s %.3s %02u %02u:%02u:%02u.%03u %04u",
        daysOfWeek[time.wDayOfWeek],
        months[time.wMonth - 1], // month value reported by windows is 1-based
        time.wDay,
        time.wHour,
        time.wMinute,
        time.wSecond,
        time.wMilliseconds,
        time.wYear);
    assert(printResult == CountOf(timeBuffer) - 1);
}

static
void
WriteTrace(
    const char* format,
    ...
    )
{
    PROFILE_FUNCTION();

    char timeBuffer[DATETIME_STRING_BUFSIZE];
    GetDateTimeString(LocalTime(), timeBuffer);

    char logBuffer[1024];
    {
        va_list args;
        va_start(args, format);
        errno_t printResult = vsprintf_s(logBuffer, CountOf(logBuffer), format, args);
        assert(printResult > 0);
        va_end(args);
    }

    static FileBuffer s_traceFile(TRACE_FILE);
    s_traceFile.WriteLineF("%s: %s", timeBuffer, logBuffer);
}

struct CmdlineArgs
{
    std::string configFilePath;
    std::string logFilePath;
};

static
CmdlineArgs
ParseArgs(
    int argc,
    char** argv
    )
{
    PROFILE_FUNCTION();

    gsl::span<char*> args { argv, static_cast<size_t>(argc) };
    // TODO: provide default args for non-test scenarios
    std::string logFilePath;
    if (args.size() > 2)
    {
        logFilePath = args[2];
    }

    std::string configFilePath;
    if (args.size() > 1)
    {
        configFilePath = args[1];
    }

    return CmdlineArgs { std::move(configFilePath), std::move(logFilePath) };
}

enum class TimePeriodUnit
{
    Unset       = 0,
    // TODO: support hours for real use
    N_Milliseconds = 'm', // for testing
};

// TODO: there's a lot of stuff thrown in this one file. I should fine a way to separate it out for better readability
class TimeOfDayIso8601
{
public:
    TimeOfDayIso8601(
        uint16_t hour,
        uint16_t min,
        uint16_t sec,
        uint16_t millis
        ) :
        m_hour(hour),
        m_min(min),
        m_sec(sec),
        m_millis(millis)
    {
        PROFILE_FUNCTION();

        assert(hour < 24);
        assert(min < 60);
        assert(sec < 60);
        assert(millis < 1000);
    }

    TimeOfDayIso8601() : TimeOfDayIso8601(0, 0, 0, 0) {}

    // TODO: setup a utest project by utilizing a shared items project
    // TODO: unit test this sucker
    static
    TimeOfDayIso8601
    FromString(
        std::string_view timeString
        )
    {
        PROFILE_FUNCTION();

        TimeOfDayIso8601 offset;
        std::string_view timeStringPlusOffset = timeString;
        if (timeStringPlusOffset[0] == '+')
        {
            timeString = timeStringPlusOffset.substr(1);

            const SYSTEMTIME localTime = LocalTime();
            offset = TimeOfDayIso8601(localTime.wHour, localTime.wMinute, localTime.wSecond, localTime.wMilliseconds);
        }
        else
        {
            // handle the non-offset case
            assert(false);
        }

        // TODO: error handling, look for other asserts
        uint16_t hour, min, sec, millis;
        int scannedItems = sscanf_s(timeString.data(), "%hu:%hu:%hu.%hu", &hour, &min, &sec, &millis);
        assert(scannedItems == 4);

        // TODO: runtime verify the hour, min, and sec params
        return TimeOfDayIso8601(hour, min, sec, millis).Offset(offset);
    }

    // TODO: write a unit-test for this sucker
    TimeOfDayIso8601
    Offset(
        const TimeOfDayIso8601& offset
        ) const
    {
        PROFILE_FUNCTION();

        uint16_t millis = m_millis + offset.m_millis;
        assert(millis < 2000);
        uint16_t secOverflow = millis / 1000;
        millis = millis % 1000;
        assert(secOverflow <= 1);

        uint16_t sec = m_sec + offset.m_sec + secOverflow;
        assert(sec < 120);
        uint16_t minOverflow = sec / 60;
        sec = sec % 60;
        assert(minOverflow <= 1);

        uint16_t min = m_min + offset.m_min + minOverflow;
        assert(min < 120);
        uint16_t hourOverflow = min / 60;
        assert(hourOverflow <= 1);
        min = min % 60;

        uint16_t hour = m_hour + offset.m_hour + hourOverflow;
        assert(hour < 48);
        uint64_t dayOverflow = hour / 24;
        assert(dayOverflow <= 1);
        hour = hour % 24;

        return TimeOfDayIso8601(hour, min, sec, millis);
    }

    TimeOfDayIso8601
    Align(
        uint16_t period,
        TimePeriodUnit unit
        ) const
    {
        uint16_t hour, min, sec, millis;
        switch (unit)
        {
            case TimePeriodUnit::N_Milliseconds:
            {
                assert(period % 2 == 0);
                const TimeOfDayIso8601 roundingOffset = Offset(TimeOfDayIso8601(0, 0, 0, period / 2));
                hour = roundingOffset.m_hour;
                min = roundingOffset.m_min;
                sec = roundingOffset.m_sec;
                // use integer division to round down to the next closest 100ms mark
                millis = (roundingOffset.m_millis / period) * period;
                break;
            }
            default:
            {
                // TODO: impl
                assert(false);
                break;
            }
        }

        return TimeOfDayIso8601(hour, min, sec, millis);
    }

    std::string
    ToString() const
    {
        PROFILE_FUNCTION();

        char buf[] = "hh:mm:ss:mil";
        errno_t err = snprintf(buf, CountOf(buf), "%02u:%02u:%02u.%03u", m_hour, m_min, m_sec, m_millis);
        assert(err > 0);

        return std::string("ToD: ") + buf;
    }

    SYSTEMTIME
    SystemTimeForToday() const
    {
        PROFILE_FUNCTION();

        SYSTEMTIME today = LocalTime();
        today.wHour = m_hour;
        today.wMinute = m_min;
        today.wSecond = m_sec;
        today.wMilliseconds = m_millis;
        return today;
    }

private:
    uint16_t m_hour; // [0,24)
    uint16_t m_min;  // [0,60)
    uint16_t m_sec;  // [0,60)
    uint16_t m_millis;  // [0,1000)
};

struct TimerConfig
{
    TimeOfDayIso8601 start;
    TimeOfDayIso8601 end;
    uint16_t period;
    TimePeriodUnit unit;
    bool cancelOnEnd;
};

static
TimerConfig
GetConfigFromFile(
    const std::string& configFilePath
    )
{
    PROFILE_FUNCTION();

    const std::string configFileContents = ReadFileToString(configFilePath);
    WriteTrace("Read config from file: \"%s\"", configFileContents.c_str());

    const nlohmann::json configJson = nlohmann::json::parse(configFileContents);
    const std::string startTimeString = configJson["startTimeOfDayIso8601"].get<std::string>();
    const std::string endTimeString = configJson["endTimeOfDayIso8601"].get<std::string>();

    TimeOfDayIso8601 startTime = TimeOfDayIso8601::FromString(startTimeString);
    TimeOfDayIso8601 endTime = TimeOfDayIso8601::FromString(endTimeString);
    uint16_t period = configJson["period"].get<uint16_t>();
    bool cancelOnEnd = configJson["cancelOnEnd"].get<bool>();
    TimePeriodUnit unit;
    {
        std::string unitString = configJson["unit"].get<std::string>();
        if (strieq(unitString, "millis"))
        {
            unit = TimePeriodUnit::N_Milliseconds;
        }
        else
        {
            assert(false);
            WriteTrace("Unexpected unit '%s'!", unitString.c_str());
            unit = TimePeriodUnit::Unset;
        }
    }
    TimeOfDayIso8601 startTimeAligned = startTime.Align(period, unit);
    TimeOfDayIso8601 endTimeAligned = endTime.Align(period, unit);

    WriteTrace("Parsed from json:");
    WriteTrace("  start: %s (aligned from %s)",
        startTimeAligned.ToString().c_str(),
        startTime.ToString().c_str());
    WriteTrace("  end: %s (aligned from %s)",
        endTimeAligned.ToString().c_str(),
        endTime.ToString().c_str());
    WriteTrace("  period: every %u %c", period, static_cast<char>(unit));

    return TimerConfig { startTimeAligned, endTimeAligned, period, unit, cancelOnEnd };
}

class PeriodicWorkController
{
public:
    PeriodicWorkController(
        const TimerConfig& timerConfig,
        std::function<void(void)> callback
        ) :
        m_timer(CreateThreadpoolTimer(StaticPeriodicWorkControllerCallback, this, nullptr)),
        m_timerConfig(timerConfig),
        m_callback(callback)
    {
        PROFILE_FUNCTION();

        SetNextTimer();
    }

    ~PeriodicWorkController()
    {
        PROFILE_FUNCTION();

        if (m_timer != nullptr)
        {
            // cancel any timers still running in the background
            SetThreadpoolTimer(m_timer, nullptr, 0, 0);
            CloseThreadpoolTimer(m_timer);
        }
    }

    void
    WaitForCancel()
    {
        PROFILE_FUNCTION();

        std::unique_lock<std::mutex> lock(m_cancelMutex);
        m_cancelCv.wait(lock, [this]() { return m_cancelSignal.exchange(false); });
        WriteTrace("Cancel completed");
    }

private:
    static
    void
    CALLBACK
    StaticPeriodicWorkControllerCallback(
        PTP_CALLBACK_INSTANCE,
        void* context,
        PTP_TIMER timer
        )
    {
        auto thisContext = static_cast<PeriodicWorkController*>(context);
        thisContext->FirePeriodicWorkControllerCallback();
        assert(timer == thisContext->m_timer);
    }

    void
    FirePeriodicWorkControllerCallback()
    {
        PROFILE_FUNCTION();

        WriteTrace("Firing callback");
        m_callback();

        if (HasEndTimeElapsed())
        {
            // TODO: in real builds, just delay until the next cycle
            if (m_timerConfig.cancelOnEnd)
            {
                WriteTrace("Cancelling...");
                m_cancelSignal = true;
                m_cancelCv.notify_one();
            }
            return;
        }

        SetNextTimer();
    }

    bool
    HasEndTimeElapsed() const
    {
        PROFILE_FUNCTION();

        const SYSTEMTIME localTime = LocalTime();
        const SYSTEMTIME localEndTime = m_timerConfig.end.SystemTimeForToday();
        const FILETIME ftLocalTime = SystemTimeToFileTime(localTime);
        const FILETIME ftLocalEndTime = SystemTimeToFileTime(localEndTime);
        return CompareFileTime(&ftLocalTime, &ftLocalEndTime) >= 0;
    }

    void
    SetNextTimer()
    {
        PROFILE_FUNCTION();

        FILETIME localDueTime;
        {
            const SYSTEMTIME localTime = LocalTime();
            const SYSTEMTIME localStartTime = m_timerConfig.start.SystemTimeForToday();

            const FILETIME ftLocalTime = SystemTimeToFileTime(localTime);
            const FILETIME ftLocalStartTime = SystemTimeToFileTime(localStartTime);
            const FILETIME localTimeToOffset = MaxFileTime(ftLocalTime, ftLocalStartTime);

            const SYSTEMTIME localSystemTimeToOffset = FileTimeToSystemTime(localTimeToOffset);
            switch (m_timerConfig.unit)
            {
                case TimePeriodUnit::N_Milliseconds:
                {
                    uint64_t millisecondssTillNextDueTime = m_timerConfig.period - (localSystemTimeToOffset.wMilliseconds % m_timerConfig.period);
                    uint64_t durationToWait = MillisecondsToFileTimeOffset(millisecondssTillNextDueTime);
                    localDueTime = OffsetFileTime(localTimeToOffset, durationToWait);
                    break;
                }
                default:
                {
                    // TODO: IMPL hourly
                    assert(false);
                    break;
                }
            }
        }

        char localDueTimeString[DATETIME_STRING_BUFSIZE];
        GetDateTimeString(FileTimeToSystemTime(localDueTime), localDueTimeString);
        WriteTrace("Setting timer for %s", localDueTimeString);

        FILETIME dueTime = LocalToUtcFileTime(localDueTime);
        SetThreadpoolTimer(m_timer, &dueTime, 0, 0);
    }

    std::mutex m_cancelMutex;
    std::condition_variable m_cancelCv;
    std::atomic_bool m_cancelSignal{false};

    PTP_TIMER m_timer;
    TimerConfig m_timerConfig;
    std::function<void(void)> m_callback;
};

int
main(
    int argc,
    char** argv
    )
{
    const CmdlineArgs args = ParseArgs(argc, argv);
    WriteTrace("Got args! config=%s, log=%s",
        args.configFilePath.c_str(),
        args.logFilePath.c_str());

    const TimerConfig timerConfig = GetConfigFromFile(args.configFilePath);

    FileBuffer logFileBuffer(args.logFilePath);
    auto WriteToLogClosure = [&logFileBuffer]()
    {
        char timeBuffer[DATETIME_STRING_BUFSIZE];
        GetDateTimeString(LocalTime(), timeBuffer);
        logFileBuffer.WriteLine(timeBuffer);
    };

    PeriodicWorkController controller(timerConfig, WriteToLogClosure);
    controller.WaitForCancel();
    return 0;
}
