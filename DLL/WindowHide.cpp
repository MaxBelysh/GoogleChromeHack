#include "pch.h"
#include "WindowHide.h"


bool CALLBACK hack::EnumHwndCallback(HWND hWnd, LPARAM lParam)
{
    std::vector<HWND>* windows = reinterpret_cast<std::vector<HWND>*>(lParam);

    DWORD processId = 0;
    GetWindowThreadProcessId(hWnd, &processId);

    if (processId != GetCurrentProcessId())
        return true;

    if (hWnd == GetConsoleWindow())
        return true;

    if (GetWindow(hWnd, GW_OWNER) != nullptr)
        return true;

    if (!IsWindowVisible(hWnd))
        return true;
    windows->push_back(hWnd);
    return true;
}

void hack::HideFromTaskBar(HWND hwnd) {
    LONG_PTR styles = GetWindowLongPtr(hwnd, GWL_EXSTYLE);
    styles |= WS_EX_TOOLWINDOW;
    styles &= ~WS_EX_APPWINDOW;
    SetWindowLongPtr(hwnd, GWL_EXSTYLE, styles);
}