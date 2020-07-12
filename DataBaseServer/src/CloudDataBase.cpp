#include "pch.h"

#include "CloudDataBase.h"
#include "DataBaseConstants.h"
#include "Log.h"

using namespace std;

namespace db
{
	int CloudDataBase::getId(const string& login) const
	{
		int id = -1;
		const string condition = "login = '" + login + "'";

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

	string CloudDataBase::getLogin(int id) const
	{
		string login;
		const string condition = "id = " + to_string(id);

		auto callback = [](void* userData, int argc, char** argv, char** columns)
		{
			(*static_cast<string*>(userData)) = argv[0];

			return 0;
		};

		db.select
		(
			usersTable,
			condition,
			callback,
			&login,
			"login"
		);

		return login;
	}

	unsigned int CloudDataBase::getPassword(const string& login) const
	{
		unsigned int password;
		const string condition = "login = '" + login + "'";

		auto callback = [](void* userData, int argc, char** argv, char** columns)
		{
			char* end;
			(*static_cast<unsigned int*>(userData)) = strtoul(argv[0], &end, 10);

			return 0;
		};

		db.select
		(
			usersTable,
			condition,
			callback,
			&password,
			"password"
		);

		return password;
	}

	unsigned int CloudDataBase::getPassword(int id) const
	{
		unsigned int password;
		const string condition = "id = " + to_string(id);

		auto callback = [](void* userData, int argc, char** argv, char** columns)
		{
			char* end;
			(*static_cast<unsigned int*>(userData)) = strtoul(argv[0], &end, 10);

			return 0;
		};

		db.select
		(
			usersTable,
			condition,
			callback,
			&password,
			"password"
		);

		return password;
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

		db.createTable
		(
			filesTable,
			"id", "INTEGER PRIMARY KEY AUTOINCREMENT",
			"userId", "INTEGER NOT NULL",
			"fileName", "TEXT NOT NULL",
			"filePath", "TEXT DEFAULT 'Home' NOT NULL",
			"fileExtension", "TEXT DEFAULT '' NOT NULL",
			"fileSize", "INTEGER DEFAULT 0 NOT NULL",
			"uploadData", "TIMESTAMP DEFAULT CURRENT_TIMESTAMP NOT NULL",
			"dateOfChange", "TIMESTAMP DEFAULT CURRENT_TIMESTAMP NOT NULL",
			"FOREIGN KEY (userId)", "REFERENCES Users(id)"
		);
	}

	bool CloudDataBase::uploadFile(const string& login, std::string&& fileName, std::string&& filePath, std::string&& fileExtension) const
	{
		try
		{
			db.insert
			(
				filesTable,
				"userId", this->getId(login),
				"fileName", move(fileName),
				"filePath", move(filePath),
				"fileExtension", move(fileExtension)
			);

			return true;
		}
		catch (const db::DataBaseException& e)
		{
			Log::warning(e.what());
			return false;
		}
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
		int id = -1;
		const string condition = "login = '" + login + "' AND password = " + to_string(customHash(password));

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

		if (id != -1)
		{
			return id;
		}

		Log::warning("Fail authorization with login: {} and password: {}\n", login, password);

		return false;
	}
}