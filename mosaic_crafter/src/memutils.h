#pragma once

#include "pch.h"

enum class MemType
{
    BitmapPixelData = 0,
    TotalMemTypes
};

namespace MemUtils
{
    static uint64_t allocationCounts[MemType::TotalMemTypes]{0};

    template<typename alloc_t>
    void*
    Alloc(
        _In_ MemType memType,
        _In_ size_t length
        )
    {
        void* result;
        switch (memType)
        {
            case MemType::BitmapPixelData:
                result = new alloc_t[length];
                break;

            case MemType::TotalMemTypes:
            default:
                result = nullptr;
                assert(false);
                break;
        }

        allocationCounts[static_cast<uint32_t>(memType)] += 1;
        return result;
    }

    template<typename alloc_t>
    void
    Free(
        _In_ MemType memType,
        _In_ alloc_t* data
        )
    {
        assert(memType < MemType::TotalMemTypes);
        assert(allocationCounts[static_cast<uint32_t>(memType)] >= 1);

        delete[] data;
        allocationCounts[static_cast<uint32_t>(memType)] -= 1;
    }


    void
    VerifyNoMemoryLeaks()
    {
        bool foundMemoryLeaks = false;
        for (uint32_t i = 0; i < static_cast<uint32_t>(MemType::TotalMemTypes); ++i)
        {
            if (allocationCounts[i] > 0)
            {
                foundMemoryLeaks = true;
                printf("Leak! memType = %lu, count = %llu\n", i, allocationCounts[i]);
            }
        }

        if (!foundMemoryLeaks)
        {
            printf("No leaks!\n");
        }
    }
};
