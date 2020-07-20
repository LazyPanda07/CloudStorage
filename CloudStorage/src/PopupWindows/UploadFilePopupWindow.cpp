#include "pch.h"

#include "UploadFilePopupWindow.h"
#include "UtilityFunctions.h"
#include "../UIConstants.h"

#include <commctrl.h>

using namespace std;

namespace UI
{
	UploadFilePopupWindow::UploadFilePopupWindow(HWND disableWindow, const wstring& popupWindowTitle, WNDPROC procedure, const wstring& message) :
		BasePopupWindow(disableWindow, popupWindows::uploadFilePopupWindow, popupWindowTitle, procedure, message)
	{
		RECT messagePos = utility::getWindowPosition(BasePopupWindow::messageStatic);
		RECT sizes;
		LONG width;
		LONG height;

		GetClientRect(BasePopupWindow::popupWindow, &sizes);

		width = sizes.right - sizes.left;
		height = sizes.bottom - sizes.top;

		progressBar = CreateWindowExW
		(
			NULL,
			PROGRESS_CLASSW,
			nullptr,
			WS_CHILDWINDOW | WS_VISIBLE,
			0,
			messagePos.top + basePopupWindow::messageStaticHeight,
			width,
			GetSystemMetrics(SM_CYVSCROLL),
			BasePopupWindow::popupWindow,
			HMENU(),
			nullptr,
			nullptr
		);

		SendMessageW(BasePopupWindow::popupWindow, UI::events::initPopupWindowE, reinterpret_cast<WPARAM>(this), NULL);
	}

	HWND UploadFilePopupWindow::getProgressBar() const
	{
		return progressBar;
	}

	void UploadFilePopupWindow::setProgressBarRange(int min, intmax_t max)
	{
		SendMessageW(progressBar, PBM_SETRANGE32, static_cast<WPARAM>(min), static_cast<LPARAM>(max));
	}
}