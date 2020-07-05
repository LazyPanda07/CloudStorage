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

constexpr int nameColumnIndex = 0;
constexpr int dateColumnIndex = 1;
constexpr int typeColumnIndex = 2;
constexpr int sizeColumnIndex = 3;

LRESULT __stdcall MainWindowProcedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

LRESULT __stdcall DragAndDrop(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData);

void getFiles(HWND list, streams::IOSocketStream<char>& clientStream, bool showError);

void uploadFile(streams::IOSocketStream<char>& clientStream, const vector<wstring>& files);

void uploadFile(streams::IOSocketStream<char>& clientStream, const wstring& filePath);

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
			L"Обновить список файлов",
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
	static streams::IOSocketStream<char> clientStream(new buffers::IOSocketBuffer<char>(new web::HTTPNetwork()));

	switch (msg)
	{
	case WM_COMMAND:

		switch (wparam)
		{
		case UI::buttons::refresh:
			getFiles(list, clientStream, true);

			break;

		}

		return 0;

#pragma region CustomEvents
	case UI::events::getFiles:
		getFiles(list, clientStream, false);

		return 0;

	case UI::events::uploadFile:

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
			data[i].resize(DragQueryFileW(drop, i, nullptr, data[i].size()) + 1);

			DragQueryFileW(drop, i, data[i].data(), data[i].size());
		}

		SendMessageW(GetParent(hwnd), UI::events::uploadFile, reinterpret_cast<WPARAM>(&data), NULL);
	}

	return 0;

	default:
		return DefSubclassProc(hwnd, msg, wparam, lparam);
	}
}

void getFiles(HWND list, streams::IOSocketStream<char>& clientStream, bool showError)
{
	string request = web::HTTPBuilder().postRequest().headers
	(
		requestType::filesType, filesRequests::showAllFilesInDirectory,
		"Login", "Admin",
		"Directory", "Home"
	).build();
	string response;
	vector<wstring> data;

	utility::insertSizeHeaderToHTTPMessage(request);

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
			L"Ошибка",
			MB_OK
		);
	}
}

void uploadFile(streams::IOSocketStream<char>& clientStream, const vector<wstring>& files)
{
	for (const auto& i : files)
	{
		uploadFile(clientStream, i);
	}
}

void uploadFile(streams::IOSocketStream<char>& clientStream, const wstring& filePath)
{
	const filesystem::path file(filePath);
	uintmax_t fileSize = filesystem::file_size(file);

	string fileData(filePacketSize, '\0');
	string lastPacket;
	string response;
	bool isLast;

	ifstream in(file);

	do
	{
		const string* data;
		uintmax_t offset = in.tellg();

		if (fileSize - offset >= filePacketSize)
		{
			in.read(fileData.data(), fileData.size());

			data = &fileData;
			isLast = false;
		}
		else
		{
			lastPacket.resize(fileSize - offset);

			in.read(lastPacket.data(), lastPacket.size());

			data = &lastPacket;
			isLast = true;
		}

		string message = web::HTTPBuilder().postRequest().headers
		(
			requestType::filesType, filesRequests::uploadFile,
			"Login", "Admin",
			"Directory", "Home",
			"File-Name", file.filename().string(),
			"Range", offset,
			"Content-Range", data->size(),
			isLast ? "Total-File-Size" : "Reserved", isLast ? fileSize : 0
		).build(data);

		utility::insertSizeHeaderToHTTPMessage(message);

		clientStream << message;

	} while (!isLast);

	in.close();

	clientStream >> response;
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

	columns[0].pszText = const_cast<wchar_t*>(L"Имя");
	columns[1].pszText = const_cast<wchar_t*>(L"Дата изменения");
	columns[2].pszText = const_cast<wchar_t*>(L"Тип");
	columns[3].pszText = const_cast<wchar_t*>(L"Размер");

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