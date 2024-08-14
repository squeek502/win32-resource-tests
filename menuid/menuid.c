#include <windows.h>
#include <stdbool.h>
#include <commctrl.h>
#include <stdio.h>
#include <fcntl.h>

#define WINDOW_CLASS_NAME L"DialogTest"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK DialogProc(HWND, UINT, WPARAM, LPARAM);

HINSTANCE g_hInst;

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR lpCmdLine, int nCmdShow)
{
  g_hInst = hInstance;

  AllocConsole();
  freopen("CONOUT$", "w", stdout);

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

LRESULT CALLBACK DialogProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
  printf("DialogProc hwnd: %p msg: %d wParam: %p lParam: %p\n", hwnd, msg, (void*)wParam, (void*)lParam);
  switch (msg) {
  case WM_INITDIALOG:
    printf("Dialog WM_INITDIALOG: hwnd: %p msg: %d wParam: %p lParam: %p\n", hwnd, msg, (void*)wParam, (void*)lParam);
    break;

  case WM_CREATE:
    printf("Dialog WM_CREATE: hwnd: %p msg: %d wParam: %p lParam: %p\n", hwnd, msg, (void*)wParam, (void*)lParam);
    break;
  }

  return 0;
  //return DefDlgProc(hwnd, msg, wParam, lParam);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
  switch (msg) {

  case WM_INITDIALOG:
    printf("Wnd WM_INITDIALOG: hwnd: %p msg: %d wParam: %p lParam: %p\n", hwnd, msg, (void*)wParam, (void*)lParam);
    break;

  case WM_CREATE:
    printf("Wnd WM_CREATE: hwnd: %p msg: %d wParam: %p lParam: %p\n", hwnd, msg, (void*)wParam, (void*)lParam);
    HWND result = CreateDialogParamW(g_hInst, MAKEINTRESOURCEW(1), hwnd, DialogProc, (LPARAM)NULL);
    if (!result) {
      char buf[256];
      FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        buf, sizeof(buf), NULL);
      printf("error creating dialog: %s\n", buf);
    }
    ShowWindow(result, SW_SHOWNORMAL);
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