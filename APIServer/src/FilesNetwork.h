#pragma once

#include "BaseNetwork.h"

namespace web
{
	class FilesNetwork : public Network
	{
	private:
		void log(const std::string& message, std::any&& data = "") override;

	private:
		static inline std::string cloudStorageServerIp;
		static inline std::string cloudStorageServerPort;

	public:
		FilesNetwork();

		static void setCloudStorageServerIp(std::string&& ip) noexcept;

		static void setCloudStorageServerPort(std::string&& port) noexcept;

		~FilesNetwork() = default;
	};
}