#pragma once

#include "BaseTCPServer.h"

namespace web
{
	class DataBaseServer : public BaseTCPServer
	{
	private:
		void clientConnection(SOCKET clientSocket, sockaddr addr) override;

	public:
		DataBaseServer();

		~DataBaseServer() = default;
	};
}