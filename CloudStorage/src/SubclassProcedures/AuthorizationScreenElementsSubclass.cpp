#include "pch.h"

#include "AuthorizationScreenElementsSubclass.h"
#include "../UIConstants.h"

#include <commctrl.h>

using namespace std;

LRESULT __stdcall AuthorizationScreenElementsSubclass(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
{
	switch (msg)
	{
	case WM_KEYUP:
		if (wparam == VK_TAB)
		{
			SendMessageW(GetParent(hwnd), msg, wparam, lparam);
		}

		return 0;

	case WM_KEYDOWN:
		if (wparam == VK_RETURN)
		{
			switch (uIdSubclass)
			{
			case UI::buttons::authorization:
				SendMessageW(GetParent(hwnd), WM_COMMAND, UI::buttons::authorization, NULL);

				break;

			case UI::buttons::toRegistrationScreen:
				SendMessageW(GetParent(hwnd), WM_COMMAND, UI::buttons::toRegistrationScreen, NULL);

				break;
			}
		}

		return 0;

	case WM_CHAR:
		if (wparam == VK_TAB)
		{
			return 0;
		}

	default:
		return DefSubclassProc(hwnd, msg, wparam, lparam);
	}
}