#include "pch.h"

#include "UtilityFunction.h"
#include "Constants.h"

#include <Windows.h>

using namespace std;

//returns size of customHTTPHeaderSize.size() + HTTPMessage.size() + sizeof(\r\n) + digits of size itself
string calculateHTTPMessageSize(const string& HTTPMessage);

namespace utility
{
	wstring to_wstring(const string& source)
	{
		wstring res;

		res.resize(MultiByteToWideChar
		(
			CP_ACP,
			NULL,
			source.data(),
			source.size(),
			res.data(),
			NULL
		));

		MultiByteToWideChar
		(
			CP_ACP,
			NULL,
			source.data(),
			source.size(),
			res.data(),
			res.size()
		);

		return res;
	}

	string to_string(const wstring& source)
	{
		string result;

		result.resize(WideCharToMultiByte
		(
			CP_ACP,
			NULL,
			source.data(),
			source.size(),
			result.data(),
			NULL,
			NULL,
			NULL
		));

		WideCharToMultiByte
		(
			CP_ACP,
			NULL,
			source.data(),
			source.size(),
			result.data(),
			result.size(),
			NULL,
			NULL
		);

		return result;
	}

	POINT centerCoordinates(HWND window)
	{
		RECT sizes;

		GetClientRect(window, &sizes);

		return { static_cast<LONG>((sizes.right - sizes.left) * 0.5), static_cast<LONG>((sizes.bottom - sizes.top) * 0.5) };
	}

	POINT centerCoordinates(LONG width, LONG height, HWND window)
	{
		RECT sizes;

		GetClientRect(window, &sizes);

		LONG x = (sizes.right - sizes.left) * 0.5;
		LONG y = (sizes.bottom - sizes.top) * 0.5;

		return { static_cast<LONG>(x - width * 0.5), static_cast<LONG>(y - height * 0.5) };
	}

	void insertSizeHeaderToHTTPMessage(string& HTTPMessage)
	{
		string totalHTTPMessageSize = customHTTPHeaderSize.data() + calculateHTTPMessageSize(HTTPMessage) + "\r\n";
		HTTPMessage.insert(begin(HTTPMessage) + HTTPMessage.find("\r\n") + 2, begin(totalHTTPMessageSize), end(totalHTTPMessageSize));
	}
}

string calculateHTTPMessageSize(const string& HTTPMessage)
{
	int iSize = HTTPMessage.size() + customHTTPHeaderSize.size() + 2;
	string sSize = ::to_string(iSize);
	iSize += sSize.size();
	sSize = ::to_string(iSize);

	return sSize;
}