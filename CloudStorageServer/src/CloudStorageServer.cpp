#include "pch.h"

#include "CloudStorageServer.h"
#include "IOSocketStream.h"
#include "Log.h"
#include "Constants.h"
#include "FilesNetwork.h"
#include "FilesFunctions.h"

#pragma comment (lib, "BaseTCPServer.lib")
#pragma comment (lib, "SocketStreams.lib")
#pragma comment (lib, "Log.lib")

using namespace std;

namespace web
{
	void CloudStorageServer::clientConnection(SOCKET clientSocket, sockaddr addr)
	{
		streams::IOSocketStream<char> clientStream(new buffers::IOSocketBuffer<char>(new FilesNetwork(clientSocket)));
		string login;
		filesystem::path currentPath(filesystem::current_path());

		while (true)
		{
			try
			{
				string request;

				clientStream >> request;

				if (!filesystem::exists(currentPath))
				{
					filesystem::create_directories(currentPath);
				}

				if (request == filesRequests::uploadFile)
				{
					uploadFile(clientStream, currentPath);
				}
				else if (request == filesRequests::downloadFile)
				{
					downloadFile(clientStream, currentPath);
				}
				else if (request == filesRequests::removeFile)
				{
					removeFile(clientStream, currentPath);
				}
				else if (request == networkRequests::cancelOperation)
				{
					cancelOperation(clientStream, currentPath);
				}
				else if (request == accountRequests::setLogin)
				{
					clientStream >> login;

					currentPath = filesystem::current_path().append(usersFolder).append(login);
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
					setPath(clientStream, currentPath, login);
				}
				else if (request == networkRequests::exit)
				{
					return;
				}
			}
			catch (const WebException&)
			{
				return;
			}
		}
	}

	CloudStorageServer::CloudStorageServer() :
		BaseTCPServer(cloudStorageServerPort, serverTimeoutRecv, false)
	{

	}

	void CloudStorageServer::setCloudStorageServerPort(string&& port) noexcept
	{
		cloudStorageServerPort = move(port);
	}
}