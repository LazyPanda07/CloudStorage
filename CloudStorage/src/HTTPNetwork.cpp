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

	HTTPNetwork::HTTPNetwork() : 
		Network(APIServerIp, APIServerPort)
	{
		setsockopt(Network::clientSocket, SOL_SOCKET, SO_RCVTIMEO, reinterpret_cast<const char*>(&clientTimeoutRecv), sizeof(clientTimeoutRecv));
	}

	void HTTPNetwork::setAPIServerIp(string&& ip) noexcept
	{
		HTTPNetwork::APIServerIp = move(ip);
	}

	void HTTPNetwork::setAPIServerPort(string&& port) noexcept
	{
		HTTPNetwork::APIServerPort = move(port);
	}
}