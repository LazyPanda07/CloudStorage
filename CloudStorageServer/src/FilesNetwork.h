#pragma once

#include "BaseNetwork.h"

namespace web
{
	class FilesNetwork : public Network
	{
	private:
		void log(const std::string& message, std::any&& data = "") override;

	public:
		FilesNetwork(SOCKET clientSocket);

		~FilesNetwork() = default;
	};
}