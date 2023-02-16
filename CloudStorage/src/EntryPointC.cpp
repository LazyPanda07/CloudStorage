#include "pch.h"

#include "CustomHTTPNetwork.h"
#include "MainWindow.h"
#include "Log.h"
#include "UIConstants.h"
#include "Constants.h"
#include "INIParser.h"

#pragma comment (lib, "INIParser.lib")

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow)
{
	Log::init();

	utility::INIParser parser(settingsFile);

	web::CustomHTTPNetwork::setAPIServerIp(parser.getKeyValueData("APIServer", "ServerIp"));
	web::CustomHTTPNetwork::setAPIServerPort(parser.getKeyValueData("APIServer", "ServerPort"));

	UI::MainWindow::setDownloadFolder(parser.getKeyValueData("UserSettings", "DownloadFolder"));

	UI::MainWindow& instance = UI::MainWindow::get();

	SendMessageW(instance.getMainWindow(), UI::events::initMainWindowPtrE, reinterpret_cast<WPARAM>(&instance), NULL);

	MSG msg = {};
	BOOL errorCode;

	while ((errorCode = GetMessage(&msg, nullptr, 0, 0)) != 0)
	{
		if (errorCode == -1)
		{
			break;
		}
		else
		{
			TranslateMessage(&msg);

			DispatchMessageW(&msg);
		}
	}

	return errorCode;
}