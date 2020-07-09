#include "pch.h"

#include <winsqlite/winsqlite3.h>

#include "DataBaseException.h"

#pragma comment (lib, "winsqlite3.lib")

namespace db
{
	DataBaseException::DataBaseException(char* data) : data(data)
	{

	}

	DataBaseException::DataBaseException(DataBaseException&& other) noexcept
	{
		data = other.data;
		other.data = nullptr;
	}

	const char* DataBaseException::what() const noexcept
	{
		return data;
	}

	DataBaseException::~DataBaseException()
	{
		sqlite3_free(data);
	}
}
