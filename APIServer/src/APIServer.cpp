#include "pch.h"

#include "APIServer.h"
#include "Constants.h"
#include "IOSocketStream.h"
#include "HTTPParser.h"
#include "NetworkFunctions.h"
#include "HTTPNetwork.h"
#include "FilesNetwork.h"
#include "DataBaseNetwork.h"
#include "ConnectToOtherServers.h"

#pragma comment (lib, "BaseTCPServer.lib")
#pragma comment (lib, "SocketStreams.lib")
#pragma comment (lib, "Log.lib")
#pragma comment (lib, "HTTP.lib")

using namespace std;

namespace web
{
	void APIServer::clientConnection(SOCKET clientSocket, sockaddr addr)
	{
		streams::IOSocketStream<char> clientStream(new buffers::IOSocketBuffer<char>(new HTTPNetwork(clientSocket)));
		unique_ptr<streams::IOSocketStream<char>> filesStream = nullptr;
		unique_ptr<streams::IOSocketStream<char>> dataBaseStream = nullptr;
		string HTTPRequest;
		const string ip = getIpV4(addr);

		connectToFilesServer(filesStream);

		connectToDataBaseServer(dataBaseStream);

		while (true)
		{
			try
			{
				clientStream >> HTTPRequest;

				if (checkHTTP(HTTPRequest))
				{
					HTTPParser parser(HTTPRequest);
					const unordered_map<string, string>& headers = parser.getHeaders();
					auto it = headers.find(requestType::accountType);

					if (it != end(headers))
					{
						const string& request = it->second;

						if (request == accountRequests::authorization)
						{
							authorization(clientStream, *filesStream, *dataBaseStream, parser.getBody());
						}
						else if (request == accountRequests::registration)
						{
							registration(clientStream, *filesStream, *dataBaseStream, parser.getBody());
						}
						else if (request == accountRequests::setLogin)
						{
							setLogin(*filesStream, *dataBaseStream, parser.getBody());
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
								showAllFilesInFolder(clientStream, *dataBaseStream);
							}
							else if (request == filesRequests::uploadFile)
							{
								uploadFile(clientStream, *filesStream, *dataBaseStream, parser.getBody(), headers);
							}
							if (request == filesRequests::downloadFile)
							{
								downloadFile(clientStream, *filesStream, headers);
							}
							else if (request == filesRequests::removeFile)
							{
								removeFile(clientStream, *filesStream, *dataBaseStream, headers);
							}
							else if (request == filesRequests::createFolder)
							{
								createFolder(*filesStream, *dataBaseStream, parser.getBody());
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
									data.erase(ip);
									*dataBaseStream << networkRequests::exit;
									*filesStream << networkRequests::exit;
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
										cancelUploadFile(clientStream, *filesStream, headers);
									}
								}
								else
								{
									it = headers.find(requestType::controlType);

									if (it != end(headers))
									{
										const string& request = it->second;

										if (request == controlRequests::nextFolder)
										{
											nextFolder(clientStream, *filesStream, *dataBaseStream, parser.getBody());
										}
										else if (request == controlRequests::prevFolder)
										{
											prevFolder(clientStream, *filesStream, *dataBaseStream);
										}
										else if (request == controlRequests::setPath)
										{
											setPath(clientStream, *filesStream, *dataBaseStream, parser.getBody());
										}
									}
								}
							}
						}
					}
				}
			}
			catch (const WebException&)
			{
				data.erase(ip);
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
}