#ifndef HELPERS_H
#define HELPERS_H

#include <windows.h>

const char g_szClassName[] = "exampleWindowClass";
unsigned char *screenData = nullptr;

int makeWindow(HINSTANCE hInstance, int nCmdShow, void *WndProc, const char windowName[], int width, int height)
{
    WNDCLASSEX wc;
    HWND hwnd;

    screenData = (unsigned char *)malloc(width * height * 4);

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

    // Step 2: Creating the Window
    hwnd = CreateWindowEx(
        WS_EX_CLIENTEDGE,
        g_szClassName,
        windowName,
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU,
        CW_USEDEFAULT, CW_USEDEFAULT, width, height,
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
    HBITMAP bitmap = CreateBitmap(320, 240, 1, 32, screenData);

    HDC hdcMem = CreateCompatibleDC(hdc);
    HBITMAP hbmT = SelectObject(hdcMem, bitmap);

    BitBlt(hdc, 0, 0, 320, 240, hdcMem, 0, 0, SRCCOPY);

    SelectObject(hdcMem, hbmT);
    DeleteDC(hdcMem);

    DeleteObject(bitmap);
}

#endif