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
		unsigned int fileSize;

		fileData(std::string&& fileName, std::string&& filePath, std::string&& fileExtension, std::string&& uploadDate, std::string&& dateOfChange, unsigned int fileSize) noexcept;

		operator std::string () const;
	};
}