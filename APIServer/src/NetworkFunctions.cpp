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

void showAllFilesInFolder(streams::IOSocketStream<char>& clientStream, streams::IOSocketStream<char>& filesStream)
{
	string responseMessage;
	bool error;
	string response;

	try
	{
		filesStream << filesRequests::showAllFilesInFolder;

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

		utility::web::insertSizeHeaderToHTTPMessage(response);

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
	intmax_t offset = stoull(headers.at("Range"));
	auto it = headers.find("Total-File-Size");
	bool needResponse = it != end(headers);

	filesStream << filesRequests::uploadFile;

	filesStream << fileName;
	filesStream << offset;
	filesStream << data;
	filesStream << needResponse;

	if (needResponse)
	{
		intmax_t uploadSize = stoull(it->second);
		string responseMessage;
		string isSuccess;

		filesStream << uploadSize;

		filesStream >> isSuccess;
		filesStream >> responseMessage;

		if (isSuccess == responses::okResponse)
		{
			string extension;

			filesStream >> extension;

			dataBaseStream << filesRequests::uploadFile;
			dataBaseStream << fileName;
			dataBaseStream << extension;
			dataBaseStream << uploadSize;

		}

		string response = web::HTTPBuilder().responseCode(web::ResponseCodes::ok).headers
		(
			"Error", isSuccess == responses::okResponse ? false : true,
			"Content-Length", responseMessage.size()
		).build(&responseMessage);

		utility::web::insertSizeHeaderToHTTPMessage(response);

		try
		{
			clientStream << response;
		}
		catch (const web::WebException& e)
		{

		}
	}
}

void downloadFile(streams::IOSocketStream<char>& clientStream, streams::IOSocketStream<char>& filesStream, const map<string, string>& headers)
{
	const string& fileName = headers.at("File-Name");
	bool isLast;
	string data;
	intmax_t offset = stoull(headers.at("Range"));
	intmax_t totalFileSize;

	try
	{
		filesStream << filesRequests::downloadFile;

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

		utility::web::insertSizeHeaderToHTTPMessage(response);

		clientStream << response;
	}
	catch (const web::WebException& e)
	{
		cout << e.what() << endl;
	}
}

void removeFile(streams::IOSocketStream<char>& clientStream, streams::IOSocketStream<char>& filesStream, streams::IOSocketStream<char>& dataBaseStream, const map<string, string>& headers)
{
	const string& fileName = headers.at("File-Name");
	bool error;
	string responseMessage;

	try
	{
		filesStream << filesRequests::removeFile;

		filesStream << fileName;

		filesStream >> responseMessage;

		error = responseMessage != responses::okResponse;

		if (!error)
		{
			dataBaseStream << filesRequests::removeFile;

			dataBaseStream << fileName;
		}

		string response = web::HTTPBuilder().responseCode(web::ResponseCodes::ok).headers
		(
			"Error", error
		).build();

		utility::web::insertSizeHeaderToHTTPMessage(response);

		clientStream << response;
	}
	catch (const web::WebException& e)
	{
		cout << e.what() << endl;
	}
}

void cancelUploadFile(streams::IOSocketStream<char>& clientStream, streams::IOSocketStream<char>& filesStream, const map<string, string>& headers)
{
	const string& fileName = headers.at("File-Name");
	string response = web::HTTPBuilder().responseCode(web::ResponseCodes::ok).headers
	(
		"Reserved", 0
	).build();

	utility::web::insertSizeHeaderToHTTPMessage(response);

	filesStream << networkRequests::cancelOperation;

	filesStream << fileName;
	filesStream << filesRequests::uploadFile;

	clientStream << response;
}

void setLogin(streams::IOSocketStream<char>& filesStream, streams::IOSocketStream<char>& dataBaseStream, const string& data)
{
	auto [login, password] = userDataParse(data);
	string response;
	string responseMessage;
	bool error;

	try
	{
		dataBaseStream << accountRequests::authorization;
		dataBaseStream << login;
		dataBaseStream << password;

		dataBaseStream >> response;

		error = response != responses::okResponse;

		if (error)
		{
			//TODO: error message
		}
		else
		{
			filesStream << accountRequests::setLogin;
			filesStream << login;
		}
	}
	catch (const web::WebException& e)
	{
		cout << e.what() << endl;
	}
}

void nextFolder(streams::IOSocketStream<char>& clientStream, streams::IOSocketStream<char>& filesStream, streams::IOSocketStream<char>& dataBaseStream, const string& data)
{
	const string folder(begin(data) + data.find('=' + 1), end(data));
	string ok(responses::okResponse);
	string fail(responses::failResponse);
	bool filesStreamResponse = false;
	bool dataBaseStreamResponse = false;

	filesStream << controlRequests::nextFolder;
	filesStream << folder;
	filesStream >> filesStreamResponse;

	dataBaseStream << controlRequests::nextFolder;
	dataBaseStream << folder;
	dataBaseStream >> dataBaseStreamResponse;

	if (filesStreamResponse && dataBaseStreamResponse)
	{
		string response = web::HTTPBuilder().responseCode(web::ResponseCodes::ok).headers
		(
			"Content-Length", ok.size()
		).build(&ok);

		utility::web::insertSizeHeaderToHTTPMessage(response);

		clientStream << response;
	}
	else
	{
		//TODO:: error handling
	}
}

void prevFolder(streams::IOSocketStream<char>& clientStream, streams::IOSocketStream<char>& filesStream, streams::IOSocketStream<char>& dataBaseStream)
{
	string ok(responses::okResponse);
	string fail(responses::failResponse);
	bool filesStreamResponse = false;
	bool dataBaseStreamResponse = false;

	filesStream << controlRequests::prevFolder;
	filesStream >> filesStreamResponse;

	dataBaseStream << controlRequests::prevFolder;
	dataBaseStream >> dataBaseStreamResponse;

	if (filesStreamResponse && dataBaseStreamResponse)
	{
		string response = web::HTTPBuilder().responseCode(web::ResponseCodes::ok).headers
		(
			"Content-Length", ok.size()
		).build(&ok);

		utility::web::insertSizeHeaderToHTTPMessage(response);

		clientStream << response;
	}
	else
	{
		//TODO:: error handling
	}
}

void setPath(streams::IOSocketStream<char>& clientStream, streams::IOSocketStream<char>& filesStream, streams::IOSocketStream<char>& dataBaseStream, const string& data)
{
	const string folder(begin(data) + data.find('=' + 1), end(data));
	string ok(responses::okResponse);
	string fail(responses::failResponse);

	bool filesStreamResponse = false;
	bool dataBaseStreamResponse = false;

	filesStream << controlRequests::setPath;
	filesStream << folder;
	filesStream >> filesStreamResponse;

	dataBaseStream << controlRequests::setPath;
	dataBaseStream << folder;
	dataBaseStream >> dataBaseStreamResponse;

	if (filesStreamResponse && dataBaseStreamResponse)
	{
		string response = web::HTTPBuilder().responseCode(web::ResponseCodes::ok).headers
		(
			"Content-Length", ok.size()
		).build(&ok);

		utility::web::insertSizeHeaderToHTTPMessage(response);

		clientStream << response;
	}
	else
	{
		//TODO:: error handling
	}
}

void createFolder(streams::IOSocketStream<char>& filesStream, streams::IOSocketStream<char>& dataBaseStream, const string& data)
{
	const string folderName(begin(data) + data.find('=') + 1, end(data));
	string fileExtension;

	filesStream << filesRequests::createFolder;
	filesStream << folderName;

	filesStream >> fileExtension;

	dataBaseStream << filesRequests::createFolder;
	dataBaseStream << folderName;
	dataBaseStream << fileExtension;
}

void authorization(streams::IOSocketStream<char>& clientStream, streams::IOSocketStream<char>& filesStream, streams::IOSocketStream<char>& dataBaseStream, const string& data)
{
	auto [login, password] = userDataParse(data);
	string response;
	string responseMessage;
	bool error;

	try
	{
		dataBaseStream << accountRequests::authorization;
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

			filesStream << accountRequests::setLogin;
			filesStream << login;
		}

		utility::web::insertSizeHeaderToHTTPMessage(response);

		clientStream << response;
	}
	catch (const web::WebException& e)
	{
		cout << e.what() << endl;
	}
}

void registration(streams::IOSocketStream<char>& clientStream, streams::IOSocketStream<char>& filesStream, streams::IOSocketStream<char>& dataBaseStream, const string& data)
{
	auto [login, password] = userDataParse(data);
	string response;
	string responseMessage;
	bool error;

	try
	{
		dataBaseStream << accountRequests::registration;
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

			filesStream << accountRequests::setLogin;
			filesStream << login;
		}

		utility::web::insertSizeHeaderToHTTPMessage(response);

		clientStream << response;
	}
	catch (const web::WebException& e)
	{
		cout << e.what() << endl;
	}
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