#pragma once

#include <Windows.h>

#include "../UIInterfaces/IIterable.h"
#include "../BaseClasses/BaseScreen.h"

namespace UI
{
	class AuthorizationScreen : public BaseScreen, public IIterable<HWND>
	{
	private:
		HWND screenName;

		HWND login;
		HWND password;
		
		HWND enterButton;
		HWND toRegistrationScreenButton;

	private:
		void show() override;

		void destroy() override;

	public:
		AuthorizationScreen(HWND parentWindow, const std::wstring& wrapperClassName, WNDPROC procedure);

		void resize() override;

		const HWND& next(const HWND& value) const override;

		HWND getAuthorizationLoginEdit() const;

		HWND getAuthorizationPasswordEdit() const;

		~AuthorizationScreen() = default;
	};
}