#include "pch.h"

#include "FilesNetwork.h"
#include "Log.h"
#include "Constants.h"

using namespace std;

namespace web
{
	void FilesNetwork::log(const char* message)
	{
		Log::error(message);
	}

	FilesNetwork::FilesNetwork() : parent(cloudStorageServerIp, cloudStorageServerPort)
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