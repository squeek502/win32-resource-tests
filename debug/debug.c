#include <windows.h>
#include <stdio.h>

int main() {
  HRSRC resource = FindResource(
    NULL,
    MAKEINTRESOURCE(1),
    RT_RCDATA
  );
  if (!resource) {
    return 1;
  }

  HGLOBAL data = LoadResource(
    NULL,
    resource
  );
  if (!data) {
    return 1;
  }

  DWORD len = SizeofResource(NULL, resource);

  printf("%.*s", (int)len, (const char*)data);

  return 0;
}