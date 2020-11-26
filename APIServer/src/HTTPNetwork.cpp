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

	int HTTPNetwork::sendData(const vector<char>& data)
	{
		try
		{
			return Network::sendBytes(data.data(), data.size());
		}
		catch (const WebException& e)
		{
			this->log(e.what());
			return -1;
		}
	}

	int HTTPNetwork::receiveData(vector<char>& data)
	{
		try
		{
			data.resize(HTTPPacketSize);

			int size = 0;
			int lastPacket = 0;
			int totalReceive = 0;

			do
			{
				lastPacket = recv(Network::clientSocket, data.data() + totalReceive, data.size() - totalReceive, NULL);

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
					const unordered_map<string, string>& headers = parser.getHeaders();

					auto it = headers.find("Total-HTTP-Message-Size");

					if (it == end(headers))
					{
						return -1;
					}

					size = stoi(it->second);

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

	HTTPNetwork::HTTPNetwork(SOCKET clientSocket) :
		Network(clientSocket, Network::ReceiveMode::prohibitResize)
	{

	}

	int HTTPNetwork::sendData(const string_view& data)
	{
		try
		{
			return Network::sendBytes(data.data(), data.size());
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
				lastPacket = recv(Network::clientSocket, data.data() + totalReceive, data.size() - totalReceive, NULL);

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
					const unordered_map<string, string>& headers = parser.getHeaders();

					auto it = headers.find("Total-HTTP-Message-Size");

					if (it == end(headers))
					{
						return -1;
					}

					size = stoi(it->second);

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
}