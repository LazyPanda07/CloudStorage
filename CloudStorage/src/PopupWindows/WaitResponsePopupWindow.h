#pragma once

#include "../BaseClasses/BasePopupWindow.h"

namespace UI
{
	class WaitResponsePopupWindow : public BasePopupWindow
	{
	private:
		HWND progressBar;

	public:
		WaitResponsePopupWindow(HWND disableWindow);

		HWND getWaitResponseProgressBar() const;

		void startAnimateProgressBar() const;

		void stopAnimateProgressBar() const;

		~WaitResponsePopupWindow() = default;
	};
}