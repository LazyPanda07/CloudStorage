#include "pch.h"

#include "getFiles.h"
#include "PopupWindows/PopupWindowFunctions.h"
#include "PopupWindows/WaitResponsePopupWindow.h"
#include "ClientTime.h"
#include "HTTPBuilder.h"
#include "Constants.h"
#include "UtilityFunctions.h"
#include "UIConstants.h"
#include "ErrorHandling.h"
#include "HTTPParser.h"
#include "Screens/ScreenFunctions.h"

using namespace std;

void asyncGetFiles(UI::MainWindow& ref, unique_ptr<streams::IOSocketStream<char>>& clientStream, vector<db::fileDataRepresentation>& fileNames, bool showError, bool& isCancel);

void getFiles(UI::MainWindow& ref, unique_ptr<streams::IOSocketStream<char>>& clientStream, vector<db::fileDataRepresentation>& fileNames, bool showError, bool& isCancel, bool isDetach)
{
	if (isDetach)
	{
		initWaitResponsePopupWindow(ref);
	}

	thread otherThread(asyncGetFiles, std::ref(ref), std::ref(clientStream), std::ref(fileNames), showError, std::ref(isCancel));

	if (isDetach)
	{
		otherThread.detach();
	}
	else
	{
		otherThread.join();
	}
}

void asyncGetFiles(UI::MainWindow& ref, unique_ptr<streams::IOSocketStream<char>>& clientStream, vector<db::fileDataRepresentation>& fileNames, bool showError, bool& isCancel)
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

	utility::ClientTime& instance = utility::ClientTime::get();
	string request = web::HTTPBuilder().postRequest().headers
	(
		requestType::filesType, filesRequests::showAllFilesInFolder
	).build();
	string response;
	int ok;

	isCancel = false;

	fileNames.clear();

	utility::web::insertSizeHeaderToHTTPMessage(request);

	if (isCancel)
	{
		if (ref.getCurrentPopupWindow())
		{
			ref.getCurrentPopupWindow()->showPopupWindowVar() = false;
		}
		return;
	}

	try
	{
		*clientStream << request;
	}
	catch (const web::WebException&)
	{
		if (UI::serverRequestError(ref) == IDOK)
		{
			SendMessageW(ref.getMainWindow(), UI::events::deletePopupWindowE, NULL, NULL);
		}

		return;
	}

	try
	{
		*clientStream >> response;
	}
	catch (const web::WebException&)
	{
		if (UI::serverResponseError(ref) == IDOK)
		{
			SendMessageW(ref.getMainWindow(), UI::events::deletePopupWindowE, NULL, NULL);
		}

		return;
	}

	if (isCancel)
	{
		if (ref.getCurrentPopupWindow())
		{
			ref.getCurrentPopupWindow()->showPopupWindowVar() = false;
		}
		return;
	}

	web::HTTPParser parser(response);
	const string& error = parser.getHeaders().at("Error");
	const string& body = parser.getBody();

	if (ref.getCurrentPopupWindow())
	{
		static_cast<UI::WaitResponsePopupWindow*>(ref.getCurrentPopupWindow())->stopAnimateProgressBar();
	}

	if (error == "0")
	{
		array<string, 6> tem;	//each index equals member position in fileData struct
		vector<db::fileData> data;
		size_t curIndex = 0;

		for (const auto& i : body)
		{
			if (i == dataDelimiter[0])
			{
				fileNames.emplace_back
				(
					utility::conversion::to_wstring(tem[0]),
					utility::conversion::to_wstring(tem[1]),
					utility::conversion::to_wstring(tem[2]),
					instance.convertToLocal(tem[3]),
					instance.convertToLocal(tem[4]),
					stoul(tem[5])
				);

				curIndex = 0;

				for (auto& i : tem)
				{
					i.clear();
				}
			}
			else if (i == dataPartDelimiter[0])
			{
				curIndex++;
			}
			else
			{
				tem[curIndex] += i;
			}
		}
	}

	updateColumns(ref, fileNames);

	if (error == "1" && showError)
	{
		ok = MessageBoxW
		(
			ref.getPopupWindow(),
			utility::conversion::to_wstring(body).data(),
			L"Ошибка",
			MB_OK
		);
	}
	else
	{
		ok = IDOK;
	}

	if (ok == IDOK)
	{
		SendMessageW(ref.getMainWindow(), UI::events::deletePopupWindowE, NULL, NULL);
	}
}