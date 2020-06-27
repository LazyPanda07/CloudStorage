#pragma once

#include <Windows.h>

namespace UI
{
	class MainWindow
	{
	private:
		HWND mainWindow;

	private:
		MainWindow();

		~MainWindow();

	public:
		static MainWindow& get();
	};
}