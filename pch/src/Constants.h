#pragma once

#include <string>

inline constexpr std::string_view APIServerPort = "8500";
inline constexpr std::string_view filesServerPort = "12500";
inline constexpr std::string_view filesSeverIp = "31.207.173.47";

inline constexpr uint_fast16_t HTTPPacketSize = 4096;

namespace accountRequest
{
	inline constexpr std::string_view authorization = "Authorization";
	inline constexpr std::string_view registration = "Registration";
}

namespace filesRequests
{
	inline constexpr std::string_view uploadFile = "Upload file";
	inline constexpr std::string_view downloadFile = "Download file";
	inline constexpr std::string_view showAllFilesInDirectory = "Show all files in directory";
}

namespace responses
{
	inline constexpr std::string_view okResponse = "OK";
	inline constexpr std::string_view unknownRequest = "Unknown request";
}