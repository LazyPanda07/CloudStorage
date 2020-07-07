#include "pch.h"

#include "DataBaseNetwork.h"
#include "Log.h"

using namespace std;

namespace web
{
	void DataBaseNetwork::log(const char* message)
	{
		Log::error(message);
	}

	DataBaseNetwork::DataBaseNetwork(SOCKET clientSocket) : parent(clientSocket)
	{

	}
}