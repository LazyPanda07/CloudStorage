#pragma once

#include "BaseNetwork.h"

namespace web
{
	class HTTPNetwork : public Network
	{
	private:
		void log(const std::string& message, std::any&& data = "") override;

		int sendData(const std::vector<char>& data) override;

		int receiveData(std::vector<char>& data) override;

	private:
		static inline std::string APIServerIp;
		static inline std::string APIServerPort;

	public:
		HTTPNetwork();

		int sendData(std::string_view data) override;

		int receiveData(std::string& data) override;

		static void setAPIServerIp(std::string&& ip) noexcept;
		
		static void setAPIServerPort(std::string&& port) noexcept;

		~HTTPNetwork() = default;
	};
}