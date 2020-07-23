#include "pch.h"

#include "APIServer.h"
#include "Constants.h"
#include "IOSocketStream.h"
#include "HTTPParser.h"
#include "NetworkFunctions.h"
#include "HTTPNetwork.h"
#include "FilesNetwork.h"
#include "DataBaseNetwork.h"

#pragma comment (lib, "BaseTCPServer.lib")
#pragma comment (lib, "SocketStreams.lib")
#pragma comment (lib, "Log.lib")
#pragma comment (lib, "HTTP.lib")

using namespace std;

string getIp(sockaddr& addr);

namespace web
{
	void APIServer::clientConnection(SOCKET clientSocket, sockaddr addr)
	{
		streams::IOSocketStream<char> clientStream(new buffers::IOSocketBuffer<char>(new HTTPNetwork(clientSocket)));
		streams::IOSocketStream<char> filesStream(new buffers::IOSocketBuffer<char>(new FilesNetwork()));
		streams::IOSocketStream<char> dataBaseStream(new buffers::IOSocketBuffer<char>(new DataBaseNetwork()));
		string HTTPRequest;

		clients.insert(this_thread::get_id(), getIp(addr));

		while (true)
		{
			try
			{
				clientStream >> HTTPRequest;

				if (checkHTTP(HTTPRequest))
				{
					HTTPParser parser(HTTPRequest);
					const map<string, string>& headers = parser.getHeaders();
					auto it = headers.find(requestType::accountType);

					if (it != end(headers))
					{
						const string& request = it->second;

						if (request == accountRequests::authorization)
						{
							authorization(clientStream, dataBaseStream, parser.getBody());
						}
						else if (request == accountRequests::registration)
						{
							registration(clientStream, dataBaseStream, parser.getBody());
						}
						else if (request == accountRequests::setLogin)
						{
							setLogin(filesStream, dataBaseStream, parser.getBody());
						}
					}
					else
					{
						it = headers.find(requestType::filesType);

						if (it != end(headers))
						{
							const string& request = it->second;

							if (request == filesRequests::showAllFilesInFolder)
							{
								showAllFilesInFolder(clientStream, dataBaseStream, headers.at("Folder"));
							}
							else if (request == filesRequests::uploadFile)
							{
								uploadFile(clientStream, filesStream, dataBaseStream, parser.getBody(), headers);
							}
							if (request == filesRequests::downloadFile)
							{
								downloadFile(clientStream, filesStream, headers);
							}
							else if (request == filesRequests::removeFile)
							{
								removeFile(clientStream, filesStream, dataBaseStream, headers);
							}
						}
						else
						{
							it = headers.find(requestType::exitType);

							if (it != end(headers))
							{
								const string& request = it->second;

								if (request == networkRequests::exit)
								{
									clients.erase(this_thread::get_id());
									dataBaseStream << networkRequests::exit;
									filesStream << networkRequests::exit;
									this_thread::sleep_for(0.6s);
									return;
								}
							}
							else
							{
								it = headers.find(requestType::cancelType);

								if (it != end(headers))
								{
									const string& operationType = headers.at("Operation-Type");

									if (operationType == filesRequests::uploadFile)
									{
										cancelUploadFile(clientStream,filesStream, headers);
									}
								}
							}
						}
					}
				}
			}
			catch (const WebException&)
			{
				clients.erase(this_thread::get_id());
				return;
			}
		}
	}

	APIServer::APIServer() :
		BaseTCPServer(APIServerPort, serverTimeoutRecv, false)
	{

	}

	void APIServer::setAPIServerPort(string&& port) noexcept
	{
		APIServerPort = move(port);
	}

	vector<pair<thread::id, string>> APIServer::getClients() noexcept
	{
		return clients.getClients();
	}
}

string getIp(sockaddr& addr)
{
	string ip;

	ip.resize(16);

	inet_ntop(AF_INET, reinterpret_cast<const char*>(&reinterpret_cast<sockaddr_in*>(&addr)->sin_addr), ip.data(), ip.size());

	while (ip.back() == '\0')
	{
		ip.pop_back();
	}

	return ip;
}