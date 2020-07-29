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

void setPath(UI::MainWindow& ref, streams::IOSocketStream<char>& clientStream, string&& path, vector<db::fileDataRepresentation>& fileNames, bool& isCancel);

void uploadFile(UI::MainWindow& ref, streams::IOSocketStream<char>& clientStream, const wstring& filePath, vector<db::fileDataRepresentation>& fileNames, bool& isCancel)
{
	wstring message = L"Загрузка файла " + wstring(begin(filePath) + filePath.rfind('\\') + 1, end(filePath));

	initUploadFilePopupWindow(ref, message);

	thread(asyncUploadFile, std::ref(ref), std::ref(clientStream), std::ref(filePath), std::ref(fileNames), std::ref(isCancel)).detach();
}

int downloadFile(UI::MainWindow& ref, streams::IOSocketStream<char>& clientStream, const vector<db::fileDataRepresentation>& fileNames, bool& isCancel, int searchId)
{
	int id = SendMessageW(ref.getList(), LVM_GETNEXTITEM, searchId, LVNI_SELECTED);

	if (id != -1)
	{
		initDownloadFilePopupWindow(ref, wstring(L"Загрузка файла ") + fileNames[id].fileName);

		thread(asyncDownloadFile, std::ref(ref), std::ref(clientStream), std::ref(fileNames[id].fileName), fileNames[id].fileSize, std::ref(isCancel)).detach();
	}

	return id;
}

void reconnect(UI::MainWindow& ref, streams::IOSocketStream<char>& clientStream, string&& currentPath, const wstring& login, const wstring& password, vector<db::fileDataRepresentation>& fileNames, bool& isCancel)
{
	initWaitResponsePopupWindow(ref);

	thread(asyncReconnect, std::ref(ref), std::ref(clientStream), move(currentPath), std::ref(login), std::ref(password), std::ref(fileNames), std::ref(isCancel)).detach();
}

void nextFolder(UI::MainWindow& ref, streams::IOSocketStream<char>& clientStream, const wstring& folderName, vector<db::fileDataRepresentation>& fileNames, bool& isCancel)
{
	initWaitResponsePopupWindow(ref);

	thread(asyncFolderControlMessages, std::ref(ref), std::ref(clientStream), std::ref(controlRequests::nextFolder), std::ref(fileNames), std::ref(isCancel), utility::conversion::to_string(folderName)).detach();
}

void prevFolder(UI::MainWindow& ref, streams::IOSocketStream<char>& clientStream, filesystem::path& currentPath, vector<db::fileDataRepresentation>& fileNames, bool& isCancel)
{
	if (currentPath != "Home")
	{
		const string path = currentPath.string();

		currentPath = string(begin(path), begin(path) + path.rfind('\\'));
	}

	initWaitResponsePopupWindow(ref);

	thread(asyncFolderControlMessages, std::ref(ref), std::ref(clientStream), std::ref(controlRequests::prevFolder), std::ref(fileNames), std::ref(isCancel), "").detach();
}

void createFolder(UI::MainWindow& ref, streams::IOSocketStream<char>& clientStream, vector<db::fileDataRepresentation>& fileNames, bool& isCancel)
{
	wstring wFolderName;
	HWND enterFolderNameEdit = ref.getEnterFolderNameEdit();
	string request;
	string body;

	wFolderName.resize(GetWindowTextLengthW(enterFolderNameEdit));

	GetWindowTextW(enterFolderNameEdit, wFolderName.data(), wFolderName.size() + 1);

	body = "folder=" + utility::conversion::to_string(wFolderName);

	request = web::HTTPBuilder().postRequest().headers
	(
		requestType::filesType, filesRequests::createFolder,
		"Content-Length", body.size()
	).build(&body);

	utility::web::insertSizeHeaderToHTTPMessage(request);

	try
	{
		clientStream << request;
	}
	catch (const web::WebException&)
	{

	}

	SendMessageW(ref.getMainWindow(), UI::events::deletePopupWindowE, NULL, NULL);

	getFiles(ref, clientStream, fileNames, true, isCancel);
}

void exitFromApplication(UI::MainWindow& ref, streams::IOSocketStream<char>& clientStream)
{
	string request = web::HTTPBuilder().postRequest().headers
	(
		requestType::exitType, networkRequests::exit
	).build();

	utility::web::insertSizeHeaderToHTTPMessage(request);

	try
	{
		clientStream << request;
	}
	catch (const web::WebException&)
	{

	}
}

void authorization(UI::MainWindow& ref, streams::IOSocketStream<char>& clientStream, wstring& login, wstring& password, bool& isCancel)
{
	initWaitResponsePopupWindow(ref);

	thread(asyncAuthorization, std::ref(ref), std::ref(clientStream), std::ref(login), std::ref(password), std::ref(isCancel)).detach();
}

void registration(UI::MainWindow& ref, streams::IOSocketStream<char>& clientStream, wstring& login, wstring& password, bool& isCancel)
{
	initWaitResponsePopupWindow(ref);

	thread(asyncRegistration, std::ref(ref), std::ref(clientStream), std::ref(login), std::ref(password), std::ref(isCancel)).detach();
}