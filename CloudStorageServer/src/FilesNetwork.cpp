#include "pch.h"

#include "FilesNetwork.h"
#include "Log.h"
#include "Constants.h"

using namespace std;

namespace web
{
	void FilesNetwork::log(string&& message) noexcept
	{
		Log::error(move(message));
	}

	FilesNetwork::FilesNetwork(SOCKET clientSocket) :
		parent(clientSocket)
	{

	}
}