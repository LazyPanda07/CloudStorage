#pragma once

#include "BaseNetwork.h"

namespace web
{
	class HTTPNetwork : public Network
	{
	private:
		void log(const std::string& message, std::any&& data = "") override;

	public:
		HTTPNetwork(SOCKET clientSocket);

		~HTTPNetwork() = default;
	};
}