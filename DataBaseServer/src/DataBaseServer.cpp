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
}