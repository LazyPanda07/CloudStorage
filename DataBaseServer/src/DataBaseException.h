#pragma once

namespace db
{
	class DataBaseException
	{
	private:
		char* data;

	public:
		DataBaseException(char* data);

		DataBaseException(DataBaseException&& other) noexcept;

		const char* what() const noexcept;

		~DataBaseException();
	};
}