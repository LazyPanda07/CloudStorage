#pragma once

#include "BaseTCPServer.h"

namespace web
{
	class CloudStorageServer : public BaseTCPServer
	{
	private:
		void clientConnection(SOCKET clientSocket, sockaddr addr) override;

	private:
		static inline std::string cloudStorageServerPort;

	public:
		CloudStorageServer();

		static void setCloudStorageServerPort(std::string&& port) noexcept;

		~CloudStorageServer() = default;
	};
}