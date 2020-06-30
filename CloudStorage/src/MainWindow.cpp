#include "pch.h"

#include "HTTPNetwork.h"
#include "MainWindow.h"
#include "HTTPBuilder.h"
#include "HTTPParser.h"
#include "IOSocketStream.h"
#include "UtilityFunction.h"
#include "UIConstants.h"

#pragma comment (lib, "HTTP.lib")
#pragma comment (lib, "SocketStreams.lib")
#pragma comment (lib, "Log.lib")

using namespace std;

LRESULT __stdcall MainWindowProcedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

void getFiles(HWND list);

namespace UI
{
	MainWindow::MainWindow()
	{
		WNDCLASSEXW wndClass = {};
		POINT monitorCenter = utility::centerCoordinates(mainWindowUI::mainWindowWidth, mainWindowUI::mainWindowHeight);
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
			mainWindowUI::mainWindowWidth, mainWindowUI::mainWindowHeight,
			nullptr,
			HMENU(),
			nullptr,
			nullptr
		);

		getFilesButton = CreateWindowExW
		(
			NULL,
			L"BUTTON",
			L"Получить список файлов",
			WS_CHILDWINDOW | WS_VISIBLE,
			0, 0,
			200, 20,
			mainWindow,
			HMENU(buttons::refresh),
			nullptr,
			nullptr
		);

		list = CreateWindowExW
		(
			WS_EX_CLIENTEDGE,
			L"LISTBOX",
			nullptr,
			WS_CHILDWINDOW | WS_VISIBLE | LBS_SORT | LBS_EXTENDEDSEL | LBS_MULTIPLESEL | WS_VSCROLL | LBS_NOINTEGRALHEIGHT,
			400, 0,
			200, 500,
			mainWindow,
			HMENU(),
			nullptr,
			nullptr
		);

		SendMessageW(mainWindow, WM_CREATE, reinterpret_cast<WPARAM>(list), NULL);
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
}

LRESULT __stdcall MainWindowProcedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	static HWND list;

	switch (msg)
	{
	case WM_COMMAND:

		switch (wparam)
		{
		case UI::buttons::refresh:
			getFiles(list);

			break;

		}

		return 0;

#pragma region CustomEvents
	case UI::events::getFiles:
		getFiles(list);

		return 0;
	case UI::events::uploadFile:

		return 0;
	case UI::events::uploadFiles:

		return 0;
	case UI::events::downloadFile:

		return 0;
	case UI::events::downLoadFiles:

		return 0;
	case UI::events::networkEventsCount:

		return 0;
#pragma endregion

	case WM_CREATE:
		list = reinterpret_cast<HWND>(wparam);

		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);

		return 0;

	default:
		return DefWindowProcW(hwnd, msg, wparam, lparam);
	}
}

void getFiles(HWND list)
{
	streams::IOSocketStream<char> clientStream(new buffers::IOSocketBuffer<char>(new web::HTTPNetwork()));
	string request = web::HTTPBuilder().postRequest().headers
	(
		"Files request", "Show all files in directory",
		"Login", "Admin",
		"Directory", "Home"
	).build();
	string response;
	vector<string> data;

	clientStream << request;
	clientStream >> response;

	clientStream << responses::okResponse;

	web::HTTPParser parser(response);
	const string& error = parser.getHeaders().at("Error");
	const string& body = parser.getBody();

	if (error == "1")
	{
		MessageBoxW
		(
			GetParent(list),
			utility::to_wstring(body).data(),
			L"Ошибка",
			MB_OK
		);
	}
	else
	{
		string tem;

		for (const auto& i : body)
		{
			if (i == ':')
			{
				data.push_back(move(tem));
			}
			else
			{
				tem += i;
			}
		}

		for (const auto& i : data)
		{
			SendMessageW(list, LB_ADDSTRING, NULL, reinterpret_cast<LPARAM>(utility::to_wstring(i).data()));
		}
	}
}