#pragma once

#include "../BaseClasses/BaseScreen.h"

#include <Windows.h>

namespace UI
{
	class CloudStorageScreen : public BaseScreen
	{
	private:
		HWND backArrowButton;
		HWND refreshButton;
		HWND downloadButton;
		HWND removeButton;
		HWND list;
		HWND reconnectButton;

	private:
		void show() override;

		void destroy() override;

	public:
		CloudStorageScreen(HWND parentWindow);

		void resize() override;

		const std::string getName() const override;

		HWND getBackArrowButton() const;

		HWND getRefreshButton() const;

		HWND getDownloadButton() const;

		HWND getRemoveButton() const;

		HWND getReconnectButton() const;

		HWND getList() const;

		~CloudStorageScreen() = default;
	};
}