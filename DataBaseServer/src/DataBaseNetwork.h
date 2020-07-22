#pragma once

#include "Network.h"

namespace web
{
	class DataBaseNetwork : public Network<char>
	{
	private:
		void log(std::string&& message) noexcept override;

	public:
		DataBaseNetwork(SOCKET clientSocket);

		~DataBaseNetwork() = default;
	};
}