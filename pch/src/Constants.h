#pragma once

#include <string>

inline constexpr std::string_view settingsFile = "settings.ini";

inline constexpr std::string_view usersDirectory = "Data";

inline constexpr short HTTPPacketSize = 4096;
inline constexpr int filePacketSize = 1 * 1024 * 1024;	//1 MB

inline constexpr std::string_view customHTTPHeaderSize = "Total-HTTP-Message-Size: ";

inline constexpr std::string_view dataDelimiter = "/";
inline constexpr std::string_view dataPartDelimiter = "|";

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
	inline constexpr std::string_view removeFile = "Remove file";
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