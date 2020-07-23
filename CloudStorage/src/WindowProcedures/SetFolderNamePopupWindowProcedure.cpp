#include "pch.h"

#include "SetFolderNamePopupWindowProcedure.h"
#include "../PopupWindows/SetFolderNamePopupWindow.h"
#include "../UIConstants.h"

using namespace std;

LRESULT __stdcall SetFolderNamePopupWindowProcedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	static UI::SetFolderNamePopupWindow* ptr = nullptr;

	switch (msg)
	{
	case WM_COMMAND:
		switch (wparam)
		{
		case UI::BasePopupWindow::cancel:
			SendMessageW(ptr->getDisableWindow(), WM_COMMAND, UI::BasePopupWindow::cancel, NULL);

			break;

		case UI::buttons::saveFolderName:
			SendMessageW(ptr->getDisableWindow(), WM_COMMAND, UI::buttons::saveFolderName, NULL);

			break;
		}

		return 0;

	case UI::events::initPopupWindowE:
		ptr = reinterpret_cast<UI::SetFolderNamePopupWindow*>(wparam);

		return 0;

	case WM_DESTROY:
		SendMessageW(ptr->getDisableWindow(), WM_COMMAND, UI::BasePopupWindow::cancel, NULL);

		return 0;

	default:
		return DefWindowProcW(hwnd, msg, wparam, lparam);
	}
}