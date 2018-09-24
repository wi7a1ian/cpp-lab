#include "Main.h"

int __stdcall WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	int j = DLLCALL::DLLCall<int>(L"USER32.DLL", "MessageBoxTimeoutW", std::make_tuple<>(nullptr, L"Do you like me?", L"Message", MB_YESNO | MB_ICONINFORMATION, 0, 3000));
	if (j == 32000)
		MessageBox(nullptr, L"Was timeout", L"Message", MB_OK);
	return 0;
}
