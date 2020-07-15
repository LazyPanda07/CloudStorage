#include "pch.h"

#include "DataBaseServer.h"
#include "Constants.h"
#include "DataBaseNetwork.h"
#include "IOSocketStream.h"
#include "DataBaseFunctions.h"

#pragma comment (lib, "BaseTCPServer.lib")
#pragma comment (lib, "Log.lib")
#pragma comment (lib, "SocketStreams.lib")

using namespace std;

namespace web
{
	void DataBaseServer::clientConnection(SOCKET clientSocket, sockaddr addr)
	{
		streams::IOSocketStream<char> clientStream(new buffers::IOSocketBuffer<char>(new DataBaseNetwork(clientSocket)));
		string login;
		//TODO: filesystem::path currentDirectory that save user's current directory

		while (true)
		{
			try
			{
				string request;

				clientStream >> request;

				if (request == accountRequest::authorization)
				{
					authorization(clientStream, db, login);
				}
				else if (request == accountRequest::registration)
				{
					registration(clientStream, db, login);
				}
				else if (request == filesRequests::showAllFilesInDirectory)
				{
					showAllFilesInDirectory(clientStream, db, login);
				}
				else if (request == filesRequests::uploadFile)
				{
					uploadFileData(clientStream, db, login);
				}
				else if (request == filesRequests::removeFile)
				{
					removeFileData(clientStream, db, login);
				}
			}
			catch (const WebException&)
			{
				return;
			}
		}
	}

	DataBaseServer::DataBaseServer() : 
		BaseTCPServer(dataBaseServerPort, false)
	{

	}

	void DataBaseServer::setDataBaseServerPort(string&& port) noexcept
	{
		dataBaseServerPort = move(port);
	}
}