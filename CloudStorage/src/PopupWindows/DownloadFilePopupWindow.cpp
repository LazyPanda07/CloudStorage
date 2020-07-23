#include "pch.h"

#include "DownloadFilePopupWindow.h"
#include "UtilityFunctions.h"
#include "../UIConstants.h"
#include "../WindowProcedures/DownloadFilePopupWindowProcedure.h"

#include <CommCtrl.h>

using namespace std;

namespace UI
{
	DownloadFilePopupWindow::DownloadFilePopupWindow(HWND disableWindow, const wstring& message) :
		BasePopupWindow(disableWindow, popupWindows::downloadFilePopupWindowName, popupWindows::downloadFilePopupWindow::title, DownloadFilePopupWindowProcedure, message)
	{
		RECT messagePos = utility::UI::getWindowPosition(BasePopupWindow::messageStatic);
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
			WS_CHILDWINDOW | WS_VISIBLE | PBS_MARQUEE,
			0,
			messagePos.top + popupWindows::basePopupWindow::messageStaticHeight,
			width,
			GetSystemMetrics(SM_CYVSCROLL),
			BasePopupWindow::popupWindow,
			HMENU(),
			nullptr,
			nullptr
		);

		SendMessageW(BasePopupWindow::popupWindow, UI::events::initPopupWindowE, reinterpret_cast<WPARAM>(this), NULL);
	}

	HWND DownloadFilePopupWindow::getDownloadProgressBar() const
	{
		return progressBar;
	}

	void DownloadFilePopupWindow::startAnimateProgressBar() const
	{
		SendMessageW(progressBar, PBM_SETMARQUEE, static_cast<WPARAM>(true), 0);
	}

	void DownloadFilePopupWindow::stopAnimateProgressBar() const
	{
		SendMessageW(progressBar, PBM_SETMARQUEE, static_cast<WPARAM>(false), 0);
	}
}