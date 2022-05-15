#include "pch.h"

struct FrequencyGenerator
{
    FrequencyGenerator()
    {
        LARGE_INTEGER windowsPerformanceFrequency;
        QueryPerformanceFrequency(&windowsPerformanceFrequency);
        frequency = windowsPerformanceFrequency.QuadPart;
    }

    int64_t frequency;
};

static const FrequencyGenerator FREQUENCY_GEN;

int64_t
GetTimestamp()
{
    LARGE_INTEGER timestamp;
    QueryPerformanceCounter(&timestamp);
    return timestamp.QuadPart;
}

int64_t
DiffTimestampsInMilliseconds(
    int64_t endTimestamp,
    int64_t startTimestamp)
{
    return ((endTimestamp - startTimestamp) * 1000) / FREQUENCY_GEN.frequency;
}

int64_t
GenerateTimestamp(
    int64_t startTimestamp,
    int64_t diffInMilliseconds)
{
    return ((FREQUENCY_GEN.frequency * diffInMilliseconds) / 1000) + startTimestamp;
}
