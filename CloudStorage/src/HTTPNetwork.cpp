#include "pch.h"

#include "HTTPNetwork.h"
#include "Log.h"

using namespace std;

namespace web
{
	void HTTPNetwork::log(const char* message)
	{
		Log::error(message);
	}

	int_fast32_t HTTPNetwork::sendData(const dataContainer& data)
	{
		try
		{
			return parent::sendBytes(data.data(), data.size());
		}
		catch (const WebException& e)
		{
			this->log(e.what());
			return -1;
		}
	}

	int_fast32_t HTTPNetwork::receiveData(dataContainer& data)
	{
		try
		{
			data.resize(HTTPPacketSize);

			return recv(parent::clientSocket, data.data(), data.size(), NULL);
		}
		catch (const WebException& e)
		{
			this->log(e.what());
			return -1;
		}
	}

	HTTPNetwork::HTTPNetwork() : parent(APIServerIp, APIServerPort)
	{

	}

	int_fast32_t HTTPNetwork::sendData(const string_view& data)
	{
		try
		{
			return parent::sendBytes(data.data(), data.size());
		}
		catch (const WebException& e)
		{
			this->log(e.what());
			return -1;
		}
	}

	int_fast32_t HTTPNetwork::receiveData(string& data)
	{
		try
		{
			data.resize(HTTPPacketSize);

			return recv(parent::clientSocket, data.data(), data.size(), NULL);
		}
		catch (const WebException& e)
		{
			this->log(e.what());
			return -1;
		}
	}

	HTTPNetwork::~HTTPNetwork()
	{
		closesocket(parent::clientSocket);
	}
}