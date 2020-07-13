#pragma once

#include <Windows.h>

#include "../BaseClasses/BaseScreen.h"

namespace UI
{
	class CloudStorageScreen : public BaseScreen
	{
	private:
		HWND refreshButton;
		HWND downloadButton;
		HWND removeButton;
		HWND list;

	private:
		void show() override;

		void destroy() override;

	public:
		CloudStorageScreen(HWND parentWindow, const std::wstring& wrapperClassName, WNDPROC procedure);

		void resize() override;

		HWND getRefreshButton() const;

		HWND getDownloadButton() const;

		HWND getRemoveButton() const;

		HWND getList() const;

		~CloudStorageScreen() = default;
	};
}