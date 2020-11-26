#pragma once

#include "BaseNetwork.h"

namespace web
{
	class DataBaseNetwork : public Network
	{
	private:
		void log(const std::string& message, std::any&& data = "") override;

	public:
		DataBaseNetwork(SOCKET clientSocket);

		~DataBaseNetwork() = default;
	};
}