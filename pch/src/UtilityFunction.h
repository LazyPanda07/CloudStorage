#pragma once

#include <string>
#include <Windows.h>

namespace utility
{
	std::wstring to_wstring(const std::string& source);

	std::string to_string(const std::wstring& source);

	POINT centerCoordinates(HWND window = GetDesktopWindow());

	POINT centerCoordinates(LONG width, LONG height, HWND window = GetDesktopWindow());
}