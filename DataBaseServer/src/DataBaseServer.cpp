#include "pch.h"

#include "DataBaseServer.h"
#include "Constants.h"
#include "DataBaseNetwork.h"
#include "IOSocketStream.h"

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
		//TODO: Сделать переменную currentDirectory, которая будет изменяться каждый раз, когда пользователь будет переходить по директориям

		while (true)
		{
			try
			{
				string request;

				clientStream >> request;

				if (request == accountRequest::authorization)
				{
					string password;

					clientStream >> login;
					clientStream >> password;

					if (db.authorization(login, password))
					{
						clientStream << responses::okResponse;
					}
					else
					{
						clientStream << accountResponses::failAuthorization;
					}
				}
				else if (request == accountRequest::registration)
				{
					string password;

					clientStream >> login;
					clientStream >> password;

					if (db.registration(login, password).size())
					{
						clientStream << accountResponses::failRegistration;
					}
					else
					{
						clientStream << accountResponses::successRegistration;
					}
				}
				else if (request == filesRequests::showAllFilesInDirectory)
				{
					string directory;
					string result;

					clientStream >> directory;

					vector<db::fileData> data = db.getFiles(login);

					for (const auto& i : data)
					{
						result += i;
					}

					if (data.empty())
					{
						clientStream << responses::failResponse;

						clientStream << filesResponses::emptyDirectory;
					}
					else
					{
						clientStream << responses::okResponse;

						clientStream << result;
					}
				}
			}
			catch (const WebException&)
			{
				return;
			}
		}
	}

	DataBaseServer::DataBaseServer() : BaseTCPServer(dataBaseServerPort, false)
	{

	}

	void DataBaseServer::setDataBaseServerPort(string&& port) noexcept
	{
		dataBaseServerPort = move(port);
	}
}