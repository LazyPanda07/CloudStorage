#pragma once

#include "Network.h"

namespace web
{
	class FilesNetwork : public Network<char>
	{
	private:
		void log(const char* message) override;

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