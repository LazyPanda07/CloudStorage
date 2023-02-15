#include "pch.h"

#include "ClientTime.h"
#include "UtilityFunctions.h"

using namespace std;

namespace utility
{
	ClientTime::ClientTime()
	{
		time_t currentTime = time(nullptr);
		tm local;
		tm gmt;

		localtime_s(&local, &currentTime);

		gmtime_s(&gmt, &currentTime);

		difference = static_cast<time_t>(difftime(mktime(&local), mktime(&gmt)));
	}

	ClientTime& ClientTime::get()
	{
		static ClientTime instance;

		return instance;
	}

	wstring ClientTime::convertToLocal(const string& dateTime)
	{
		array<int, 4> date = {};
		array<int, 4> time = {};
		tm calendar = {};
		time_t timeWithOffset;
		string result;
		string tem;

		result.resize(20);

		for (size_t i = 0, dateIndex = 0, timeIndex = 0; i < dateTime.size(); i++)
		{
			if (dateTime[i] == '-' || dateTime[i] == ' ')
			{
				date[dateIndex++] = stoul(tem);
				tem.clear();
			}
			else if (dateTime[i] == ':')
			{
				time[timeIndex++] = stoul(tem);
				tem.clear();
			}
			else
			{
				tem += dateTime[i];
			}
		}

		time[2] = stoul(tem);
		date[0] -= 1900;
		date[1]--;

		calendar.tm_year = date[0];
		calendar.tm_mon = date[1];
		calendar.tm_mday = date[2];

		calendar.tm_hour = time[0];
		calendar.tm_min = time[1];
		calendar.tm_sec = time[2];

		timeWithOffset = mktime(&calendar) + difference;

		localtime_s(&calendar, &timeWithOffset);

		strftime(result.data(), result.size(), "%d.%m.%Y %H:%M:%S", &calendar);

		result.pop_back();

		return utility::conversion::to_wstring(result);
	}
}