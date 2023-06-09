#include "blitz.h"

int main()
{
    BLITZ_FUNC("kernel32.dll", Beep, void(*)(DWORD, DWORD));
    Beep(1000, 1000);
  
    LoadLibraryA("user32.dll"); // not loaded by default
    BLITZ_FUNC("user32.dll", MessageBoxA, int(*)(HWND, LPCSTR, LPCSTR, UINT));
    MessageBoxA(0, "Hello world!", "blitz", 0);
}
