#include "pch.h"

#include "DataBaseNetwork.h"
#include "Constants.h"
#include "Log.h"

using namespace std;

namespace web
{
	void DataBaseNetwork::log(const string& message, any&& data)
	{
		Log::error(string(message));
	}

	DataBaseNetwork::DataBaseNetwork() :
		Network(dataBaseServerIp, dataBaseServerPort)
	{

	}

	void DataBaseNetwork::setDataBaseServerIp(string&& ip) noexcept
	{
		dataBaseServerIp = move(ip);
	}

	void DataBaseNetwork::setDataBaseServerPort(string&& port) noexcept
	{
		dataBaseServerPort = move(port);
	}
}