#pragma once

#include "../UIInterfaces/IIterable.h"
#include "../BaseClasses/BaseScreen.h"

#include <Windows.h>

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
		AuthorizationScreen(HWND parentWindow, WNDPROC procedure);

		void resize() override;

		const std::string getName() const override;

		const HWND& next(const HWND& value) const override;

		HWND getAuthorizationLoginEdit() const;

		HWND getAuthorizationPasswordEdit() const;

		~AuthorizationScreen() = default;
	};
}