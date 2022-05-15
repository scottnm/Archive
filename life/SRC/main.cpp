#include "pch.h"

#ifdef SIMPLE_RUN
int
main()
{
    int numRows = 3;
    int numCols = 3;
    CellGrid activeGrid(numCols, numRows);
    CellGrid scratchGrid(numCols, numRows);

    constexpr bool startingGrid[] = {
        false, true, false,
        false, true, false,
        false, true, false};

    FillCellGrid(activeGrid, startingGrid);

    bool continueRendering = true;
    do
    {
        // Render the grid
        printf("\n\n");
        ForRange(int, cellRow, 0, numRows)
        {
            ForRange(int, cellCol, 0, numCols)
            {
                if (activeGrid.cells[GetCellIndex(cellRow, cellCol, numCols)])
                {
                    printf("X ");
                }
                else
                {
                    printf("- ");
                }
            }
            printf("\n");
        }

        // Decide if we want to continue
        bool readingInput;
        do
        {
            printf("\n");
            printf("[N]ext; [Q]uit: ");
            char readBuffer[2];
            fgets(&readBuffer[0], CountOf(readBuffer), stdin);
            (void) getchar();

            readingInput = false;
            switch(readBuffer[0])
            {
                case 'N':
                case 'n':
                {
                    continueRendering = true;
                    // Update for next step
                    UpdateCellGrids(activeGrid, scratchGrid);
                    break;
                }
                case 'Q':
                case 'q':
                {
                    continueRendering = false;
                    break;
                }
                default:
                {
                    readingInput = true;
                    break;
                }
            }
        }
        while (readingInput);
    }
    while (continueRendering);
}
#else // else !SIMPLE_RUN

RenderBuffer g_renderBuffer;
CellGrid g_activeGrid;
CellGrid g_scratchGrid;

bool g_gameRunning;

static
RectXywh
GetWindowRect(
    HWND window)
{
    RECT rect;
    BOOL getRectSucceeded = GetClientRect(window, &rect);
    assert(getRectSucceeded);

    return RectXywh { rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top };
}

static
void
UpdateWindow(
    HWND window,
    RenderBuffer& rb,
    HDC destDeviceContext)
{
    RectXywh winRect = GetWindowRect(window);
    StretchDIBits(
        destDeviceContext,
        0, 0, winRect.width, winRect.height,
        0, 0, rb.GetWidthMag(), rb.GetHeightMag(),
        rb.pixelBuffer.get(),
        &rb.bitmapInfo,
        DIB_RGB_COLORS,
        SRCCOPY);
}

static
void
FillColor(
    RenderBuffer& buffer,
    uint32_t rgbaColor)
{
    std::fill(buffer.pixelBuffer.get(), buffer.pixelBuffer.get() + buffer.GetNumPixels(), rgbaColor);
}

static
void
InitializeRandomStatic(
    CellGrid& cg)
{
    std::srand(GetTickCount()); // initialize our random engine.

    cg.Zero();
    ForRange(uint32_t, cellIndex, 0, (cg.width * cg.height) / 4)
    {
        uint32_t cellCol = std::rand() % cg.width;
        uint32_t cellRow = std::rand() % cg.height;
        cg.cells[GetCellIndex(cellRow, cellCol, cg.width)] = true;
    }
}

static
void
UpdateNextFrame(
    RenderBuffer& buffer,
    CellGrid& grid)
{
    span<bool> cellSpan = grid.GetCellSpan();
    ForRange(ptrdiff_t, i, 0, cellSpan.size())
    {
        if (cellSpan[i])
        {
            buffer.pixelBuffer[i] = Colors::WHITE;
        }
        else
        {
            buffer.pixelBuffer[i] = Colors::BLACK;
        }
    }
}

static
HWND
SetupWindow(
    HINSTANCE instance,
    WNDPROC messageRouter)
{
    WNDCLASS windowClass{};
    // TODO (scott): re-evaluate which of these actually matter
    windowClass.style = CS_HREDRAW | CS_VREDRAW;
    windowClass.lpfnWndProc = messageRouter;
    windowClass.hInstance = instance;
    windowClass.lpszClassName = "LifeWindowClass";

    if (RegisterClass(&windowClass) == 0)
    {
        Log("Window class registration failed! (le=%u)", GetLastError());
        return nullptr;
    }

    HWND window = CreateWindowExA(
        0, // dwExStyle,
        windowClass.lpszClassName,
        "Life",
        WS_OVERLAPPEDWINDOW | WS_VISIBLE, // dwStyle
        CW_USEDEFAULT, // X
        CW_USEDEFAULT, // Y
        CW_USEDEFAULT, // nWidth
        CW_USEDEFAULT, // nHeight
        0, // hWndParent
        0, // hMenu
        instance,
        0); // lpParam
    if (window == nullptr)
    {
        Log("Window creation failed! (le=%u)", GetLastError());
        return nullptr;
    }

    return window;
}

LRESULT
CALLBACK
WindowMessageRouter(
    HWND window,
    UINT message,
    WPARAM wParam,
    LPARAM lParam)
{
    constexpr LRESULT MESSAGE_HANDLED = 0;
    LRESULT messageResult = MESSAGE_HANDLED;

    switch (message)
    {
        case WM_DESTROY:
        {
            Log("WM_DESTROY");
            exit(0);
            break;
        }
        case WM_CLOSE:
        {
            Log("WM_CLOSE");
            exit(0);
            break;
        }
        case WM_QUIT:
        {
            Log("WM_QUIT");
            exit(0);
            break;
        }
        case WM_SYSKEYDOWN:
        {
            if ((wParam == VK_F4) && (lParam & (1 << 29)) != 0)
            {
                Log("Alt-F4 quit");
                exit(0);
            }
            break;
        }
        case WM_ACTIVATEAPP:
        {
            Log("WM_ACTIVATEAPP");
            break;
        }
        case WM_PAINT:
        {
            PAINTSTRUCT paint;
            HDC deviceContext = BeginPaint(window, &paint);
            defer { EndPaint(window, &paint); };

            UpdateWindow(window, g_renderBuffer, deviceContext);
            break;
        }
        default:
        {
            messageResult = DefWindowProc(window, message, wParam, lParam);
            break;
        }
    }

    return messageResult;
}

void
InitializeBitmapInfo(
    BITMAPINFO& bmpInfo)
{
    bmpInfo.bmiHeader.biSize = sizeof(bmpInfo.bmiHeader);
    bmpInfo.bmiHeader.biPlanes = 1;
    bmpInfo.bmiHeader.biBitCount = 32;
    bmpInfo.bmiHeader.biCompression = BI_RGB;
}

void
ResizeDibSection(
    RenderBuffer& buffer,
    CellGrid& cgFront,
    CellGrid& cgBack,
    HWND window)
{
    RectXywh windowRect = GetWindowRect(window);
    buffer.bitmapInfo.bmiHeader.biWidth = windowRect.width;
    buffer.bitmapInfo.bmiHeader.biHeight = -windowRect.height;
    buffer.pixelBuffer.reset(new uint32_t[windowRect.width * windowRect.height * buffer.BYTES_PER_PIXEL]);
    FillColor(buffer, Colors::BLACK);

    for (CellGrid* cg : {&cgFront, &cgBack})
    {
        *cg = CellGrid(windowRect.width, windowRect.height);
        cg->Zero();
    }
}

int
CALLBACK
WinMain(
    HINSTANCE instance,
    HINSTANCE,
    LPSTR,
    int)
{
    Log("Main");
    HWND window = SetupWindow(instance, WindowMessageRouter);

    InitializeBitmapInfo(g_renderBuffer.bitmapInfo);
    ResizeDibSection(g_renderBuffer, g_activeGrid, g_scratchGrid, window);
    InitializeRandomStatic(g_activeGrid);

    int64_t logDueTime = GenerateTimestamp(GetTimestamp(), 1000);
    RollingAverageGenerator<int64_t, 60> rollingAverage(17);

    g_gameRunning = true;
    while (g_gameRunning)
    {
        int64_t startTimeStamp = GetTimestamp();

        // Handle the message loop
        MSG message;
        while (PeekMessage(&message, 0, 0, 0, PM_REMOVE))
        {
            if (message.message == WM_QUIT)
            {
                g_gameRunning = false;
            }

            TranslateMessage(&message);
            DispatchMessage(&message);
        }

        UpdateCellGrids(g_activeGrid, g_scratchGrid);
        UpdateNextFrame(g_renderBuffer, g_activeGrid);

        HDC deviceContext = GetDC(window);
        defer { ReleaseDC(window, deviceContext); };

        UpdateWindow(window, g_renderBuffer, deviceContext);

        int64_t endTimestamp = GetTimestamp();
        int64_t timeToRenderFrameInMilliseconds = DiffTimestampsInMilliseconds(endTimestamp, startTimeStamp);
        int64_t newAverage = rollingAverage.Update(timeToRenderFrameInMilliseconds);
        if (logDueTime <= endTimestamp)
        {
            logDueTime = GenerateTimestamp(endTimestamp, 1000);
            Log("inst: %lld, rolling average: %lld", timeToRenderFrameInMilliseconds, newAverage);
        }
    }

    return 0;
}
#endif // end else !SIMPLE_RUN
