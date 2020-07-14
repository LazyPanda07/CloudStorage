#pragma once

#include "Network.h"

namespace web
{
	class HTTPNetwork : public Network<char>
	{
	private:
		void log(const char* message) override;

		int sendData(const dataContainer& data) override;

		int receiveData(dataContainer& data) override;

	private:
		static inline std::string APIServerIp;
		static inline std::string APIServerPort;

	public:
		HTTPNetwork();

		int sendData(const std::string_view& data) override;

		int receiveData(std::string& data) override;

		static void setAPIServerIp(std::string&& ip) noexcept;
		
		static void setAPIServerPort(std::string&& port) noexcept;

		~HTTPNetwork() = default;
	};
}