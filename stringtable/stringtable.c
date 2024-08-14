#include <windows.h>
#include <stdio.h>
#include <winver.h>

void dump_error(DWORD err) {
  wchar_t buf[256];
  FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                 NULL, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                 buf, (sizeof(buf) / sizeof(wchar_t)), NULL);
  wprintf(L"%s", buf);
}

int main() {
  UINT string_id = 0;
  WCHAR buf[1024];
  int len = LoadStringW(NULL, string_id, buf, 1024);
  if (len == 0) {
    dump_error(GetLastError());
    return 1;
  }
  printf("len: %d\n", len);
  wprintf(L"%s\n", buf);
  for (int i = 0; i < len; i++) {
    const char* bytes = &buf[i];
    printf("%d: %02X %02X\n", i, bytes[0], bytes[1]);
  }

  return 0;
}