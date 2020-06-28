#pragma once

#include "Network.h"
#include "Constants.h"

namespace web
{
	class HTTPNetwork : public Network<char>
	{
	private:
		void log(const char* message) override;

		int_fast32_t sendData(const dataContainer& data) override;

		int_fast32_t receiveData(dataContainer& data) override;

	public:
		HTTPNetwork(SOCKET clientSocket);

		int_fast32_t sendData(const std::string_view& data) override;

		int_fast32_t receiveData(std::string& data) override;

		~HTTPNetwork();
	};
}