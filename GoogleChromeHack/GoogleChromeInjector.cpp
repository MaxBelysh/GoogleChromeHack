#include "utils.h"
#include <iostream>

int main() {
	const wchar_t processname[] = L"chrome.exe";

	uint32_t processID = 0;

	processID = utils::GetProcessIDByProcessName(processname);

	if (processID == 0) {
		std::cout << "ERROR: process not found" << std::endl;
		return -1;
	}

	std::cout << "INFO: processID of eldenring.exe = " << processID << std::endl;

	HANDLE EldenRingProcess = OpenProcess(PROCESS_ALL_ACCESS, TRUE, processID);

	if (EldenRingProcess == INVALID_HANDLE_VALUE) {
		std::cout << "ERROR: failed to open process" << std::endl;
		return -1;
	}

	std::string path = "C:\\Users\\maxma\\source\\repos\\MaxBelysh\\EldenRingCHEATS\\x64\\Debug\\EldenRingCHEATS_DLL.dll";

	LPVOID PathAddress = VirtualAllocEx(EldenRingProcess, NULL, strlen(path.c_str()) + 1, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);

	if (PathAddress == nullptr) {
		std::cout << "ERROR: failed to allocate memory for dll path" << std::endl;
		return -1;
	}

	if (!WriteProcessMemory(EldenRingProcess, PathAddress, path.c_str(), strlen(path.c_str()) + 1, NULL)) {
		std::cout << "ERROR: failed to write process memory" << std::endl;
		return -1;
	}

	HMODULE Kernel32DLL = GetModuleHandleA("kernel32.dll");

	if (Kernel32DLL == INVALID_HANDLE_VALUE || Kernel32DLL == NULL) {
		std::cout << "ERROR: failed to get Kernel32.dll" << std::endl;
		return -1;
	}

	FARPROC LoadLibraryA_Address = GetProcAddress(Kernel32DLL, "LoadLibraryA");

	if (LoadLibraryA_Address == NULL) {
		std::cout << "ERROR: failed to get LoadLibraryA address" << std::endl;
		return -1;
	}
	Sleep(5000);

	HANDLE EldenRingRemoteThread = CreateRemoteThread(EldenRingProcess, NULL, NULL, (LPTHREAD_START_ROUTINE)LoadLibraryA_Address,
		PathAddress, 0, NULL);

	if (EldenRingRemoteThread == INVALID_HANDLE_VALUE || EldenRingRemoteThread == 0) {
		std::cout << "ERROR: failed to create remote thread" << std::endl;
		return -1;
	}

	CloseHandle(EldenRingProcess);
	//CloseHandle(EldenRingRemoteThread);

	std::cout << "INFO: DLL injected successfully" << std::endl;
	return 0;
}