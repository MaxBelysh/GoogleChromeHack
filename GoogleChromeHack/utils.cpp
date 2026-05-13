#include "utils.h"
#include <fstream>

std::wstring cachedDllPath = L"";
std::wstring cachedDllName = L"";

uint32_t utils::GetProcessIDByProcessName(const wchar_t* processName)
{
	HANDLE Tool32Snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	if (Tool32Snapshot == INVALID_HANDLE_VALUE) {
		return 0;
	}

	PROCESSENTRY32 pe32;
	pe32.dwSize = sizeof(PROCESSENTRY32);

	if (!Process32First(Tool32Snapshot, &pe32)) {
		return 0;
	}

	bool foundProcess = false;

	while (!foundProcess) {
		if (wcscmp(processName, pe32.szExeFile) == 0) {
			foundProcess = true;
			CloseHandle(Tool32Snapshot);
			return pe32.th32ProcessID;
		}
		if (!Process32NextW(Tool32Snapshot, &pe32)) {
			return 0;
		}
	}
	CloseHandle(Tool32Snapshot);
	return 0;
}

bool utils::DropFileFromBuffer(std::wstring desired_file_name, const char* buffer_addr, size_t size)
{
	std::ofstream file_ofstream(desired_file_name.c_str(), std::ios::out | std::ios::binary);

	if (!file_ofstream.write(buffer_addr, size))
	{
		std::cout << "ERROR: failed to create dll from buffer" << std::endl;
		file_ofstream.close();
		return false;
	}
	std::cout << "INFO: Successfully created dll from buffer" << std::endl;
	file_ofstream.close();

	return true;
}

std::wstring utils::GetFullTempPath()
{
	wchar_t temp_directory[MAX_PATH + 1] = {};
	const uint32_t get_temp_path_ret = GetTempPathW(sizeof(temp_directory) / sizeof(wchar_t), temp_directory);
	if (!get_temp_path_ret || get_temp_path_ret > MAX_PATH + 1)
	{
		return L"";
	}

	if (temp_directory[wcslen(temp_directory) - 1] == L'\\')
	{
		temp_directory[wcslen(temp_directory) - 1] = 0x0;
	}

	return std::wstring(temp_directory);
}

std::wstring utils::GetDllPath()
{

	if (!cachedDllPath.empty())
	{
		return cachedDllPath;
	}

	std::wstring temp = utils::GetFullTempPath();
	if (temp.empty())
	{
		return L"";
	}

	return temp + L"\\" + utils::GetDllName();
}

std::wstring utils::GetDllName()
{
	if (!cachedDllName.empty())
	{
		return cachedDllName;
	}
	else
	{
		srand((unsigned int)time(nullptr));
		char buffer[100]{};
		static const char alphabet[] = "QWERTYUIOPASDFGHJKLZXCVBNMqwertyuiopasdfghjklzxcvbnm";
		int len = rand() % 20 + 10;
		for (int i = 0; i < len; i++)
		{
			buffer[i] = alphabet[rand() % (sizeof(alphabet) - 1)];
		}
		std::string str(buffer);
		std::wstring name(str.begin(), str.end());
		cachedDllName = name;
		return name;
	}
}