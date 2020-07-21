#pragma once

#include "../BaseClasses/BaseScreen.h"

#include <Windows.h>

namespace UI
{
	class CloudStorageScreen : public BaseScreen
	{
	private:
		HWND refreshButton;
		HWND downloadButton;
		HWND removeButton;
		HWND list;

		//TODO: add reconnect button

	private:
		void show() override;

		void destroy() override;

	public:
		CloudStorageScreen(HWND parentWindow);

		void resize() override;

		const std::string getName() const override;

		HWND getRefreshButton() const;

		HWND getDownloadButton() const;

		HWND getRemoveButton() const;

		HWND getList() const;

		~CloudStorageScreen() = default;
	};
}