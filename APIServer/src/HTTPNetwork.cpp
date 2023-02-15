#include "pch.h"

#include "HTTPNetwork.h"
#include "Log.h"
#include "HTTPParser.h"
#include "Constants.h"

using namespace std;

namespace web
{
	void HTTPNetwork::log(const string& message, any&& data)
	{
		Log::error(string(message));
	}

	HTTPNetwork::HTTPNetwork(SOCKET clientSocket) :
		Network(clientSocket, Network::receiveMode::prohibitResize)
	{

	}
}