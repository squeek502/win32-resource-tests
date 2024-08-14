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
  WCHAR path[MAX_PATH];
  GetModuleFileNameW(NULL, path, MAX_PATH);

  DWORD ver_handle;
  DWORD version_size = GetFileVersionInfoSizeW(path, &ver_handle);
  if (version_size == 0) {
    dump_error(GetLastError());
    return 1;
  }
  printf("version_size: %lu\n", version_size);

  unsigned char* verbuf = malloc(version_size);
  if (!verbuf) {
    printf("oom\n");
    return 1;
  }
  ver_handle = 0;
  if (!GetFileVersionInfoW(path, ver_handle, version_size, verbuf)) {
    dump_error(GetLastError());
    return 1;
  }

  LPWSTR querybuf = NULL;
  DWORD querysize;
  if (!VerQueryValueW(verbuf, L"\\key", &querybuf, &querysize)) {
    dump_error(GetLastError());
    return 1;
  }

  printf("size: %lu\n", querysize);
  wprintf(L"%s\n", querybuf);
  //for (int i = 0; i < querysize * 2; i++) {
  //  printf("byte i=%d: %d\n", i, querybuf[i]);
  //}

  return 0;
}