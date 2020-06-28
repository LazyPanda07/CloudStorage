#pragma once

#include "Network.h"

namespace web
{
	class FilesNetwork : public Network<char>
	{
	private:
		void log(const char* message) override;

	public:
		FilesNetwork();

	};
}