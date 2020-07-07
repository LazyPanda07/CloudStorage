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

		while (true)
		{
			try
			{
				
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