#include <windows.h>
#include <stdio.h>

void test_bitmap(HINSTANCE inst, const char* name, int id) {
  HBITMAP bitmap = LoadBitmap(inst, MAKEINTRESOURCE(id));
  if (bitmap != NULL) {
    printf("%s:LoadBitmap: ok\n", name);
    DeleteObject(bitmap);
  }
  else {
    printf("%s:LoadBitmap: not ok\n", name);
  }
  HANDLE image = LoadImage(inst, MAKEINTRESOURCE(id), IMAGE_BITMAP, 0, 0, 0);
  if (image != NULL) {
    printf("%s:LoadImage: ok\n", name);
    DeleteObject(image);
  }
  else {
    printf("%s:LoadImage: not ok\n", name);
  }
}

int main() {
  HINSTANCE inst = GetModuleHandle(NULL);
  test_bitmap(inst, "BITMAPCOREHEADER", 1);
  test_bitmap(inst, "BITMAPINFOHEADER", 2);
  test_bitmap(inst, "BITMAPV4HEADER", 3);
  test_bitmap(inst, "BITMAPV5HEADER", 4);
  test_bitmap(inst, "shortfile", 5);
  return 0;
}