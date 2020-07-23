#pragma once

#include <string>
#include <Windows.h>

namespace utility
{
	namespace conversion
	{
		std::wstring to_wstring(const std::string& source);

		std::string to_string(const std::wstring& source);
	}

	namespace UI
	{
		POINT centerCoordinates(HWND window = GetDesktopWindow());

		POINT centerCoordinates(LONG width, LONG height, HWND window = GetDesktopWindow());

		RECT getWindowPosition(HWND window);
	}

	namespace web
	{
		void insertSizeHeaderToHTTPMessage(std::string& HTTPMessage);
	}
}