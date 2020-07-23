#include "pch.h"

#include "CloudStorageScreenProcedure.h"
#include "../UIConstants.h"

#include <CommCtrl.h>

LRESULT __stdcall CloudStorageScreenProcedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	static NMITEMACTIVATE* listViewHdr;

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

		case UI::buttons::remove:
			SendMessageW(GetParent(hwnd), WM_COMMAND, UI::buttons::remove, NULL);

			break;
			
		case UI::buttons::reconnect:
			SendMessageW(GetParent(hwnd), WM_COMMAND, UI::buttons::reconnect, NULL);

			break;
		}

		return 0;

	case UI::events::uploadFileE:
		SendMessageW(GetParent(hwnd), UI::events::uploadFileE, wparam, NULL);

		return 0;

	case WM_NOTIFY:
		switch (reinterpret_cast<LPNMHDR>(lparam)->code)
		{
		case NM_DBLCLK:
			//TODO: open file or open folder
			listViewHdr = reinterpret_cast<NMITEMACTIVATE*>(lparam);

			return 0;

		case NM_RCLICK:
			//TODO: popup menu for element
			listViewHdr = reinterpret_cast<NMITEMACTIVATE*>(lparam);

			return 0;
		}

		return 0;

	case WM_RBUTTONDOWN:
		//TODO: popup menu with create folder and upload files

		return 0;

	default:
		return DefWindowProcW(hwnd, msg, wparam, lparam);
	}
}