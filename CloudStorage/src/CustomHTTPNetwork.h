#pragma once

#include "HTTPNetwork.h"

namespace web
{
	class CustomHTTPNetwork : public HTTPNetwork
	{
	private:
		static inline std::string APIServerIp;
		static inline std::string APIServerPort;

	public:
		CustomHTTPNetwork();

		static void setAPIServerIp(std::string&& ip) noexcept;
		
		static void setAPIServerPort(std::string&& port) noexcept;

		~CustomHTTPNetwork() = default;
	};
}