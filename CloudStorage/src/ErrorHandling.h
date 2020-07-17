#pragma once

#include "MainWindow.h"

namespace UI
{
	void customError(MainWindow& ref, const std::wstring& message, const std::wstring& title);
	
	void serverResponseError(MainWindow& ref);

	void serverRequestError(MainWindow& ref);
}