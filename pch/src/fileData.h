#pragma once

#include <string>

namespace db
{
	struct fileData
	{
		std::string fileName;
		std::string filePath;
		std::string fileExtension;
		std::string uploadDate;
		std::string dateOfChange;
		intmax_t fileSize;

		fileData(std::string&& fileName, std::string&& filePath, std::string&& fileExtension, std::string&& uploadDate, std::string&& dateOfChange, intmax_t fileSize) noexcept;

		operator std::string () const;
	};

	struct fileDataRepresentation
	{
		std::wstring fileName;
		std::wstring filePath;
		std::wstring fileExtension;
		std::wstring uploadDate;
		std::wstring dateOfChange;
		std::wstring fileSizeS;
		intmax_t fileSize;

		fileDataRepresentation(std::wstring&& fileName, std::wstring&& filePath, std::wstring&& fileExtension, std::wstring&& uploadDate, std::wstring&& dateOfChange, intmax_t fileSize) noexcept;
	};
}