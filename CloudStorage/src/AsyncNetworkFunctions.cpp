#include "pch.h"

#include "AsyncNetworkFunctions.h"
#include "HTTPBuilder.h"
#include "HTTPParser.h"
#include "UIConstants.h"
#include "Constants.h"
#include "UtilityFunctions.h"
#include "Screens/ScreenFunctions.h"
#include "PopupWindows/PopupWindowFunctions.h"
#include "PopupWindows/UploadFilePopupWindow.h"
#include "PopupWindows/DownloadFilePopupWindow.h"
#include "PopupWindows/WaitResponsePopupWindow.h"
#include "ClientTime.h"
#include "CustomHTTPNetwork.h"
#include "ErrorHandling.h"
#include "getFiles.h"
#include "removeFile.h"

#include <CommCtrl.h>

using namespace std;

void setPath(UI::MainWindow& ref, unique_ptr<streams::IOSocketStream>& clientStream, string&& path, vector<db::fileDataRepresentation>& fileNames, bool& isCancel);

void setLogin(UI::MainWindow& ref, unique_ptr<streams::IOSocketStream>& clientStream, const wstring& login, const wstring& password);

string cancelUploadFile(const string& fileName)
{
	string result = web::HTTPBuilder().postRequest().headers
	(
		requestType::cancelType, networkRequests::cancelOperation,
		"Operation-Type", filesRequests::uploadFile,
		"File-Name", fileName
	).build();

	return result;
}

string asyncFolderControlMessages(UI::MainWindow& ref, unique_ptr<streams::IOSocketStream>& clientStream, const string_view& controlRequest, vector<db::fileDataRepresentation>& fileNames, bool& isCancel, string&& data)
{
	if (!clientStream)
	{
		if (ref.getCurrentPopupWindow())
		{
			SendMessageW(ref.getMainWindow(), UI::events::deletePopupWindowE, NULL, NULL);
		}

		UI::noConnectionWithServer(ref);

		return "";
	}

	string request;
	string response;
	string body = "folder=" + move(data);
	isCancel = false;

	if (controlRequest == controlRequests::prevFolder)
	{
		request = web::HTTPBuilder().postRequest().headers
		(
			requestType::controlType, controlRequest
		).build();
	}
	else
	{
		request = web::HTTPBuilder().postRequest().headers
		(
			requestType::controlType, controlRequest
		).build(body);
	}

	if (isCancel)
	{
		if (ref.getCurrentPopupWindow())
		{
			ref.getCurrentPopupWindow()->showPopupWindowVar() = false;
		}
		return string();
	}

	try
	{
		*clientStream << request;
	}
	catch (const web::exceptions::WebException&)
	{

	}

	if (isCancel)
	{
		if (ref.getCurrentPopupWindow())
		{
			ref.getCurrentPopupWindow()->showPopupWindowVar() = false;
		}
		return string();
	}

	try
	{
		*clientStream >> response;
	}
	catch (const web::exceptions::WebException&)
	{

	}

	string responseBody = web::HTTPParser(response).getBody();

	if (responseBody == responses::okResponse)
	{
		getFiles(ref, clientStream, fileNames, false, isCancel, false);
	}
	else
	{
		if (ref.getCurrentPopupWindow())
		{
			ref.getCurrentPopupWindow()->showPopupWindowVar() = false;

			SendMessageW(ref.getMainWindow(), UI::events::deletePopupWindowE, NULL, NULL);

			UI::serverResponseError(ref);
		}
	}

	return responseBody;
}

void asyncUploadFile(UI::MainWindow& ref, unique_ptr<streams::IOSocketStream>& clientStream, const wstring& filePath, vector<db::fileDataRepresentation>& fileNames, bool& isCancel, bool removeBeforeUpload)
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

	const filesystem::path file(filePath);
	intmax_t fileSize = filesystem::file_size(file);

	string fileData;
	string lastPacket;
	string response;
	bool isLast;

	ifstream in(file, ios::binary);

	static_cast<UI::UploadFilePopupWindow*>(ref.getCurrentPopupWindow())->setProgressBarRange(0, fileSize);

	fileData.resize(filePacketSize);

	isCancel = false;

	removeFile(ref, clientStream, wstring(begin(filePath) + filePath.rfind('\\') + 1, end(filePath)), isCancel, false);

	do
	{
		const string* data;
		intmax_t offset = in.tellg();

		if (isCancel)
		{
			if (ref.getCurrentPopupWindow())
			{
				ref.getCurrentPopupWindow()->showPopupWindowVar() = false;
			}
			string cancelRequest = cancelUploadFile(file.filename().string());

			*clientStream << cancelRequest;

			*clientStream >> cancelRequest;

			SendMessageW(ref.getMainWindow(), UI::events::multipleUploadE, NULL, NULL);
			in.close();
			return;
		}

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
			"File-Name", file.filename().string(),
			"Range", offset,
			isLast ? "Total-File-Size" : "Reserved", isLast ? fileSize : 0
		).build(*data);

		try
		{
			*clientStream << message;
		}
		catch (const web::exceptions::WebException&)
		{
			UI::serverRequestError(ref);
			SendMessageW(ref.getMainWindow(), UI::events::deletePopupWindowE, NULL, NULL);
			in.close();
			return;
		}

		if (!isCancel)
		{
			SendMessageW(ref.getPopupWindow(), UI::events::updateProgressBarE, static_cast<WPARAM>(offset), NULL);
		}

	} while (!isLast);

	in.close();

	try
	{
		*clientStream >> response;
	}
	catch (const web::exceptions::WebException&)
	{
		UI::serverResponseError(ref);
		SendMessageW(ref.getMainWindow(), UI::events::deletePopupWindowE, NULL, NULL);
		return;
	}

	web::HTTPParser parser(response);

	if (!isCancel)
	{
		SendMessageW(ref.getPopupWindow(), UI::events::updateProgressBarE, static_cast<WPARAM>(fileSize), NULL);
	}

	this_thread::sleep_for(1s);

	const string& error = parser.getHeaders().at("Error");

	if (error == "1")
	{
		int ok;

		if (ref.getCurrentPopupWindow() && ref.getPopupWindow())
		{
			ok = MessageBoxW
			(
				ref.getPopupWindow(),
				utility::conversion::to_wstring(parser.getBody()).data(),
				L"������",
				MB_OK
			);
		}
		else
		{
			ok = MessageBoxW
			(
				ref.getMainWindow(),
				utility::conversion::to_wstring(parser.getBody()).data(),
				L"������",
				MB_OK
			);
		}

		if (ok == IDOK)
		{
			SendMessageW(ref.getMainWindow(), UI::events::deletePopupWindowE, NULL, NULL);
			SendMessageW(ref.getMainWindow(), UI::events::multipleUploadE, NULL, NULL);
		}
	}
	else
	{
		int ok;

		if (ref.getCurrentPopupWindow() && ref.getPopupWindow())
		{
			ok = MessageBoxW
			(
				ref.getPopupWindow(),
				L"���� ��������",
				L"����������",
				MB_OK
			);
		}
		else
		{
			ok = MessageBoxW
			(
				ref.getMainWindow(),
				L"���� ��������",
				L"����������",
				MB_OK
			);
		}

		if (ok == IDOK)
		{
			getFiles(ref, clientStream, fileNames, true, isCancel, false);
			SendMessageW(ref.getMainWindow(), UI::events::deletePopupWindowE, NULL, NULL);
			SendMessageW(ref.getMainWindow(), UI::events::multipleUploadE, NULL, NULL);
		}
	}
}

void asyncDownloadFile(UI::MainWindow& ref, unique_ptr<streams::IOSocketStream>& clientStream, const wstring& fileName, intmax_t fileSize, bool& isCancel)
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

	intmax_t offset = 0;
	intmax_t totalFileSize;
	const string sFileName = utility::conversion::to_string(fileName);
	bool lastPacket;
	string response;

	ofstream out(ref.getDownloadFolder().append(sFileName), ios::binary);

	isCancel = false;

	static_cast<UI::DownloadFilePopupWindow*>(ref.getCurrentPopupWindow())->setProgressBarRange(0, fileSize);

	while (true)
	{
		string request = web::HTTPBuilder().postRequest().headers
		(
			requestType::filesType, filesRequests::downloadFile,
			"File-Name", sFileName,
			"Range", offset
		).build();

		if (isCancel)
		{
			if (ref.getCurrentPopupWindow())
			{
				ref.getCurrentPopupWindow()->showPopupWindowVar() = false;
			}
			SendMessageW(ref.getMainWindow(), UI::events::multipleDownloadE, NULL, NULL);
			out.close();

			filesystem::remove(filesystem::path(ref.getDownloadFolder()).append(sFileName));

			return;
		}

		try
		{
			*clientStream << request;
		}
		catch (const web::exceptions::WebException&)
		{
			UI::serverRequestError(ref);
			out.close();
			return;
		}

		try
		{
			*clientStream >> response;
		}
		catch (const web::exceptions::WebException&)
		{
			UI::serverResponseError(ref);
			out.close();
			return;
		}

		web::HTTPParser parser(response);
		const auto& headers = parser.getHeaders();
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

		if (!isCancel)
		{
			SendMessageW(ref.getPopupWindow(), UI::events::updateProgressBarE, static_cast<WPARAM>(offset), NULL);
		}
	}

	out.close();

	if (!isCancel)
	{
		SendMessageW(ref.getPopupWindow(), UI::events::updateProgressBarE, static_cast<WPARAM>(fileSize), NULL);
	}

	this_thread::sleep_for(1s);

	if (filesystem::file_size(ref.getDownloadFolder().append(sFileName)) == totalFileSize)
	{
		wstring message = fileName + L"\r\n" + filesResponses::successDownloadFile.data();
		int ok;

		if (ref.getCurrentPopupWindow() && ref.getPopupWindow())
		{
			ok = MessageBoxW(ref.getPopupWindow(), message.data(), L"�����", MB_OK);
		}
		else
		{
			ok = MessageBoxW(ref.getMainWindow(), message.data(), L"�����", MB_OK);
		}
		if (ok == IDOK)
		{
			SendMessageW(ref.getMainWindow(), UI::events::deletePopupWindowE, NULL, NULL);
			SendMessageW(ref.getMainWindow(), UI::events::multipleDownloadE, NULL, NULL);
		}
	}
	else
	{
		wstring message = fileName + L"\r\n" + filesResponses::failDownloadFile.data();
		int ok;

		if (ref.getCurrentPopupWindow() && ref.getPopupWindow())
		{
			ok = MessageBoxW(ref.getPopupWindow(), message.data(), L"������", MB_OK);
		}
		else
		{
			ok = MessageBoxW(ref.getMainWindow(), message.data(), L"������", MB_OK);
		}

		if (ok == IDOK)
		{
			SendMessageW(ref.getMainWindow(), UI::events::deletePopupWindowE, NULL, NULL);
			SendMessageW(ref.getMainWindow(), UI::events::multipleDownloadE, NULL, NULL);
		}
	}
}

void asyncReconnect(UI::MainWindow& ref, unique_ptr<streams::IOSocketStream>& clientStream, string&& currentPath, const wstring& login, const wstring& password, vector<db::fileDataRepresentation>& fileNames, bool& isCancel)
{
	isCancel = false;

	string request = web::HTTPBuilder().postRequest().headers
	(
		requestType::exitType, networkRequests::exit
	).build();

	try
	{
		*clientStream << request;
	}
	catch (const web::exceptions::WebException&)
	{

	}

	try
	{
		clientStream = make_unique<streams::IOSocketStream>(make_unique<web::CustomHTTPNetwork>());
	}
	catch (const web::exceptions::WebException&)
	{
		if (UI::serverRequestError(ref) == IDOK)
		{
			if (ref.getCurrentPopupWindow())
			{
				ref.getCurrentPopupWindow()->showPopupWindowVar() = false;
			}
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

	if (isCancel)
	{
		if (ref.getCurrentPopupWindow())
		{
			ref.getCurrentPopupWindow()->showPopupWindowVar() = false;
		}
		return;
	}

	setLogin(ref, clientStream, login, password);

	setPath(ref, clientStream, move(currentPath), fileNames, isCancel);
}

void asyncCreateFolder(UI::MainWindow& ref, unique_ptr<streams::IOSocketStream>& clientStream, wstring&& wFolderName, vector<db::fileDataRepresentation>& fileNames, bool& isCancel)
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

	string request;
	string body;
	string response;

	body = "folder=" + utility::conversion::to_string(wFolderName);

	request = web::HTTPBuilder().postRequest().headers
	(
		requestType::filesType, filesRequests::createFolder
	).build(body);

	try
	{
		*clientStream << request;

		*clientStream >> response;
	}
	catch (const web::exceptions::WebException&)
	{

	}

	SendMessageW(ref.getMainWindow(), UI::events::deletePopupWindowE, NULL, NULL);

	getFiles(ref, clientStream, fileNames, true, isCancel, false);
}

void asyncRegistration(UI::MainWindow& ref, unique_ptr<streams::IOSocketStream>& clientStream, wstring& login, wstring& password, bool& isCancel)
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

	wstring wLogin;
	wstring wPassword;
	wstring wRepeatPassword;
	string sendLogin;
	string sendPassword;
	string response;
	isCancel = false;

	HWND registrationLoginEdit = ref.getRegistrationLoginEdit();
	HWND registrationPasswordEdit = ref.getRegistrationPasswordEdit();
	HWND registrationRepeatPasswordEdit = ref.getRegistrationRepeatPasswordEdit();

	wLogin.resize(GetWindowTextLengthW(registrationLoginEdit));
	wPassword.resize(GetWindowTextLengthW(registrationPasswordEdit));
	wRepeatPassword.resize(GetWindowTextLengthW(registrationRepeatPasswordEdit));

	if (wLogin.empty())
	{
		if (UI::customError(ref, L"����� �� ����� ���� ������", L"������"))
		{
			SendMessageW(ref.getMainWindow(), UI::events::deletePopupWindowE, NULL, NULL);
		}

		return;
	}

	if (wPassword.empty())
	{
		if (UI::customError(ref, L"������ �� ����� ���� ������", L"������"))
		{
			SendMessageW(ref.getMainWindow(), UI::events::deletePopupWindowE, NULL, NULL);
		}

		return;
	}

	GetWindowTextW(registrationLoginEdit, wLogin.data(), static_cast<int>(wLogin.size() + 1));
	GetWindowTextW(registrationPasswordEdit, wPassword.data(), static_cast<int>(wPassword.size() + 1));
	GetWindowTextW(registrationRepeatPasswordEdit, wRepeatPassword.data(), static_cast<int>(wRepeatPassword.size() + 1));

	if (wPassword != wRepeatPassword)
	{
		int ok;

		SetWindowTextW(registrationLoginEdit, L"");
		SetWindowTextW(registrationPasswordEdit, L"");
		SetWindowTextW(registrationRepeatPasswordEdit, L"");

		if (ref.getCurrentPopupWindow() && ref.getPopupWindow())
		{
			ok = MessageBoxW(ref.getPopupWindow(), L"������ �� ���������", L"������", MB_OK);
		}
		else
		{
			ok = MessageBoxW(ref.getMainWindow(), L"������ �� ���������", L"������", MB_OK);
		}

		if (ok == IDOK)
		{
			if (ref.getCurrentPopupWindow())
			{
				ref.getCurrentPopupWindow()->showPopupWindowVar() = false;
			}

			SendMessageW(ref.getMainWindow(), UI::events::deletePopupWindowE, NULL, NULL);

			return;
		}
	}

	sendLogin = utility::conversion::to_string(wLogin);
	sendPassword = utility::conversion::to_string(wPassword);

	if (!utility::validation::validationUserData(sendLogin) || !utility::validation::validationUserData(sendPassword))
	{
		SetWindowTextW(registrationLoginEdit, L"");
		SetWindowTextW(registrationPasswordEdit, L"");
		SetWindowTextW(registrationRepeatPasswordEdit, L"");

		SendMessageW(ref.getMainWindow(), UI::events::deletePopupWindowE, NULL, NULL);

		UI::validationDataError(ref);
	}

	string body = "login=" + move(sendLogin) + "&" + "password=" + move(sendPassword);

	string request = web::HTTPBuilder().postRequest().headers
	(
		requestType::accountType, accountRequests::registration
	).build(body);

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
	catch (const web::exceptions::WebException&)
	{
		if (UI::serverRequestError(ref) == IDOK)
		{
			if (ref.getCurrentPopupWindow())
			{
				ref.getCurrentPopupWindow()->showPopupWindowVar() = false;
			}
			SendMessageW(ref.getMainWindow(), UI::events::deletePopupWindowE, NULL, NULL);
		}
		return;
	}

	try
	{
		*clientStream >> response;
	}
	catch (const web::exceptions::WebException&)
	{
		if (UI::serverResponseError(ref) == IDOK)
		{
			if (ref.getCurrentPopupWindow())
			{
				ref.getCurrentPopupWindow()->showPopupWindowVar() = false;
			}
			SendMessageW(ref.getMainWindow(), UI::events::deletePopupWindowE, NULL, NULL);
		}
		return;
	}

	web::HTTPParser parser(response);

	if (parser.getHeaders().at("Error") == "0")
	{
		login = move(wLogin);
		password = move(wPassword);

		SendMessageW(ref.getMainWindow(), UI::events::initCloudStorageScreenE, NULL, NULL);
	}
	else
	{
		int ok;

		SetWindowTextW(registrationLoginEdit, L"");
		SetWindowTextW(registrationPasswordEdit, L"");
		SetWindowTextW(registrationRepeatPasswordEdit, L"");

		if (ref.getCurrentPopupWindow())
		{
			ok = MessageBoxW(ref.getPopupWindow(), utility::conversion::to_wstring(parser.getBody()).data(), L"������", MB_OK);
		}
		else
		{
			ok = MessageBoxW(ref.getMainWindow(), utility::conversion::to_wstring(parser.getBody()).data(), L"������", MB_OK);
		}

		if (ok == IDOK)
		{
			SendMessageW(ref.getMainWindow(), UI::events::deletePopupWindowE, NULL, NULL);
		}
	}
}

void asyncAuthorization(UI::MainWindow& ref, unique_ptr<streams::IOSocketStream>& clientStream, wstring& login, wstring& password, bool& isCancel)
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

	wstring wLogin;
	wstring wPassword;
	string sendLogin;
	string sendPassword;
	string response;
	HWND authorizationLoginEdit;
	HWND authorizationPasswordEdit;
	isCancel = false;

	authorizationLoginEdit = ref.getAuthorizationLoginEdit();
	authorizationPasswordEdit = ref.getAuthorizationPasswordEdit();

	wLogin.resize(GetWindowTextLengthW(authorizationLoginEdit));
	wPassword.resize(GetWindowTextLengthW(authorizationPasswordEdit));

	if (wLogin.empty())
	{
		if (UI::customError(ref, L"����� �� ����� ���� ������", L"������"))
		{
			SendMessageW(ref.getMainWindow(), UI::events::deletePopupWindowE, NULL, NULL);
		}

		return;
	}

	if (wPassword.empty())
	{
		if (UI::customError(ref, L"������ �� ����� ���� ������", L"������"))
		{
			SendMessageW(ref.getMainWindow(), UI::events::deletePopupWindowE, NULL, NULL);
		}

		return;
	}

	GetWindowTextW(authorizationLoginEdit, wLogin.data(), static_cast<int>(wLogin.size() + 1));
	GetWindowTextW(authorizationPasswordEdit, wPassword.data(), static_cast<int>(wPassword.size() + 1));

	sendLogin = utility::conversion::to_string(wLogin);
	sendPassword = utility::conversion::to_string(wPassword);

	if (!utility::validation::validationUserData(sendLogin) || !utility::validation::validationUserData(sendPassword))
	{
		SetWindowTextW(authorizationLoginEdit, L"");
		SetWindowTextW(authorizationPasswordEdit, L"");

		if (UI::validationDataError(ref) == IDOK)
		{
			SendMessageW(ref.getMainWindow(), UI::events::deletePopupWindowE, NULL, NULL);
		}

		return;
	}

	string body = "login=" + move(sendLogin) + "&" + "password=" + move(sendPassword);

	string request = web::HTTPBuilder().postRequest().headers
	(
		requestType::accountType, accountRequests::authorization
	).build(body);

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

	if (isCancel)
	{
		if (ref.getCurrentPopupWindow())
		{
			ref.getCurrentPopupWindow()->showPopupWindowVar() = false;
		}
		return;
	}

	web::HTTPParser parser(response);

	if (parser.getHeaders().at("Error") == "0")
	{
		login = move(wLogin);
		password = move(wPassword);

		SendMessageW(ref.getMainWindow(), UI::events::initCloudStorageScreenE, NULL, NULL);
	}
	else
	{
		int ok;

		SetWindowTextW(authorizationLoginEdit, L"");
		SetWindowTextW(authorizationPasswordEdit, L"");

		if (ref.getCurrentPopupWindow())
		{
			ok = MessageBoxW(ref.getPopupWindow(), utility::conversion::to_wstring(parser.getBody()).data(), L"������", MB_OK);
		}
		else
		{
			ok = MessageBoxW(ref.getMainWindow(), utility::conversion::to_wstring(parser.getBody()).data(), L"������", MB_OK);
		}

		if (ok == IDOK)
		{
			SendMessageW(ref.getMainWindow(), UI::events::deletePopupWindowE, NULL, NULL);
		}
	}
}

void setPath(UI::MainWindow& ref, unique_ptr<streams::IOSocketStream>& clientStream, string&& path, vector<db::fileDataRepresentation>& fileNames, bool& isCancel)
{
	asyncFolderControlMessages(ref, clientStream, controlRequests::setPath, fileNames, isCancel, move(path));
}

void setLogin(UI::MainWindow& ref, unique_ptr<streams::IOSocketStream>& clientStream, const wstring& login, const wstring& password)
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

	string body = "login=" + utility::conversion::to_string(login) + "&" + "password=" + utility::conversion::to_string(password);
	string response;
	string request = web::HTTPBuilder().postRequest().headers
	(
		requestType::accountType, accountRequests::setLogin
	).build(body);

	try
	{
		*clientStream << request;
	}
	catch (const web::exceptions::WebException&)
	{

	}

	try
	{
		*clientStream >> response;
	}
	catch (const web::exceptions::WebException&)
	{

	}
}

void asyncFirstConnect(UI::MainWindow& ref, unique_ptr<streams::IOSocketStream>& clientStream, bool& isCancel)
{
	isCancel = false;

	try
	{
		if (isCancel)
		{
			if (ref.getCurrentPopupWindow())
			{
				ref.getCurrentPopupWindow()->showPopupWindowVar() = false;
			}
			return;
		}

		clientStream = make_unique<streams::IOSocketStream>(make_unique<web::CustomHTTPNetwork>());
	}
	catch (const web::exceptions::WebException&)
	{
		if (UI::serverRequestError(ref) == IDOK)
		{
			SendMessageW(ref.getMainWindow(), UI::events::deletePopupWindowE, NULL, NULL);
		}

		return;
	}

	SendMessageW(ref.getMainWindow(), UI::events::deletePopupWindowE, NULL, NULL);
}