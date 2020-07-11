#pragma once

#include <Windows.h>

#include "../BaseClasses/BaseScreen.h"

namespace UI
{
	class AuthorizationScreen : public BaseScreen
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

		HWND getAuthorizationLoginEdit() const;

		HWND getAuthorizationPasswordEdit() const;

		~AuthorizationScreen() = default;
	};
}