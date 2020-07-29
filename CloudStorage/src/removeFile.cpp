#include "pch.h"

#include "removeFile.h"
#include "getFiles.h"
#include "PopupWindows/PopupWindowFunctions.h"
#include "UIConstants.h"
#include "Screens/ScreenFunctions.h"
#include "HTTPBuilder.h"
#include "Constants.h"
#include "UtilityFunctions.h"
#include "ErrorHandling.h"
#include "HTTPParser.h"

#include <CommCtrl.h>

using namespace std;

void removeFile(UI::MainWindow& ref, streams::IOSocketStream<char>& clientStream, const wstring& fileName, bool& isCancel);

void removeFile(UI::MainWindow& ref, streams::IOSocketStream<char>& clientStream, vector<db::fileDataRepresentation>& fileNames, bool& isCancel)
{
	initWaitResponsePopupWindow(ref);

	thread(asyncRemoveFile, std::ref(ref), std::ref(clientStream), std::ref(fileNames), std::ref(isCancel)).detach();
}

void asyncRemoveFile(UI::MainWindow& ref, streams::IOSocketStream<char>& clientStream, vector<db::fileDataRepresentation>& fileNames, bool& isCancel)
{
	int id = SendMessageW(ref.getList(), LVM_GETNEXTITEM, -1, LVNI_SELECTED);

	while (id != -1)
	{
		if (removeFileDialog(ref, fileNames[id].fileName))
		{
			removeFile(ref, clientStream, fileNames[id].fileName, isCancel);
		}

		id = SendMessageW(ref.getList(), LVM_GETNEXTITEM, id, LVNI_SELECTED);
	}

	getFiles(ref, clientStream, fileNames, false, isCancel, false);

	SendMessageW(ref.getMainWindow(), UI::events::deletePopupWindowE, NULL, NULL);
}

void removeFile(UI::MainWindow& ref, streams::IOSocketStream<char>& clientStream, const wstring& fileName, bool& isCancel)
{
	string request = web::HTTPBuilder().postRequest().headers
	(
		requestType::filesType, filesRequests::removeFile,
		"File-Name", utility::conversion::to_string(fileName)
	).build();
	string response;
	isCancel = false;

	utility::web::insertSizeHeaderToHTTPMessage(request);

	if (isCancel)
	{
		return;
	}

	try
	{
		clientStream << request;
	}
	catch (const web::WebException&)
	{
		UI::serverRequestError(ref);
		return;
	}

	try
	{
		clientStream >> response;
	}
	catch (const web::WebException&)
	{
		UI::serverResponseError(ref);
		return;
	}

	web::HTTPParser parser(response);
	const map<string, string>& headers = parser.getHeaders();

	if (headers.at("Error") == "0")
	{
		//TODO: success message
	}
	else
	{
		if (ref.getCurrentPopupWindow())
		{
			MessageBoxW(ref.getPopupWindow(), wstring(L"Не удалось удалить " + fileName).data(), L"Ошибка", MB_OK);
		}
	}
}