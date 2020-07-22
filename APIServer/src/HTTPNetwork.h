#pragma once

#include "Network.h"

namespace web
{
	class HTTPNetwork : public Network<char>
	{
	private:
		void log(std::string&& message) noexcept override;

		int sendData(const dataContainer& data) override;

		int receiveData(dataContainer& data) override;

	public:
		HTTPNetwork(SOCKET clientSocket);

		int sendData(const std::string_view& data) override;

		int receiveData(std::string& data) override;

		~HTTPNetwork() = default;
	};
}