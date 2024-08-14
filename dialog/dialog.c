#include <windows.h>
#include <stdbool.h>
#include <commctrl.h>
#include <stdio.h>
#include <fcntl.h>
#include "custom.h"
#include "wm.h"

//#pragma comment(linker,"\"/manifestdependency:type='win32' \
//name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
//processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#define WINDOW_CLASS_NAME L"DialogTest"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK DialogProc(HWND, UINT, WPARAM, LPARAM);

HINSTANCE g_hInst;
WNDPROC ButtonProc;

static LRESULT CALLBACK
ButtonSubProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
{
  printf("ButtonSubProc hwnd: %p msg: %s wParam: %p lParam: %p\n", hwnd, window_messages[uMsg], (void*)wParam, (void*)lParam);
  if (!window_messages[uMsg]) {
    printf(" -> unknown msg is %X\n", uMsg);
  }
  switch (uMsg) {
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
  }
  return DefSubclassProc(hwnd, uMsg, wParam, lParam);
}

static LRESULT CALLBACK
ButtonSuperProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  printf("ButtonSuperProc hwnd: %p msg: %s wParam: %p lParam: %p\n", hwnd, window_messages[uMsg], (void*)wParam, (void*)lParam);
  if (!window_messages[uMsg]) {
    printf(" -> unknown msg is %X\n", uMsg);
  }
  switch (uMsg) {
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
  }
  return ButtonProc(hwnd, uMsg, wParam, lParam);
}

void dump_error(DWORD err) {
  wchar_t buf[256];
  FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
    NULL, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
    buf, (sizeof(buf) / sizeof(wchar_t)), NULL);
  wprintf(L"%s", buf);
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR lpCmdLine, int nCmdShow)
{
  g_hInst = hInstance;

  AllocConsole();
  freopen("CONOUT$", "w", stdout);

  HICON hDefaultIcon = LoadIcon(NULL, IDI_APPLICATION);
  HCURSOR hDefaultCursor = LoadCursor(NULL, IDC_ARROW);

  WNDCLASSEXW buttonwc = { 0 };
  buttonwc.cbSize = sizeof(WNDCLASSEXW);
  if (GetClassInfoExW(NULL, L"BUTTON", &buttonwc) == 0) {
    printf("GetClassInfoW failed: ");
    dump_error(GetLastError());
  }

  ButtonProc = buttonwc.lpfnWndProc;
  buttonwc.hInstance = hInstance;
  buttonwc.lpfnWndProc = ButtonSuperProc;

  // Note: If a manifest was used to set the version of the common controls to 6.0.0.0,
  // then the RegisterClassExW would fail with "Class already registered", seemingly
  // because the GetClassInfoExW call above implicitly registers the BUTTON class with
  // our hInstance when using the newer common controls.
  //
  // So, if using common controls >= 6.0.0.0, an UnregisterClassW(L"BUTTON", hInstance)
  // call is necessary before the RegisterClassExW call

  // Don't care about the result of this call
  UnregisterClassW(L"BUTTON", hInstance);

  if (RegisterClassExW(&buttonwc) == 0) {
    printf("RegisterClassW failed: ");
    dump_error(GetLastError());
  }

  CustomRegister();

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

  CustomUnregister();

  return (int) msg.wParam;
}

LRESULT CALLBACK DialogProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
  printf("DialogProc hwnd: %p msg: %s wParam: %p lParam: %p\n", hwnd, window_messages[msg], (void*)wParam, (void*)lParam);
  switch (msg) {
  case WM_INITDIALOG:
    HWND button = GetDlgItem(hwnd, 902);
    SetWindowSubclass(button, ButtonSubProc, 0, 0);
    break;

  case WM_CTLCOLORDLG:
  case WM_CTLCOLORSTATIC:
  case WM_CTLCOLORBTN:
    SetBkMode((HDC)wParam, TRANSPARENT);
    return (LRESULT)GetSysColorBrush(COLOR_WINDOW);

  case WM_CLOSE:
    DestroyWindow(hwnd);
    break;

  case WM_DESTROY:
    PostQuitMessage(0);
    return FALSE;
  }

  return 0;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
  printf("WndProc hwnd: %p msg: %s wParam: %p lParam: %p\n", hwnd, window_messages[msg], (void*)wParam, (void*)lParam);
  if (!window_messages[msg]) {
    printf(" -> unknown msg is %X\n", msg);
  }
  switch(msg) {

  case WM_INITDIALOG:
    break;

  case WM_CREATE:
    HWND result = CreateDialogParamW(g_hInst, MAKEINTRESOURCEW(100), hwnd, DialogProc, (LPARAM)NULL);
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
    return (LRESULT) GetSysColorBrush(COLOR_WINDOW);

  case WM_GETMINMAXINFO:
    break;
  }

  return DefWindowProcW(hwnd, msg, wParam, lParam);
}