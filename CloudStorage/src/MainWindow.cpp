#include "pch.h"

#include "IOSocketStream.h"
#include "HTTPNetwork.h"
#include "UtilityFunctions.h"
#include "NetworkFunctions.h"
#include "Screens/ScreenFunctions.h"
#include "Screens/AuthorizationScreen.h"
#include "Screens/RegistrationScreen.h"
#include "Screens/CloudStorageScreen.h"
#include "UIConstants.h"
#include "MainWindow.h"

#include <commctrl.h>

#pragma comment (lib, "Comctl32.lib")

#pragma comment (lib, "HTTP.lib")
#pragma comment (lib, "SocketStreams.lib")
#pragma comment (lib, "Log.lib")

#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

using namespace std;

LRESULT __stdcall MainWindowProcedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

namespace UI
{
	MainWindow::MainWindow() : currentScreen(nullptr)
	{
		INITCOMMONCONTROLSEX init;
		WNDCLASSEXW wndClass = {};
		POINT monitorCenter = utility::centerCoordinates(UI::mainWindowUI::mainWindowWidth, UI::mainWindowUI::mainWindowHeight);

		init.dwICC = ICC_LISTVIEW_CLASSES;
		wndClass.cbSize = sizeof(WNDCLASSEXW);
		wndClass.lpfnWndProc = MainWindowProcedure;
		wndClass.hbrBackground = CreateSolidBrush(RGB(255, 255, 255));
		wndClass.hCursor = LoadCursorW(nullptr, IDC_ARROW);
		wndClass.lpszClassName = L"MainWindow";
		wndClass.hIcon = LoadIconW(nullptr, IDI_APPLICATION);

		RegisterClassExW(&wndClass);

		mainWindow = CreateWindowExW
		(
			NULL,
			wndClass.lpszClassName,
			L"Cloud Storage",
			WS_OVERLAPPEDWINDOW | WS_VISIBLE,
			monitorCenter.x, monitorCenter.y,
			UI::mainWindowUI::mainWindowWidth, UI::mainWindowUI::mainWindowHeight,
			nullptr,
			HMENU(),
			nullptr,
			nullptr
		);

		InitCommonControlsEx(&init);

		initAuthorizationScreen(*this);

		currentScreen->pubShow();
	}

	HWND MainWindow::getHWND(elementsEnum id) const
	{
		switch (id)
		{
		case UI::MainWindow::elementsEnum::mainWindow:
			return mainWindow;

		case UI::MainWindow::elementsEnum::refreshButton:
			return static_cast<CloudStorageScreen*>(currentScreen)->getRefreshButton();

		case UI::MainWindow::elementsEnum::downloadButton:
			return static_cast<CloudStorageScreen*>(currentScreen)->getDownloadButton();

		case UI::MainWindow::elementsEnum::list:
			return static_cast<CloudStorageScreen*>(currentScreen)->getList();

		case UI::MainWindow::elementsEnum::authorizationLoginEdit:
			return static_cast<AuthorizationScreen*>(currentScreen)->getAuthorizationLoginEdit();

		case UI::MainWindow::elementsEnum::authorizationPasswordEdit:
			return static_cast<AuthorizationScreen*>(currentScreen)->getAuthorizationPasswordEdit();

		case UI::MainWindow::elementsEnum::registrationLoginEdit:
			return static_cast<RegistrationScreen*>(currentScreen)->getRegistrationLoginEdit();

		case UI::MainWindow::elementsEnum::registrationPasswordEdit:
			return static_cast<RegistrationScreen*>(currentScreen)->getRegistrationPasswordEdit();

		case UI::MainWindow::elementsEnum::registrationRepeatPasswordEdit:
			return static_cast<RegistrationScreen*>(currentScreen)->getRegistrationRepeatPasswordEdit();

		default:
			return nullptr;
		}
	}

	MainWindow::~MainWindow()
	{
		DestroyWindow(mainWindow);
	}

	MainWindow& MainWindow::get()
	{
		static MainWindow instance;

		return instance;
	}

	BaseScreen* MainWindow::getCurrentScreen()
	{
		return currentScreen;
	}

	void MainWindow::setCurrentScreen(BaseScreen* screen)
	{
		currentScreen = screen;
	}

	void MainWindow::resize()
	{
		if (currentScreen)
		{
			currentScreen->resize();
		}
	}

	HWND MainWindow::getMainWindow() const
	{
		return this->getHWND(elementsEnum::mainWindow);
	}

	HWND MainWindow::getRefreshButton() const
	{
		return this->getHWND(elementsEnum::refreshButton);
	}

	HWND MainWindow::getDownloadButton() const
	{
		return this->getHWND(elementsEnum::downloadButton);
	}

	HWND MainWindow::getList() const
	{
		return this->getHWND(elementsEnum::list);
	}

	HWND MainWindow::getAuthorizationLoginEdit() const
	{
		return this->getHWND(elementsEnum::authorizationLoginEdit);
	}

	HWND MainWindow::getAuthorizationPasswordEdit() const
	{
		return this->getHWND(elementsEnum::authorizationPasswordEdit);
	}

	HWND MainWindow::getRegistrationLoginEdit() const
	{
		return this->getHWND(elementsEnum::registrationLoginEdit);
	}

	HWND MainWindow::getRegistrationPasswordEdit() const
	{
		return this->getHWND(elementsEnum::registrationPasswordEdit);
	}

	HWND MainWindow::getRegistrationRepeatPasswordEdit() const
	{
		return this->getHWND(elementsEnum::registrationRepeatPasswordEdit);
	}
}

LRESULT __stdcall MainWindowProcedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	static streams::IOSocketStream<char> clientStream(new buffers::IOSocketBuffer<char>(new web::HTTPNetwork()));
	static UI::MainWindow* ptr = nullptr;
	static vector<wstring> filesNames;
	static wstring login;

	switch (msg)
	{
	case WM_KEYUP:
		if (wparam == VK_TAB)
		{
			try
			{
				SetFocus(dynamic_cast<UI::IIterable<HWND>&>(*ptr->getCurrentScreen()).next(GetFocus()));
			}
			catch (const bad_cast&)
			{

			}
		}

		return 0;

	case WM_COMMAND:
		switch (wparam)
		{
		case UI::buttons::refresh:
			getFiles(*ptr, clientStream, filesNames, true, login);

			break;

		case UI::buttons::download:
			downloadFile(*ptr, clientStream, filesNames, login);

			break;

		case UI::buttons::authorization:
			login = authorization(*ptr, clientStream);

			if (login.size())
			{
				initCloudStorageScreen(*ptr);

				ptr->getCurrentScreen()->pubShow();

				SendMessageW(ptr->getMainWindow(), UI::events::getFilesE, NULL, NULL);
			}

			break;

		case UI::buttons::toRegistrationScreen:
			initRegistrationScreen(*ptr);

			ptr->getCurrentScreen()->pubShow();

			break;

		case UI::buttons::registration:
			login = registration(*ptr, clientStream);

			if (login.size())
			{
				initCloudStorageScreen(*ptr);

				ptr->getCurrentScreen()->pubShow();

				SendMessageW(ptr->getMainWindow(), UI::events::getFilesE, NULL, NULL);
			}

			break;

		case UI::buttons::toAuthorizationScreen:
			initAuthorizationScreen(*ptr);

			ptr->getCurrentScreen()->pubShow();

			break;
		}

		return 0;

#pragma region CustomEvents
	case UI::events::getFilesE:
		getFiles(*ptr, clientStream, filesNames, false, login);

		return 0;

	case UI::events::uploadFileE:
		uploadFile(clientStream, *reinterpret_cast<vector<wstring>*>(wparam), login);

		return 0;

	case UI::events::downLoadFilesE:
		downloadFile(*ptr, clientStream, filesNames, login);

		return 0;

	case UI::events::initMainWindowPtrE:
		ptr = reinterpret_cast<UI::MainWindow*>(wparam);

		return 0;
#pragma endregion

	case WM_SIZE:
		if (ptr)
		{
			ptr->resize();
		}

		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);

		return 0;

	default:
		return DefWindowProcW(hwnd, msg, wparam, lparam);
	}
}
