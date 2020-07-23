#include "pch.h"

#include "IOSocketStream.h"
#include "HTTPNetwork.h"
#include "UtilityFunctions.h"
#include "NetworkFunctions.h"
#include "Screens/ScreenFunctions.h"
#include "Screens/AuthorizationScreen.h"
#include "Screens/RegistrationScreen.h"
#include "Screens/CloudStorageScreen.h"
#include "PopupWindows/UploadFilePopupWindow.h"
#include "PopupWindows/DownloadFilePopupWindow.h"
#include "fileData.h"
#include "UIConstants.h"
#include "MainWindow.h"
#include "Log.h"

#include <CommCtrl.h>

#pragma comment (lib, "HTTP.lib")
#pragma comment (lib, "SocketStreams.lib")
#pragma comment (lib, "Log.lib")

#pragma comment (lib, "Comctl32.lib")

#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

using namespace std;

LRESULT __stdcall MainWindowProcedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

namespace UI
{
	MainWindow::MainWindow() :
		currentScreen(nullptr)
	{
		INITCOMMONCONTROLSEX init = {};
		WNDCLASSEXW wndClass = {};
		POINT centerOfDesktop = utility::centerCoordinates(UI::mainWindowUI::mainWindowWidth, UI::mainWindowUI::mainWindowHeight);

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
			centerOfDesktop.x,
			centerOfDesktop.y,
			UI::mainWindowUI::mainWindowWidth,
			UI::mainWindowUI::mainWindowHeight,
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

		case UI::MainWindow::elementsEnum::wrapper:
			return currentScreen->getWrapper();

		case UI::MainWindow::elementsEnum::refreshButton:
			return static_cast<CloudStorageScreen*>(currentScreen.get())->getRefreshButton();

		case UI::MainWindow::elementsEnum::downloadButton:
			return static_cast<CloudStorageScreen*>(currentScreen.get())->getDownloadButton();

		case UI::MainWindow::elementsEnum::removeButton:
			return static_cast<CloudStorageScreen*>(currentScreen.get())->getRemoveButton();

		case UI::MainWindow::elementsEnum::reconnectButton:
			return static_cast<CloudStorageScreen*>(currentScreen.get())->getReconnectButton();

		case UI::MainWindow::elementsEnum::list:
			return static_cast<CloudStorageScreen*>(currentScreen.get())->getList();

		case UI::MainWindow::elementsEnum::authorizationLoginEdit:
			return static_cast<AuthorizationScreen*>(currentScreen.get())->getAuthorizationLoginEdit();

		case UI::MainWindow::elementsEnum::authorizationPasswordEdit:
			return static_cast<AuthorizationScreen*>(currentScreen.get())->getAuthorizationPasswordEdit();

		case UI::MainWindow::elementsEnum::registrationLoginEdit:
			return static_cast<RegistrationScreen*>(currentScreen.get())->getRegistrationLoginEdit();

		case UI::MainWindow::elementsEnum::registrationPasswordEdit:
			return static_cast<RegistrationScreen*>(currentScreen.get())->getRegistrationPasswordEdit();

		case UI::MainWindow::elementsEnum::registrationRepeatPasswordEdit:
			return static_cast<RegistrationScreen*>(currentScreen.get())->getRegistrationRepeatPasswordEdit();

		case UI::MainWindow::elementsEnum::popupWindow:
			return currentPopupWindow->getPopupWindow();

		case UI::MainWindow::elementsEnum::cancelButton:
			return currentPopupWindow->getCancelButton();

		case UI::MainWindow::elementsEnum::disableWindow:
			return currentPopupWindow->getDisableWindow();

		case UI::MainWindow::elementsEnum::uploadProgressBar:
			return static_cast<UI::UploadFilePopupWindow*>(currentPopupWindow.get())->getUploadProgressBar();

		case UI::MainWindow::elementsEnum::downloadProgressBar:
			return static_cast<UI::DownloadFilePopupWindow*>(currentPopupWindow.get())->getDownloadProgressBar();

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

	void MainWindow::setDownloadFolder(string&& path) noexcept
	{
		if (path == "Default")
		{
			downloadFolder = filesystem::current_path();
		}
		else
		{
			filesystem::path check(move(path));

			if (filesystem::exists(check))
			{
				downloadFolder = move(check);
			}
			else
			{
				downloadFolder = filesystem::current_path();
			}
		}
	}

	filesystem::path MainWindow::getDownloadFolder() noexcept
	{
		return downloadFolder;
	}

	BaseScreen* MainWindow::getCurrentScreen()
	{
		return currentScreen.get();
	}

	BasePopupWindow* MainWindow::getCurrentPopupWindow()
	{
		return currentPopupWindow.get();
	}

	void MainWindow::setCurrentScreen(BaseScreen* screen)
	{
		currentScreen.reset(screen);
	}

	void MainWindow::setCurrentPopupWindow(BasePopupWindow* popupWindow)
	{
		currentPopupWindow.reset(popupWindow);
	}

	void MainWindow::deleteCurrentPopupWindow()
	{
		currentPopupWindow.reset();
	}

	void MainWindow::resize()
	{
		if (currentScreen.get())
		{
			currentScreen->pubResize();
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

	HWND MainWindow::getRemoveButton() const
	{
		return this->getHWND(elementsEnum::removeButton);
	}

	HWND MainWindow::getReconnectButton() const
	{
		return this->getHWND(elementsEnum::reconnectButton);
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

	HWND MainWindow::getPopupWindow() const
	{
		return this->getHWND(elementsEnum::popupWindow);
	}

	HWND MainWindow::getCancelButton() const
	{
		return this->getHWND(elementsEnum::cancelButton);
	}

	HWND MainWindow::getDisableWindow() const
	{
		return this->getHWND(elementsEnum::disableWindow);
	}

	HWND MainWindow::getWrapper() const
	{
		return this->getHWND(elementsEnum::wrapper);
	}

	HWND MainWindow::getUploadProgressBar() const
	{
		return this->getHWND(elementsEnum::uploadProgressBar);
	}

	HWND MainWindow::getDownloadProgressBar() const
	{
		return this->getHWND(elementsEnum::downloadProgressBar);
	}
}

LRESULT __stdcall MainWindowProcedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
#pragma region Variables
	static streams::IOSocketStream<char> clientStream(new buffers::IOSocketBuffer<char>(new web::HTTPNetwork()));
	static UI::MainWindow* ptr = nullptr;
	static vector<db::fileDataRepresentation> fileNames;
	static wstring login;
	static wstring password;
	static vector<wstring> dragAndDropFiles;
	static size_t uploadFileIndex;
	static int downloadFileIndex;
	static bool isCancel;
#pragma endregion

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
				Log::info("User attempts to cast {} to IIterable\n", ptr->getCurrentScreen()->getName());
			}
		}

		return 0;

	case WM_COMMAND:
		switch (wparam)
		{
		case UI::buttons::refresh:
			getFiles(*ptr, clientStream, fileNames, true);

			break;

		case UI::buttons::download:
			SendMessageW(ptr->getMainWindow(), UI::events::downloadFileE, NULL, NULL);

			break;

		case UI::buttons::remove:
			removeFile(*ptr, clientStream, fileNames, login);

			SendMessageW(ptr->getMainWindow(), UI::events::getFilesE, NULL, NULL);

			break;

		case UI::buttons::authorization:
			tie(login, password) = authorization(*ptr, clientStream, move(login), move(password));

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
			tie(login, password) = registration(*ptr, clientStream);

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

		case UI::buttons::reconnect:
			reconnect(*ptr, clientStream);

			SendMessageW(ptr->getMainWindow(), WM_COMMAND, UI::buttons::authorization, NULL);

			SendMessageW(ptr->getMainWindow(), UI::events::getFilesE, NULL, NULL);

			break;

		case UI::BasePopupWindow::cancel:
			SendMessageW(ptr->getMainWindow(), UI::events::deletePopupWindowE, NULL, NULL);

			break;
		}

		return 0;

#pragma region CustomEvents
	case UI::events::getFilesE:
		getFiles(*ptr, clientStream, fileNames, false);

		return 0;

	case UI::events::multipleUploadE:
		if (uploadFileIndex != dragAndDropFiles.size())
		{
			uploadFile(*ptr, clientStream, dragAndDropFiles[uploadFileIndex++], login, isCancel);
		}

		return 0;

	case UI::events::multipleDownloadE:
		if (downloadFileIndex != -1)
		{
			downloadFileIndex = downloadFile(*ptr, clientStream, fileNames, login, isCancel, downloadFileIndex);
		}

		return 0;

	case UI::events::uploadFileE:
		uploadFileIndex = 0;
		dragAndDropFiles = move(*reinterpret_cast<vector<wstring>*>(wparam));

		uploadFile(*ptr, clientStream, dragAndDropFiles[uploadFileIndex++], login, isCancel);

		return 0;

	case UI::events::downloadFileE:
		downloadFileIndex = -1;

		downloadFileIndex = downloadFile(*ptr, clientStream, fileNames, login, isCancel, downloadFileIndex);

		return 0;

	case UI::events::initMainWindowPtrE:
		ptr = reinterpret_cast<UI::MainWindow*>(wparam);

		return 0;

	case UI::events::deletePopupWindowE:
		isCancel = true;

		ptr->deleteCurrentPopupWindow();

		return 0;
#pragma endregion

	case WM_SIZE:
		if (ptr)
		{
			ptr->resize();
		}

		return 0;

	case WM_DESTROY:
		exitFromApplication(*ptr, clientStream);

		this_thread::sleep_for(0.6s);

		PostQuitMessage(0);

		return 0;

	default:
		return DefWindowProcW(hwnd, msg, wparam, lparam);
	}
}