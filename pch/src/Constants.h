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

inline constexpr int allowableCharactersAlphabetSize = 26 + 26 + 10 + 1; //a-z, A-Z, 0-9, _

inline constexpr int maxFilesFromExplorer = 10;

namespace requestType
{
	inline const std::string accountType = "Account-Request";
	inline const std::string filesType = "Files-Request";
	inline const std::string exitType = "Exit-Request";
	inline const std::string cancelType = "Cancel-Request";
	inline const std::string controlType = "Control-Request";
}

namespace networkRequests
{
	inline constexpr std::string_view exit = "EOSS";
	inline constexpr std::string_view cancelOperation = "Cancel-Operation";
}

namespace controlRequests
{
	inline constexpr std::string_view nextFolder = "Next-Folder";
	inline constexpr std::string_view prevFolder = "Previous-Folder";
	inline constexpr std::string_view setPath = "Set-Path";
}

namespace accountRequests
{
	inline constexpr std::string_view authorization = "Authorization";
	inline constexpr std::string_view registration = "Registration";
	inline constexpr std::string_view setLogin = "Set-Login";
}

namespace filesRequests
{
	inline constexpr std::string_view uploadFile = "Upload-File";
	inline constexpr std::string_view downloadFile = "Download-File";
	inline constexpr std::string_view showAllFilesInFolder = "Show-All-Files-In-Folder";
	inline constexpr std::string_view removeFile = "Remove-File";
	inline constexpr std::string_view createFolder = "Create-Folder";
}

namespace responses
{
	inline const std::string noConnectionWithFilesServer = "�� ������� ���������� ���������� � �������� ��������";
	inline const std::string noConnectionWithDataBaseServer = "�� ������� ���������� ���������� � �������� ���� ������";

	inline constexpr std::string_view okResponse = "OK";
	inline constexpr std::string_view failResponse = "FAIL";
	inline constexpr std::string_view unknownRequest = "Unknown request";
}

namespace accountResponses
{
	inline constexpr std::string_view failAuthorization = "������������ ����� ��� ������";
	inline constexpr std::string_view successRegistration = "����������� ������ �������";
	inline constexpr std::string_view failRegistration = "����� ������������ ��� ����������";
	inline constexpr std::string_view incorrectCharacters = "��������� ������������ ������ ��������� ������� : a-z, A-Z, 0-9, _";
}

namespace filesResponses
{
	inline constexpr std::string_view emptyFolder = "��� ����� �����";
	inline constexpr std::string_view successUploadFile = "���� ������� ��������";
	inline constexpr std::string_view failUploadFile = "������ ��� �������� �����";
	inline constexpr std::wstring_view successDownloadFile = L"���� ������� ������";
	inline constexpr std::wstring_view failDownloadFile = L"������ ��� ���������� �����";
}