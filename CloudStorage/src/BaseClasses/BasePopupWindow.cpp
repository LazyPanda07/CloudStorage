#include "pch.h"

#include "BasePopupWindow.h"
#include "UtilityFunctions.h"
#include "../UIConstants.h"

using namespace std;

namespace UI
{
	BasePopupWindow::BasePopupWindow(HWND disableWindow, const wstring& popupWindowClassName, const wstring& popupWindowTitle, WNDPROC procedure, const wstring& message) : 
		disableWindow(disableWindow)
	{
		WNDCLASSEXW popup = {};
		POINT centerOfDesktop = utility::centerCoordinates(basePopupWindow::popupWindowWidth, basePopupWindow::popupWindowHeight);
		POINT centerPopup;

		popup.cbSize = sizeof(popup);
		popup.lpfnWndProc = procedure;
		popup.lpszClassName = popupWindowClassName.data();
		popup.hbrBackground = CreateSolidBrush(RGB(255, 255, 255));

		RegisterClassExW(&popup);

		EnableWindow(disableWindow, FALSE);

		popupWindow = CreateWindowExW
		(
			NULL,
			popup.lpszClassName,
			popupWindowTitle.data(),
			WS_OVERLAPPEDWINDOW | WS_VISIBLE,
			centerOfDesktop.x,
			centerOfDesktop.y,
			basePopupWindow::popupWindowWidth,
			basePopupWindow::popupWindowHeight,
			nullptr,
			HMENU(),
			nullptr,
			nullptr
		);

		centerPopup = utility::centerCoordinates(basePopupWindow::messageStaticWidth, basePopupWindow::messageStaticHeight, popupWindow);

		messageStatic = CreateWindowExW
		(
			NULL,
			L"STATIC",
			message.data(),
			WS_CHILDWINDOW | SS_CENTER | SS_SIMPLE | WS_VISIBLE,
			centerPopup.x + basePopupWindow::messageStaticWidth / 3,
			centerPopup.y - basePopupWindow::messageStaticHeight,
			basePopupWindow::messageStaticWidth,
			basePopupWindow::messageStaticHeight,
			popupWindow,
			HMENU(),
			nullptr,
			nullptr
		);

		cancelButton = CreateWindowExW
		(
			NULL,
			L"BUTTON",
			L"Отмена",
			WS_CHILDWINDOW | WS_VISIBLE,
			basePopupWindow::popupWindowWidth - basePopupWindow::cancelButtonWidth,
			basePopupWindow::popupWindowHeight - basePopupWindow::cancelButtonHeight,
			basePopupWindow::cancelButtonWidth,
			basePopupWindow::cancelButtonHeight,
			popupWindow,
			HMENU(cancel),
			nullptr,
			nullptr
		);

	}

	HWND BasePopupWindow::getPopupWindow() const
	{
		return popupWindow;
	}

	HWND BasePopupWindow::getDisableWindow() const
	{
		return disableWindow;
	}

	HWND BasePopupWindow::getCancelButton() const
	{
		return cancelButton;
	}

	BasePopupWindow::~BasePopupWindow()
	{
		EnableWindow(disableWindow, TRUE);
		DestroyWindow(popupWindow);
	}
}