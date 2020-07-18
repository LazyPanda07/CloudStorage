#pragma once

#include <string>

#include <Windows.h>

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

	public:
		BasePopupWindow(HWND disableWindow, const std::wstring& popupWindowClassName, const std::wstring& popupWindowTitle, WNDPROC procedure, const std::wstring& message);

		virtual HWND getPopupWindow() const final;

		virtual HWND getDisableWindow() const final;

		virtual HWND getCancelButton() const final;

		virtual ~BasePopupWindow();
	};
}