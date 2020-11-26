#pragma once

#include "BaseNetwork.h"

namespace web
{
	class DataBaseNetwork : public Network
	{
	private:
		void log(const std::string& message, std::any&& data = "") override;

	private:
		static inline std::string dataBaseServerIp;
		static inline std::string dataBaseServerPort;

	public:
		DataBaseNetwork();

		static void setDataBaseServerIp(std::string&& ip) noexcept;

		static void setDataBaseServerPort(std::string&& port) noexcept;

		~DataBaseNetwork() = default;
	};
}