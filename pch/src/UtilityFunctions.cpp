#include "pch.h"

#include "UtilityFunctions.h"
#include "Constants.h"

#include <Windows.h>

using namespace std;

constexpr array<char, allowableCharactersAlphabetSize> initAlphabet()
{
#pragma warning(push)
#pragma warning(disable : 28020)

	array<char, allowableCharactersAlphabetSize> result{ 0 };

	size_t currentIndex = 0;

	for (size_t i = 'A'; i <= 'Z'; i++)
	{
		result[currentIndex++] = i;
	}

	for (size_t i = 'a'; i <= 'z'; i++)
	{
		result[currentIndex++] = i;
	}

	for (size_t i = '0'; i <= '9'; i++)
	{
		result[currentIndex++] = i;
	}

	result[currentIndex] = '_';

	return result;
#pragma warning(pop)
}

constexpr array<char, allowableCharactersAlphabetSize> alphabet = initAlphabet();

unordered_set<char> allowableCharacters(begin(alphabet), end(alphabet));

//returns size of customHTTPHeaderSize.size() + HTTPMessage.size() + sizeof(\r\n) + digits of size itself
string calculateHTTPMessageSize(const string& HTTPMessage);

namespace utility
{
	namespace conversion
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
	}

	namespace UI
	{
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

		RECT getWindowPosition(HWND window)
		{
			RECT result;
			GetWindowRect(window, &result);
			MapWindowPoints(GetDesktopWindow(), GetParent(window), reinterpret_cast<LPPOINT>(&result), 2);
			return result;
		}
	}

	namespace validation
	{
		bool validationUserData(const string& data)
		{
			return data.size() < 33 ? all_of(begin(data), end(data), [](char character) { return allowableCharacters.find(character) != end(allowableCharacters); }) : false;
		}
	}

	namespace web
	{
		void insertSizeHeaderToHTTPMessage(string& HTTPMessage)
		{
			string totalHTTPMessageSize = customHTTPHeaderSize.data() + calculateHTTPMessageSize(HTTPMessage) + "\r\n";
			HTTPMessage.insert(begin(HTTPMessage) + HTTPMessage.find("\r\n") + 2, begin(totalHTTPMessageSize), end(totalHTTPMessageSize));
		}
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