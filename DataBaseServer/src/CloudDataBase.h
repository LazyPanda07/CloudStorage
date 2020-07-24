#pragma once

#include <vector>

#include "DataBaseOperations.h"
#include "fileData.h"

namespace db
{
	class CloudDataBase
	{
	private:
		DataBaseOperations db;

	private:
		int getId(const std::string& login) const;

		std::string getLogin(int id) const;

		unsigned int getPassword(const std::string& login) const;

		unsigned int getPassword(int id) const;

	private:
		static unsigned int customHash(const std::string& source);

	public:
		CloudDataBase();

		bool uploadFileData(const std::string& login, std::string&& fileName, std::string&& filePath, std::string&& fileExtension, unsigned int fileSize) const;

		bool removeFileData(const std::string& login, std::string&& fileName, std::string&& filePath) const;

		void addFolder(const std::string& login, std::string&& folderName, std::string&& filePath, std::string&& fileExtension) const;

		std::vector<fileData> getFiles(const std::string& login, const std::string& path) const;

		std::string registration(const std::string& login, const std::string& password) const;

		bool authorization(const std::string& login, const std::string& password) const;

		~CloudDataBase() = default;
	};
}