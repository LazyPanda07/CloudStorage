#include "pch.h"

#include "FilesNetwork.h"
#include "Log.h"
#include "Constants.h"

using namespace std;

namespace web
{
	void FilesNetwork::log(const string& message, any&& data)
	{
		Log::error(string(message));
	}

	FilesNetwork::FilesNetwork() :
		Network(cloudStorageServerIp, cloudStorageServerPort)
	{

	}

	void FilesNetwork::setCloudStorageServerIp(string&& ip) noexcept
	{
		cloudStorageServerIp = move(ip);
	}

	void FilesNetwork::setCloudStorageServerPort(string&& port) noexcept
	{
		cloudStorageServerPort = move(port);
	}
}