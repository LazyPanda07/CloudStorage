#pragma once

#include "BaseTCPServer.h"

namespace web
{
	class APIServer : public BaseTCPServer
	{
	private:
		void clientConnection(SOCKET clientSocket, sockaddr addr) override;

	private:
		static inline std::string APIServerPort;

	public:
		APIServer();

		static void setAPIServerPort(std::string&& port) noexcept;

		~APIServer() = default;
	};
}