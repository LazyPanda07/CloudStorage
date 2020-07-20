#pragma once

#include "../BaseClasses/BasePopupWindow.h"

namespace UI
{
	class UploadFilePopupWindow : public BasePopupWindow
	{
	private:
		HWND progressBar;

	public:
		UploadFilePopupWindow(HWND disableWindow, const std::wstring& popupWindowClassName, const std::wstring& popupWindowTitle, WNDPROC procedure, const std::wstring& message);

		~UploadFilePopupWindow() = default;
	};
}