#pragma once

#include "DataBaseOperations.h"

namespace db
{
	class CloudDataBase
	{
	private:
		DataBaseOperations db;

	private:
		int getId(const std::string& login, unsigned int password) const;

		static unsigned int customHash(const std::string& source);

	public:
		CloudDataBase();

		std::string registration(const std::string& login, const std::string& password) const;

		bool authorization(const std::string& login, const std::string& password) const;

		~CloudDataBase() = default;
	};
}