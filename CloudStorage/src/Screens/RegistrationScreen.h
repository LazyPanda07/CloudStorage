#pragma once

#include "../BaseClasses/BaseScreen.h"
#include "../UIInterfaces/IIterable.h"

namespace UI
{
	class RegistrationScreen : public BaseScreen, public IIterable<HWND>
	{
	private:
		HWND screenName;

		HWND login;
		HWND password;
		HWND repeatPassword;

		HWND registrationButton;
		HWND toAuthorizationScreenButton;

	private:
		void show() override;

		void destroy() override;

	public:
		RegistrationScreen(HWND parentWindow);

		void resize() override;

		const std::string getName() const override;

		const HWND& next(const HWND& value) const override;

		HWND getRegistrationLoginEdit() const;

		HWND getRegistrationPasswordEdit() const;

		HWND getRegistrationRepeatPasswordEdit() const;

		~RegistrationScreen() = default;
	};
}