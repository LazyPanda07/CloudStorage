#pragma once

#include <memory>

#include "BaseIOSocketStream.h"

namespace web
{
	void connectToFilesServer(std::unique_ptr<streams::IOSocketStream>& filesStream);

	void connectToDataBaseServer(std::unique_ptr<streams::IOSocketStream>& dataBaseStream);
}