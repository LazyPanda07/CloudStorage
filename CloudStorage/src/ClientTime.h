#pragma once

#include <ctime>
#include <string>

namespace utility
{
	class ClientTime
	{
	private:
		static inline time_t difference;

	private:
		ClientTime();

		~ClientTime() = default;

	public:
		static ClientTime& get();

		//dateTime format YYYY-MM-DD HH:MM:SS
		std::wstring convertToLocal(const std::string& dateTime);
	};
}