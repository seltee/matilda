#include "matilda.h"
#include "helpers.h"
#include "stdlib.h"
#include "stdio.h"

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
struct MTLDContext context;

const unsigned char bitDataHelloWorld[88] = {
    0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
    0b01100110, 0b01111110, 0b01100000, 0b01100000, 0b00111100, 0b00000000, 0b01001010, 0b00111100, 0b01111100, 0b01100000, 0b01111100,
    0b01100110, 0b01100000, 0b01100000, 0b01100000, 0b01100110, 0b00000000, 0b01001010, 0b01100110, 0b01100010, 0b01100000, 0b01100010,
    0b01100110, 0b01100000, 0b01100000, 0b01100000, 0b01100110, 0b00000000, 0b01001010, 0b01100110, 0b01111100, 0b01100000, 0b01100010,
    0b01111110, 0b01111110, 0b01100000, 0b01100000, 0b01100110, 0b00000000, 0b01001010, 0b01100110, 0b01110000, 0b01100000, 0b01100010,
    0b01100110, 0b01100000, 0b01100000, 0b01100000, 0b01100110, 0b00000000, 0b01001010, 0b01100110, 0b01101000, 0b01100000, 0b01100010,
    0b01100110, 0b01111110, 0b01111110, 0b01111110, 0b00111100, 0b00000000, 0b00110100, 0b00111100, 0b01100110, 0b01111110, 0b01111100,
    0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000};

const unsigned char byteDataHelloWorld[64 * 11] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 1, 1, 0, 0, 1, 1, 0, 0, 2, 2, 2, 2, 2, 2, 0, 0, 3, 3, 0, 0, 0, 0, 0, 0, 4, 4, 0, 0, 0, 0, 0, 0, 0, 5, 5, 5, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 6, 0, 6, 0, 0, 0, 7, 7, 7, 7, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 2, 2, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 3, 0, 0,
    0, 1, 1, 0, 0, 1, 1, 0, 0, 2, 2, 0, 0, 0, 0, 0, 0, 3, 3, 0, 0, 0, 0, 0, 0, 4, 4, 0, 0, 0, 0, 0, 0, 5, 5, 0, 0, 5, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 6, 0, 6, 0, 0, 7, 7, 0, 0, 7, 7, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 2, 2, 0, 0, 0, 0, 0, 0, 3, 3, 0, 0, 0, 3, 0,
    0, 1, 1, 0, 0, 1, 1, 0, 0, 2, 2, 0, 0, 0, 0, 0, 0, 3, 3, 0, 0, 0, 0, 0, 0, 4, 4, 0, 0, 0, 0, 0, 0, 5, 5, 0, 0, 5, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 6, 0, 6, 0, 0, 7, 7, 0, 0, 7, 7, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 2, 2, 0, 0, 0, 0, 0, 0, 3, 3, 0, 0, 0, 3, 0,
    0, 1, 1, 1, 1, 1, 1, 0, 0, 2, 2, 2, 2, 2, 2, 0, 0, 3, 3, 0, 0, 0, 0, 0, 0, 4, 4, 0, 0, 0, 0, 0, 0, 5, 5, 0, 0, 5, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 6, 0, 6, 0, 0, 7, 7, 0, 0, 7, 7, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 2, 2, 0, 0, 0, 0, 0, 0, 3, 3, 0, 0, 0, 3, 0,
    0, 1, 1, 0, 0, 1, 1, 0, 0, 2, 2, 0, 0, 0, 0, 0, 0, 3, 3, 0, 0, 0, 0, 0, 0, 4, 4, 0, 0, 0, 0, 0, 0, 5, 5, 0, 0, 5, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 6, 0, 6, 0, 0, 7, 7, 0, 0, 7, 7, 0, 0, 1, 1, 0, 1, 0, 0, 0, 0, 2, 2, 0, 0, 0, 0, 0, 0, 3, 3, 0, 0, 0, 3, 0,
    0, 1, 1, 0, 0, 1, 1, 0, 0, 2, 2, 2, 2, 2, 2, 0, 0, 3, 3, 3, 3, 3, 3, 0, 0, 4, 4, 4, 4, 4, 4, 0, 0, 0, 5, 5, 5, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 6, 0, 6, 0, 0, 0, 0, 7, 7, 7, 7, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 2, 2, 2, 2, 2, 2, 0, 0, 3, 3, 3, 3, 3, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

const unsigned short pallete1[8] = {
    0b0000000000000000,
    0b1111100000000000,
    0b0000011111100000,
    0b1111000000011111,
    0b1110011100011100,
    0b0000011111111111,
    0b1111111111100000,
    0b1111100000011111};

const unsigned short pallete2[8] = {
    0b0000000000000000,
    0b1111100000011111,
    0b0000000000011111,
    0b1110011100011100,
    0b0000011111111111,
    0b1111111111100000,
    0b1111100011000000,
    0b1100011111100000};

const unsigned short pallete3[8] = {
    0b0000000000000000,
    0b0011000110011111,
    0b1110011100011100,
    0b0000011111100000,
    0b0110010111111000,
    0b1111111111111110,
    0b1111100000000000,
    0b1111100110011111,
};

const unsigned short pallete4[8] = {
    0b0000000000000000,
    0b0011100111000111,
    0b0101101011001011,
    0b0111101111001111,
    0b1011110111010111,
    0b1100011000011000,
    0b1110011100011100,
    0b1111111111111111,
};

int main()
{
    MSG Msg;

    unsigned char spriteMem[4096];
    mtldInitDoubleBuffer(&context, spriteMem, 4096);

    makeWindow(GetModuleHandle(nullptr), 1, WndProc, "Example 1: Hello World", 320, 240);

    while (GetMessage(&Msg, nullptr, 0, 0) > 0)
    {
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
    }

    return Msg.wParam;
}

// Called on each WM_PAINT
void updateLoop()
{
    mtldAddSpriteBitMask(&context, 0b1110011100011100, bitDataHelloWorld, 11, 11, 8, 4, 4, 0);
    mtldAddSpriteBitMask(&context, 0b0000000000011100, bitDataHelloWorld, 11, 11, 8, 4, 4 + 16, 0);
    mtldAddSpriteBitMask(&context, 0b0000011100000000, bitDataHelloWorld, 11, 11, 8, 4, 4 + 32, 0);
    mtldAddSpriteBitMask(&context, 0b1110000000000000, bitDataHelloWorld, 11, 11, 8, 4, 4 + 48, 0);

    mtldAddSpritePallete(&context, (const unsigned char *)pallete1, byteDataHelloWorld, 88, 88, 8, 4, 4 + 64, 0);
    mtldAddSpritePallete(&context, (const unsigned char *)pallete2, byteDataHelloWorld, 88, 88, 8, 4, 4 + 80, 0);
    mtldAddSpritePallete(&context, (const unsigned char *)pallete3, byteDataHelloWorld, 88, 88, 8, 4, 4 + 96, 0);
    mtldAddSpritePallete(&context, (const unsigned char *)pallete4, byteDataHelloWorld, 88, 88, 8, 4, 4 + 112, 0);
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
    case WM_PAINT:
        updateLoop();
        unsigned char buffer[320 * 2];
        for (int iy = 0; iy < 240; iy++)
        {
            mtldDrawFromDrawBuffer(&context, MTLD_MODE_565, iy, 320, buffer);
            for (int ix = 0; ix < 320; ix++)
            {
                int p = ix * 2;
                char r = buffer[p + 1] & 0b11111000;
                char g = ((buffer[p + 1] & 0b00000111) << 5) + ((buffer[p] & 0b11100000) >> 3);
                char b = (buffer[p] & 0b00011111) << 3;

                screenData[iy * 320 * 4 + ix * 4] = b;
                screenData[iy * 320 * 4 + ix * 4 + 1] = g;
                screenData[iy * 320 * 4 + ix * 4 + 2] = r;
            }
        }
        PAINTSTRUCT ps;
        HDC hDC = BeginPaint(hwnd, &ps);
        drawToScreen(hDC);
        EndPaint(hwnd, &ps);

        InvalidateRect(hwnd, nullptr, 0);
        break;
    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}