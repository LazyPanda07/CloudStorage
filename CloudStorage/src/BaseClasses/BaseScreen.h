#pragma once

#include <string>

#include "../UIInterfaces/IResizable.h"

#include <Windows.h>

namespace UI
{
	class BaseScreen : public IResizable
	{
	private:
		const std::wstring wrapperClassName;

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

		virtual const std::string getName() const = 0;

		virtual void pubResize() final;

		virtual void pubShow() final;

		virtual void pubDestroy() final;

		virtual HWND getParent() const final;

		virtual HWND getWrapper() const final;

		//TODO: unregister class
		virtual ~BaseScreen();
	};
}