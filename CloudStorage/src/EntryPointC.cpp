#include "pch.h"

#include "HTTPNetwork.h"
#include "MainWindow.h"
#include "Log.h"
#include "UIConstants.h"
#include "Constants.h"
#include "INIParser.h"

#pragma comment (lib, "INIParser.lib")

int __stdcall wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow)
{
	Log::init(false);

	utility::INIParser parser(settingsFile);

	web::HTTPNetwork::setAPIServerIp(parser.getKeyValue("APIServer", "ServerIp"));
	web::HTTPNetwork::setAPIServerPort(parser.getKeyValue("APIServer", "ServerPort"));

	UI::MainWindow& instance = UI::MainWindow::get();

	SendMessageW(instance.getMainWindow(), UI::events::initMainWindowPtrE, reinterpret_cast<WPARAM>(&instance), NULL);

	MSG msg = {};

	while (GetMessageW(&msg, nullptr, NULL, NULL))
	{
		TranslateMessage(&msg);
		DispatchMessageW(&msg);
	}

	return 0;
}