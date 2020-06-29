#include "pch.h"

#include "MainWindow.h"

using namespace std;

enum ButtonsEvents
{
	getFilesE = 0xfff
};

LRESULT __stdcall MainWindowProcedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

void getFiles(HWND list);

namespace UI
{
	MainWindow::MainWindow()
	{
		WNDCLASSEXW wndClass = {};
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
			CW_USEDEFAULT, CW_USEDEFAULT,
			CW_USEDEFAULT, CW_USEDEFAULT,
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
			HMENU(getFilesE),
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
		case getFilesE:
			getFiles(list);

			break;

		}

		return 0;

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

	//SendMessageW(list, LB_ADDSTRING, NULL, reinterpret_cast<LPARAM>(files[i].data()));
}