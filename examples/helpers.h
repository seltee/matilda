#ifndef HELPERS_H
#define HELPERS_H

#include <windows.h>

const char g_szClassName[] = "exampleWindowClass";
unsigned char *screenData = nullptr;
struct MTLDContext *context;

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
void (*updateGameCallback)(struct MTLDContext *) = nullptr;

int makeWindow(HINSTANCE hInstance, int nCmdShow, void (*callback)(struct MTLDContext *), struct MTLDContext *gameContext, const char windowName[])
{
    WNDCLASSEX wc;
    HWND hwnd;

    screenData = (unsigned char *)malloc(640 * 480 * 4);
    updateGameCallback = callback;
    context = gameContext;

    // Step 1: Registering the Window Class
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = 0;
    wc.lpfnWndProc = WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszMenuName = NULL;
    wc.lpszClassName = g_szClassName;
    wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

    if (!RegisterClassEx(&wc))
    {
        MessageBox(NULL, "Window Registration Failed!", "Error!",
                   MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    const int titleBarSize = 42;

    // Step 2: Creating the Window
    hwnd = CreateWindowEx(
        WS_EX_CLIENTEDGE,
        g_szClassName,
        windowName,
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU,
        CW_USEDEFAULT, CW_USEDEFAULT, 640, 480 + titleBarSize,
        NULL, NULL, hInstance, NULL);

    if (hwnd == NULL)
    {
        MessageBox(NULL, "Window Creation Failed!", "Error!",
                   MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    return 1;
}

void drawToScreen(HDC hdc)
{
    // Create bitmap and copy it to windows scree hdc
    HBITMAP bitmap = CreateBitmap(640, 480, 1, 32, screenData);

    HDC hdcMem = CreateCompatibleDC(hdc);
    HBITMAP hbmT = SelectObject(hdcMem, bitmap);

    BitBlt(hdc, 0, 0, 640, 480, hdcMem, 0, 0, SRCCOPY);

    SelectObject(hdcMem, hbmT);
    DeleteDC(hdcMem);

    DeleteObject(bitmap);
}

void redraw()
{
    // Example defined update callback
    if (updateGameCallback)
        updateGameCallback(context);
    InvalidateRect(nullptr, nullptr, 0);
}

int runGameLoop()
{
    MSG Msg;
    SetUserObjectInformationW(GetCurrentProcess(), UOI_TIMERPROC_EXCEPTION_SUPPRESSION, 0, 1);

    // 40 frames per second, 25ms timer, 1000 / 25 = 40
    SetTimer(NULL, 0, 25, &redraw);

    while (GetMessage(&Msg, nullptr, 0, 0) > 0)
    {
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
    }

    return Msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_CLOSE:
        DestroyWindow(hwnd);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    case WM_MOVE:
    case WM_ERASEBKGND:
        break;
    case WM_PAINT:
        unsigned char buffer[320 * 2];
        for (int iy = 0; iy < 480; iy++)
        {
            mtldDrawFromDrawBuffer(context, iy / 2, 320, buffer);
            for (int ix = 0; ix < 320; ix++)
            {
                int p = ix * 2;
                char r = buffer[p + 1] & 0b11111000;
                char g = ((buffer[p + 1] & 0b00000111) << 5) + ((buffer[p] & 0b11100000) >> 3);
                char b = (buffer[p] & 0b00011111) << 3;

                screenData[iy * 640 * 4 + ix * 8] = b;
                screenData[iy * 640 * 4 + ix * 8 + 1] = g;
                screenData[iy * 640 * 4 + ix * 8 + 2] = r;
                screenData[iy * 640 * 4 + ix * 8 + 4] = b;
                screenData[iy * 640 * 4 + ix * 8 + 5] = g;
                screenData[iy * 640 * 4 + ix * 8 + 6] = r;
            }
        }
        PAINTSTRUCT ps;
        HDC hDC = BeginPaint(hwnd, &ps);
        drawToScreen(hDC);
        EndPaint(hwnd, &ps);
        break;

    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

#endif