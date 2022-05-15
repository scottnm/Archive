#pragma once

// UNUSED
// Helper to suppress unused params
#define UNUSED(x) (void)(x)

//
// DEFER
// Useful helper for golang like defer statements. Helps avoid extra RAII wrapping.
//
template<typename F>
class defer_finalizer {
    F f;
    bool moved;
  public:
    template<typename T>
    defer_finalizer(T && f_) : f(std::forward<T>(f_)), moved(false) { }

    defer_finalizer(const defer_finalizer &) = delete;

    defer_finalizer(defer_finalizer && other) : f(std::move(other.f)), moved(other.moved) {
        other.moved = true;
    }

    ~defer_finalizer() {
        if (!moved) f();
    }
};

struct DeferrerType {
    template<typename F>
    defer_finalizer<F> operator<<(F && f) const {
        return defer_finalizer<F>(std::forward<F>(f));
    }
};

constexpr DeferrerType deferrer;

#define TOKENPASTE(x, y) x ## y
#define TOKENPASTE2(x, y) TOKENPASTE(x, y)
#define defer auto TOKENPASTE2(__deferred_lambda_call, __COUNTER__) = deferrer << [&]


//
// COUNTOF
// Helper to return the element size of an array that doesn't get confused with ptrs
//
template<typename T, size_t N> inline constexpr size_t CountOf(T(&)[N]) { return N; }

//
// LOG
// Helper to avoid writing newlines with every printf and also to just be shorter to write than printf :)
inline
void
Log(
    const char* format,
    ...)
{
    va_list args;
    va_start(args, format);
    char printBuffer[1024];
    char printBufferWithNewLine[1024];
    (void) vsnprintf(printBuffer, CountOf(printBuffer), format, args);
    (void) snprintf(printBufferWithNewLine, CountOf(printBufferWithNewLine), "%hs\n", printBuffer);
    OutputDebugString(printBufferWithNewLine);
    va_end(args);
}

// Loop macros
#define ForRange(type, i, start, end) \
    for (type i = (start), i##forRangeEnd = (end); i < i##forRangeEnd; ++i)
