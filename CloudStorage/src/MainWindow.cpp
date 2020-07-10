#include "pch.h"

#include "HTTPNetwork.h"
#include "MainWindow.h"
#include "HTTPBuilder.h"
#include "HTTPParser.h"
#include "IOSocketStream.h"
#include "UtilityFunctions.h"
#include "UIConstants.h"

#include "Screens/AuthorizationScreen.h"
#include "Screens/CloudStorageScreen.h"

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

LRESULT __stdcall MainWindowProcedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

void getFiles(UI::MainWindow& ref, streams::IOSocketStream<char>& clientStream, vector<wstring>& filesNames, bool showError);

void uploadFile(streams::IOSocketStream<char>& clientStream, const vector<wstring>& files);

void uploadFile(streams::IOSocketStream<char>& clientStream, const wstring& filePath);

void downloadFile(UI::MainWindow& ref, streams::IOSocketStream<char>& clientStream, const vector<wstring>& filesNames);

void downloadFile(streams::IOSocketStream<char>& clientStream, const wstring& fileName);

wstring authorization(UI::MainWindow& ref, streams::IOSocketStream<char>& clientStream);

void updateNameColumn(UI::MainWindow& ref, const vector<wstring>& data);

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

		currentScreen = new AuthorizationScreen(mainWindow, L"Authorization", MainWindowProcedure);

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

		case UI::MainWindow::elementsEnum::loginEdit:
			return static_cast<AuthorizationScreen*>(currentScreen)->getLoginEdit();

		case UI::MainWindow::elementsEnum::passwordEdit:
			return static_cast<AuthorizationScreen*>(currentScreen)->getPasswordEdit();

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

	HWND MainWindow::getLoginEdit() const
	{
		return this->getHWND(elementsEnum::loginEdit);
	}

	HWND MainWindow::getPasswordEdit() const
	{
		return this->getHWND(elementsEnum::passwordEdit);
	}
}

LRESULT __stdcall MainWindowProcedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	static streams::IOSocketStream<char> clientStream(new buffers::IOSocketBuffer<char>(new web::HTTPNetwork()));
	static UI::MainWindow* ptr = nullptr;
	static vector<wstring> filesNames;
	static UI::BaseScreen* currentScreen = nullptr;

	switch (msg)
	{
	case WM_COMMAND:

		switch (wparam)
		{
		case UI::buttons::refresh:
			getFiles(*ptr, clientStream, filesNames, true);

			break;

		case UI::buttons::download:
			downloadFile(*ptr, clientStream, filesNames);

			break;

		case UI::buttons::authorization:
			authorization(*ptr, clientStream);

			break;
		}

		return 0;

#pragma region CustomEvents
	case UI::events::getFilesE:
		getFiles(*ptr, clientStream, filesNames, false);

		return 0;

	case UI::events::uploadFileE:
		uploadFile(clientStream, *reinterpret_cast<vector<wstring>*>(wparam));

		return 0;

	case UI::events::downLoadFilesE:
		downloadFile(*ptr, clientStream, filesNames);

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

void getFiles(UI::MainWindow& ref, streams::IOSocketStream<char>& clientStream, vector<wstring>& filesNames, bool showError)
{
	string request = web::HTTPBuilder().postRequest().headers
	(
		requestType::filesType, filesRequests::showAllFilesInDirectory,
		"Login", "Admin",
		"Directory", "Home"
	).build();
	string response;
	filesNames.clear();

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
				filesNames.push_back(utility::to_wstring(tem));

				tem.clear();
			}
			else
			{
				tem += i;
			}
		}
	}

	updateNameColumn(ref, filesNames);

	if (error == "1" && showError)
	{
		MessageBoxW
		(
			ref.getMainWindow(),
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

	string fileData;
	string lastPacket;
	string response;
	bool isLast;

	ifstream in(file, ios::binary);

	fileData.resize(filePacketSize);

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
			"Content-Length", data->size(),
			isLast ? "Total-File-Size" : "Reserved", isLast ? fileSize : 0
		).build(data);

		utility::insertSizeHeaderToHTTPMessage(message);

		clientStream << message;

	} while (!isLast);

	in.close();

	clientStream >> response;

	web::HTTPParser parser(response);

	const string& error = parser.getHeaders().at("Error");

	if (error == "1")
	{
		MessageBoxW
		(
			FindWindowW(L"MainWindow", L"Cloud Storage"),
			utility::to_wstring(parser.getBody()).data(),
			L"Ошибка",
			MB_OK
		);
	}
	else
	{
		SendMessageW(FindWindowW(L"MainWindow", L"Cloud Storage"), UI::events::getFilesE, NULL, NULL);
	}
}

void downloadFile(UI::MainWindow& ref, streams::IOSocketStream<char>& clientStream, const vector<wstring>& filesNames)
{
	int id = SendMessageW(ref.getList(), LVM_GETNEXTITEM, -1, LVNI_SELECTED);

	while (id != -1)
	{
		downloadFile(clientStream, filesNames[id]);

		id = SendMessageW(ref.getList(), LVM_GETNEXTITEM, id, LVNI_SELECTED);
	}
}

void downloadFile(streams::IOSocketStream<char>& clientStream, const wstring& fileName)
{
	uintmax_t offset = 0;
	uintmax_t totalFileSize;
	const string sFileName = utility::to_string(fileName);
	bool lastPacket;
	string response;

	ofstream out(sFileName, ios::binary);

	while (true)
	{
		string request = web::HTTPBuilder().postRequest().headers
		(
			requestType::filesType, filesRequests::downloadFile,
			"Login", "Admin",
			"Directory", "Home",
			"File-Name", sFileName,
			"Range", offset
		).build();

		utility::insertSizeHeaderToHTTPMessage(request);

		clientStream << request;

		clientStream >> response;

		web::HTTPParser parser(response);
		const map<string, string>& headers = parser.getHeaders();
		auto it = headers.find("Total-File-Size");
		lastPacket = it != end(headers);
		const string& data = parser.getBody();

		out.write(data.data(), data.size());

		offset = out.tellp();

		if (lastPacket)
		{
			totalFileSize = stoull(it->second);
			break;
		}
	}

	out.close();

	if (filesystem::file_size(filesystem::current_path().append(sFileName)) == totalFileSize)
	{
		wstring message = fileName + L"\r\n" + filesResponses::successDownloadFile.data();

		MessageBoxW(nullptr, message.data(), L"Успех", MB_OK);
	}
	else
	{
		wstring message = fileName + L"\r\n" + filesResponses::failDownloadFile.data();

		MessageBoxW(nullptr, message.data(), L"Ошибка", MB_OK);
	}
}

wstring authorization(UI::MainWindow& ref, streams::IOSocketStream<char>& clientStream)
{
	wstring wLogin;
	wstring wPassword;
	string response;

	HWND loginEdit = ref.getLoginEdit();
	HWND passwordEdit = ref.getPasswordEdit();

	wLogin.resize(GetWindowTextLengthW(loginEdit));
	wPassword.resize(GetWindowTextLengthW(passwordEdit));

	GetWindowTextW(loginEdit, wLogin.data(), wLogin.size() + 1);
	GetWindowTextW(passwordEdit, wPassword.data(), wPassword.size() + 1);

	string login = utility::to_string(wLogin);
	string password = utility::to_string(wPassword);
	string body = "login=" + login + "&" + "password=" + password;

	string request = web::HTTPBuilder().postRequest().headers
	(
		requestType::accountType, accountRequest::authorization
	).build(&body);

	utility::insertSizeHeaderToHTTPMessage(request);

	clientStream << request;

	clientStream >> response;

	web::HTTPParser parser(response);

	if (parser.getHeaders().at("Error") == "0")
	{
		return wLogin;
	}
	else
	{
		MessageBoxW(nullptr, utility::to_wstring(parser.getBody()).data(), L"Ошибка", MB_OK);

		return wstring();
	}
}

void updateNameColumn(UI::MainWindow& ref, const vector<wstring>& data)
{
	bool success = false;

	do
	{
		LVITEMW item = {};
		item.iSubItem = UI::mainWindowUI::nameColumnIndex;

		success = SendMessageW(ref.getList(), LVM_GETITEMW, NULL, reinterpret_cast<LPARAM>(&item));

		SendMessageW(ref.getList(), LVM_DELETEITEM, item.iItem, NULL);
	} while (success);

	LVITEMW lvi = {};

	RECT rect;

	GetClientRect(ref.getList(), &rect);

	lvi.mask = LVIF_TEXT;

	for (size_t i = 0; i < data.size(); i++)
	{
		lvi.pszText = const_cast<wchar_t*>(data[i].data());
		lvi.iItem = i;
		lvi.iSubItem = UI::mainWindowUI::nameColumnIndex;

		SendMessageW(ref.getList(), LVM_INSERTITEM, NULL, reinterpret_cast<LPARAM>(&lvi));
	}

	InvalidateRect(ref.getList(), &rect, TRUE);
}