#include "pch.h"

#include "HTTPNetwork.h"
#include "Log.h"
#include "HTTPParser.h"

using namespace std;

namespace web
{
	void HTTPNetwork::log(const char* message)
	{
		Log::error(message);
	}

	int HTTPNetwork::sendData(const dataContainer& data)
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

	int HTTPNetwork::receiveData(dataContainer& data)
	{
		try
		{
			data.resize(HTTPPacketSize);

			int size = 0;
			int lastPacket = 0;
			int totalReceive = 0;

			do
			{
				lastPacket = recv(parent::clientSocket, data.data() + totalReceive, data.size(), NULL);

				if (lastPacket < 0)
				{
					throw WebException();
				}
				else if (lastPacket == 0)
				{
					return totalReceive;
				}

				totalReceive += lastPacket;

				if (totalReceive > 25 && !size)
				{
					HTTPParser parser(data);
					const map<string, string>& headers = parser.getHeaders();

					size = atoi(headers.at("Total-HTTP-Message-Size").data());

					if (data.size() < size)
					{
						data.resize(size);
					}
				}
				else if (!size)
				{
					continue;
				}

			} while (totalReceive < size);

			return totalReceive;
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

	int HTTPNetwork::sendData(const string_view& data)
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

	int HTTPNetwork::receiveData(string& data)
	{
		try
		{
			data.resize(HTTPPacketSize);

			int size = 0;
			int lastPacket = 0;
			int totalReceive = 0;

			do
			{
				lastPacket = recv(parent::clientSocket, data.data() + totalReceive, data.size(), NULL);

				if (lastPacket < 0)
				{
					throw WebException();
				}
				else if (lastPacket == 0)
				{
					return totalReceive;
				}

				totalReceive += lastPacket;

				if (totalReceive > 25 && !size)
				{
					HTTPParser parser(data);
					const map<string, string>& headers = parser.getHeaders();

					size = atoi(headers.at("Total-HTTP-Message-Size").data());

					if (data.size() < size)
					{
						data.resize(size);
					}
				}
				else if (!size)
				{
					continue;
				}

			} while (totalReceive < size);

			return totalReceive;
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