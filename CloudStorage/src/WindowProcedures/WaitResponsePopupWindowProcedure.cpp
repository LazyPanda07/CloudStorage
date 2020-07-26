#include "pch.h"

#include "WaitResponsePopupWindowProcedure.h"
#include "../UIConstants.h"
#include "../PopupWindows/WaitResponsePopupWindow.h"

using namespace std;

LRESULT __stdcall WaitResponsePopupWindowProcedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	static UI::WaitResponsePopupWindow* ptr = nullptr;

	switch (msg)
	{
	case WM_COMMAND:
		switch (wparam)
		{
		case UI::BasePopupWindow::cancel:
			SendMessageW(ptr->getDisableWindow(), WM_COMMAND, UI::BasePopupWindow::cancel, NULL);

			break;
		}

		return 0;

	case UI::events::initPopupWindowE:
		ptr = reinterpret_cast<UI::WaitResponsePopupWindow*>(wparam);

		return 0;

	case WM_DESTROY:
		SendMessageW(ptr->getDisableWindow(), WM_COMMAND, UI::BasePopupWindow::cancel, NULL);

		return 0;

	default:
		return DefWindowProcW(hwnd, msg, wparam, lparam);
	}
}