#pragma once

#include "DataBaseException.h"

#include <winsqlite/winsqlite3.h>

namespace db
{
	class DataBaseOperations
	{
	private:
		sqlite3* connection;
		sqlite3_mutex* lock;

	private:
		char* applyQuery(const std::string& query, sqlite3_callback callback = nullptr, void* userData = nullptr) const noexcept;

	private:
		template<typename StringT, typename... Args>
		static void buildSelectQuery(std::string& query, StringT&& column, Args&&... args);

		template<typename FirstStringT, typename SecondStringT, typename... Args>
		static void buildCreateTableQuery(std::string& query, FirstStringT&& column, SecondStringT&& description, Args&&... args);

		template<typename StringT, typename T, typename... Args>
		static void buildInsertQuery(std::string& query, std::string& columns, std::string& values, StringT&& column, T&& value, Args&&... args);

	public:
		DataBaseOperations(const std::string_view& dataBaseName);

		template<typename FirstStringT, typename SecondStringT, typename... Args>
		void createTable(const std::string_view& tableName, FirstStringT&& column, SecondStringT&& description, Args&&... args) const;

		template<typename StringT, typename... Args>
		void select(const std::string_view& tableName, const std::string& condition, sqlite3_callback callback, void* userData, StringT&& column, Args&&... otherColumns) const;

		template<typename StringT, typename T, typename... Args>
		void insert(const std::string_view& tableName, StringT&& column, T&& value, Args&&... args) const;

		void rawQuery(const std::string& query, sqlite3_callback callback = nullptr, void* userData = nullptr) const;

		void deleteFromTable(const std::string_view& tableName, const std::string& condition) const;

		~DataBaseOperations();
	};

	template<typename StringT, typename... Args>
	void DataBaseOperations::buildSelectQuery(std::string& query, StringT&& column, Args&&... args)
	{
		query.append("`").append(column).append("`, ");

		if constexpr (sizeof...(args))
		{
			buildSelectQuery(query, std::forward<Args>(args)...);
		}
		else
		{
			query.replace(std::end(query) - 2, std::end(query), " ");
		}
	}

	template<typename FirstStringT, typename SecondStringT, typename... Args>
	void DataBaseOperations::buildCreateTableQuery(std::string& query, FirstStringT&& column, SecondStringT&& description, Args&&... args)
	{
		query.append(column).append(" ").append(description).append(", ");

		if constexpr (sizeof...(args))
		{
			buildCreateTableQuery(query, std::forward<Args>(args)...);
		}
		else
		{
			query.pop_back();
			query.pop_back();
		}
	}

	template<typename StringT, typename T, typename... Args>
	void DataBaseOperations::buildInsertQuery(std::string& query, std::string& columns, std::string& values, StringT&& column, T&& value, Args&&... args)
	{
		columns.append("`").append(column).append("`, ");

		if constexpr (std::is_constructible_v<std::string, T>)
		{
			values.append("\'").append(value).append("\', ");
		}
		else if constexpr (std::is_fundamental_v<std::remove_reference_t<T>>)
		{
			values.append(std::to_string(value)).append(", ");
		}

		if constexpr (sizeof...(args))
		{
			buildInsertQuery(query, columns, values, std::forward<Args>(args)...);
		}
		else
		{
			columns.replace(std::end(columns) - 2, std::end(columns), ")");

			values.replace(std::end(values) - 2, std::end(values), ")");
		}
	}

	template<typename FirstStringT, typename SecondStringT, typename... Args>
	void DataBaseOperations::createTable(const std::string_view& tableName, FirstStringT&& column, SecondStringT&& description, Args&&... args) const
	{
		std::string query;
		query.append("CREATE TABLE IF NOT EXISTS ").append(tableName).append(" (");

		buildCreateTableQuery(query, std::forward<FirstStringT>(column), std::forward<SecondStringT>(description), std::forward<Args>(args)...);

		query += ")";

		char* message = this->applyQuery(query);

		if (message)
		{
			throw DataBaseException(message);
		}
	}

	template<typename StringT, typename... Args>
	void DataBaseOperations::select(const std::string_view& tableName, const std::string& condition, sqlite3_callback callback, void* userData, StringT&& column, Args&&... otherColumns) const
	{
		std::string query = "SELECT ";

		buildSelectQuery(query, std::forward<StringT>(column), std::forward<Args>(otherColumns)...);

		query.append("FROM ").append(tableName).append(" WHERE ").append(condition);

		char* message = this->applyQuery(query, callback, userData);

		if (message)
		{
			throw DataBaseException(message);
		}
	}

	template<typename StringT, typename T, typename... Args>
	void DataBaseOperations::insert(const std::string_view& tableName, StringT&& column, T&& value, Args&&... args) const
	{
		std::string query = "INSERT INTO ";
		std::string columns = "(";
		std::string values = "VALUES (";

		buildInsertQuery(query, columns, values, std::forward<StringT>(column), std::forward<T>(value), std::forward<Args>(args)...);

		query.append(tableName).append(columns).append(" ").append(values);

		char* message = this->applyQuery(query);

		if (message)
		{
			throw DataBaseException(message);
		}
	}
}