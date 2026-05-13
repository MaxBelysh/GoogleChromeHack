#include "pch.h"
#include "ChromeThread.h"
#include "WindowHide.h"
#include <vector>

DWORD WINAPI CreateChromeThread(LPVOID lpParam) {
	std::vector<HWND> windows;

	do
	{
		windows.clear();
		EnumWindows((WNDENUMPROC)hack::EnumHwndCallback, reinterpret_cast<LPARAM>(&windows));
		for (auto window : windows) {
			SetWindowDisplayAffinity(window, WDA_EXCLUDEFROMCAPTURE);
			hack::HideFromTaskBar(window);
		}
	} while (true);
	
	return 0;
}
