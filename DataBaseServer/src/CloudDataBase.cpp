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

		auto callback = [](void* userData, int argc, char** argv, char** columns) -> int
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

		auto callback = [](void* userData, int argc, char** argv, char** columns) -> int
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

		auto callback = [](void* userData, int argc, char** argv, char** columns) -> int
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

		auto callback = [](void* userData, int argc, char** argv, char** columns) -> int
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

	CloudDataBase::CloudDataBase() :
		db(dataBaseName)
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
			"uploadDate", "TIMESTAMP DEFAULT CURRENT_TIMESTAMP NOT NULL",
			"dateOfChange", "TIMESTAMP DEFAULT CURRENT_TIMESTAMP NOT NULL",
			"FOREIGN KEY (userId)", "REFERENCES Users(id)"
		);
	}

	bool CloudDataBase::uploadFileData(const string& login, string&& fileName, string&& filePath, string&& fileExtension, unsigned int fileSize) const
	{
		try
		{
			db.insert
			(
				filesTable,
				"userId", this->getId(login),
				"fileName", move(fileName),
				"filePath", move(filePath),
				"fileExtension", move(fileExtension),
				"fileSize", fileSize
			);

			return true;
		}
		catch (const DataBaseException& e)
		{
			Log::warning(e.what());
			return false;
		}
	}

	bool CloudDataBase::removeFileData(const string& login, string&& fileName, string&& filePath) const
	{
		try
		{
			string condition = "userId = " + to_string(this->getId(login)) + " AND fileName = '" + move(fileName) + "' AND filePath = '" + move(filePath) + '\'';

			db.deleteFromTable(filesTable, condition);

			return true;
		}
		catch (const DataBaseException& e)
		{
			Log::warning(e.what());
			return false;
		}
	}

	void CloudDataBase::addFolder(const string& login, string&& folderName, string&& filePath, string&& fileExtension) const
	{
		try
		{
			db.insert
			(
				filesTable,
				"userId", this->getId(login),
				"fileName", move(folderName),
				"filePath", move(filePath),
				"fileExtension", move(fileExtension)
			);
		}
		catch (const DataBaseException& e)
		{
			Log::warning(e.what());
		}
	}

	vector<fileData> CloudDataBase::getFiles(const string& login) const
	{
		vector<fileData> result;
		const string condition = "userId = " + to_string(this->getId(login));

		auto callback = [](void* userData, int argc, char** argv, char** columns) -> int
		{
			vector<fileData>& ref = *static_cast<vector<fileData>*>(userData);
			char* end;

			ref.emplace_back
			(
				string(argv[0]),
				string(argv[1]),
				string(argv[2]),
				string(argv[3]),
				string(argv[4]),
				strtoul(argv[5], &end, 10)
			);

			return 0;
		};

		db.select
		(
			filesTable,
			condition,
			callback,
			&result,
			"fileName",	//argv[0]
			"filePath",	//argv[1]
			"fileExtension",	//argv[2]
			"uploadDate",	//argv[3]
			"dateOfChange",	//argv[4]
			"fileSize"	//argv[5]
		);

		return result;
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
		catch (const DataBaseException& e)
		{
			Log::warning(e.what());
			return e.what();
		}
	}

	bool CloudDataBase::authorization(const string& login, const string& password) const
	{
		int id = -1;
		const string condition = "login = '" + login + "' AND password = " + to_string(customHash(password));

		auto callback = [](void* userData, int argc, char** argv, char** columns) -> int
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