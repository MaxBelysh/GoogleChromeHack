#pragma once

#include "includes.h"
#include <random>

extern std::wstring cachedDllPath;
extern std::wstring cachedDllName;

namespace utils {
	uint32_t GetProcessIDByProcessName(const wchar_t* processName);

	bool DropFileFromBuffer(std::wstring desired_file_path, const char* buffer_addr, size_t size);

	std::wstring GetFullTempPath();

	std::wstring GetDllPath();

	std::wstring GetDllName();
}