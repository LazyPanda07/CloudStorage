#include "pch.h"

#include "DataBaseNetwork.h"
#include "Log.h"

using namespace std;

namespace web
{
	void DataBaseNetwork::log(const string& message, any&& data)
	{
		Log::error(string(message));
	}

	DataBaseNetwork::DataBaseNetwork(SOCKET clientSocket) :
		Network(clientSocket)
	{

	}
}