#pragma once

#include <string>

#include <Windows.h>

using namespace std::chrono_literals;

namespace UI
{
	class BasePopupWindow
	{
	public:
		static inline constexpr int cancel = 0xFFFF;

	protected:
		HWND popupWindow;
		HWND disableWindow;
		HWND messageStatic;
		HWND cancelButton;

		bool showPopupWindow;

	public:
		BasePopupWindow(HWND disableWindow, const std::wstring& popupWindowClassName, const std::wstring& popupWindowTitle, WNDPROC procedure, const std::wstring& message, const std::chrono::duration<double>& revealDelay = 0s);

		virtual HWND getPopupWindow() const final;

		virtual HWND getDisableWindow() const final;

		virtual HWND getCancelButton() const final;

		virtual void setShowPopupWindow(bool isShow) final;

		virtual bool getShowPopupWindow() final;

		virtual ~BasePopupWindow();
	};
}