#include <windows.h>
#include <stdbool.h>
#include <commctrl.h>
#include <stdio.h>
#include <fcntl.h>

#define WINDOW_CLASS_NAME L"bitmapview"
#define TEST_BMP_ID MAKEINTRESOURCE(1)

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK DialogProc(HWND, UINT, WPARAM, LPARAM);

HINSTANCE g_hInst;
HBITMAP g_hBitmap;

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR lpCmdLine, int nCmdShow)
{
  g_hInst = hInstance;

  AllocConsole();
  freopen("CONOUT$", "w", stdout);

  g_hBitmap = LoadBitmap(g_hInst, TEST_BMP_ID);
  if (!g_hBitmap) {
    return 1;
  }

  HICON hDefaultIcon = LoadIcon(NULL, IDI_APPLICATION);
  HCURSOR hDefaultCursor = LoadCursor(NULL, IDC_ARROW);

  WNDCLASSW wc = { 0 };
  wc.lpszClassName = WINDOW_CLASS_NAME;
  wc.hInstance = hInstance;
  wc.hbrBackground = GetSysColorBrush(COLOR_WINDOW);
  wc.lpfnWndProc = WndProc;
  wc.hCursor = hDefaultCursor;
  wc.hIcon = hDefaultIcon;

  RegisterClassW(&wc);
  HWND window = CreateWindowW(wc.lpszClassName, WINDOW_CLASS_NAME,
    WS_OVERLAPPEDWINDOW | WS_VISIBLE,
    CW_USEDEFAULT, CW_USEDEFAULT, 400, 200, NULL, NULL, hInstance, NULL);

  MSG msg;
  while (GetMessage(&msg, NULL, 0, 0) > 0)
  {
    if (IsDialogMessage(window, &msg))
      continue;

    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }

  return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
  switch (msg) {

  case WM_PAINT:
  {
    BITMAP bm;
    PAINTSTRUCT ps;

    HDC hdc = BeginPaint(hwnd, &ps);

    HDC hdcMem = CreateCompatibleDC(hdc);
    HBITMAP hOldBitmap = (HBITMAP)SelectObject(hdcMem, g_hBitmap);

    GetObject(g_hBitmap, sizeof(BITMAP), &bm);
    BitBlt(hdc, 0, 0, bm.bmWidth, bm.bmHeight, hdcMem, 0, 0, SRCCOPY);

    printf("%dx%d\n", bm.bmWidth, bm.bmHeight);

    SelectObject(hdcMem, hOldBitmap);
    DeleteDC(hdcMem);

    EndPaint(hwnd, &ps);
  }
  break;

  case WM_INITDIALOG:
    printf("Wnd WM_INITDIALOG: hwnd: %p msg: %d wParam: %p lParam: %p\n", hwnd, msg, (void*)wParam, (void*)lParam);
    break;

  case WM_CREATE:
    printf("Wnd WM_CREATE: hwnd: %p msg: %d wParam: %p lParam: %p\n", hwnd, msg, (void*)wParam, (void*)lParam);
    break;

  case WM_SIZE:
    break;

  case WM_COMMAND:
    break;

  case WM_DESTROY:
    PostQuitMessage(0);
    break;

  case WM_HSCROLL:
    break;

  case WM_CTLCOLORSTATIC:
  case WM_CTLCOLORBTN:
    SetBkMode((HDC)wParam, TRANSPARENT);
    return (LRESULT)GetSysColorBrush(COLOR_WINDOW);

  case WM_GETMINMAXINFO:
    break;
  }

  return DefWindowProcW(hwnd, msg, wParam, lParam);
}