#pragma once

#include "BaseTCPServer.h"

namespace web
{
	class CloudStorageServer : public BaseTCPServer
	{
	private:
		void clientConnection(SOCKET clientSocket, sockaddr addr) override;

	public:
		CloudStorageServer();

	};
}