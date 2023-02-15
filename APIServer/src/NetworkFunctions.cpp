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

void showAllFilesInFolder(streams::IOSocketStream& clientStream, unique_ptr<streams::IOSocketStream>& dataBaseStream)
{
	if (!dataBaseStream)
	{
		string response = web::HTTPBuilder().responseCode(web::responseCodes::ok).headers
		(
			"Error", 1
		).build(responses::noConnectionWithDataBaseServer);

		try
		{
			clientStream << response;
		}
		catch (const web::exceptions::WebException&)
		{

		}

		return;
	}

	string responseMessage;
	bool error;
	string response;

	try
	{
		*dataBaseStream << filesRequests::showAllFilesInFolder;

		*dataBaseStream >> responseMessage;

		if (responseMessage == responses::okResponse)
		{
			*dataBaseStream >> responseMessage;
			error = false;
		}
		else
		{
			*dataBaseStream >> responseMessage;
			error = true;
		}

		response = web::HTTPBuilder().responseCode(web::responseCodes::ok).headers
		(
			"Error", error
		).build(responseMessage);

		try
		{
			clientStream << response;
		}
		catch (const web::exceptions::WebException&)
		{

		}
	}
	catch (const web::exceptions::WebException& e)
	{
		cout << e.what() << endl;
	}
}

void uploadFile(streams::IOSocketStream& clientStream, unique_ptr<streams::IOSocketStream>& filesStream, unique_ptr<streams::IOSocketStream>& dataBaseStream, const string& data, const unordered_map<string, string, web::HTTPParser::insensitiveStringHash, web::HTTPParser::insensitiveStringEqual>& headers)
{
	if (!filesStream)
	{
		string response = web::HTTPBuilder().responseCode(web::responseCodes::ok).headers
		(
			"Error", 1
		).build(responses::noConnectionWithFilesServer);

		try
		{
			clientStream << response;
		}
		catch (const web::exceptions::WebException&)
		{

		}

		return;
	}

	if (!dataBaseStream)
	{
		string response = web::HTTPBuilder().responseCode(web::responseCodes::ok).headers
		(
			"Error", 1
		).build(responses::noConnectionWithDataBaseServer);

		try
		{
			clientStream << response;
		}
		catch (const web::exceptions::WebException&)
		{

		}

		return;
	}

	const string& fileName = headers.at("File-Name");
	intmax_t offset = stoull(headers.at("Range"));
	auto it = headers.find("Total-File-Size");
	bool needResponse = it != end(headers);

	*filesStream << filesRequests::uploadFile;

	*filesStream << fileName;
	*filesStream << offset;
	*filesStream << data;
	*filesStream << needResponse;

	if (needResponse)
	{
		intmax_t uploadSize = stoull(it->second);
		string responseMessage;
		string isSuccess;

		try
		{
			*filesStream << uploadSize;

			*filesStream >> isSuccess;
			*filesStream >> responseMessage;

			if (isSuccess == responses::okResponse)
			{
				string extension;

				*filesStream >> extension;

				*dataBaseStream << filesRequests::uploadFile;
				*dataBaseStream << fileName;
				*dataBaseStream << extension;
				*dataBaseStream << uploadSize;
			}

			string response = web::HTTPBuilder().responseCode(web::responseCodes::ok).headers
			(
				"Error", isSuccess == responses::okResponse ? false : true
			).build(responseMessage);

			try
			{
				clientStream << response;
			}
			catch (const web::exceptions::WebException&)
			{

			}
		}
		catch (const web::exceptions::WebException&)
		{

		}
	}
}

void downloadFile(streams::IOSocketStream& clientStream, unique_ptr<streams::IOSocketStream>& filesStream, const unordered_map<string, string, web::HTTPParser::insensitiveStringHash, web::HTTPParser::insensitiveStringEqual>& headers)
{
	if (!filesStream)
	{
		string response = web::HTTPBuilder().responseCode(web::responseCodes::ok).headers
		(
			"Error", 1
		).build(responses::noConnectionWithFilesServer);

		try
		{
			clientStream << response;
		}
		catch (const web::exceptions::WebException&)
		{

		}

		return;
	}

	const string& fileName = headers.at("File-Name");
	bool isLast;
	string data;
	intmax_t offset = stoull(headers.at("Range"));
	intmax_t totalFileSize;

	try
	{
		*filesStream << filesRequests::downloadFile;

		*filesStream << fileName;
		*filesStream << offset;

		*filesStream >> data;
		*filesStream >> isLast;

		if (isLast)
		{
			*filesStream >> totalFileSize;
		}

		string response = web::HTTPBuilder().responseCode(web::responseCodes::ok).headers
		(
			isLast ? "Total-File-Size" : "Reserved", isLast ? totalFileSize : 0
		).build(data);

		try
		{
			clientStream << response;
		}
		catch (const web::exceptions::WebException&)
		{

		}
	}
	catch (const web::exceptions::WebException& e)
	{
		cout << e.what() << endl;
	}
}

void removeFile(streams::IOSocketStream& clientStream, unique_ptr<streams::IOSocketStream>& filesStream, unique_ptr<streams::IOSocketStream>& dataBaseStream, const unordered_map<string, string, web::HTTPParser::insensitiveStringHash, web::HTTPParser::insensitiveStringEqual>& headers)
{
	if (!filesStream)
	{
		string response = web::HTTPBuilder().responseCode(web::responseCodes::ok).headers
		(
			"Error", 1
		).build(responses::noConnectionWithFilesServer);

		try
		{
			clientStream << response;
		}
		catch (const web::exceptions::WebException&)
		{

		}

		return;
	}

	if (!dataBaseStream)
	{
		string response = web::HTTPBuilder().responseCode(web::responseCodes::ok).headers
		(
			"Error", 1
		).build(responses::noConnectionWithDataBaseServer);

		try
		{
			clientStream << response;
		}
		catch (const web::exceptions::WebException&)
		{

		}

		return;
	}

	const string& fileName = headers.at("File-Name");
	bool error;
	string responseMessage;

	try
	{
		*filesStream << filesRequests::removeFile;

		*filesStream << fileName;

		*filesStream >> responseMessage;

		error = responseMessage != responses::okResponse;

		if (!error)
		{
			*dataBaseStream << filesRequests::removeFile;

			*dataBaseStream << fileName;
		}

		string response = web::HTTPBuilder().responseCode(web::responseCodes::ok).headers
		(
			"Error", error
		).build();

		try
		{
			clientStream << response;
		}
		catch (const web::exceptions::WebException&)
		{

		}
	}
	catch (const web::exceptions::WebException& e)
	{
		cout << e.what() << endl;
	}
}

void cancelUploadFile(streams::IOSocketStream& clientStream, unique_ptr<streams::IOSocketStream>& filesStream, const unordered_map<string, string, web::HTTPParser::insensitiveStringHash, web::HTTPParser::insensitiveStringEqual>& headers)
{
	if (!filesStream)
	{
		string response = web::HTTPBuilder().responseCode(web::responseCodes::ok).headers
		(
			"Error", 1
		).build(responses::noConnectionWithFilesServer);

		try
		{
			clientStream << response;
		}
		catch (const web::exceptions::WebException&)
		{

		}

		return;
	}

	const string& fileName = headers.at("File-Name");
	string response = web::HTTPBuilder().responseCode(web::responseCodes::ok).headers
	(
		"Reserved", 0
	).build();

	try
	{
		*filesStream << networkRequests::cancelOperation;

		*filesStream << fileName;
		*filesStream << filesRequests::uploadFile;

		clientStream << response;
	}
	catch (const web::exceptions::WebException&)
	{

	}
}

void setLogin(streams::IOSocketStream& clientStream, unique_ptr<streams::IOSocketStream>& filesStream, unique_ptr<streams::IOSocketStream>& dataBaseStream, const string& data)
{
	auto [login, password] = userDataParse(data);
	string response;
	string responseMessage;
	bool error;

	try
	{
		*dataBaseStream << accountRequests::authorization;
		*dataBaseStream << login;
		*dataBaseStream << password;

		*dataBaseStream >> response;

		error = response != responses::okResponse;

		responseMessage = web::HTTPBuilder().responseCode(web::responseCodes::ok).headers
		(
			"Error", error
		).build();

		if (error)
		{
			//TODO: error message
		}
		else
		{
			*filesStream << accountRequests::setLogin;
			*filesStream << login;
		}

		clientStream << responseMessage;
	}
	catch (const web::exceptions::WebException& e)
	{
		cout << e.what() << endl;
	}
}

void nextFolder(streams::IOSocketStream& clientStream, unique_ptr<streams::IOSocketStream>& filesStream, unique_ptr<streams::IOSocketStream>& dataBaseStream, const string& data)
{
	if (!filesStream)
	{
		string response = web::HTTPBuilder().responseCode(web::responseCodes::ok).headers
		(
			"Error", 1
		).build(responses::noConnectionWithFilesServer);

		try
		{
			clientStream << response;
		}
		catch (const web::exceptions::WebException&)
		{

		}

		return;
	}

	if (!dataBaseStream)
	{
		string response = web::HTTPBuilder().responseCode(web::responseCodes::ok).headers
		(
			"Error", 1
		).build(responses::noConnectionWithDataBaseServer);

		try
		{
			clientStream << response;
		}
		catch (const web::exceptions::WebException&)
		{

		}

		return;
	}

	const string folder(begin(data) + data.find('=') + 1, end(data));
	string ok(responses::okResponse);
	string fail(responses::failResponse);
	bool filesStreamResponse = false;
	bool dataBaseStreamResponse = false;

	try
	{
		*filesStream << controlRequests::nextFolder;
		*filesStream << folder;
		*filesStream >> filesStreamResponse;

		*dataBaseStream << controlRequests::nextFolder;
		*dataBaseStream << folder;
		*dataBaseStream >> dataBaseStreamResponse;

		if (filesStreamResponse && dataBaseStreamResponse)
		{
			string response = web::HTTPBuilder().responseCode(web::responseCodes::ok).headers
			(
				"Error", 0
			).build(ok);

			clientStream << response;
		}
		else
		{
			string response = web::HTTPBuilder().responseCode(web::responseCodes::ok).headers
			(
				"Error", 1
			).build(fail);

			clientStream << response;
		}
	}
	catch (const web::exceptions::WebException&)
	{

	}
}

void prevFolder(streams::IOSocketStream& clientStream, unique_ptr<streams::IOSocketStream>& filesStream, unique_ptr<streams::IOSocketStream>& dataBaseStream)
{
	if (!filesStream)
	{
		string response = web::HTTPBuilder().responseCode(web::responseCodes::ok).headers
		(
			"Error", 1
		).build(responses::noConnectionWithFilesServer);

		try
		{
			clientStream << response;
		}
		catch (const web::exceptions::WebException&)
		{

		}

		return;
	}

	if (!dataBaseStream)
	{
		string response = web::HTTPBuilder().responseCode(web::responseCodes::ok).headers
		(
			"Error", 1
		).build(responses::noConnectionWithDataBaseServer);

		try
		{
			clientStream << response;
		}
		catch (const web::exceptions::WebException&)
		{

		}

		return;
	}

	string ok(responses::okResponse);
	string fail(responses::failResponse);
	bool filesStreamResponse = false;
	bool dataBaseStreamResponse = false;

	*filesStream << controlRequests::prevFolder;
	*filesStream >> filesStreamResponse;

	*dataBaseStream << controlRequests::prevFolder;
	*dataBaseStream >> dataBaseStreamResponse;

	if (filesStreamResponse && dataBaseStreamResponse)
	{
		string response = web::HTTPBuilder().responseCode(web::responseCodes::ok).headers
		(
			"Error", 0
		).build(ok);

		clientStream << response;
	}
	else
	{
		string response = web::HTTPBuilder().responseCode(web::responseCodes::ok).headers
		(
			"Error", 1
		).build(fail);

		clientStream << response;
	}
}

void setPath(streams::IOSocketStream& clientStream, unique_ptr<streams::IOSocketStream>& filesStream, unique_ptr<streams::IOSocketStream>& dataBaseStream, const string& data)
{
	if (!filesStream)
	{
		string response = web::HTTPBuilder().responseCode(web::responseCodes::ok).headers
		(
			"Error", 1
		).build(responses::noConnectionWithFilesServer);

		try
		{
			clientStream << response;
		}
		catch (const web::exceptions::WebException&)
		{

		}

		return;
	}

	if (!dataBaseStream)
	{
		string response = web::HTTPBuilder().responseCode(web::responseCodes::ok).headers
		(
			"Error", 1
		).build(responses::noConnectionWithDataBaseServer);

		try
		{
			clientStream << response;
		}
		catch (const web::exceptions::WebException&)
		{

		}

		return;
	}

	const string folder(begin(data) + data.find('=') + 1, end(data));
	string ok(responses::okResponse);
	string fail(responses::failResponse);

	bool filesStreamResponse = false;
	bool dataBaseStreamResponse = false;

	*filesStream << controlRequests::setPath;
	*filesStream << folder;
	*filesStream >> filesStreamResponse;

	*dataBaseStream << controlRequests::setPath;
	*dataBaseStream << folder;
	*dataBaseStream >> dataBaseStreamResponse;

	if (filesStreamResponse && dataBaseStreamResponse)
	{
		string response = web::HTTPBuilder().responseCode(web::responseCodes::ok).headers
		(
			"Error", 0
		).build(ok);

		clientStream << response;
	}
	else
	{
		string response = web::HTTPBuilder().responseCode(web::responseCodes::ok).headers
		(
			"Error", 1
		).build(fail);

		clientStream << response;
	}
}

void createFolder(streams::IOSocketStream& clientStream, unique_ptr<streams::IOSocketStream>& filesStream, unique_ptr<streams::IOSocketStream>& dataBaseStream, const string& data)
{
	const string folderName(begin(data) + data.find('=') + 1, end(data));
	string fileExtension;
	string response;

	try
	{
		*filesStream << filesRequests::createFolder;
		*filesStream << folderName;

		*filesStream >> fileExtension;

		*dataBaseStream << filesRequests::createFolder;
		*dataBaseStream << folderName;
		*dataBaseStream << fileExtension;

		response = web::HTTPBuilder().responseCode(web::responseCodes::ok).headers
		(
			"Error", 0
		).build();

		clientStream << response;
	}
	catch (const web::exceptions::WebException&)
	{

	}
}

void authorization(streams::IOSocketStream& clientStream, unique_ptr<streams::IOSocketStream>& filesStream, unique_ptr<streams::IOSocketStream>& dataBaseStream, const string& data)
{
	if (!dataBaseStream)
	{
		string response = web::HTTPBuilder().responseCode(web::responseCodes::ok).headers
		(
			"Error", 1
		).build(responses::noConnectionWithDataBaseServer);

		try
		{
			clientStream << response;
		}
		catch (const web::exceptions::WebException&)
		{

		}

		return;
	}

	if (!filesStream)
	{
		string response = web::HTTPBuilder().responseCode(web::responseCodes::ok).headers
		(
			"Error", 1
		).build(responses::noConnectionWithFilesServer);

		try
		{
			clientStream << response;
		}
		catch (const web::exceptions::WebException&)
		{

		}

		return;
	}

	auto [login, password] = userDataParse(data);
	string response;
	string responseMessage;
	bool error;

	try
	{
		*dataBaseStream << accountRequests::authorization;
		*dataBaseStream << login;
		*dataBaseStream << password;

		*dataBaseStream >> response;

		error = response != responses::okResponse;

		if (error)
		{
			responseMessage = move(response);

			response = web::HTTPBuilder().responseCode(web::responseCodes::ok).headers
			(
				"Error", error
			).build(responseMessage);

		}
		else
		{
			response = web::HTTPBuilder().responseCode(web::responseCodes::ok).headers
			(
				"Error", error
			).build();

			*filesStream << accountRequests::setLogin;
			*filesStream << login;
		}

		clientStream << response;
	}
	catch (const web::exceptions::WebException& e)
	{
		cout << e.what() << endl;
	}
}

void registration(streams::IOSocketStream& clientStream, unique_ptr<streams::IOSocketStream>& filesStream, unique_ptr<streams::IOSocketStream>& dataBaseStream, const string& data)
{
	if (!dataBaseStream)
	{
		string response = web::HTTPBuilder().responseCode(web::responseCodes::ok).headers
		(
			"Error", 1
		).build(responses::noConnectionWithDataBaseServer);

		try
		{
			clientStream << response;
		}
		catch (const web::exceptions::WebException&)
		{

		}

		return;
	}

	if (!filesStream)
	{
		string response = web::HTTPBuilder().responseCode(web::responseCodes::ok).headers
		(
			"Error", 1
		).build(responses::noConnectionWithFilesServer);

		try
		{
			clientStream << response;
		}
		catch (const web::exceptions::WebException&)
		{

		}

		return;
	}

	auto [login, password] = userDataParse(data);
	string response;
	string responseMessage;
	bool error;

	try
	{
		*dataBaseStream << accountRequests::registration;
		*dataBaseStream << login;
		*dataBaseStream << password;

		*dataBaseStream >> response;

		error = response == accountResponses::failRegistration;

		if (error)
		{
			responseMessage = move(response);

			response = web::HTTPBuilder().responseCode(web::responseCodes::ok).headers
			(
				"Error", error
			).build(responseMessage);

		}
		else
		{
			response = web::HTTPBuilder().responseCode(web::responseCodes::ok).headers
			(
				"Error", error
			).build();

			*filesStream << accountRequests::setLogin;
			*filesStream << login;
		}

		clientStream << response;
	}
	catch (const web::exceptions::WebException& e)
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