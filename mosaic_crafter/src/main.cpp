#include "pch.h"

#define LOGINFO

void
LogInfo(
    cstr fmt,
    ...
    )
{
#ifdef LOGINFO
    va_list args;
    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);
#else // LOGINFO
    (void) fmt;
#endif
}

template<typename TInt>
constexpr
float
FloatDiv(
    TInt numerator,
    TInt denominator
    )
{
    return static_cast<float>(numerator) / denominator;
}

template<typename TData>
inline
TData
FileRead(
    FILE* fp
    )
{
    TData val;
    size_t elemsScanned = fread(&val, sizeof(val), 1, fp);
    if (elemsScanned != 1)
    {
        LogInfo("Failed to scan. Scanned %u elements! (%i) eof? %i\n", elemsScanned, ferror(fp), feof(fp));
        assert(false);
    }
    return val;
}

/*
 * size_t fread(void *ptr, size_t sizeOfElements, size_t numberOfElements, FILE *aFile);
 * size_t fwrite(const void *ptr, size_t sizeOfElements, size_t numberOfElements, FILE *aFile);
 */

template<typename TData>
inline
void
FileWrite(
    FILE* file,
    TData* elementsToWrite,
    size_t elementsToWriteCount
    )
{
    size_t elemsWritten = fwrite(elementsToWrite, sizeof(*elementsToWrite), elementsToWriteCount, file);
    if (elemsWritten != elementsToWriteCount)
    {
        LogInfo("Failed to write. Wrote %u elements! (%i) eof? %i\n", elemsWritten, ferror(file), feof(file));
        assert(false);
    }
}

template<typename TData>
inline
void
FileWrite(
    FILE* file,
    TData Data
    )
{
    FileWrite(file, &Data, 1);
}

template<typename t>
class HeapArray
{
public:
    HeapArray()
        : memType(MemType::TotalMemTypes)
        , buffer(nullptr)
        , length(0)
    {
    }

    HeapArray(
        MemType memType,
        size_t length
        )
        : memType(memType)
        , buffer(reinterpret_cast<t*>(MemUtils::Alloc<t>(memType, length)))
        , length(length)
    {
        LogInfo("__ CTOR 0x%p\n", this);
    }

    ~HeapArray()
    {
        LogInfo("__ DTOR 0x%p\n", this);
        Cleanup();
    }

    HeapArray(
        HeapArray&& src
        )
    {
        MoveCommon(std::move(src));
    }

    HeapArray&
    operator=(
        HeapArray&& src
        )
    {
        Cleanup();
        MoveCommon(std::move(src));
        return *this;
    }

    // Prohibit copying
    HeapArray(const HeapArray&) = delete;
    HeapArray& operator=(const HeapArray&) = delete;

    // Buffer interface
    t& operator[](size_t i) { return buffer[i]; }
    const t& operator[](size_t i) const { return buffer[i]; }
    size_t Length() { return length; }
    t* Data() { return buffer; }

private:
    inline
    void
    Cleanup()
    {
        if (buffer != nullptr)
        {
            assert(length != 0);
            MemUtils::Free(memType, buffer);
        }
        else
        {
            assert(length == 0);
        }
    }

    inline
    void
    MoveCommon(
        HeapArray&& src
        )
    {
        LogInfo("__ MOVE from 0x%p to 0x%p\n", &src, this);

        memType = src.memType;
        src.memType = MemType::TotalMemTypes;

        length = src.length;
        src.length = 0;

        buffer = src.buffer;
        src.buffer = nullptr;
    }

    MemType memType;
    t* buffer;
    size_t length;
};

template<typename TData, MemType memType>
inline
HeapArray<TData>
FileRead(
    FILE* file,
    size_t Length
    )
{
    size_t startingPos = ftell(file);

    HeapArray<TData> buffer(memType, Length);
    size_t elemsScanned = fread(buffer.Data(), sizeof(buffer[0]), Length, file);
    if (elemsScanned != Length)
    {
        LogInfo("Failed to scan.\n");
        LogInfo("Scanned %u elements. Expected to scan %u\n", elemsScanned, Length);
        LogInfo("Data = 0x%p\telemSize = %zd\n", buffer.Data(), sizeof(buffer[0]));
        LogInfo("Error? %i; Eof? %i\n", ferror(file), feof(file));
        if (feof(file))
        {
            LogInfo("Started scanning at %zd\n", startingPos);
        }
        assert(false);
    }
    return buffer;
}

inline
void
SeekForwardInFile(
    FILE* fp,
    u32 offset
    )
{
    i32 skipResult = fseek(fp, offset, defs::fseek_current_pos);
    assert(skipResult == defs::fseek_success);
}

inline
void
fseekTo(
    FILE* fp,
    u32 offset
    )
{
    i32 seekResult = fseek(fp, offset, defs::fseek_from_start);
    assert(seekResult == defs::fseek_success);
    assert(ftell(fp) == static_cast<i32>(offset));
}

class Bitmap
{
public:
    template<typename T>
    struct Rgb
    {
        Rgb(T b, T g, T r) : b(b), g(g), r(r) {}

        Rgb(const Rgb& v, float scale) : b(static_cast<T>(v.b * scale)), g(static_cast<T>(v.g * scale)), r(static_cast<T>(v.r * scale)) {}

        T b;
        T g;
        T r;
    };
    typedef Rgb<u8> Rgb8;

    template<typename U, typename V>
    static
    void
    RgbSum(
        Rgb<U>& u,
        const Rgb<V>& v
        )
    {
        u.b += v.b;
        u.g += v.g;
        u.r += v.r;
    }

    template<typename U, typename V>
    static
    Rgb<U>
    RgbCast(
        const Rgb<V>& v
        )
    {
        return Rgb<U>(static_cast<U>(v.b), static_cast<U>(v.g), static_cast<U>(v.r));
    }

    static_assert(sizeof(Rgb8) == 3, "Bitmap::Rgb8 is not supported as 3 bytes");

    static
    inline
    size_t
    GetPackedRowByteCount(
        u16 bitsPerPixel,
        i32 widthInPixels
        )
    {
        assert(bitsPerPixel % 8 == 0);
        return (bitsPerPixel / 8) * abs(widthInPixels);
    }

    static
    inline
    size_t
    GetPaddedRowByteCount(
        u16 bitsPerPixel,
        i32 widthInPixels
        )
    {
        return ((bitsPerPixel * abs(widthInPixels) + 31) / 32) * 4;
    }

    // Packs the bytes from the aligned buffer in a Bitmap file into an in-memory array without any padding.
    static
    void
    ReadPackedBitmapBytesFromFile(
        FILE* file,
        u16 bitsPerPixel,
        i32 srcPxWidth,
        i32 srcPxHeight,
        u8* dest
        )
    {
        // Only support byte-aligned pixel sizes right now
        assert(bitsPerPixel % 8 == 0);
        const size_t packedRowByteCount = GetPackedRowByteCount(bitsPerPixel, srcPxWidth);
        for (i32 row = 0; row < srcPxHeight; ++row)
        {
            fread(dest + (row * packedRowByteCount), sizeof(*dest), packedRowByteCount, file);
        }
    }

    static
    void
    WritePaddedBitmapBytesToFile(
        FILE* file,
        u16 bitsPerPixel,
        i32 srcPxWidth,
        i32 srcPxHeight,
        u8* src
        )
    {
        // Only support byte-aligned pixel sizes right now
        assert(bitsPerPixel % 8 == 0);

        const size_t paddedRowByteCount = GetPaddedRowByteCount(bitsPerPixel, srcPxWidth);
        const size_t packedRowByteCount = GetPackedRowByteCount(bitsPerPixel, srcPxWidth);
        const size_t padding = paddedRowByteCount - packedRowByteCount;

        for (i32 row = 0; row < srcPxHeight; ++row)
        {
            FileWrite(file, src + (row * packedRowByteCount), packedRowByteCount);
            SeekForwardInFile(file, padding);
        }
    }

    static
    Bitmap
    LoadFromFile(
        cstr filename
        )
    {
        FILE* bmpFile;
        errno_t error = fopen_s(&bmpFile, filename, "rb");
        if (error)
        {
            LogInfo("Error [%d]: Failed to open Bitmap file %s!\n", error, filename);
            assert(false);
            return Bitmap{};
        }

        assert(bmpFile != nullptr);

        //
        // Read the Bitmap file header
        //
        char headerId[2];
        fscanf_s(bmpFile, "%c%c", &headerId[0], 1, &headerId[1], 1);
        if (headerId[0] != 'B' || headerId[1] != 'M')
        {
            LogInfo("Failed to read 'BM' header!\n");
            assert(false);
            fclose(bmpFile);
            return Bitmap{};
        }

        Bitmap bmp;
        bmp.fileByteCount = FileRead<u32>(bmpFile);
        SeekForwardInFile(bmpFile, 4);
        bmp.dataByteOffset = FileRead<u32>(bmpFile);
        bmp.headerByteCount = FileRead<u32>(bmpFile);
        bmp.widthInPixels = FileRead<i32>(bmpFile);
        bmp.heightInPixels = FileRead<i32>(bmpFile);
        bmp.colorPaneCount = FileRead<u16>(bmpFile);
        assert(bmp.colorPaneCount == 1);
        bmp.bitsPerPixel = FileRead<u16>(bmpFile);
        assert(bmp.bitsPerPixel == 24);
        bmp.compressionType = FileRead<u32>(bmpFile);
        assert(bmp.compressionType == 0);
        bmp.imageByteCount = FileRead<u32>(bmpFile);
        assert(bmp.imageByteCount == 0);
        bmp.horizontalResolution = FileRead<i32>(bmpFile);
        assert(bmp.horizontalResolution == 0);
        bmp.verticalResolution = FileRead<i32>(bmpFile);
        assert(bmp.verticalResolution == 0);
        bmp.colorsInPaletteCount = FileRead<u32>(bmpFile);
        assert(bmp.colorsInPaletteCount == 0);
        bmp.importantColorCount = FileRead<u32>(bmpFile);
        assert(bmp.importantColorCount == 0);

        // Diagnostic log
        LogInfo("File info:\n");
        LogInfo("%-18s\t%s\n", "name:", filename);
        LogInfo("%-18s\t%lu\n", "fileByteCount:", bmp.fileByteCount);
        LogInfo("%-18s\t%lu\n", "Data ofs:", bmp.dataByteOffset);
        LogInfo("%-18s\t%lu\n", "header size:", bmp.headerByteCount);
        LogInfo("%-18s\t%li\n", "width (pixels):", bmp.widthInPixels);
        LogInfo("%-18s\t%li\n", "height (pixels):", bmp.heightInPixels);
        LogInfo("%-18s\t%u\n", "# of color panes:", bmp.colorPaneCount);
        LogInfo("%-18s\t%u\n", "bits per pixel:", bmp.bitsPerPixel);
        LogInfo("%-18s\t%lu\n", "compression:", bmp.compressionType);
        LogInfo("%-18s\t%lu\n", "image size:", bmp.imageByteCount);
        LogInfo("%-18s\t%li X %li\n", "resolution:", bmp.horizontalResolution, bmp.verticalResolution);
        LogInfo("%-18s\t%lu\n", "# of colors in palette:", bmp.colorsInPaletteCount);
        LogInfo("%-18s\t%lu\n", "# of important colors:", bmp.importantColorCount);

        //
        // Read the Bitmap pixel Data
        //
        fseekTo(bmpFile, bmp.dataByteOffset);

        // Pack the raw Bitmap Data into a packed buffer
        // Only support byte-aligned pixel sizes right now
        assert(bmp.bitsPerPixel % 8 == 0);
        size_t packedDataByteCount = GetPackedRowByteCount(bmp.bitsPerPixel, bmp.widthInPixels) * bmp.heightInPixels;
        bmp.pixelBytes = HeapArray<u8>(MemType::BitmapPixelData, packedDataByteCount);

        ReadPackedBitmapBytesFromFile(bmpFile, bmp.bitsPerPixel, bmp.widthInPixels, bmp.heightInPixels, bmp.pixelBytes.Data());

        LogInfo("\n");
        LogInfo("Pixel buffer info:\n");
        LogInfo("%-18s\t0x%p\n", "Data:", bmp.pixelBytes.Data());
        LogInfo("%-18s\t%zd\n", "Length (in bytes):", bmp.pixelBytes.Length());

        fclose(bmpFile);
        return bmp;
    }

    void
    WriteOutDuplicateBitmap(
        cstr fileName
        )
    {
        FILE* duplicateBmpFile;
        errno_t error = fopen_s(&duplicateBmpFile, fileName, "wb+");
        if (error != 0)
        {
            LogInfo("Failed to open duplicate bmp file\n");
            return;
        }

        assert(duplicateBmpFile != nullptr);

        FileWrite(duplicateBmpFile, 'B');
        FileWrite(duplicateBmpFile, 'M');
        FileWrite(duplicateBmpFile, fileByteCount);
        SeekForwardInFile(duplicateBmpFile, 4);
        FileWrite(duplicateBmpFile, dataByteOffset);
        FileWrite(duplicateBmpFile, headerByteCount);
        FileWrite(duplicateBmpFile, widthInPixels);
        FileWrite(duplicateBmpFile, heightInPixels);
        FileWrite(duplicateBmpFile, colorPaneCount);
        FileWrite(duplicateBmpFile, bitsPerPixel);
        FileWrite(duplicateBmpFile, compressionType);
        FileWrite(duplicateBmpFile, imageByteCount);
        FileWrite(duplicateBmpFile, horizontalResolution);
        FileWrite(duplicateBmpFile, verticalResolution);
        FileWrite(duplicateBmpFile, colorsInPaletteCount);
        FileWrite(duplicateBmpFile, importantColorCount);

        // Write the Bitmap pixel data
        WritePaddedBitmapBytesToFile(duplicateBmpFile, bitsPerPixel, widthInPixels, heightInPixels, pixelBytes.Data());

        fclose(duplicateBmpFile);
    }

    Rgb8
    SuperSamplePixel(
        i32 row,
        i32 col,
        float vInc,
        float uInc
        )
    {
        auto uRadPix = static_cast<i32>((uInc * widthInPixels) / 2);
        auto vRadPix = static_cast<i32>((vInc * heightInPixels) / 2);
        auto xCenterPix = static_cast<i32>(col * uInc * widthInPixels);
        auto yCenterPix = static_cast<i32>(row * vInc * heightInPixels);

        auto CalculateRing = [](i32 x, i32 xc, i32 y, i32 yc) -> i32
        {
            return max(abs(x-xc), abs(y-yc)) + 1;
        };

        auto PixelsInRing = [](i32 ring) -> i32
        {
            assert(ring >= 1);
            if (ring == 1)
            {
                return 1;
            }
            return 8 * (ring - 1);
        };

        auto ReflectAtBounds = [](i32 v, i32 lo, i32 hi)
        {
            if (v < lo)
            {
                return (lo - v) + lo;
            }
            else if (v > hi)
            {
                return (hi - v) - hi;
            }
            return v;
        };

        Rgb<u64> superSampledColor{0,0,0};
        for (i32 r = yCenterPix - vRadPix; r <= yCenterPix + vRadPix; r += 1)
        {
            const i32 rReflect = ReflectAtBounds(r, 0, heightInPixels);
            for (i32 c = xCenterPix - uRadPix; c <= xCenterPix + uRadPix; c += 1)
            {
                const i32 cReflect = ReflectAtBounds(c, 0, widthInPixels);
                const i32 ring = CalculateRing(cReflect, rReflect, xCenterPix, yCenterPix);
                const i32 inversePixelWeight = (ring * ring) + PixelsInRing(ring);
                const Rgb<u64> weightedPixel64 = RgbCast<u64, u8>(Rgb8(GetPixel(rReflect, cReflect), 1.f/inversePixelWeight));
                RgbSum(superSampledColor, weightedPixel64);
            }
        }

        assert(superSampledColor.r <= UINT8_MAX);
        assert(superSampledColor.g <= UINT8_MAX);
        assert(superSampledColor.b <= UINT8_MAX);

        return RgbCast<u8, u64>(superSampledColor);
    }

    void
    WriteOutScaledDownCopy(
        cstr fileName,
        float scale,
        bool writeScaled
        )
    {
        i32 scaledWidthInPixels = static_cast<i32>(abs(widthInPixels * scale));
        i32 scaledHeightInPixels = static_cast<i32>(abs(heightInPixels * scale));
        HeapArray<u8> scaledImagePixelBuffer(MemType::BitmapPixelData, GetPackedRowByteCount(bitsPerPixel, scaledWidthInPixels) * scaledHeightInPixels);
        const float uInc = FloatDiv(1, scaledWidthInPixels);
        const float vInc = FloatDiv(1, scaledHeightInPixels);

        for (i32 row = 0; row < scaledHeightInPixels; row += 1)
        {
            for (i32 col = 0; col < scaledWidthInPixels; col += 1)
            {
                const Rgb8 superSampledValue = SuperSamplePixel(row, col, vInc, uInc);
                StaticGetPixel(scaledImagePixelBuffer.Data(), bitsPerPixel, scaledWidthInPixels, row, col) = superSampledValue;
            }
        }

        FILE* scaledBmpFile;
        errno_t error = fopen_s(&scaledBmpFile, fileName, "wb+");
        if (error != 0)
        {
            LogInfo("Failed to open scaled bmp file\n");
            return;
        }

        assert(scaledBmpFile != nullptr);

        const i32 scaledPaddedByteCount = GetPaddedRowByteCount(bitsPerPixel, scaledWidthInPixels) * scaledHeightInPixels;
        const i32 unscaledPaddedByteCount = GetPaddedRowByteCount(bitsPerPixel, widthInPixels) * heightInPixels;
        const i32 scaledFileByteCount = fileByteCount + scaledPaddedByteCount - unscaledPaddedByteCount;
        LogInfo("Scaled file bytecount %i", scaledFileByteCount);

        FileWrite(scaledBmpFile, 'B');
        FileWrite(scaledBmpFile, 'M');
        if (writeScaled)
        {
            FileWrite(scaledBmpFile, scaledFileByteCount);
        }
        else
        {
            FileWrite(scaledBmpFile, fileByteCount);
        }
        SeekForwardInFile(scaledBmpFile, 4);
        FileWrite(scaledBmpFile, dataByteOffset);
        FileWrite(scaledBmpFile, headerByteCount);
        if (writeScaled)
        {
            FileWrite(scaledBmpFile, scaledWidthInPixels);
            FileWrite(scaledBmpFile, scaledHeightInPixels);
        }
        else
        {
            FileWrite(scaledBmpFile, widthInPixels);
            FileWrite(scaledBmpFile, heightInPixels);
        }
        FileWrite(scaledBmpFile, colorPaneCount);
        FileWrite(scaledBmpFile, bitsPerPixel);
        FileWrite(scaledBmpFile, compressionType);
        if (writeScaled)
        {
            FileWrite(scaledBmpFile, 0);
        }
        else
        {
            FileWrite(scaledBmpFile, imageByteCount);
        }
        FileWrite(scaledBmpFile, horizontalResolution);
        FileWrite(scaledBmpFile, verticalResolution);
        FileWrite(scaledBmpFile, colorsInPaletteCount);
        FileWrite(scaledBmpFile, importantColorCount);

        // Write the Bitmap pixel data
        if (writeScaled)
        {
            WritePaddedBitmapBytesToFile(scaledBmpFile, bitsPerPixel, scaledWidthInPixels, scaledHeightInPixels, scaledImagePixelBuffer.Data());
        }
        else
        {
            WritePaddedBitmapBytesToFile(scaledBmpFile, bitsPerPixel, widthInPixels, heightInPixels, pixelBytes.Data());
        }

        fclose(scaledBmpFile);
    }

    Rgb8
    PixelAverage24()
    {
        // Right now I only support 24bits per pixel
        assert(bitsPerPixel == 24);

        Rgb<u64> colorSum{0, 0, 0};

        for (i32 row = 0; row < heightInPixels; ++row)
        {
            for (i32 col = 0; col < widthInPixels; ++col)
            {
                RgbSum(colorSum, GetPixel(row, col));
            }
        }

        const i32 pixelsRead = heightInPixels * widthInPixels;

        const Rgb<u64> average(colorSum, 1.f / pixelsRead);
        assert(average.r <= UINT8_MAX);
        assert(average.g <= UINT8_MAX);
        assert(average.b <= UINT8_MAX);

        return RgbCast<u8, u64>(average);
    }

    static
    Rgb8&
    StaticGetPixel(
        u8* pixelBytes,
        u16 bitsPerPixel,
        i32 widthInPixels,
        i32 row,
        i32 col
        )
    {
        size_t byteOffset = (row * GetPackedRowByteCount(bitsPerPixel, widthInPixels)) + (col * bitsPerPixel / 8);
        return *reinterpret_cast<Rgb8*>(&pixelBytes[byteOffset]);
    }

    Rgb8&
    GetPixel(
        i32 row,
        i32 col
        )
    {
        return StaticGetPixel(pixelBytes.Data(), bitsPerPixel, widthInPixels, row, col);
    }

    // Enable move construction
    Bitmap(
        Bitmap&& src
        )
        : fileByteCount(src.fileByteCount)
        , dataByteOffset(src.dataByteOffset)
        , headerByteCount(src.headerByteCount)
        , widthInPixels(src.widthInPixels)
        , heightInPixels(src.heightInPixels)
        , colorPaneCount(src.colorPaneCount)
        , bitsPerPixel(src.bitsPerPixel)
        , compressionType(src.compressionType)
        , imageByteCount(src.imageByteCount)
        , horizontalResolution(src.horizontalResolution)
        , verticalResolution(src.verticalResolution)
        , colorsInPaletteCount(src.colorsInPaletteCount)
        , importantColorCount(src.importantColorCount)
        , pixelBytes(std::move(src.pixelBytes))
    {
        src.fileByteCount = 0;
        src.dataByteOffset = 0;
        src.headerByteCount = 0;
        src.widthInPixels = 0;
        src.heightInPixels = 0;
        src.colorPaneCount = 0;
        src.bitsPerPixel = 0;
        src.compressionType = 0;
        src.imageByteCount = 0;
        src.horizontalResolution = 0;
        src.verticalResolution = 0;
        src.colorsInPaletteCount = 0;
        src.importantColorCount = 0;
    }

    // Prohibit move assignment
    Bitmap& operator=(Bitmap&&) = delete;

    // Prohibit copying
    Bitmap(const Bitmap&) = delete;
    Bitmap& operator=(const Bitmap&) = delete;

private:
    Bitmap()
        : fileByteCount(0)
        , dataByteOffset(0)
        , headerByteCount(0)
        , widthInPixels(0)
        , heightInPixels(0)
        , colorPaneCount(0)
        , bitsPerPixel(0)
        , compressionType(0)
        , imageByteCount(0)
        , horizontalResolution(0)
        , verticalResolution(0)
        , colorsInPaletteCount(0)
        , importantColorCount(0)
        , pixelBytes()
    {
    }

    u32 fileByteCount;
    u32 dataByteOffset;
    u32 headerByteCount;
    i32 widthInPixels;
    i32 heightInPixels;
    u16 colorPaneCount;
    u16 bitsPerPixel;
    u32 compressionType;
    u32 imageByteCount;
    i32 horizontalResolution;
    i32 verticalResolution;
    u32 colorsInPaletteCount;
    u32 importantColorCount;
    HeapArray<u8> pixelBytes;
};

int
main(
    int argc,
    char** argv
    )
{
    if (argc == 2)
    {
        Bitmap bmp = Bitmap::LoadFromFile(argv[1]);
        Bitmap::Rgb8 color = bmp.PixelAverage24();
        LogInfo("Average color R=%u, G=%u, B=%u\n", color.r, color.g, color.b);

        bmp.WriteOutDuplicateBitmap("duplicate.bmp");
        LogInfo("Wrote duplicate");
        bmp.WriteOutScaledDownCopy("scaled_fake.bmp", 0.5f, false);
        bmp.WriteOutScaledDownCopy("scaled_real.bmp", 0.5f, true);
        LogInfo("Wrote scaled");
    }
    else
    {
        LogInfo("usage: main.exe <image name>\n");
    }

    MemUtils::VerifyNoMemoryLeaks();
}
