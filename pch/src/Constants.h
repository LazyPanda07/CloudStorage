#pragma once

#ifndef _WINDEF_
typedef unsigned long DWORD;
#endif

#include <string>

inline constexpr std::string_view settingsFile = "settings.ini";

inline constexpr std::string_view usersDirectory = "Data";

inline constexpr short HTTPPacketSize = 4096;
inline constexpr int filePacketSize = 10 * 1024 * 1024;	//10 MB

inline constexpr DWORD serverTimeoutRecv = 600000;	//10 minutes
inline constexpr DWORD clientTimeoutRecv = 30000;	//30 seconds

inline constexpr std::string_view customHTTPHeaderSize = "Total-HTTP-Message-Size: ";

inline constexpr std::string_view dataDelimiter = "/";
inline constexpr std::string_view dataPartDelimiter = "|";

namespace requestType
{
	inline const std::string accountType = "Account request";
	inline const std::string filesType = "Files request";
	inline const std::string exitType = "Exit request";
	inline const std::string cancelType = "Cancel request";
}

namespace networkRequests
{
	inline constexpr std::string_view exit = "EOSS";
	inline constexpr std::string_view cancelOperation = "Cancel operation";
}

namespace accountRequests
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
	inline constexpr std::string_view failAuthorization = "Неправильный логин или пароль";
	inline constexpr std::string_view successRegistration = "Регистрация прошла успешно";
	inline constexpr std::string_view failRegistration = "Такой пользователь уже существует";
}

namespace filesResponses
{
	inline constexpr std::string_view emptyDirectory = "Эта папка пуста";
	inline constexpr std::string_view successUploadFile = "Файл успешно загружен";
	inline constexpr std::string_view failUploadFile = "Ошибка при загрузке файла";
	inline constexpr std::wstring_view successDownloadFile = L"Файл успешно скачан";
	inline constexpr std::wstring_view failDownloadFile = L"Ошибка при скачивании файла";
}