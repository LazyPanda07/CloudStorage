#pragma once

#include "../BaseClasses/BasePopupWindow.h"

namespace UI
{
	class DownloadFilePopupWindow : public BasePopupWindow
	{
	private:
		HWND progressBar;

	public:
		DownloadFilePopupWindow(HWND disableWindow, const std::wstring& message);

		HWND getProgressBar() const;

		void startAnimateProgressBar() const;

		void stopAnimateProgressBar() const;

		~DownloadFilePopupWindow() = default;
	};
}