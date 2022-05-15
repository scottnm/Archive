#pragma once

int64_t
GetTimestamp();

int64_t
DiffTimestampsInMilliseconds(
    int64_t endTimestamp,
    int64_t startTimestamp);

int64_t
GenerateTimestamp(
    int64_t startTimestamp,
    int64_t diffInMilliseconds);
