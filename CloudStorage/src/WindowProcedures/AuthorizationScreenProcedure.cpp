#include "pch.h"

#include "AuthorizationScreenProcedure.h"
#include "../UIConstants.h"

LRESULT __stdcall AuthorizationScreenProcedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg)
	{
	case WM_COMMAND:
		switch (wparam)
		{
		case UI::buttons::authorization:
			SendMessageW(GetParent(hwnd), WM_COMMAND, UI::buttons::authorization, NULL);

			break;

		case UI::buttons::toRegistrationScreen:
			SendMessageW(GetParent(hwnd), WM_COMMAND, UI::buttons::toRegistrationScreen, NULL);

			break;
		}

		return 0;

	default:
		return DefWindowProcW(hwnd, msg, wparam, lparam);
	}
}