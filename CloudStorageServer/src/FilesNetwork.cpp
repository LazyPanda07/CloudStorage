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

	FilesNetwork::FilesNetwork(SOCKET clientSocket) : parent(clientSocket)
	{

	}
}