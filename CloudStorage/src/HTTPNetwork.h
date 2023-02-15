#pragma once

#include "BaseNetwork.h"

namespace web
{
	class HTTPNetwork : public Network
	{
	private:
		void log(const std::string& message, std::any&& data = "") override;

	private:
		static inline std::string APIServerIp;
		static inline std::string APIServerPort;

	public:
		HTTPNetwork();

		static void setAPIServerIp(std::string&& ip) noexcept;
		
		static void setAPIServerPort(std::string&& port) noexcept;

		~HTTPNetwork() = default;
	};
}