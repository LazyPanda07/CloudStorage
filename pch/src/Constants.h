#pragma once

#ifndef _WINDEF_
typedef unsigned long DWORD;
#endif

#include <string>

inline constexpr std::string_view settingsFile = "settings.ini";

inline constexpr std::string_view usersFolder = "Data";

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
	inline const std::string controlType = "Control request";
}

namespace networkRequests
{
	inline constexpr std::string_view exit = "EOSS";
	inline constexpr std::string_view cancelOperation = "Cancel operation";
}

namespace controlRequests
{
	inline constexpr std::string_view nextFolder = "Next folder";
	inline constexpr std::string_view prevFolder = "Previous folder";
	inline constexpr std::string_view setPath = "Set path";
}

namespace accountRequests
{
	inline constexpr std::string_view authorization = "Authorization";
	inline constexpr std::string_view registration = "Registration";
	inline constexpr std::string_view setLogin = "Set login";
}

namespace filesRequests
{
	inline constexpr std::string_view uploadFile = "Upload file";
	inline constexpr std::string_view downloadFile = "Download file";
	inline constexpr std::string_view showAllFilesInFolder = "Show all files in directory";
	inline constexpr std::string_view removeFile = "Remove file";
	inline constexpr std::string_view createFolder = "Create folder";
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
	inline constexpr std::string_view emptyFolder = "Эта папка пуста";
	inline constexpr std::string_view successUploadFile = "Файл успешно загружен";
	inline constexpr std::string_view failUploadFile = "Ошибка при загрузке файла";
	inline constexpr std::wstring_view successDownloadFile = L"Файл успешно скачан";
	inline constexpr std::wstring_view failDownloadFile = L"Ошибка при скачивании файла";
}