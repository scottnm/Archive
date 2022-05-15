#pragma once

typedef const char* cstr;

typedef LRESULT (*MessageRouterFunc)(HWND window, uint16_t message, WPARAM wParam, LPARAM lParam);

template <typename T> using span = const gsl::span<const T>;
template <typename T> using mut_span = gsl::span<const T>;
template <typename T> using span_mut = const gsl::span<T>;
template <typename T> using mut_span_mut = gsl::span<T>;

struct RenderBuffer
{
    BITMAPINFO bitmapInfo;
    std::unique_ptr<uint32_t[]> pixelBuffer;

    static constexpr int32_t BYTES_PER_PIXEL = 4;

    // the width is stored normally
    inline int32_t GetWidthMag() { return bitmapInfo.bmiHeader.biWidth; }
    // the height is stored negatively
    inline int32_t GetHeightMag() { return -bitmapInfo.bmiHeader.biHeight; }
    inline size_t GetNumPixels() { return GetWidthMag() * GetHeightMag() * BYTES_PER_PIXEL; }
};

struct RectXywh
{
    int32_t x;
    int32_t y;
    int32_t width;
    int32_t height;
};

template <typename T, size_t N>
class RollingAverageGenerator
{
public:
    RollingAverageGenerator(
        T initialAverage) :
        next(0),
        rollingAverage(static_cast<float>(initialAverage))
    {
        std::fill(std::begin(ringBuffer), std::end(ringBuffer), initialAverage);
    }

    T
    Update(
        T nextValue
        )
    {
        constexpr float N_FLOAT = N;
        rollingAverage += ((nextValue - ringBuffer[next]) / N_FLOAT);
        ringBuffer[next] = nextValue;
        next += 1;
        if (next == N)
        {
            next = 0;
        }
        return static_cast<T>(rollingAverage);
    }

private:
    size_t next;
    float rollingAverage;
    T ringBuffer[N];
};
