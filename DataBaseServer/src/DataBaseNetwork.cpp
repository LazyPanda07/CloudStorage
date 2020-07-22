#include "pch.h"

#include "DataBaseNetwork.h"
#include "Log.h"

using namespace std;

namespace web
{
	void DataBaseNetwork::log(string&& message) noexcept
	{
		Log::error(move(message));
	}

	DataBaseNetwork::DataBaseNetwork(SOCKET clientSocket) :
		parent(clientSocket)
	{

	}
}