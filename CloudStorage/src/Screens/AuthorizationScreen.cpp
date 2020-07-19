#include "pch.h"

#include "AuthorizationScreen.h"
#include "UtilityFunctions.h"
#include "../UIConstants.h"
#include "../SubclassProcedures/AuthorizationScreenElementsSubclass.h"

#include <Commctrl.h>

using namespace std;

namespace UI
{
	void AuthorizationScreen::show()
	{
		ShowWindow(screenName, SW_SHOW);
		ShowWindow(login, SW_SHOW);
		ShowWindow(password, SW_SHOW);
		ShowWindow(enterButton, SW_SHOW);
		ShowWindow(toRegistrationScreenButton, SW_SHOW);
	}

	void AuthorizationScreen::destroy()
	{

	}

	AuthorizationScreen::AuthorizationScreen(HWND parentWindow, const wstring& wrapperClassName, WNDPROC procedure) :
		BaseScreen(parentWindow, wrapperClassName, procedure)
	{
		POINT centerWrapper = utility::centerCoordinates(authorizationScreen::loginEditWidth, authorizationScreen::loginEditHeight, BaseScreen::wrapper);
		centerWrapper.y /= 2;

		screenName = CreateWindowExW
		(
			NULL,
			L"STATIC",
			L"Авторизация",
			WS_CHILDWINDOW | SS_CENTER | SS_SIMPLE,
			centerWrapper.x + authorizationScreen::loginEditWidth / 3,
			centerWrapper.y - authorizationScreen::defaultOffset - authorizationScreen::loginEditHeight,
			authorizationScreen::screenNameStaticWidth,
			authorizationScreen::screenNameStaticHeight,
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
			authorizationScreen::loginEditWidth,
			authorizationScreen::loginEditHeight,
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
			centerWrapper.y + authorizationScreen::defaultOffset + authorizationScreen::loginEditHeight,
			authorizationScreen::passwordEditWidth,
			authorizationScreen::passwordEditHeight,
			BaseScreen::wrapper,
			HMENU(),
			nullptr,
			nullptr
		);

		enterButton = CreateWindowExW
		(
			NULL,
			L"BUTTON",
			L"Вход",
			WS_CHILDWINDOW,
			centerWrapper.x,
			centerWrapper.y + authorizationScreen::defaultOffset * 2 + authorizationScreen::loginEditHeight + authorizationScreen::passwordEditHeight,
			authorizationScreen::enterButtonWidth,
			authorizationScreen::enterButtonHeight,
			BaseScreen::wrapper,
			HMENU(UI::buttons::authorization),
			nullptr,
			nullptr
		);

		toRegistrationScreenButton = CreateWindowExW
		(
			NULL,
			L"BUTTON",
			L"Регистрация",
			WS_CHILDWINDOW,
			centerWrapper.x + authorizationScreen::enterButtonWidth,
			centerWrapper.y + authorizationScreen::defaultOffset * 2 + authorizationScreen::loginEditHeight + authorizationScreen::passwordEditHeight,
			authorizationScreen::toRegistrationScreenButtonWidth,
			authorizationScreen::toRegistrationScreenButtonHeight,
			BaseScreen::wrapper,
			HMENU(UI::buttons::toRegistrationScreen),
			nullptr,
			nullptr
		);

		SendMessageW(login, EM_SETCUEBANNER, TRUE, reinterpret_cast<LPARAM>(L"Логин"));
		SendMessageW(password, EM_SETCUEBANNER, TRUE, reinterpret_cast<LPARAM>(L"Пароль"));

		SendMessageW(login, EM_LIMITTEXT, UI::authorizationScreen::loginLimitCharacters, NULL);
		SendMessageW(password, EM_LIMITTEXT, UI::authorizationScreen::passwordLimitCharacters, NULL);

		SendMessageW(password, EM_SETPASSWORDCHAR, static_cast<WPARAM>(L'●'), NULL);

		SetWindowSubclass(login, AuthorizationScreenElementsSubclass, 0, NULL);
		SetWindowSubclass(password, AuthorizationScreenElementsSubclass, 0, NULL);
		SetWindowSubclass(enterButton, AuthorizationScreenElementsSubclass, UI::buttons::authorization, NULL);
		SetWindowSubclass(toRegistrationScreenButton, AuthorizationScreenElementsSubclass, UI::buttons::toRegistrationScreen, NULL);

		SetFocus(login);
	}

	void AuthorizationScreen::resize()
	{
		POINT centerWrapper = utility::centerCoordinates(authorizationScreen::loginEditWidth, authorizationScreen::loginEditHeight, BaseScreen::wrapper);
		centerWrapper.y /= 2;

		SetWindowPos
		(
			screenName,
			HWND_BOTTOM,
			centerWrapper.x + authorizationScreen::loginEditWidth / 3,
			centerWrapper.y - authorizationScreen::defaultOffset - authorizationScreen::loginEditHeight,
			authorizationScreen::screenNameStaticWidth,
			authorizationScreen::screenNameStaticHeight,
			SWP_SHOWWINDOW
		);
		SetWindowPos
		(
			login,
			HWND_BOTTOM,
			centerWrapper.x,
			centerWrapper.y,
			authorizationScreen::loginEditWidth,
			authorizationScreen::loginEditHeight,
			SWP_SHOWWINDOW
		);
		SetWindowPos
		(
			password,
			HWND_BOTTOM,
			centerWrapper.x,
			centerWrapper.y + authorizationScreen::defaultOffset + authorizationScreen::loginEditHeight,
			authorizationScreen::passwordEditWidth,
			authorizationScreen::passwordEditHeight,
			SWP_SHOWWINDOW
		);
		SetWindowPos
		(
			enterButton,
			HWND_BOTTOM,
			centerWrapper.x,
			centerWrapper.y + authorizationScreen::defaultOffset * 2 + authorizationScreen::loginEditHeight + authorizationScreen::passwordEditHeight,
			authorizationScreen::enterButtonWidth,
			authorizationScreen::enterButtonHeight,
			SWP_SHOWWINDOW
		);
		SetWindowPos
		(
			toRegistrationScreenButton,
			HWND_BOTTOM,
			centerWrapper.x + authorizationScreen::enterButtonWidth,
			centerWrapper.y + authorizationScreen::defaultOffset * 2 + authorizationScreen::loginEditHeight + authorizationScreen::passwordEditHeight,
			authorizationScreen::toRegistrationScreenButtonWidth,
			authorizationScreen::toRegistrationScreenButtonHeight,
			SWP_SHOWWINDOW
		);
	}

	const string AuthorizationScreen::getName() const
	{
		return "AuthorizationScreen";
	}

	const HWND& AuthorizationScreen::next(const HWND& value) const
	{
		if (value == login)
		{
			return password;
		}
		else if (value == password)
		{
			return enterButton;
		}
		else if (value == enterButton)
		{
			return toRegistrationScreenButton;
		}
		else if (value == toRegistrationScreenButton || value == BaseScreen::parentWindow)
		{
			return login;
		}
	}

	HWND AuthorizationScreen::getAuthorizationLoginEdit() const
	{
		return login;
	}

	HWND AuthorizationScreen::getAuthorizationPasswordEdit() const
	{
		return password;
	}
}