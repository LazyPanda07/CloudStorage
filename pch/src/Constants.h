#pragma once

#include <string>

inline constexpr std::string_view APIServerIp = "31.207.173.47";
inline constexpr std::string_view APIServerPort = "8500";

inline constexpr std::string_view cloudStorageServerIp = "31.207.173.47";
inline constexpr std::string_view cloudStorageServerPort = "12500";

inline constexpr std::string_view dataBaseServerIp = "31.207.173.47";
inline constexpr std::string_view dataBaseServerPort = "14500";

inline constexpr std::string_view usersDirectory = "Data";

inline constexpr short HTTPPacketSize = 4096;
inline constexpr int filePacketSize = 1 * 1024 * 1024;	//1 MB

inline constexpr std::string_view customHTTPHeaderSize = "Total-HTTP-Message-Size: ";

namespace requestType
{
	inline const std::string accountType = "Account request";
	inline const std::string filesType = "Files request";
}

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
	inline constexpr std::string_view failResponse = "FAIL";
	inline constexpr std::string_view unknownRequest = "Unknown request";
}

namespace accountResponses
{
	inline constexpr std::string_view failAuthorization = "Wrong login or password";
	inline constexpr std::string_view successRegistration = "Registration was successfully";
	inline constexpr std::string_view failRegistration = "This user already exists";
}

namespace filesResponses
{
	inline constexpr std::string_view emptyDirectory = "Directory is empty";
	inline constexpr std::string_view successUploadFile = "File successfully uploaded";
	inline constexpr std::string_view failUploadFile = "Fail while file upload";
	inline constexpr std::wstring_view successDownloadFile = L"File successfully downloaded";
	inline constexpr std::wstring_view failDownloadFile = L"Fail while file download";
}