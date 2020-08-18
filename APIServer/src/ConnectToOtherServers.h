#pragma once

#include <memory>

#include "IOSocketStream.h"

namespace web
{
	void connectToFilesServer(std::unique_ptr<streams::IOSocketStream<char>>& filesStream);

	void connectToDataBaseServer(std::unique_ptr<streams::IOSocketStream<char>>& dataBaseStream);
}