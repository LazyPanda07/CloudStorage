#include "pch.h"

#include "DataBaseServer.h"
#include "Constants.h"
#include "DataBaseNetwork.h"
#include "IOSocketStream.h"

#pragma comment (lib, "BaseTCPServer.lib")
#pragma comment (lib, "Log.lib")
#pragma comment (lib, "SocketStreams.lib")

using namespace std;

//login initialize in this function
void authorization(streams::IOSocketStream<char>& clientStream, const db::CloudDataBase& db, string& login);

//login initialize in this function
void registration(streams::IOSocketStream<char>& clientStream, const db::CloudDataBase& db, string& login);

void showAllFilesInDirectory(streams::IOSocketStream<char>& clientStream, const db::CloudDataBase& db, const string& login);

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

void authorization(streams::IOSocketStream<char>& clientStream, const db::CloudDataBase& db, string& login)
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

void registration(streams::IOSocketStream<char>& clientStream, const db::CloudDataBase& db, string& login)
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

void showAllFilesInDirectory(streams::IOSocketStream<char>& clientStream, const db::CloudDataBase& db, const string& login)
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