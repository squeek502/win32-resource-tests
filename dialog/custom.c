// From https://www.codeproject.com/Articles/559385/Custom-Controls-in-Win-API-The-Basics

#include <stdio.h>
#include "custom.h"
#include "wm.h"

static void
CustomPaint(HWND hwnd)
{
    PAINTSTRUCT ps;
    HDC hdc;
    RECT rect;

    GetClientRect(hwnd, &rect);

    hdc = BeginPaint(hwnd, &ps);
    SetTextColor(hdc, RGB(0,0,0));
    SetBkMode(hdc, TRANSPARENT);
    DrawText(hdc, _T("Hello World!"), -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
    EndPaint(hwnd, &ps);
}


static LRESULT CALLBACK
CustomProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    printf("CustomProc hwnd: %p msg: %s wParam: %p lParam: %p\n", hwnd, window_messages[uMsg], (void*)wParam, (void*)lParam);
    if (!window_messages[uMsg]) {
        printf(" -> unknown msg is %X\n", uMsg);
    }
    switch(uMsg) {
        case WM_NCCREATE:
        case WM_CREATE:
            if (lParam) {
                CREATESTRUCT* create_params = (CREATESTRUCT*)lParam;
                const BYTE* data = create_params->lpCreateParams;
                if (data) {
                    WORD len = *((WORD*)data);
                    printf("control data len: %d\n", len);
                    for (WORD i = 0; i < len; i++) {
                        printf("%02X ", data[2 + i]);
                    }
                    printf("\n");
                }
            }
            break;

        case WM_PAINT:
            CustomPaint(hwnd);
            return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void
CustomRegister(void)
{
    WNDCLASS wc = { 0 };

    wc.style = CS_GLOBALCLASS | CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = CustomProc;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.lpszClassName = CUSTOM_WC;
    RegisterClass(&wc);
}

void
CustomUnregister(void)
{
    UnregisterClass(CUSTOM_WC, NULL);
}