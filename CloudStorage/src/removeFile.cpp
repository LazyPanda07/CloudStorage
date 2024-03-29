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

void removeFile(UI::MainWindow& ref, unique_ptr<streams::IOSocketStream>& clientStream, vector<db::fileDataRepresentation>& fileNames, bool& isCancel)
{
	initWaitResponsePopupWindow(ref);

	thread(asyncRemoveFile, std::ref(ref), std::ref(clientStream), std::ref(fileNames), std::ref(isCancel)).detach();
}

void removeFile(UI::MainWindow& ref, unique_ptr<streams::IOSocketStream>& clientStream, const wstring& fileName, bool& isCancel, bool showError)
{
	if (!clientStream)
	{
		if (ref.getCurrentPopupWindow())
		{
			SendMessageW(ref.getMainWindow(), UI::events::deletePopupWindowE, NULL, NULL);
		}

		UI::noConnectionWithServer(ref);

		return;
	}

	string request = web::HTTPBuilder().postRequest().headers
	(
		requestType::filesType, filesRequests::removeFile,
		"File-Name", utility::conversion::to_string(fileName)
	).build();
	string response;
	isCancel = false;

	if (isCancel)
	{
		return;
	}

	try
	{
		*clientStream << request;
	}
	catch (const web::exceptions::WebException&)
	{
		UI::serverRequestError(ref);
		return;
	}

	try
	{
		*clientStream >> response;
	}
	catch (const web::exceptions::WebException&)
	{
		UI::serverResponseError(ref);
		return;
	}

	web::HTTPParser parser(response);
	const auto& headers = parser.getHeaders();

	if (headers.at("Error") == "0")
	{
		//TODO: success message
	}
	else
	{
		if (ref.getCurrentPopupWindow() && showError)
		{
			MessageBoxW(ref.getPopupWindow(), wstring(L"�� ������� ������� " + fileName).data(), L"������", MB_OK);
		}
	}
}

void asyncRemoveFile(UI::MainWindow& ref, unique_ptr<streams::IOSocketStream>& clientStream, vector<db::fileDataRepresentation>& fileNames, bool& isCancel)
{
	LRESULT id = SendMessageW(ref.getList(), LVM_GETNEXTITEM, -1, LVNI_SELECTED);

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