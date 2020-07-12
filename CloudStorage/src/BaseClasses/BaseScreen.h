#pragma once

#include <string>
#include <Windows.h>

#include "../UIInterfaces/IResizable.h"

namespace UI
{
	class BaseScreen : public IResizable
	{
	private:
		BaseScreen(const BaseScreen&) = delete;

		BaseScreen& operator = (const BaseScreen&) = delete;

		BaseScreen(BaseScreen&&) noexcept = delete;

		BaseScreen& operator = (BaseScreen&&) noexcept = delete;

	protected:
		HWND parentWindow;
		HWND wrapper;

	protected:
		virtual void show() = 0;

		virtual void destroy() = 0;

	public:
		//wrapperClassName used in RegisterClassExW
		BaseScreen(HWND parentWindow, const std::wstring& wrapperClassName, WNDPROC procedure);

		virtual void pubResize() final;

		virtual void pubShow() final;

		virtual void pubDestroy() final;

		virtual HWND getParent() const final;

		virtual ~BaseScreen();
	};
}