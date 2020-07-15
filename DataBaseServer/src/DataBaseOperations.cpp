#include "pch.h"

#include "DataBaseOperations.h"
#include "DataBaseException.h"

#pragma comment (lib, "winsqlite3.lib")

using namespace std;

namespace db
{
	char* DataBaseOperations::applyQuery(const string& query, sqlite3_callback callback, void* userData) const noexcept
	{
		char* errorMessage = nullptr;

		sqlite3_mutex_enter(lock);

		sqlite3_exec(connection, query.data(), callback, userData, &errorMessage);

		sqlite3_mutex_leave(lock);

		return errorMessage;
	}

	DataBaseOperations::DataBaseOperations(const string_view& dataBaseName) :
		lock(sqlite3_mutex_alloc(SQLITE_MUTEX_FAST))
	{
		sqlite3_open(dataBaseName.data(), &connection);
	}

	void DataBaseOperations::rawQuery(const string& query, sqlite3_callback callback, void* userData) const
	{
		char* message = this->applyQuery(query, callback, userData);

		if (message)
		{
			throw DataBaseException(message);
		}
	}

	void DataBaseOperations::deleteFromTable(const string_view& tableName, const string& condition) const
	{
		string query(string("DELETE FROM ").append(tableName).append(" WHERE ").append(condition));

		char* message = this->applyQuery(query);

		if (message)
		{
			throw DataBaseException(message);
		}
	}

	DataBaseOperations::~DataBaseOperations()
	{
		sqlite3_close(connection);
	}
}