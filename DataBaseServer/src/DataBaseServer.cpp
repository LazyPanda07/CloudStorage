#include "pch.h"

#include "DataBaseServer.h"
#include "Constants.h"
#include "DataBaseNetwork.h"
#include "BaseIOSocketStream.h"
#include "DataBaseFunctions.h"

#pragma comment (lib, "BaseTCPServer.lib")
#pragma comment (lib, "Log.lib")
#pragma comment (lib, "SocketStreams.lib")

using namespace std;

namespace web
{
	void DataBaseServer::clientConnection(SOCKET clientSocket, sockaddr addr)
	{
		streams::IOSocketStream clientStream(make_unique<DataBaseNetwork>(clientSocket));
		string login;
		filesystem::path currentPath("Home");

		while (true)
		{
			try
			{
				string request;

				clientStream >> request;

				if (request == accountRequests::authorization)
				{
					authorization(clientStream, db, login);
				}
				else if (request == accountRequests::registration)
				{
					registration(clientStream, db, login);
				}
				else if (request == filesRequests::showAllFilesInFolder)
				{
					showAllFilesInFolder(clientStream, db, login, currentPath);
				}
				else if (request == filesRequests::uploadFile)
				{
					uploadFileData(clientStream, db, login, currentPath);
				}
				else if (request == filesRequests::removeFile)
				{
					removeFileData(clientStream, db, login, currentPath);
				}
				else if (request == controlRequests::nextFolder)
				{
					nextFolder(clientStream, currentPath);
				}
				else if (request == controlRequests::prevFolder)
				{
					prevFolder(clientStream, currentPath);
				}
				else if (request == controlRequests::setPath)
				{
					setPath(clientStream, currentPath);
				}
				else if (request == filesRequests::createFolder)
				{
					createFolder(clientStream, db, login, currentPath);
				}
				else if (request == networkRequests::exit)
				{
					data.erase(ip);
					return;
				}
			}
			catch (const exceptions::WebException&)
			{
				return;
			}
		}
	}

	DataBaseServer::DataBaseServer() : 
		BaseTCPServer(dataBaseServerPort, "0.0.0.0", serverTimeoutRecv, false)
	{

	}

	void DataBaseServer::setDataBaseServerPort(string&& port) noexcept
	{
		dataBaseServerPort = move(port);
	}
}