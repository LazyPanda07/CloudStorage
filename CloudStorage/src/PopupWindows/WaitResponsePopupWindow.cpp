#include "pch.h"

#include "WaitResponsePopupWindow.h"
#include "UtilityFunctions.h"
#include "../WindowProcedures/WaitResponsePopupWindowProcedure.h"
#include "../UIConstants.h"

#include <CommCtrl.h>

using namespace std;

namespace UI
{
	WaitResponsePopupWindow::WaitResponsePopupWindow(HWND disableWindow) :
		BasePopupWindow(disableWindow, popupWindows::waitResponsePopupWindowName, popupWindows::waitResponsePopupWindow::title, WaitResponsePopupWindowProcedure, popupWindows::waitResponsePopupWindow::message, 1s)
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

	HWND WaitResponsePopupWindow::getWaitResponseProgressBar() const
	{
		return progressBar;
	}

	void WaitResponsePopupWindow::startAnimateProgressBar() const
	{
		SendMessageW(progressBar, PBM_SETMARQUEE, static_cast<WPARAM>(true), 0);
	}

	void WaitResponsePopupWindow::stopAnimateProgressBar() const
	{
		SendMessageW(progressBar, PBM_SETMARQUEE, static_cast<WPARAM>(false), 0);
	}
}