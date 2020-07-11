#include "pch.h"

#include "CloudStorageScreenProcedure.h"
#include "../UIConstants.h"

LRESULT __stdcall CloudStorageScreenProcedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg)
	{
	case WM_COMMAND:
		switch (wparam)
		{
		case UI::buttons::refresh:
			SendMessageW(GetParent(hwnd), WM_COMMAND, UI::buttons::refresh, NULL);

			break;

		case UI::buttons::download:
			SendMessageW(GetParent(hwnd), WM_COMMAND, UI::buttons::download, NULL);

			break;
		}

		return 0;

	case UI::events::uploadFileE:
		SendMessageW(GetParent(hwnd), UI::events::uploadFileE, wparam, NULL);

		return 0;

	default:
		return DefWindowProcW(hwnd, msg, wparam, lparam);
	}
}