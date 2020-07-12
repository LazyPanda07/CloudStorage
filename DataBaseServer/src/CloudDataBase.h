#pragma once

#include "DataBaseOperations.h"

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

		static unsigned int customHash(const std::string& source);

	public:
		CloudDataBase();

		bool uploadFile(const std::string& login, std::string&& fileName, std::string&& filePath, std::string&& fileExtension) const;

		std::string registration(const std::string& login, const std::string& password) const;

		bool authorization(const std::string& login, const std::string& password) const;

		~CloudDataBase() = default;
	};
}