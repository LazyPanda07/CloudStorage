#include "pch.h"

#include "RegistrationScreen.h"
#include "UtilityFunctions.h"
#include "../UIConstants.h"
#include "../SubclassProcedures/RegistrationScreenElementsSubclass.h"
#include "../WindowProcedures/RegistrationScreenProcedure.h"

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

	RegistrationScreen::RegistrationScreen(HWND parentWindow) :
		BaseScreen(parentWindow, screens::registrationScreenName, RegistrationScreenProcedure)
	{

		POINT centerWrapper = utility::UI::centerCoordinates(screens::registrationScreen::loginEditWidth, screens::registrationScreen::loginEditHeight, BaseScreen::wrapper);
		centerWrapper.y /= 2;

		screenName = CreateWindowExW
		(
			NULL,
			L"STATIC",
			L"Регистрация",
			WS_CHILDWINDOW | SS_CENTER | SS_SIMPLE,
			centerWrapper.x + screens::registrationScreen::loginEditWidth / 3,
			centerWrapper.y - screens::registrationScreen::defaultOffset - screens::registrationScreen::loginEditHeight,
			screens::registrationScreen::screenNameStaticWidth,
			screens::registrationScreen::screenNameStaticHeight,
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
			screens::registrationScreen::loginEditWidth,
			screens::registrationScreen::loginEditHeight,
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
			centerWrapper.y + screens::registrationScreen::defaultOffset + screens::registrationScreen::loginEditHeight,
			screens::registrationScreen::passwordEditWidth,
			screens::registrationScreen::passwordEditHeight,
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
			centerWrapper.y + screens::registrationScreen::defaultOffset * 2 + screens::registrationScreen::loginEditHeight + screens::registrationScreen::passwordEditHeight,
			screens::registrationScreen::repeatPasswordEditWidth,
			screens::registrationScreen::repeatPasswordEditHeight,
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
			centerWrapper.y + screens::registrationScreen::defaultOffset * 3 + screens::registrationScreen::loginEditHeight + screens::registrationScreen::passwordEditHeight + screens::registrationScreen::repeatPasswordEditHeight,
			screens::registrationScreen::registrationButtonWidth,
			screens::registrationScreen::registrationButtonHeight,
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
			centerWrapper.x + screens::registrationScreen::registrationButtonWidth,
			centerWrapper.y + screens::registrationScreen::defaultOffset * 3 + screens::registrationScreen::loginEditHeight + screens::registrationScreen::passwordEditHeight + screens::registrationScreen::repeatPasswordEditHeight,
			screens::registrationScreen::toAuthorizationScreenButtonWidth,
			screens::registrationScreen::toAuthorizationScreenButtonHeight,
			BaseScreen::wrapper,
			HMENU(UI::buttons::toAuthorizationScreen),
			nullptr,
			nullptr
		);

		SendMessageW(login, EM_SETCUEBANNER, TRUE, reinterpret_cast<LPARAM>(L"Логин"));
		SendMessageW(password, EM_SETCUEBANNER, TRUE, reinterpret_cast<LPARAM>(L"Пароль"));
		SendMessageW(repeatPassword, EM_SETCUEBANNER, TRUE, reinterpret_cast<LPARAM>(L"Подтвердите пароль"));

		SendMessageW(login, EM_LIMITTEXT, UI::screens::registrationScreen::loginLimitCharacters, NULL);
		SendMessageW(password, EM_LIMITTEXT, UI::screens::registrationScreen::passwordLimitCharacters, NULL);
		SendMessageW(password, EM_LIMITTEXT, UI::screens::registrationScreen::passwordLimitCharacters, NULL);

		SendMessageW(password, EM_SETPASSWORDCHAR, static_cast<WPARAM>(L'●'), NULL);
		SendMessageW(repeatPassword, EM_SETPASSWORDCHAR, static_cast<WPARAM>(L'●'), NULL);

		SetWindowSubclass(login, RegistrationScreenElementsSubclass, 0, NULL);
		SetWindowSubclass(password, RegistrationScreenElementsSubclass, 0, NULL);
		SetWindowSubclass(repeatPassword, RegistrationScreenElementsSubclass, 0, NULL);
		SetWindowSubclass(registrationButton, RegistrationScreenElementsSubclass, UI::buttons::registration, NULL);
		SetWindowSubclass(toAuthorizationScreenButton, RegistrationScreenElementsSubclass, UI::buttons::toAuthorizationScreen, NULL);

		SetFocus(login);
	}

	void RegistrationScreen::resize()
	{
		POINT centerWrapper = utility::UI::centerCoordinates(screens::registrationScreen::loginEditWidth, screens::registrationScreen::loginEditHeight, BaseScreen::wrapper);
		centerWrapper.y /= 2;

		SetWindowPos
		(
			screenName,
			HWND_BOTTOM,
			centerWrapper.x + screens::registrationScreen::loginEditWidth / 3,
			centerWrapper.y - screens::registrationScreen::defaultOffset - screens::registrationScreen::loginEditHeight,
			screens::registrationScreen::screenNameStaticWidth,
			screens::registrationScreen::screenNameStaticHeight,
			SWP_SHOWWINDOW
		);
		SetWindowPos
		(
			login,
			HWND_BOTTOM,
			centerWrapper.x,
			centerWrapper.y,
			screens::registrationScreen::loginEditWidth,
			screens::registrationScreen::loginEditHeight,
			SWP_SHOWWINDOW
		);
		SetWindowPos
		(
			password,
			HWND_BOTTOM,
			centerWrapper.x,
			centerWrapper.y + screens::registrationScreen::defaultOffset + screens::registrationScreen::loginEditHeight,
			screens::registrationScreen::passwordEditWidth,
			screens::registrationScreen::passwordEditHeight,
			SWP_SHOWWINDOW
		);
		SetWindowPos
		(
			repeatPassword,
			HWND_BOTTOM,
			centerWrapper.x,
			centerWrapper.y + screens::registrationScreen::defaultOffset * 2 + screens::registrationScreen::loginEditHeight + screens::registrationScreen::passwordEditHeight,
			screens::registrationScreen::repeatPasswordEditWidth,
			screens::registrationScreen::repeatPasswordEditHeight,
			SWP_SHOWWINDOW
		);
		SetWindowPos
		(
			registrationButton,
			HWND_BOTTOM,
			centerWrapper.x,
			centerWrapper.y + screens::registrationScreen::defaultOffset * 3 + screens::registrationScreen::loginEditHeight + screens::registrationScreen::passwordEditHeight + screens::registrationScreen::repeatPasswordEditHeight,
			screens::registrationScreen::registrationButtonWidth,
			screens::registrationScreen::registrationButtonHeight,
			SWP_SHOWWINDOW
		);
		SetWindowPos
		(
			toAuthorizationScreenButton,
			HWND_BOTTOM,
			centerWrapper.x + screens::registrationScreen::registrationButtonWidth,
			centerWrapper.y + screens::registrationScreen::defaultOffset * 3 + screens::registrationScreen::loginEditHeight + screens::registrationScreen::passwordEditHeight + screens::registrationScreen::repeatPasswordEditHeight,
			screens::registrationScreen::toAuthorizationScreenButtonWidth,
			screens::registrationScreen::toAuthorizationScreenButtonHeight,
			SWP_SHOWWINDOW
		);
	}

	const string RegistrationScreen::getName() const
	{
		return "RegistrationScreen";
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
		else if (toAuthorizationScreenButton == value || BaseScreen::parentWindow)
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