#pragma once

#include "BaseTCPServer.h"

namespace web
{
	class APIServer : public BaseTCPServer
	{
	private:
		void clientConnection(SOCKET clientSocket, sockaddr addr) override;

	public:
		APIServer();

	};
}