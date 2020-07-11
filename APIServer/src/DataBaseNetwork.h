#pragma once

#include "Network.h"

namespace web
{
	class DataBaseNetwork : public Network<char>
	{
	private:
		void log(const char* message) override;

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