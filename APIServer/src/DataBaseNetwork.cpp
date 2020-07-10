#include "pch.h"

#include "DataBaseNetwork.h"
#include "Constants.h"
#include "Log.h"

using namespace std;

namespace web
{
	void DataBaseNetwork::log(const char* message)
	{
		Log::error(message);
	}

	DataBaseNetwork::DataBaseNetwork() : parent(dataBaseServerIp, dataBaseServerPort)
	{

	}
}