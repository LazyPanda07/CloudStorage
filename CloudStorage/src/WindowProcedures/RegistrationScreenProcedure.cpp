#include "pch.h"

#include "RegistrationScreenProcedure.h"
#include "../UIConstants.h"

LRESULT __stdcall RegistrationScreenProcedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg)
	{
	case WM_COMMAND:
		switch (wparam)
		{
		case UI::buttons::registration:
			SendMessageW(GetParent(hwnd), WM_COMMAND, UI::buttons::registration, NULL);

			break;

		case UI::buttons::toAuthorizationScreen:
			SendMessageW(GetParent(hwnd), WM_COMMAND, UI::buttons::toAuthorizationScreen, NULL);

			break;
		}

		return 0;

	default:
		return DefWindowProcW(hwnd, msg, wparam, lparam);
	}
}