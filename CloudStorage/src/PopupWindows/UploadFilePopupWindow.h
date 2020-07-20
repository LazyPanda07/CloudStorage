#pragma once

#include "../BaseClasses/BasePopupWindow.h"

namespace UI
{
	class UploadFilePopupWindow : public BasePopupWindow
	{
	private:
		HWND progressBar;

	public:
		UploadFilePopupWindow(HWND disableWindow, const std::wstring& popupWindowTitle, WNDPROC procedure, const std::wstring& message);

		HWND getProgressBar() const;

		void setProgressBarRange(int min, intmax_t max);

		~UploadFilePopupWindow() = default;
	};
}