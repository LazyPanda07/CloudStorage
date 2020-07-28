#pragma once

#include <string>

#include <Windows.h>

using namespace std::chrono_literals;

namespace UI
{
	class BasePopupWindow
	{
	private:
		const std::wstring popupWindowClassName;

	public:
		static inline constexpr int cancel = 0xFFFF;

	protected:
		HWND popupWindow;
		HWND disableWindow;
		HWND messageStatic;
		HWND cancelButton;

		bool showPopupWindow;

	public:
		//reveal delay doesn't work
		BasePopupWindow(HWND disableWindow, const std::wstring& popupWindowClassName, const std::wstring& popupWindowTitle, WNDPROC procedure, const std::wstring& message, const std::chrono::duration<double>& revealDelay = 0s);

		virtual HWND getPopupWindow() const final;

		virtual HWND getDisableWindow() const final;

		virtual HWND getCancelButton() const final;

		//showPopupWindow does nothing
		virtual bool& showPopupWindowVar() final;

		//showPopupWindow does nothing
		virtual const bool& showPopupWindowVar() const final;

		virtual ~BasePopupWindow();
	};
}