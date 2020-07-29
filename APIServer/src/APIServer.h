#pragma once

#include "BaseTCPServer.h"
#include "Clients.h"

namespace web
{
	class APIServer : public BaseTCPServer
	{
	private:
		Clients clients;

	private:
		void clientConnection(SOCKET clientSocket, sockaddr addr) override;

	private:
		static inline std::string APIServerPort;

	public:
		APIServer();

		static void setAPIServerPort(std::string&& port) noexcept;

		std::vector<std::pair<std::thread::id, clientData>> getClients() noexcept;

		~APIServer() = default;
	};
}