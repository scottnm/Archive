#pragma once

template <typename T, size_t N>
constexpr
size_t CountOf(
    const T(&arr)[N]
    )
{
    return N;
}
