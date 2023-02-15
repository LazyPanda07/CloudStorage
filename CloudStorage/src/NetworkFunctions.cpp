#include "pch.h"

#include "NetworkFunctions.h"
#include "PopupWindows/PopupWindowFunctions.h"
#include "Constants.h"
#include "UtilityFunctions.h"
#include "HTTPBuilder.h"
#include "UIConstants.h"
#include "AsyncNetworkFunctions.h"

#include <CommCtrl.h>

using namespace std;

void setPath(UI::MainWindow& ref, unique_ptr<streams::IOSocketStream>& clientStream, string&& path, vector<db::fileDataRepresentation>& fileNames, bool& isCancel);

void uploadFile(UI::MainWindow& ref, unique_ptr<streams::IOSocketStream>& clientStream, const wstring& filePath, vector<db::fileDataRepresentation>& fileNames, bool& isCancel)
{
	const wstring fileName = wstring(begin(filePath) + filePath.rfind('\\') + 1, end(filePath));
	bool sameFileName = false;
	int choose = IDYES;

	for (const auto& i : fileNames)
	{
		if (fileName == i.fileName)
		{
			sameFileName = true;

			break;
		}
	}

	if (sameFileName)
	{
		choose = MessageBoxW(ref.getMainWindow(), wstring(L"Заменить файл " + fileName + L'?').data(), L"Замена файла", MB_YESNO);
	}

	if (choose == IDYES)
	{
		wstring message = L"Загрузка файла " + fileName;

		initUploadFilePopupWindow(ref, message);

		sameFileName ?
			thread(asyncUploadFile, std::ref(ref), std::ref(clientStream), std::ref(filePath), std::ref(fileNames), std::ref(isCancel), true).detach() :
			thread(asyncUploadFile, std::ref(ref), std::ref(clientStream), std::ref(filePath), std::ref(fileNames), std::ref(isCancel), false).detach();
	}
	else
	{
		SendMessageW(ref.getMainWindow(), UI::events::multipleUploadE, NULL, NULL);
	}
}

int downloadFile(UI::MainWindow& ref, unique_ptr<streams::IOSocketStream>& clientStream, const vector<db::fileDataRepresentation>& fileNames, bool& isCancel, int searchId)
{
	int id = SendMessageW(ref.getList(), LVM_GETNEXTITEM, searchId, LVNI_SELECTED);

	if (id != -1)
	{
		initDownloadFilePopupWindow(ref, wstring(L"Загрузка файла ") + fileNames[id].fileName);

		thread(asyncDownloadFile, std::ref(ref), std::ref(clientStream), std::ref(fileNames[id].fileName), fileNames[id].fileSize, std::ref(isCancel)).detach();
	}

	return id;
}

void reconnect(UI::MainWindow& ref, unique_ptr<streams::IOSocketStream>& clientStream, string&& currentPath, const wstring& login, const wstring& password, vector<db::fileDataRepresentation>& fileNames, bool& isCancel)
{
	initWaitResponsePopupWindow(ref);

	thread(asyncReconnect, std::ref(ref), std::ref(clientStream), move(currentPath), std::ref(login), std::ref(password), std::ref(fileNames), std::ref(isCancel)).detach();
}

void nextFolder(UI::MainWindow& ref, unique_ptr<streams::IOSocketStream>& clientStream, const wstring& folderName, vector<db::fileDataRepresentation>& fileNames, bool& isCancel)
{
	initWaitResponsePopupWindow(ref);

	thread(asyncFolderControlMessages, std::ref(ref), std::ref(clientStream), std::ref(controlRequests::nextFolder), std::ref(fileNames), std::ref(isCancel), utility::conversion::to_string(folderName)).detach();
}

void prevFolder(UI::MainWindow& ref, unique_ptr<streams::IOSocketStream>& clientStream, filesystem::path& currentPath, vector<db::fileDataRepresentation>& fileNames, bool& isCancel)
{
	if (currentPath != "Home")
	{
		const string path = currentPath.string();

		currentPath = string(begin(path), begin(path) + path.rfind('\\'));
	}

	initWaitResponsePopupWindow(ref);

	thread(asyncFolderControlMessages, std::ref(ref), std::ref(clientStream), std::ref(controlRequests::prevFolder), std::ref(fileNames), std::ref(isCancel), "").detach();
}

void createFolder(UI::MainWindow& ref, unique_ptr<streams::IOSocketStream>& clientStream, vector<db::fileDataRepresentation>& fileNames, bool& isCancel)
{
	wstring wFolderName;
	HWND enterFolderNameEdit = ref.getEnterFolderNameEdit();

	wFolderName.resize(GetWindowTextLengthW(enterFolderNameEdit));

	GetWindowTextW(enterFolderNameEdit, wFolderName.data(), wFolderName.size() + 1);

	initWaitResponsePopupWindow(ref);

	thread(asyncCreateFolder, std::ref(ref), std::ref(clientStream), move(wFolderName), std::ref(fileNames), std::ref(isCancel)).detach();
}

void exitFromApplication(UI::MainWindow& ref, unique_ptr<streams::IOSocketStream>& clientStream)
{
	if (!clientStream)
	{
		return;
	}

	string request = web::HTTPBuilder().postRequest().headers
	(
		requestType::exitType, networkRequests::exit
	).build();

	utility::web::insertSizeHeaderToHTTPMessage(request);

	try
	{
		*clientStream << request;
	}
	catch (const web::exceptions::WebException&)
	{

	}
}

void authorization(UI::MainWindow& ref, unique_ptr<streams::IOSocketStream>& clientStream, wstring& login, wstring& password, bool& isCancel)
{
	initWaitResponsePopupWindow(ref);

	thread(asyncAuthorization, std::ref(ref), std::ref(clientStream), std::ref(login), std::ref(password), std::ref(isCancel)).detach();
}

void registration(UI::MainWindow& ref, unique_ptr<streams::IOSocketStream>& clientStream, wstring& login, wstring& password, bool& isCancel)
{
	initWaitResponsePopupWindow(ref);

	thread(asyncRegistration, std::ref(ref), std::ref(clientStream), std::ref(login), std::ref(password), std::ref(isCancel)).detach();
}

void firstConnect(UI::MainWindow& ref, unique_ptr<streams::IOSocketStream>& clientStream, bool& isCancel)
{
	initWaitResponsePopupWindow(ref);

	thread(asyncFirstConnect, std::ref(ref), std::ref(clientStream), std::ref(isCancel)).detach();
}