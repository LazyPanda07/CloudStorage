#include "pch.h"

#include "DownloadFilePopupWindowProcedure.h"
#include "../PopupWindows/DownloadFilePopupWindow.h"
#include "../UIConstants.h"

#include <commctrl.h>

using namespace std;

LRESULT __stdcall DownloadFilePopupWindowProcedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	static UI::DownloadFilePopupWindow* ptr = nullptr;

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
		SendMessageW(ptr->getDownloadProgressBar(), PBM_SETPOS, wparam, NULL);

		return 0;

	case UI::events::initPopupWindowE:
		ptr = reinterpret_cast<UI::DownloadFilePopupWindow*>(wparam);

		return 0;

	case WM_DESTROY:
		SendMessageW(ptr->getDisableWindow(), WM_COMMAND, UI::BasePopupWindow::cancel, NULL);

		return 0;

	default:
		return DefWindowProcW(hwnd, msg, wparam, lparam);
	}
}