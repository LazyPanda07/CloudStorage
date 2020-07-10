#include "pch.h"

#include "BaseScreen.h"

using namespace std;

namespace UI
{
	BaseScreen::BaseScreen(HWND parentWindow, const std::wstring& wrapperClassName, WNDPROC procedure) : parentWindow(parentWindow)
	{
		WNDCLASSEXW screen = {};
		RECT parentSizes;
		screen.cbSize = sizeof(WNDCLASSEXW);
		screen.lpfnWndProc = procedure;
		screen.hbrBackground = CreateSolidBrush(RGB(255, 255, 255));
		screen.hCursor = LoadCursorW(nullptr, IDC_ARROW);
		screen.lpszClassName = wrapperClassName.data();
		screen.hIcon = LoadIconW(nullptr, IDI_APPLICATION);

		RegisterClassExW(&screen);

		GetClientRect(parentWindow, &parentSizes);

		const LONG width = parentSizes.right - parentSizes.left;
		const LONG height = parentSizes.bottom - parentSizes.top;

		wrapper = CreateWindowExW
		(
			NULL,
			screen.lpszClassName,
			nullptr,
			WS_CHILDWINDOW,
			0, 0,
			width, height,
			parentWindow,
			HMENU(),
			nullptr,
			nullptr
		);
	}

	void BaseScreen::pubShow()
	{
		ShowWindow(wrapper, SW_SHOW);

		this->show();
	}

	void BaseScreen::pubDestroy()
	{
		DestroyWindow(wrapper);

		this->destroy();
	}

	HWND BaseScreen::getParent() const
	{
		return parentWindow;
	}

	BaseScreen::~BaseScreen()
	{
		this->pubDestroy();
	}
}