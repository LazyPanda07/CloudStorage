#include "pch.h"

#include "NetworkFunctions.h"
#include "HTTPBuilder.h"
#include "HTTPParser.h"
#include "UIConstants.h"
#include "Constants.h"
#include "UtilityFunctions.h"
#include "Screens/ScreenFunctions.h"
#include "fileData.h"

#include <commctrl.h>

using namespace std;

void getFiles(UI::MainWindow& ref, streams::IOSocketStream<char>& clientStream, vector<db::wFileData>& fileNames, bool showError)
{
	string request = web::HTTPBuilder().postRequest().headers
	(
		requestType::filesType, filesRequests::showAllFilesInDirectory,
		"Directory", "Home"
	).build();
	string response;
	fileNames.clear();

	utility::insertSizeHeaderToHTTPMessage(request);

	clientStream << request;
	clientStream >> response;

	web::HTTPParser parser(response);
	const string& error = parser.getHeaders().at("Error");
	const string& body = parser.getBody();

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
					utility::to_wstring(tem[0]),
					utility::to_wstring(tem[1]),
					utility::to_wstring(tem[2]),
					utility::to_wstring(tem[3]),
					utility::to_wstring(tem[4]),
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
		MessageBoxW
		(
			ref.getMainWindow(),
			utility::to_wstring(body).data(),
			L"Ошибка",
			MB_OK
		);
	}
}

void uploadFile(UI::MainWindow& ref, streams::IOSocketStream<char>& clientStream, const vector<wstring>& files, const wstring& login)
{
	for (const auto& i : files)
	{
		uploadFile(ref,clientStream, i, login);
	}
}

void uploadFile(UI::MainWindow& ref, streams::IOSocketStream<char>& clientStream, const wstring& filePath, const wstring& login)
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
			"Login", utility::to_string(login),
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
			ref.getMainWindow(),
			utility::to_wstring(parser.getBody()).data(),
			L"Ошибка",
			MB_OK
		);
	}
	else
	{
		SendMessageW(ref.getMainWindow(), UI::events::getFilesE, NULL, NULL);
	}
}

void downloadFile(UI::MainWindow& ref, streams::IOSocketStream<char>& clientStream, const vector<db::wFileData>& fileNames, const wstring& login)
{
	int id = SendMessageW(ref.getList(), LVM_GETNEXTITEM, -1, LVNI_SELECTED);

	while (id != -1)
	{
		downloadFile(ref,clientStream, fileNames[id].fileName, login);

		id = SendMessageW(ref.getList(), LVM_GETNEXTITEM, id, LVNI_SELECTED);
	}
}

void downloadFile(UI::MainWindow& ref, streams::IOSocketStream<char>& clientStream, const wstring& fileName, const wstring& login)
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
			"Login", utility::to_string(login),
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

		MessageBoxW(ref.getMainWindow(), message.data(), L"Успех", MB_OK);
	}
	else
	{
		wstring message = fileName + L"\r\n" + filesResponses::failDownloadFile.data();

		MessageBoxW(ref.getMainWindow(), message.data(), L"Ошибка", MB_OK);
	}
}

void removeFile(UI::MainWindow& ref, streams::IOSocketStream<char>& clientStream, const vector<db::wFileData>& fileNames, const wstring& login)
{
	int id = SendMessageW(ref.getList(), LVM_GETNEXTITEM, -1, LVNI_SELECTED);

	while (id != -1)
	{
		if (removeFileDialog(ref, fileNames[id].fileName))
		{
			removeFile(ref, clientStream, fileNames[id].fileName, login);
		}

		id = SendMessageW(ref.getList(), LVM_GETNEXTITEM, id, LVNI_SELECTED);
	}
}

void removeFile(UI::MainWindow& ref, streams::IOSocketStream<char>& clientStream, const std::wstring& fileName, const std::wstring& login)
{
	string request = web::HTTPBuilder().postRequest().headers
	(
		requestType::filesType, filesRequests::removeFile,
		"Login", utility::to_string(login),
		"File-Name", utility::to_string(fileName),
		"Directory", "Home"
	).build();
	string response;

	utility::insertSizeHeaderToHTTPMessage(request);

	clientStream << request;

	clientStream >> response;

	web::HTTPParser parser(response);
	const map<string, string>& headers = parser.getHeaders();

	if(headers.at("Error") == "0")
	{
		//TODO: message
	}
	else
	{
		MessageBoxW(ref.getMainWindow(), wstring(L"Не удалось удалить " + fileName).data(), L"Ошибка", MB_OK);
	}
}

wstring authorization(UI::MainWindow& ref, streams::IOSocketStream<char>& clientStream)
{
	wstring wLogin;
	wstring wPassword;
	string response;

	HWND authorizationLoginEdit = ref.getAuthorizationLoginEdit();
	HWND authorizationPasswordEdit = ref.getAuthorizationPasswordEdit();

	wLogin.resize(GetWindowTextLengthW(authorizationLoginEdit));
	wPassword.resize(GetWindowTextLengthW(authorizationPasswordEdit));

	GetWindowTextW(authorizationLoginEdit, wLogin.data(), wLogin.size() + 1);
	GetWindowTextW(authorizationPasswordEdit, wPassword.data(), wPassword.size() + 1);

	string login = utility::to_string(wLogin);
	string password = utility::to_string(wPassword);
	string body = "login=" + login + "&" + "password=" + password;

	string request = web::HTTPBuilder().postRequest().headers
	(
		requestType::accountType, accountRequest::authorization,
		"Content-Length", body.size()
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
		SetWindowTextW(authorizationLoginEdit, L"");
		SetWindowTextW(authorizationPasswordEdit, L"");

		MessageBoxW(ref.getMainWindow(), utility::to_wstring(parser.getBody()).data(), L"Ошибка", MB_OK);

		return wstring();
	}
}

wstring registration(UI::MainWindow& ref, streams::IOSocketStream<char>& clientStream)
{
	wstring wLogin;
	wstring wPassword;
	wstring wRepeatPassword;
	string response;

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
		SetWindowTextW(registrationLoginEdit, L"");
		SetWindowTextW(registrationPasswordEdit, L"");
		SetWindowTextW(registrationRepeatPasswordEdit, L"");

		MessageBoxW(ref.getMainWindow(), L"Пароли не совпадают", L"Ошибка", MB_OK);

		return wstring();
	}

	string login = utility::to_string(wLogin);
	string password = utility::to_string(wPassword);

	string body = "login=" + login + "&" + "password=" + password;

	string request = web::HTTPBuilder().postRequest().headers
	(
		requestType::accountType, accountRequest::registration,
		"Content-Length", body.size()
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
		SetWindowTextW(registrationLoginEdit, L"");
		SetWindowTextW(registrationPasswordEdit, L"");
		SetWindowTextW(registrationRepeatPasswordEdit, L"");

		MessageBoxW(ref.getMainWindow(), utility::to_wstring(parser.getBody()).data(), L"Ошибка", MB_OK);

		return wstring();
	}
}