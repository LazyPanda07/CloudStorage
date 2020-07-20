#include "pch.h"

#include "UploadFilePopupWindowProcedure.h"
#include "../PopupWindows/UploadFilePopupWindow.h"
#include "../UIConstants.h"

#include <commctrl.h>

using namespace std;

LRESULT __stdcall UploadFilePopupWindowProcedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	static UI::UploadFilePopupWindow* ptr = nullptr;

	switch (msg)
	{
	case WM_COMMAND:
		switch (wparam)
		{
		case UI::BasePopupWindow::cancel:
			SendMessageW(ptr->getDisableWindow(), WM_COMMAND, UI::BasePopupWindow::cancel, NULL);
		}

		return 0;

	case UI::events::updateProgressBarE:
		SendMessageW(ptr->getProgressBar(), PBM_SETPOS, wparam, NULL);

		return 0;

	case UI::events::initPopupWindowE:
		ptr = reinterpret_cast<UI::UploadFilePopupWindow*>(wparam);

		return 0;

	case WM_DESTROY:
		SendMessageW(ptr->getDisableWindow(), WM_COMMAND, UI::BasePopupWindow::cancel, NULL);

		return 0;

	default:
		return DefWindowProcW(hwnd, msg, wparam, lparam);
	}
}