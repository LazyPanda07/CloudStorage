#pragma once

#include <memory>
#include <filesystem>

#include "UIInterfaces/IResizable.h"
#include "BaseClasses/BaseScreen.h"
#include "BaseClasses/BasePopupWindow.h"

#include <Windows.h>

namespace UI
{
	class MainWindow : public IResizable
	{
	public:
		enum class elementsEnum
		{
			mainWindow,
			wrapper,
#pragma region CloudStorageScreen
			backArrowButton,
			refreshButton,
			downloadButton,
			removeButton,
			reconnectButton,
			list,
#pragma endregion
#pragma region AuthorizationScreen
			authorizationLoginEdit,
			authorizationPasswordEdit,
#pragma endregion
#pragma region RegistrationScreen
			registrationLoginEdit,
			registrationPasswordEdit,
			registrationRepeatPasswordEdit,
#pragma endregion
#pragma region BasePopupWindow
			popupWindow,
			cancelButton,
			disableWindow,
#pragma endregion
#pragma region UploadFilePopupWindow
			uploadProgressBar,
#pragma endregion
#pragma region DownloadFilePopupWindow
			downloadProgressBar,
#pragma endregion
#pragma region SetFolderNamePopupWindow
			enterFolderNameEdit,
			saveFolderNameButton,
#pragma endregion
#pragma region WaitResponsePopupWindow
			waitResponseProgressBar
#pragma endregion


		};

	private:
		std::unique_ptr<BaseScreen> currentScreen;
		std::unique_ptr<BasePopupWindow> currentPopupWindow;
		
		static inline std::filesystem::path downloadFolder;

		HWND mainWindow;

	private:
		MainWindow();

		HWND getHWND(elementsEnum id) const;

		~MainWindow();

	public:
		static MainWindow& get();

		static void setDownloadFolder(std::string&& path) noexcept;

		static std::filesystem::path getDownloadFolder() noexcept;

		BaseScreen* getCurrentScreen();

		BasePopupWindow* getCurrentPopupWindow();

		void setCurrentScreen(BaseScreen* screen);

		void setCurrentPopupWindow(BasePopupWindow* popupWindow);

		void deleteCurrentPopupWindow();

		void resize() override;

		HWND getMainWindow() const;

		HWND getBackArrowButton() const;

		HWND getRefreshButton() const;

		HWND getDownloadButton() const;

		HWND getRemoveButton() const;

		HWND getReconnectButton() const;

		HWND getList() const;

		HWND getAuthorizationLoginEdit() const;

		HWND getAuthorizationPasswordEdit() const;

		HWND getRegistrationLoginEdit() const;

		HWND getRegistrationPasswordEdit() const;

		HWND getRegistrationRepeatPasswordEdit() const;

		HWND getPopupWindow() const;

		HWND getCancelButton() const;

		HWND getDisableWindow() const;

		HWND getWrapper() const;

		HWND getUploadProgressBar() const;

		HWND getDownloadProgressBar() const;

		HWND getEnterFolderNameEdit() const;

		HWND getEnterFolderNameButton() const;

		HWND getWaitResponseProgressBar() const;
	};
}