#include "pch.h"

#include "SetFolderNamePopupWindow.h"
#include "../UIConstants.h"
#include "../WindowProcedures/SetFolderNamePopupWindowProcedure.h"
#include "UtilityFunctions.h"

using namespace std;

namespace UI
{
	SetFolderNamePopupWindow::SetFolderNamePopupWindow(HWND disableWindow) :
		BasePopupWindow(disableWindow, popupWindows::setFolderNamePopupWindowName, popupWindows::setFolderNamePopupWindow::title, SetFolderNamePopupWindowProcedure, L"Укажите название папки")
	{
		RECT messageStaticPos = utility::UI::getWindowPosition(BasePopupWindow::messageStatic);
		RECT cancelButtonPos = utility::UI::getWindowPosition(BasePopupWindow::cancelButton);
		RECT sizes;
		LONG width;
		LONG height;

		GetClientRect(BasePopupWindow::popupWindow, &sizes);
		
		width = sizes.right - sizes.left;
		height = sizes.bottom - sizes.top;

		enterFolderNameEdit = CreateWindowExW
		(
			WS_EX_CLIENTEDGE,
			L"EDIT",
			nullptr,
			WS_CHILDWINDOW | WS_VISIBLE,
			0,
			messageStaticPos.top + popupWindows::basePopupWindow::messageStaticHeight,
			width,
			20,
			BasePopupWindow::popupWindow,
			HMENU(),
			nullptr,
			nullptr
		);

		saveFolderNameButton = CreateWindowExW
		(
			NULL,
			L"BUTTON",
			L"Сохранить",
			WS_CHILDWINDOW | WS_VISIBLE,
			cancelButtonPos.left - UI::popupWindows::setFolderNamePopupWindow::saveFolderNameButtonWidth,
			cancelButtonPos.top,
			UI::popupWindows::setFolderNamePopupWindow::saveFolderNameButtonWidth,
			UI::popupWindows::setFolderNamePopupWindow::saveFolderNameButtonHeight,
			BasePopupWindow::popupWindow,
			HMENU(UI::buttons::saveFolderName),
			nullptr,
			nullptr
		);

		SetFocus(enterFolderNameEdit);

		SendMessageW(BasePopupWindow::popupWindow, UI::events::initPopupWindowE, reinterpret_cast<WPARAM>(this), NULL);
	}

	HWND SetFolderNamePopupWindow::getEnterFolderNameEdit() const
	{
		return enterFolderNameEdit;
	}

	HWND SetFolderNamePopupWindow::getSaveFolderNameButton() const
	{
		return saveFolderNameButton;
	}
}