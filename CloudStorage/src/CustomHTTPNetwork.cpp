#include "pch.h"

#include "CustomHTTPNetwork.h"
#include "Log.h"
#include "HTTPParser.h"
#include "Constants.h"

using namespace std;

namespace web
{
	CustomHTTPNetwork::CustomHTTPNetwork() : 
		HTTPNetwork(APIServerIp, APIServerPort)
	{
		setsockopt(Network::clientSocket, SOL_SOCKET, SO_RCVTIMEO, reinterpret_cast<const char*>(&clientTimeoutRecv), sizeof(clientTimeoutRecv));
	}

	void CustomHTTPNetwork::setAPIServerIp(string&& ip) noexcept
	{
		CustomHTTPNetwork::APIServerIp = move(ip);
	}

	void CustomHTTPNetwork::setAPIServerPort(string&& port) noexcept
	{
		CustomHTTPNetwork::APIServerPort = move(port);
	}
}