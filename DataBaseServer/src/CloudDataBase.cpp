#include "pch.h"

#include "CloudDataBase.h"
#include "DataBaseConstants.h"
#include "Log.h"

using namespace std;

namespace db
{
	int CloudDataBase::getId(const string& login, unsigned int password) const
	{
		int id = -1;
		const string condition = "login = '" + login + "' AND password = " + to_string(password);

		auto callback = [](void* userData, int argc, char** argv, char** columns)
		{
			(*static_cast<int*>(userData)) = atoi(argv[0]);

			return 0;
		};

		db.select
		(
			usersTable,
			condition,
			callback,
			&id,
			"id"
		);

		return id;
	}

	unsigned int CloudDataBase::customHash(const string& source)
	{
		mt19937 someHash;
		unsigned int result = 0;

		someHash.seed(hash<string>()(source));

		for (size_t i = 0; i < source.size(); i++)
		{
			result = someHash();
		}

		return result;
	}

	CloudDataBase::CloudDataBase() : db(dataBaseName)
	{
		db.createTable
		(
			usersTable,
			"id", "INTEGER PRIMARY KEY AUTOINCREMENT",
			"login", "VARCHAR(32) UNIQUE NOT NULL",
			"password", "INTEGER NOT NULL"
		);
	}

	string CloudDataBase::registration(const string& login, const string& password) const
	{
		try
		{
			db.insert
			(
				usersTable,
				"login", login,
				"password", customHash(password)
			);

			return string();
		}
		catch (const db::DataBaseException& e)
		{
			Log::warning(e.what());
			return e.what();
		}
	}

	bool CloudDataBase::authorization(const string& login, const string& password) const
	{
		return this->getId(login, customHash(password)) != -1;
	}
}