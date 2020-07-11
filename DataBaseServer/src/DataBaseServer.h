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

	private:
		static inline std::string dataBaseServerPort;

	public:
		DataBaseServer();

		static void setDataBaseServerPort(std::string&& port) noexcept;

		~DataBaseServer() = default;
	};
}