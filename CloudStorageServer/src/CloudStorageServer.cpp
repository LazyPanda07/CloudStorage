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
		//TODO: filesystem::path currentDirectory that save user's current directory

		while (true)
		{
			try
			{
				filesystem::path currentPath(filesystem::current_path());
				string request;
				string login;
				string directory;
				string end;

				clientStream >> request;
				clientStream >> login;
				clientStream >> directory;

				currentPath.append(usersDirectory);
				currentPath.append(login);

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