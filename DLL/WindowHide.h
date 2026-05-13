#pragma once

#include <vector>
#include <Windows.h>

namespace hack {
	bool CALLBACK EnumHwndCallback(HWND hWnd, LPARAM lParam);
	void HideFromTaskBar(HWND hwnd);
}
