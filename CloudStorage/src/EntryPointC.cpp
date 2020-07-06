#include "pch.h"

#include "MainWindow.h"
#include "Log.h"
#include "UIConstants.h"

int __stdcall wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow)
{
	Log::init(false);

	UI::MainWindow& instance = UI::MainWindow::get();

	SendMessageW(instance.getMainWindow(), UI::events::initMainWindowPtrE, reinterpret_cast<WPARAM>(&instance), NULL);

	SendMessageW(instance.getMainWindow(), UI::events::getFilesE, NULL, NULL);

	MSG msg = {};

	while (GetMessageW(&msg, nullptr, NULL, NULL))
	{
		TranslateMessage(&msg);
		DispatchMessageW(&msg);
	}

	return 0;
}