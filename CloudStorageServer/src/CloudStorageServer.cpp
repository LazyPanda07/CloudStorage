#include "pch.h"

#include "CloudStorageServer.h"
#include "IOSocketStream.h"
#include "Log.h"
#include "Constants.h"

#include "FilesNetwork.h"

#pragma comment (lib, "BaseTCPServer.lib")
#pragma comment (lib, "SocketStreams.lib")
#pragma comment (lib, "Log.lib")

using namespace std;

void showAllFilesInDirectory(streams::IOSocketStream<char>& clientStream, const filesystem::path& currentPath);

void findDirectory(filesystem::path& currentPath, const string& directory);

namespace web
{
	void CloudStorageServer::clientConnection(SOCKET clientSocket, sockaddr addr)
	{
		streams::IOSocketStream<char> clientStream(new buffers::IOSocketBuffer<char>(new FilesNetwork(clientSocket)));
		filesystem::path currentPath(filesystem::current_path());
		string request;
		string login;
		string directory;

		clientStream >> request;
		clientStream >> login;
		clientStream >> directory;

		currentPath.append(login);

		findDirectory(currentPath, directory);

		if (request == filesRequests::showAllFilesInDirectory)
		{
			showAllFilesInDirectory(clientStream, currentPath);
		}
		else if (request == filesRequests::uploadFile)
		{

		}
		else if (request == filesRequests::downloadFile)
		{

		}
	}

	CloudStorageServer::CloudStorageServer() : BaseTCPServer(cloudStorageServerPort, false)
	{

	}
}

void showAllFilesInDirectory(streams::IOSocketStream<char>& clientStream, const filesystem::path& currentPath)
{
	string result;
	filesystem::directory_iterator it(currentPath);
	
	for (const auto& i : it)
	{
		result += i.path().filename().string() + ":";
	}

	if (result.empty())
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

void findDirectory(filesystem::path& currentPath, const string& directory)
{
	filesystem::recursive_directory_iterator it(currentPath);

	for (const auto& i : it)
	{
		if (filesystem::is_directory(i) && i.path().filename() == directory)
		{
			currentPath = i;
			return;
		}
	}
}