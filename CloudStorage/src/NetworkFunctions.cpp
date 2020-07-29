#include "pch.h"

#include "NetworkFunctions.h"
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
#include "HTTPNetwork.h"
#include "fileData.h"
#include "ClientTime.h"
#include "ErrorHandling.h"
#include "Log.h"

#include <CommCtrl.h>

using namespace std;

string cancelUploadFile(const string& fileName);

string asyncFolderControlMessages(UI::MainWindow& ref, streams::IOSocketStream<char>& clientStream, const string_view& controlRequest, vector<db::fileDataRepresentation>& fileNames, bool& isCancel, string&& data = "");

void asyncUploadFile(UI::MainWindow& ref, streams::IOSocketStream<char>& clientStream, const wstring& filePath, vector<db::fileDataRepresentation>& fileNames, bool& isCancel);

void asyncDownloadFile(UI::MainWindow& ref, streams::IOSocketStream<char>& clientStream, const wstring& fileName, intmax_t fileSize, bool& isCancel);

void asyncReconnect(UI::MainWindow& ref, streams::IOSocketStream<char>& clientStream, string&& currentPath, const wstring& login, const wstring& password, vector<db::fileDataRepresentation>& fileNames, bool& isCancel);

void asyncRemoveFile(UI::MainWindow& ref, streams::IOSocketStream<char>& clientStream, vector<db::fileDataRepresentation>& fileNames, bool& isCancel);

void asyncRegistration(UI::MainWindow& ref, streams::IOSocketStream<char>& clientStream, wstring& login, wstring& password, bool& isCancel);

void asyncAuthorization(UI::MainWindow& ref, streams::IOSocketStream<char>& clientStream, wstring& login, wstring& password, bool& isCancel);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void setPath(UI::MainWindow& ref, streams::IOSocketStream<char>& clientStream, string&& path, vector<db::fileDataRepresentation>& fileNames, bool& isCancel);

void removeFile(UI::MainWindow& ref, streams::IOSocketStream<char>& clientStream, const wstring& fileName, bool& isCancel);

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
		initDownloadFilePopupWindow(ref, wstring(L"Скачивание файла ") + fileNames[id].fileName);

		thread(asyncDownloadFile, std::ref(ref), std::ref(clientStream), std::ref(fileNames[id].fileName), fileNames[id].fileSize, std::ref(isCancel)).detach();
	}

	return id;
}

void removeFile(UI::MainWindow& ref, streams::IOSocketStream<char>& clientStream, vector<db::fileDataRepresentation>& fileNames, bool& isCancel)
{
	initWaitResponsePopupWindow(ref);

	thread(asyncRemoveFile, std::ref(ref), std::ref(clientStream), std::ref(fileNames), std::ref(isCancel)).detach();
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

void setLogin(UI::MainWindow& ref, streams::IOSocketStream<char>& clientStream, const wstring& login, const wstring& password)
{
	string body = "login=" + utility::conversion::to_string(login) + "&" + "password=" + utility::conversion::to_string(password);

	string request = web::HTTPBuilder().postRequest().headers
	(
		requestType::accountType, accountRequests::setLogin,
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
}

void exitFromApplication(UI::MainWindow& ref, streams::IOSocketStream<char>& clientSream)
{
	string request = web::HTTPBuilder().postRequest().headers
	(
		requestType::exitType, networkRequests::exit
	).build();

	utility::web::insertSizeHeaderToHTTPMessage(request);

	try
	{
		clientSream << request;
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

void setPath(UI::MainWindow& ref, streams::IOSocketStream<char>& clientStream, string&& path, vector<db::fileDataRepresentation>& fileNames, bool& isCancel)
{
	asyncFolderControlMessages(ref, clientStream, controlRequests::setPath, fileNames, isCancel, move(path));
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

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

string cancelUploadFile(const string& fileName)
{
	string result = web::HTTPBuilder().postRequest().headers
	(
		requestType::cancelType, networkRequests::cancelOperation,
		"Operation-Type", filesRequests::uploadFile,
		"File-Name", fileName
	).build();

	utility::web::insertSizeHeaderToHTTPMessage(result);

	return result;
}

string asyncFolderControlMessages(UI::MainWindow& ref, streams::IOSocketStream<char>& clientStream, const string_view& controlRequest, vector<db::fileDataRepresentation>& fileNames, bool& isCancel, string&& data)
{
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
			requestType::controlType, controlRequest,
			"Content-Length", body.size()
		).build(&body);
	}

	utility::web::insertSizeHeaderToHTTPMessage(request);

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
		clientStream << request;
	}
	catch (const web::WebException&)
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
		clientStream >> response;
	}
	catch (const web::WebException&)
	{

	}

	getFiles(ref, clientStream, fileNames, false, isCancel, false);

	return response;
}

void asyncUploadFile(UI::MainWindow& ref, streams::IOSocketStream<char>& clientStream, const wstring& filePath, vector<db::fileDataRepresentation>& fileNames, bool& isCancel)
{
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

			clientStream << cancelRequest;

			clientStream >> cancelRequest;

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
			"Content-Length", data->size(),
			isLast ? "Total-File-Size" : "Reserved", isLast ? fileSize : 0
		).build(data);

		utility::web::insertSizeHeaderToHTTPMessage(message);

		try
		{
			clientStream << message;
		}
		catch (const web::WebException&)
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
		clientStream >> response;
	}
	catch (const web::WebException&)
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
		int ok = MessageBoxW
		(
			ref.getPopupWindow(),
			utility::conversion::to_wstring(parser.getBody()).data(),
			L"Ошибка",
			MB_OK
		);

		if (ok == IDOK)
		{
			SendMessageW(ref.getMainWindow(), UI::events::deletePopupWindowE, NULL, NULL);
			SendMessageW(ref.getMainWindow(), UI::events::multipleUploadE, NULL, NULL);
		}
	}
	else
	{
		int ok = MessageBoxW
		(
			ref.getPopupWindow(),
			L"Файл загружен",
			L"Информация",
			MB_OK
		);

		if (ok == IDOK)
		{
			getFiles(ref, clientStream, fileNames, true, isCancel, false);
			SendMessageW(ref.getMainWindow(), UI::events::deletePopupWindowE, NULL, NULL);
			SendMessageW(ref.getMainWindow(), UI::events::multipleUploadE, NULL, NULL);
		}
	}
}

void asyncDownloadFile(UI::MainWindow& ref, streams::IOSocketStream<char>& clientStream, const wstring& fileName, intmax_t fileSize, bool& isCancel)
{
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

		utility::web::insertSizeHeaderToHTTPMessage(request);

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
			clientStream << request;
		}
		catch (const web::WebException&)
		{
			UI::serverRequestError(ref);
			out.close();
			return;
		}

		try
		{
			clientStream >> response;
		}
		catch (const web::WebException&)
		{
			UI::serverResponseError(ref);
			out.close();
			return;
		}

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

		int ok = MessageBoxW(ref.getPopupWindow(), message.data(), L"Успех", MB_OK);

		if (ok == IDOK)
		{
			SendMessageW(ref.getMainWindow(), UI::events::deletePopupWindowE, NULL, NULL);
			SendMessageW(ref.getMainWindow(), UI::events::multipleDownloadE, NULL, NULL);
		}
	}
	else
	{
		wstring message = fileName + L"\r\n" + filesResponses::failDownloadFile.data();

		int ok = MessageBoxW(ref.getPopupWindow(), message.data(), L"Ошибка", MB_OK);

		if (ok == IDOK)
		{
			SendMessageW(ref.getMainWindow(), UI::events::deletePopupWindowE, NULL, NULL);
			SendMessageW(ref.getMainWindow(), UI::events::multipleDownloadE, NULL, NULL);
		}
	}
}

void asyncReconnect(UI::MainWindow& ref, streams::IOSocketStream<char>& clientStream, string&& currentPath, const wstring& login, const wstring& password, vector<db::fileDataRepresentation>& fileNames, bool& isCancel)
{
	isCancel = false;

	if (ref.getCurrentPopupWindow())
	{
		static_cast<UI::WaitResponsePopupWindow*>(ref.getCurrentPopupWindow())->startAnimateProgressBar();
	}

	exitFromApplication(ref, clientStream);

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
		clientStream = streams::IOSocketStream<char>(new buffers::IOSocketBuffer<char>(new web::HTTPNetwork()));
	}
	catch (const web::WebException&)
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

	setLogin(ref, clientStream, login, password);

	setPath(ref, clientStream, move(currentPath), fileNames, isCancel);
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

void asyncRegistration(UI::MainWindow& ref, streams::IOSocketStream<char>& clientStream, wstring& login, wstring& password, bool& isCancel)
{
	wstring wLogin;
	wstring wPassword;
	wstring wRepeatPassword;
	string response;
	isCancel = false;

	HWND registrationLoginEdit = ref.getRegistrationLoginEdit();
	HWND registrationPasswordEdit = ref.getRegistrationPasswordEdit();
	HWND registrationRepeatPasswordEdit = ref.getRegistrationRepeatPasswordEdit();

	wLogin.resize(GetWindowTextLengthW(registrationLoginEdit));
	wPassword.resize(GetWindowTextLengthW(registrationPasswordEdit));
	wRepeatPassword.resize(GetWindowTextLengthW(registrationRepeatPasswordEdit));

	GetWindowTextW(registrationLoginEdit, wLogin.data(), wLogin.size() + 1);
	GetWindowTextW(registrationPasswordEdit, wPassword.data(), wPassword.size() + 1);
	GetWindowTextW(registrationRepeatPasswordEdit, wRepeatPassword.data(), wRepeatPassword.size() + 1);

	if (wPassword != wRepeatPassword)
	{
		int ok;

		SetWindowTextW(registrationLoginEdit, L"");
		SetWindowTextW(registrationPasswordEdit, L"");
		SetWindowTextW(registrationRepeatPasswordEdit, L"");

		if (ref.getPopupWindow())
		{
			ok = MessageBoxW(ref.getPopupWindow(), L"Пароли не совпадают", L"Ошибка", MB_OK);
		}
		else
		{
			ok = MessageBoxW(ref.getMainWindow(), L"Пароли не совпадают", L"Ошибка", MB_OK);
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

	string body = "login=" + utility::conversion::to_string(wLogin) + "&" + "password=" + utility::conversion::to_string(wPassword);

	string request = web::HTTPBuilder().postRequest().headers
	(
		requestType::accountType, accountRequests::registration,
		"Content-Length", body.size()
	).build(&body);

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
		clientStream << request;
	}
	catch (const web::WebException&)
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
		clientStream >> response;
	}
	catch (const web::WebException&)
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
			ok = MessageBoxW(ref.getPopupWindow(), utility::conversion::to_wstring(parser.getBody()).data(), L"Ошибка", MB_OK);
		}
		else
		{
			ok = MessageBoxW(ref.getMainWindow(), utility::conversion::to_wstring(parser.getBody()).data(), L"Ошибка", MB_OK);
		}

		if (ok == IDOK)
		{
			SendMessageW(ref.getMainWindow(), UI::events::deletePopupWindowE, NULL, NULL);
		}
	}
}

void asyncAuthorization(UI::MainWindow& ref, streams::IOSocketStream<char>& clientStream, wstring& login, wstring& password, bool& isCancel)
{
	wstring wLogin;
	wstring wPassword;
	string response;
	HWND authorizationLoginEdit;
	HWND authorizationPasswordEdit;
	isCancel = false;

	authorizationLoginEdit = ref.getAuthorizationLoginEdit();
	authorizationPasswordEdit = ref.getAuthorizationPasswordEdit();

	wLogin.resize(GetWindowTextLengthW(authorizationLoginEdit));
	wPassword.resize(GetWindowTextLengthW(authorizationPasswordEdit));

	GetWindowTextW(authorizationLoginEdit, wLogin.data(), wLogin.size() + 1);
	GetWindowTextW(authorizationPasswordEdit, wPassword.data(), wPassword.size() + 1);

	string body = "login=" + utility::conversion::to_string(wLogin) + "&" + "password=" + utility::conversion::to_string(wPassword);

	string request = web::HTTPBuilder().postRequest().headers
	(
		requestType::accountType, accountRequests::authorization,
		"Content-Length", body.size()
	).build(&body);

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
			ok = MessageBoxW(ref.getPopupWindow(), utility::conversion::to_wstring(parser.getBody()).data(), L"Ошибка", MB_OK);
		}
		else
		{
			ok = MessageBoxW(ref.getMainWindow(), utility::conversion::to_wstring(parser.getBody()).data(), L"Ошибка", MB_OK);
		}

		if (ok == IDOK)
		{
			SendMessageW(ref.getMainWindow(), UI::events::deletePopupWindowE, NULL, NULL);
		}
	}
}