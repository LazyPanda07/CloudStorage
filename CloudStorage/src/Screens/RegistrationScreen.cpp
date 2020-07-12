#include "pch.h"

#include "RegistrationScreen.h"
#include "UtilityFunctions.h"
#include "../UIConstants.h"
#include "../SubclassProcedures/RegistrationScreenElementsSubclass.h"

#include <Commctrl.h>

using namespace std;

namespace UI
{
	void RegistrationScreen::show()
	{
		ShowWindow(screenName, SW_SHOW);
		ShowWindow(login, SW_SHOW);
		ShowWindow(password, SW_SHOW);
		ShowWindow(repeatPassword, SW_SHOW);
		ShowWindow(registrationButton, SW_SHOW);
		ShowWindow(toAuthorizationScreenButton, SW_SHOW);
	}

	void RegistrationScreen::destroy()
	{

	}

	RegistrationScreen::RegistrationScreen(HWND parentWindow, const std::wstring& wrapperClassName, WNDPROC procedure) :
		BaseScreen(parentWindow, wrapperClassName, procedure)
	{

		POINT centerWrapper = utility::centerCoordinates(registrationScreen::loginEditWidth, registrationScreen::loginEditHeight, BaseScreen::wrapper);
		centerWrapper.y -= centerWrapper.y / 2;

		screenName = CreateWindowExW
		(
			NULL,
			L"STATIC",
			L"Регистрация",
			WS_CHILDWINDOW | SS_CENTER | SS_SIMPLE,
			centerWrapper.x + registrationScreen::loginEditWidth / 3,
			centerWrapper.y - registrationScreen::defaultOffset - registrationScreen::loginEditHeight,
			registrationScreen::screenNameStaticWidth,
			registrationScreen::screenNameStaticHeight,
			BaseScreen::wrapper,
			HMENU(),
			nullptr,
			nullptr
		);

		login = CreateWindowExW
		(
			WS_EX_CLIENTEDGE,
			L"EDIT",
			nullptr,
			WS_CHILDWINDOW,
			centerWrapper.x,
			centerWrapper.y,
			registrationScreen::loginEditWidth,
			registrationScreen::loginEditHeight,
			BaseScreen::wrapper,
			HMENU(),
			nullptr,
			nullptr
		);

		password = CreateWindowExW
		(
			WS_EX_CLIENTEDGE,
			L"EDIT",
			nullptr,
			WS_CHILDWINDOW,
			centerWrapper.x,
			centerWrapper.y + registrationScreen::defaultOffset + registrationScreen::loginEditHeight,
			registrationScreen::passwordEditWidth,
			registrationScreen::passwordEditHeight,
			BaseScreen::wrapper,
			HMENU(),
			nullptr,
			nullptr
		);

		repeatPassword = CreateWindowExW
		(
			WS_EX_CLIENTEDGE,
			L"EDIT",
			nullptr,
			WS_CHILDWINDOW,
			centerWrapper.x,
			centerWrapper.y + registrationScreen::defaultOffset * 2 + registrationScreen::loginEditHeight + registrationScreen::passwordEditHeight,
			registrationScreen::repeatPasswordEditWidth,
			registrationScreen::repeatPasswordEditHeight,
			BaseScreen::wrapper,
			HMENU(),
			nullptr,
			nullptr
		);

		registrationButton = CreateWindowExW
		(
			NULL,
			L"BUTTON",
			L"Зарегистрироваться",
			WS_CHILDWINDOW,
			centerWrapper.x,
			centerWrapper.y + registrationScreen::defaultOffset * 3 + registrationScreen::loginEditHeight + registrationScreen::passwordEditHeight + registrationScreen::repeatPasswordEditHeight,
			registrationScreen::registrationButtonWidth,
			registrationScreen::registrationButtonHeight,
			BaseScreen::wrapper,
			HMENU(UI::buttons::registration),
			nullptr,
			nullptr
		);

		toAuthorizationScreenButton = CreateWindowExW
		(
			NULL,
			L"BUTTON",
			L"Авторизация",
			WS_CHILDWINDOW,
			centerWrapper.x + registrationScreen::registrationButtonWidth,
			centerWrapper.y + registrationScreen::defaultOffset * 3 + registrationScreen::loginEditHeight + registrationScreen::passwordEditHeight + registrationScreen::repeatPasswordEditHeight,
			registrationScreen::toAuthorizationScreenButtonWidth,
			registrationScreen::toAuthorizationScreenButtonHeight,
			BaseScreen::wrapper,
			HMENU(UI::buttons::toAuthorizationScreen),
			nullptr,
			nullptr
		);

		SendMessageW(login, EM_SETCUEBANNER, TRUE, reinterpret_cast<LPARAM>(L"Логин"));
		SendMessageW(password, EM_SETCUEBANNER, TRUE, reinterpret_cast<LPARAM>(L"Пароль"));
		SendMessageW(repeatPassword, EM_SETCUEBANNER, TRUE, reinterpret_cast<LPARAM>(L"Подтвердите пароль"));

		SendMessageW(login, EM_LIMITTEXT, UI::registrationScreen::loginLimitCharacters, NULL);
		SendMessageW(password, EM_LIMITTEXT, UI::registrationScreen::passwordLimitCharacters, NULL);
		SendMessageW(password, EM_LIMITTEXT, UI::registrationScreen::passwordLimitCharacters, NULL);

		SendMessageW(password, EM_SETPASSWORDCHAR, static_cast<WPARAM>(L'●'), NULL);
		SendMessageW(repeatPassword, EM_SETPASSWORDCHAR, static_cast<WPARAM>(L'●'), NULL);

		SetWindowSubclass(login, RegistrationScreenElementsSubclass, 0, 0);
		SetWindowSubclass(password, RegistrationScreenElementsSubclass, 0, 0);
		SetWindowSubclass(repeatPassword, RegistrationScreenElementsSubclass, 0, 0);
		SetWindowSubclass(registrationButton, RegistrationScreenElementsSubclass, UI::buttons::registration, 0);
		SetWindowSubclass(toAuthorizationScreenButton, RegistrationScreenElementsSubclass, UI::buttons::toAuthorizationScreen, 0);
	}

	void RegistrationScreen::resize()
	{

	}

	const HWND& RegistrationScreen::next(const HWND& value) const
	{
		if (login == value)
		{
			return password;
		}
		else if (password == value)
		{
			return repeatPassword;
		}
		else if (repeatPassword == value)
		{
			return registrationButton;
		}
		else if (registrationButton == value)
		{
			return toAuthorizationScreenButton;
		}
		else if (toAuthorizationScreenButton == value)
		{
			return login;
		}
	}

	HWND RegistrationScreen::getRegistrationLoginEdit() const
	{
		return login;
	}

	HWND RegistrationScreen::getRegistrationPasswordEdit() const
	{
		return password;
	}

	HWND RegistrationScreen::getRegistrationRepeatPasswordEdit() const
	{
		return repeatPassword;
	}
}