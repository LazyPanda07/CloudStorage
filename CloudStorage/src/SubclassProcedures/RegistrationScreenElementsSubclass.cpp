#include "pch.h"

#include "RegistrationScreenElementsSubclass.h"
#include "../UIConstants.h"

#include <CommCtrl.h>

LRESULT __stdcall RegistrationScreenElementsSubclass(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
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
			case UI::buttons::registration:
				SendMessageW(GetParent(hwnd), WM_COMMAND, UI::buttons::registration, NULL);

				break;

			case UI::buttons::toAuthorizationScreen:
				SendMessageW(GetParent(hwnd), WM_COMMAND, UI::buttons::toAuthorizationScreen, NULL);

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