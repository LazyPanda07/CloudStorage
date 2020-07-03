#include "pch.h"

#include "HTTPNetwork.h"
#include "MainWindow.h"
#include "HTTPBuilder.h"
#include "HTTPParser.h"
#include "IOSocketStream.h"
#include "UtilityFunction.h"
#include "UIConstants.h"

#include <Richedit.h>
#include <commctrl.h>

#pragma comment (lib, "Comctl32.lib")

#pragma comment (lib, "HTTP.lib")
#pragma comment (lib, "SocketStreams.lib")
#pragma comment (lib, "Log.lib")

#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

using namespace std;

constexpr int_fast32_t nameColumnIndex = 0;
constexpr int_fast32_t dateColumnIndex = 1;
constexpr int_fast32_t typeColumnIndex = 2;
constexpr int_fast32_t sizeColumnIndex = 3;

LRESULT __stdcall MainWindowProcedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

LRESULT __stdcall DragAndDrop(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData);

void getFiles(HWND list, bool showError);

void createColumns(HWND list);

void updateNameColumn(HWND list, const vector<wstring>& data);

namespace UI
{
	MainWindow::MainWindow()
	{
		WNDCLASSEXW wndClass = {};
		INITCOMMONCONTROLSEX init;
		init.dwICC = ICC_LISTVIEW_CLASSES;
		POINT monitorCenter = utility::centerCoordinates(mainWindowUI::mainWindowWidth, mainWindowUI::mainWindowHeight);
		wndClass.cbSize = sizeof(WNDCLASSEXW);
		wndClass.lpfnWndProc = MainWindowProcedure;
		wndClass.hbrBackground = CreateSolidBrush(RGB(255, 255, 255));
		wndClass.hCursor = LoadCursorW(nullptr, IDC_ARROW);
		wndClass.lpszClassName = L"MainWindow";
		wndClass.hIcon = LoadIconW(nullptr, IDI_APPLICATION);

		RegisterClassExW(&wndClass);
		InitCommonControlsEx(&init);

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

		RECT mainWindowSizes;

		GetClientRect(mainWindow, &mainWindowSizes);

		const LONG width = mainWindowSizes.right - mainWindowSizes.left;
		const LONG height = mainWindowSizes.bottom - mainWindowSizes.top;

		refreshButton = CreateWindowExW
		(
			NULL,
			L"BUTTON",
			L"�������� ������ ������",
			WS_CHILDWINDOW | WS_VISIBLE,
			0, 0,
			toolbar::toolbarButtonWidth, toolbar::toolbarButtonHeight,
			mainWindow,
			HMENU(buttons::refresh),
			nullptr,
			nullptr
		);

		list = CreateWindowExW
		(
			WS_EX_CLIENTEDGE,
			WC_LISTVIEW,
			nullptr,
			WS_CHILDWINDOW | WS_VISIBLE | LVS_EDITLABELS | LVS_REPORT,
			0, toolbar::toolbarHeight,
			width, height - toolbar::toolbarHeight,
			mainWindow,
			HMENU(),
			nullptr,
			nullptr
		);

		DragAcceptFiles(list, true);
		SetWindowSubclass(list, &DragAndDrop, 1, 0);

		createColumns(list);

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

	HWND MainWindow::getHWND() const
	{
		return mainWindow;
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
			getFiles(list, true);

			break;

		}

		return 0;

#pragma region CustomEvents
	case UI::events::getFiles:
		getFiles(list, false);

		return 0;

	case UI::events::uploadFiles:

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

LRESULT __stdcall DragAndDrop(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
{
	switch (msg)
	{
	case WM_DROPFILES:
	{
		vector<wstring> data;
		HDROP drop = reinterpret_cast<HDROP>(wparam);

		UINT count = DragQueryFileW(drop, 0xFFFFFFFF, nullptr, NULL);

		data.resize(count);

		for (size_t i = 0; i < count; i++)
		{
			data[i].resize(DragQueryFileW(drop, i, nullptr, data[i].size()));

			DragQueryFileW(drop, i, data[i].data(), data[i].size());
		}

		SendMessageW(GetParent(hwnd), UI::events::uploadFiles, reinterpret_cast<WPARAM>(&data), NULL);
	}

	return 0;

	default:
		return DefSubclassProc(hwnd, msg, wparam, lparam);
	}
}

void getFiles(HWND list, bool showError)
{
	streams::IOSocketStream<char> clientStream(new buffers::IOSocketBuffer<char>(new web::HTTPNetwork()));
	string request = web::HTTPBuilder().postRequest().headers
	(
		typeRequests::filesType, filesRequests::showAllFilesInDirectory,
		"Login", "Admin",
		"Directory", "Home"
	).build();
	string response;
	vector<wstring> data;

	clientStream << request;
	clientStream >> response;

	clientStream << responses::okResponse;

	web::HTTPParser parser(response);
	const string& error = parser.getHeaders().at("Error");
	const string& body = parser.getBody();

	if (error == "0")
	{
		string tem;

		for (const auto& i : body)
		{
			if (i == ':')
			{
				data.push_back(utility::to_wstring(tem));

				tem.clear();
			}
			else
			{
				tem += i;
			}
		}
	}

	updateNameColumn(list, data);

	if (error == "1" && showError)
	{
		MessageBoxW
		(
			GetParent(list),
			utility::to_wstring(body).data(),
			L"������",
			MB_OK
		);
	}
}

void createColumns(HWND list)
{
	HWND parent = GetParent(list);
	array<LVCOLUMNW, UI::mainWindowUI::columnsInList> columns = {};
	RECT mainWindowSizes;
	RECT listSizes;

	GetClientRect(parent, &mainWindowSizes);
	GetClientRect(list, &listSizes);

	LONG width = mainWindowSizes.right - mainWindowSizes.left;

	array<LONG, UI::mainWindowUI::columnsInList> columnsSizes = { width * 0.4, width * 0.2, width * 0.2, width * 0.2 };

	columns[0].pszText = const_cast<wchar_t*>(L"���");
	columns[1].pszText = const_cast<wchar_t*>(L"���� ���������");
	columns[2].pszText = const_cast<wchar_t*>(L"���");
	columns[3].pszText = const_cast<wchar_t*>(L"������");

	for (size_t i = 0; i < columns.size(); i++)
	{
		columns[i].mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
		columns[i].iSubItem = i;
		columns[i].fmt = LVCFMT_LEFT;
		columns[i].cx = columnsSizes[i];

		SendMessageW(list, LVM_INSERTCOLUMN, static_cast<WPARAM>(i), reinterpret_cast<LPARAM>(&columns[i]));
	}

	InvalidateRect(list, &listSizes, TRUE);
}

void updateNameColumn(HWND list, const vector<wstring>& data)
{
	bool success = false;

	do
	{
		LVITEMW item = {};
		item.iSubItem = nameColumnIndex;

		success = SendMessageW(list, LVM_GETITEMW, NULL, reinterpret_cast<LPARAM>(&item));

		SendMessageW(list, LVM_DELETEITEM, item.iItem, NULL);
	} while (success);

	LVITEMW lvi = {};

	RECT rect;

	GetClientRect(list, &rect);

	lvi.mask = LVIF_TEXT;

	for (size_t i = 0; i < data.size(); i++)
	{
		lvi.pszText = const_cast<wchar_t*>(data[i].data());
		lvi.iItem = i;
		lvi.iSubItem = nameColumnIndex;

		SendMessageW(list, LVM_INSERTITEM, NULL, reinterpret_cast<LPARAM>(&lvi));
	}

	InvalidateRect(list, &rect, TRUE);
}