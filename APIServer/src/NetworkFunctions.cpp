#include "pch.h"

#include "NetworkFunctions.h"
#include "Constants.h"
#include "HTTPBuilder.h"
#include "HTTPParser.h"
#include "UtilityFunctions.h"

using namespace std;

bool checkHTTP(const string& request)
{
	return request.find("HTTP") != string::npos ? true : false;
}

void showAllFilesInDirectory(streams::IOSocketStream<char>& clientStream, streams::IOSocketStream<char>& filesStream, const string& directory)
{
	string responseMessage;
	bool error;
	string response;

	try
	{
		filesStream << filesRequests::showAllFilesInDirectory;
		filesStream << directory;

		filesStream >> responseMessage;

		if (responseMessage == responses::okResponse)
		{
			filesStream >> responseMessage;
			error = false;
		}
		else
		{
			filesStream >> responseMessage;
			error = true;
		}

		response = web::HTTPBuilder().responseCode(web::ResponseCodes::ok).headers
		(
			"Content-Length", responseMessage.size(),
			"Error", error
		).build(&responseMessage);

		utility::insertSizeHeaderToHTTPMessage(response);

		clientStream << response;
	}
	catch (const web::WebException& e)
	{
		cout << e.what() << endl;
	}
}

void uploadFile(streams::IOSocketStream<char>& clientStream, streams::IOSocketStream<char>& filesStream, streams::IOSocketStream<char>& dataBaseStream, const string& data, const map<string, string>& headers)
{
	const string& fileName = headers.at("File-Name");
	const string& login = headers.at("Login");
	const string& directory = headers.at("Directory");
	uintmax_t offset = stoull(headers.at("Range"));
	auto it = headers.find("Total-File-Size");
	bool needResponse = it != end(headers);

	filesStream << filesRequests::uploadFile;
	filesStream << login;
	filesStream << directory;

	filesStream << fileName;
	filesStream << offset;
	filesStream << data;
	filesStream << needResponse;

	if (needResponse)
	{
		uintmax_t uploadSize = stoull(it->second);
		string responseMessage;
		string isSuccess;

		filesStream << uploadSize;

		filesStream >> isSuccess;
		filesStream >> responseMessage;

		if (isSuccess == responses::okResponse)
		{
			dataBaseStream << filesRequests::uploadFile;

			dataBaseStream << fileName;

			dataBaseStream << directory;

			dataBaseStream << uploadSize;
		}

		string response = web::HTTPBuilder().responseCode(web::ResponseCodes::ok).headers
		(
			"Error", isSuccess == responses::okResponse ? false : true,
			"Content-Length", responseMessage.size()
		).build(&responseMessage);

		utility::insertSizeHeaderToHTTPMessage(response);

		clientStream << response;
	}
}

void downloadFile(streams::IOSocketStream<char>& clientStream, streams::IOSocketStream<char>& filesStream, const map<string, string>& headers)
{
	const string& fileName = headers.at("File-Name");
	const string& login = headers.at("Login");
	const string& directory = headers.at("Directory");
	bool isLast;
	string data;
	uintmax_t offset = stoull(headers.at("Range"));
	uintmax_t totalFileSize;

	filesStream << filesRequests::downloadFile;
	filesStream << login;
	filesStream << directory;

	filesStream << fileName;
	filesStream << offset;

	filesStream >> data;
	filesStream >> isLast;

	if (isLast)
	{
		filesStream >> totalFileSize;
	}

	string response = web::HTTPBuilder().responseCode(web::ResponseCodes::ok).headers
	(
		isLast ? "Total-File-Size" : "Reserved", isLast ? totalFileSize : 0,
		"Content-Length", data.size()
	).build(&data);

	utility::insertSizeHeaderToHTTPMessage(response);

	clientStream << response;
}

void removeFile(streams::IOSocketStream<char>& clientStream, streams::IOSocketStream<char>& filesStream, streams::IOSocketStream<char>& dataBaseStream, const map<string, string>& headers)
{
	const string& login = headers.at("Login");
	const string& directory = headers.at("Directory");
	const string& fileName = headers.at("File-Name");
	bool error;
	string responseMessage;

	filesStream << filesRequests::removeFile;
	filesStream << login;
	filesStream << directory;

	filesStream << fileName;

	filesStream >> responseMessage;

	error = responseMessage != responses::okResponse;

	if (!error)
	{
		dataBaseStream << filesRequests::removeFile;

		dataBaseStream << fileName;

		dataBaseStream << directory;
	}

	string response = web::HTTPBuilder().responseCode(web::ResponseCodes::ok).headers
	(
		"Error", error
	).build();

	utility::insertSizeHeaderToHTTPMessage(response);

	clientStream << response;
}

void authorization(streams::IOSocketStream<char>& clientStream, streams::IOSocketStream<char>& dataBaseStream, const string& data)
{
	auto [login, password] = userDataParse(data);
	string response;
	string responseMessage;
	bool error;

	dataBaseStream << accountRequest::authorization;
	dataBaseStream << login;
	dataBaseStream << password;

	dataBaseStream >> response;

	error = response != responses::okResponse;

	if (error)
	{
		responseMessage = move(response);

		response = web::HTTPBuilder().responseCode(web::ResponseCodes::ok).headers
		(
			"Error", error,
			"Content-Length", responseMessage.size()
		).build(&responseMessage);

	}
	else
	{
		response = web::HTTPBuilder().responseCode(web::ResponseCodes::ok).headers
		(
			"Error", error
		).build();
	}

	utility::insertSizeHeaderToHTTPMessage(response);

	clientStream << response;
}

void registration(streams::IOSocketStream<char>& clientStream, streams::IOSocketStream<char>& dataBaseStream, const string& data)
{
	auto [login, password] = userDataParse(data);
	string response;
	string responseMessage;
	bool error;

	dataBaseStream << accountRequest::registration;
	dataBaseStream << login;
	dataBaseStream << password;

	dataBaseStream >> response;

	error = response == accountResponses::failRegistration;

	if (error)
	{
		responseMessage = move(response);

		response = web::HTTPBuilder().responseCode(web::ResponseCodes::ok).headers
		(
			"Error", error,
			"Content-Length", responseMessage.size()
		).build(&responseMessage);

	}
	else
	{
		response = web::HTTPBuilder().responseCode(web::ResponseCodes::ok).headers
		(
			"Error", error
		).build();
	}

	utility::insertSizeHeaderToHTTPMessage(response);

	clientStream << response;
}

tuple<string, string> userDataParse(const string& data)
{
	if (data.find('&') == string::npos)
	{
		return { string(), string() };
	}

	return
	{
		string(begin(data) + data.find('=') + 1, begin(data) + data.find('&')),	//login
		string(begin(data) + data.rfind('=') + 1, end(data))	//password
	};
}