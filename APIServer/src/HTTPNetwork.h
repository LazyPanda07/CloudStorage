#pragma once

#include "BaseNetwork.h"

namespace web
{
	class HTTPNetwork : public Network
	{
	private:
		void log(const std::string& message, std::any&& data = "") override;

		int sendData(const std::vector<char>& data) override;

		int receiveData(std::vector<char>& data) override;

	public:
		HTTPNetwork(SOCKET clientSocket);

		int sendData(const std::string_view& data) override;

		int receiveData(std::string& data) override;

		~HTTPNetwork() = default;
	};
}