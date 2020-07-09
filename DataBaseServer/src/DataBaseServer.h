#pragma once

#include "BaseTCPServer.h"
#include "CloudDataBase.h"

namespace web
{
	class DataBaseServer : public BaseTCPServer
	{
	private:
		db::CloudDataBase db;

	private:
		void clientConnection(SOCKET clientSocket, sockaddr addr) override;

	public:
		DataBaseServer();

		~DataBaseServer() = default;
	};
}