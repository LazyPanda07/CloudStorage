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

void uploadFile(streams::IOSocketStream<char>& clientStream, const filesystem::path& currentPath);

void downloadFile(streams::IOSocketStream<char>& clientStream, const filesystem::path& currentPath);

void removeFile(streams::IOSocketStream<char>& clientStream, const filesystem::path& currentPath);

namespace web
{
	void CloudStorageServer::clientConnection(SOCKET clientSocket, sockaddr addr)
	{
		streams::IOSocketStream<char> clientStream(new buffers::IOSocketBuffer<char>(new FilesNetwork(clientSocket)));

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
			}
			catch (const WebException&)
			{
				return;
			}
		}
	}

	CloudStorageServer::CloudStorageServer() : BaseTCPServer(cloudStorageServerPort, false)
	{

	}

	void CloudStorageServer::setCloudStorageServerPort(string&& port) noexcept
	{
		cloudStorageServerPort = move(port);
	}
}

void uploadFile(streams::IOSocketStream<char>& clientStream, const filesystem::path& currentPath)
{
	string fileName;
	uintmax_t offset;
	string data;
	bool needResponse;

	clientStream >> fileName;
	clientStream >> offset;
	clientStream >> data;
	clientStream >> needResponse;

	ofstream out(filesystem::path(currentPath).append(fileName), ios::binary | ios::app);

	out.seekp(offset);

	out.write(data.data(), data.size());

	out.close();

	if (needResponse)
	{
		uintmax_t uploadSize;
		uintmax_t downloadSize = filesystem::file_size(filesystem::path(currentPath).append(fileName));

		clientStream >> uploadSize;

		if (uploadSize == downloadSize)
		{
			clientStream << responses::okResponse;
			clientStream << filesResponses::successUploadFile;
		}
		else
		{
			clientStream << responses::failResponse;
			clientStream << filesResponses::failUploadFile;
		}
	}
}

void downloadFile(streams::IOSocketStream<char>& clientStream, const filesystem::path& currentPath)
{
	string fileName;
	uintmax_t offset;
	uintmax_t size;
	string data;
	bool isLast;

	clientStream >> fileName;
	clientStream >> offset;

	filesystem::path filePath(filesystem::path(currentPath).append(fileName));
	size = filesystem::file_size(filePath);

	ifstream in(filePath, ios::binary);

	in.seekg(offset);

	if (size - offset >= filePacketSize)
	{
		data.resize(filePacketSize);
		isLast = false;
	}
	else
	{
		data.resize(size - offset);
		isLast = true;
	}

	in.read(data.data(), data.size());

	in.close();

	clientStream << data;
	clientStream << isLast;

	if (isLast)
	{
		clientStream << size;
	}
}

void removeFile(streams::IOSocketStream<char>& clientStream, const filesystem::path& currentPath)
{
	string fileName;

	clientStream >> fileName;

	if (filesystem::remove(filesystem::path(currentPath).append(fileName)))
	{
		clientStream << responses::okResponse;
	}
	else
	{
		clientStream << responses::failResponse;
	}
}