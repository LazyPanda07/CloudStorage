#include "pch.h"

#include "UploadFilePopupWindowProcedure.h"
#include "../BaseClasses/BasePopupWindow.h"
#include "../UIConstants.h"

using namespace std;

LRESULT __stdcall UploadFilePopupWindowProcedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	static HWND disableWindow;

	switch (msg)
	{
	case WM_COMMAND:
		switch (wparam)
		{
		case UI::BasePopupWindow::cancel:
			SendMessageW(disableWindow, WM_COMMAND, UI::BasePopupWindow::cancel, NULL);
		}

		return 0;

	case UI::events::initDisableWindow:
		disableWindow = reinterpret_cast<HWND>(wparam);

		return 0;

	case WM_DESTROY:
		SendMessageW(disableWindow, WM_COMMAND, UI::BasePopupWindow::cancel, NULL);

		return 0;

	default:
		return DefWindowProcW(hwnd, msg, wparam, lparam);
	}
}