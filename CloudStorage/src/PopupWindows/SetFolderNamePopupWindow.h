#pragma once

#include "../BaseClasses/BasePopupWindow.h"

namespace UI
{
	class SetFolderNamePopupWindow : public BasePopupWindow
	{
	private:
		HWND enterFolderNameEdit;
		HWND saveFolderNameButton;

	public:
		SetFolderNamePopupWindow(HWND disableWindow);

		HWND getEnterFolderNameEdit() const;

		HWND getSaveFolderNameButton() const;

		~SetFolderNamePopupWindow() = default;
	};
}