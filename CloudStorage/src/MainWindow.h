#pragma once

#include <Windows.h>

namespace UI
{
	class MainWindow
	{
	private:
		HWND mainWindow;

		HWND refreshButton;
		HWND list;

	private:
		MainWindow();

		~MainWindow();

	public:
		static MainWindow& get();

		HWND getHWND() const;
	};
}