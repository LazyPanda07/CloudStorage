#pragma once

#include <Windows.h>
#include "UIInterfaces/IResizable.h"

namespace UI
{
	class MainWindow : public IResizable
	{
	public:
		enum class elementsEnum
		{
			mainWindow,
			refreshButton,
			downloadButton,
			list
		};

	private:
		HWND mainWindow;

		HWND refreshButton;
		HWND downloadButton;
		HWND list;

	private:
		MainWindow();

		HWND getHWND(elementsEnum id) const;

		~MainWindow();

	public:
		static MainWindow& get();

		void resize() override;

		HWND getMainWindow() const;

		HWND getRefreshButton() const;

		HWND getDownloadButton() const;

		HWND getList() const;
	};
}