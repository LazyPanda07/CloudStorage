#pragma once

#include "MainWindow.h"

namespace UI
{
	int customError(MainWindow& ref, const std::wstring& message, const std::wstring& title);
	
	int serverResponseError(MainWindow& ref);

	int serverRequestError(MainWindow& ref);
}