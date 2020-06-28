#include "pch.h"

#include "APIServer.h"
#include "Constants.h"
#include "IOSocketStream.h"
#include "HTTPParser.h"
#include "HTTPBuilder.h"

#include "HTTPNetwork.h"
#include "FilesNetwork.h"

#pragma comment (lib, "BaseTCPServer.lib")
#pragma comment (lib, "SocketStreams.lib")
#pragma comment (lib, "Log.lib")
#pragma comment (lib, "HTTP.lib")

using namespace std;

bool checkHTTP(const string& request);

void showAllFilesInDirectory(streams::IOSocketStream<char>& clientStream, const string& login, const string& directory);

namespace web
{
	void APIServer::clientConnection(SOCKET clientSocket, sockaddr addr)
	{
		streams::IOSocketStream<char> clientStream(new buffers::IOSocketBuffer<char>(new HTTPNetwork(clientSocket)));
		HTTPNetwork network(clientSocket);
		string request;
		string response;

		while (true)
		{
			try
			{
				clientStream >> request;

				if (checkHTTP(request))
				{
					HTTPParser parser(request);
					const map<string, string>& headers = parser.getHeaders();
					auto it = headers.find("Account request");

					if (it != end(headers))
					{

					}
					else
					{
						it = headers.find("Files request");

						if (it != end(headers))
						{
							const string& request = it->second;

							if (request == filesRequests::downloadFile)
							{

							}
							else if (request == filesRequests::uploadFile)
							{

							}
							else if (request == filesRequests::showAllFilesInDirectory)
							{
								showAllFilesInDirectory(clientStream, headers.find("Login")->second, headers.find("Directory")->second);
							}
							else
							{

							}
						}
						else
						{
							return;
						}
					}
				}
				else
				{
					return;
				}
			}
			catch (const WebException&)
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

void showAllFilesInDirectory(streams::IOSocketStream<char>& clientStream, const string& login, const string& directory)
{
	streams::IOSocketStream<char> filesStream(new buffers::IOSocketBuffer<char>(new web::FilesNetwork()));
	string serverResponse;
	bool error;
	string response;

	try
	{
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

		clientStream << response;
	}
	catch (const web::WebException&)
	{

	}
}