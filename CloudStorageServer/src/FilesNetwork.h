#pragma once

#include "Network.h"

namespace web
{
	class FilesNetwork : public Network<char>
	{
	private:
		void log(std::string&& message, std::any&& data = "") noexcept override;

	public:
		FilesNetwork(SOCKET clientSocket);

		~FilesNetwork() = default;
	};
}