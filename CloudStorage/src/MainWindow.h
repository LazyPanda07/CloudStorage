#pragma once

#include <Windows.h>

#include "UIInterfaces/IResizable.h"
#include "BaseClasses/BaseScreen.h"

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
			list,
			loginEdit,
			passwordEdit
		};

	private:
		HWND mainWindow;

		BaseScreen* currentScreen;

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

		HWND getLoginEdit() const;

		HWND getPasswordEdit() const;
	};
}