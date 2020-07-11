#include "pch.h"

#include "APIServer.h"
#include "Constants.h"
#include "IOSocketStream.h"
#include "HTTPParser.h"
#include "HTTPBuilder.h"
#include "UtilityFunctions.h"

#include "HTTPNetwork.h"
#include "FilesNetwork.h"
#include "DataBaseNetwork.h"

#pragma comment (lib, "BaseTCPServer.lib")
#pragma comment (lib, "SocketStreams.lib")
#pragma comment (lib, "Log.lib")
#pragma comment (lib, "HTTP.lib")

using namespace std;

bool checkHTTP(const string& request);

void showAllFilesInDirectory(streams::IOSocketStream<char>& clientStream, streams::IOSocketStream<char>& filesStream, const string& login, const string& directory);

void uploadFile(streams::IOSocketStream<char>& clientStream, streams::IOSocketStream<char>& filesStream, const string& data, const map<string, string>& headers);

void downloadFile(streams::IOSocketStream<char>& clientStream, streams::IOSocketStream<char>& filesStream, const map<string, string>& headers);

void authorization(streams::IOSocketStream<char>& clientStream, streams::IOSocketStream<char>& dataBaseStream, const string& data);

void registration(streams::IOSocketStream<char>& clientStream, streams::IOSocketStream<char>& dataBaseStream, const string& data);

tuple<string, string> userDataParse(const string& data);

namespace web
{
	void APIServer::clientConnection(SOCKET clientSocket, sockaddr addr)
	{
		streams::IOSocketStream<char> clientStream(new buffers::IOSocketBuffer<char>(new HTTPNetwork(clientSocket)));
		streams::IOSocketStream<char> filesStream(new buffers::IOSocketBuffer<char>(new FilesNetwork()));
		streams::IOSocketStream<char> dataBaseStream(new buffers::IOSocketBuffer<char>(new DataBaseNetwork()));
		string request;

		while (true)
		{
			try
			{
				clientStream >> request;

				if (checkHTTP(request))
				{
					HTTPParser parser(request);
					const map<string, string>& headers = parser.getHeaders();
					auto it = headers.find(requestType::accountType);

					if (it != end(headers))
					{
						const string& requestType = it->second;

						if (requestType == accountRequest::authorization)
						{
							authorization(clientStream, dataBaseStream, parser.getBody());
						}
						else if (requestType == accountRequest::registration)
						{
							registration(clientStream, dataBaseStream, parser.getBody());
						}
						else
						{
							continue;
						}
					}
					else
					{
						it = headers.find(requestType::filesType);

						if (it != end(headers))
						{
							const string& requestType = it->second;

							if (requestType == filesRequests::showAllFilesInDirectory)
							{
								showAllFilesInDirectory(clientStream, filesStream, headers.at("Login"), headers.at("Directory"));
							}
							else if (requestType == filesRequests::uploadFile)
							{
								uploadFile(clientStream, filesStream, parser.getBody(), headers);
							}
							if (requestType == filesRequests::downloadFile)
							{
								downloadFile(clientStream, filesStream, headers);
							}
							else
							{
								continue;
							}
						}
						else
						{
							continue;
						}
					}
				}
				else
				{
					continue;
				}
			}
			catch (const WebException& e)
			{

			}
		}
	}

	APIServer::APIServer() : BaseTCPServer(APIServerPort, false)
	{

	}
}

bool checkHTTP(const string& request)
{
	return request.find("HTTP") != string::npos ? true : false;
}

void showAllFilesInDirectory(streams::IOSocketStream<char>& clientStream, streams::IOSocketStream<char>& filesStream, const string& login, const string& directory)
{
	string serverResponse;
	bool error;
	string response;

	try
	{
		filesStream << filesRequests::showAllFilesInDirectory;
		filesStream << login;
		filesStream << directory;

		filesStream >> serverResponse;

		if (serverResponse == responses::okResponse)
		{
			filesStream >> serverResponse;
			error = false;
		}
		else
		{
			filesStream >> serverResponse;
			error = true;
		}

		response = web::HTTPBuilder().responseCode(web::ResponseCodes::ok).headers
		(
			"Content-Length", serverResponse.size(),
			"Content-Type", "text/plain; charset=utf-8",
			"Error", error
		).build(&serverResponse);

		utility::insertSizeHeaderToHTTPMessage(response);

		clientStream << response;
	}
	catch (const web::WebException& e)
	{
		cout << e.what() << endl;
	}
}

void uploadFile(streams::IOSocketStream<char>& clientStream, streams::IOSocketStream<char>& filesStream, const string& data, const map<string, string>& headers)
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

		string response = web::HTTPBuilder().responseCode(web::ResponseCodes::ok).headers
		(
			"Error", isSuccess == responses::okResponse ? false : true,
			"Content-Length", responseMessage.size(),
			"Content-Type", "text/plain; charset=utf-8"
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