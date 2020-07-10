#pragma once

#include "Network.h"

namespace web
{
	class DataBaseNetwork : public Network<char>
	{
	private:
		void log(const char* message) override;

	public:
		DataBaseNetwork();

		~DataBaseNetwork() = default;
	};
}