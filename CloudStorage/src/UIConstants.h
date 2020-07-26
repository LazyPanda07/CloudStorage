#pragma once

#include <Windows.h>

namespace UI
{
	namespace mainWindowUI
	{
		inline constexpr int mainWindowWidth = 1280;
		inline constexpr int mainWindowHeight = 800;

		inline constexpr int columnsInList = 4;

		constexpr int nameColumnIndex = 0;
		constexpr int dateColumnIndex = 1;
		constexpr int typeColumnIndex = 2;
		constexpr int sizeColumnIndex = 3;

		constexpr double nameColumnCoefficientWidth = 0.35;
		constexpr double dateColumnCoefficientWidth = 0.1;
		constexpr double typeColumnCoefficientWidth = 0.1;
		constexpr double sizeColumnCoefficientWidth = 0.1;
	}

	namespace toolbar
	{
		inline constexpr int toolbarWidth = mainWindowUI::mainWindowWidth;
		inline constexpr int toolbarHeight = 32;
		inline constexpr int toolbarBackButtonWidth = 32;
		inline constexpr int toolbarBackButtonHeight = toolbarHeight;
		inline constexpr int toolbarButtonWidth = 200;
		inline constexpr int toolbarButtonHeight = toolbarHeight;
		inline constexpr int toolbarTopOffset = 10;
	}

	namespace screens
	{
		inline const std::wstring authorizationScreenName = L"Authorization";
		inline const std::wstring registrationScreenName = L"Registration";
		inline const std::wstring cloudStorageScreenName = L"CloudStorage";

		namespace authorizationScreen
		{
			inline constexpr int loginLimitCharacters = 16;
			inline constexpr int passwordLimitCharacters = 16;

			inline constexpr int defaultOffset = 10;

			inline constexpr int screenNameStaticWidth = 100;
			inline constexpr int screenNameStaticHeight = 20;

			inline constexpr int loginEditWidth = 300;
			inline constexpr int loginEditHeight = 20;

			inline constexpr int passwordEditWidth = 300;
			inline constexpr int passwordEditHeight = 20;

			inline constexpr int enterButtonWidth = 120;
			inline constexpr int enterButtonHeight = 20;

			inline constexpr int toRegistrationScreenButtonWidth = 180;
			inline constexpr int toRegistrationScreenButtonHeight = 20;
		}

		namespace registrationScreen
		{
			inline constexpr int loginLimitCharacters = 16;
			inline constexpr int passwordLimitCharacters = 16;

			inline constexpr int defaultOffset = 10;

			inline constexpr int screenNameStaticWidth = 100;
			inline constexpr int screenNameStaticHeight = 20;

			inline constexpr int loginEditWidth = 300;
			inline constexpr int loginEditHeight = 20;

			inline constexpr int passwordEditWidth = 300;
			inline constexpr int passwordEditHeight = 20;

			inline constexpr int repeatPasswordEditWidth = 300;
			inline constexpr int repeatPasswordEditHeight = 20;

			inline constexpr int registrationButtonWidth = 180;
			inline constexpr int registrationButtonHeight = 20;

			inline constexpr int toAuthorizationScreenButtonWidth = 120;
			inline constexpr int toAuthorizationScreenButtonHeight = 20;
		}
	}

	namespace popupWindows
	{
		inline const std::wstring uploadFilePopupWindowName = L"UploadFilePopupWindow";
		inline const std::wstring downloadFilePopupWindowName = L"DownloadFilePopupWindow";
		inline const std::wstring setFolderNamePopupWindowName = L"SetFolderNamePopupWindow";
		inline const std::wstring waitResponsePopupWindowName = L"WaitResponsePopupWindow";

		namespace basePopupWindow
		{
			inline constexpr int popupWindowWidth = 800;
			inline constexpr int popupWindowHeight = 600;

			inline constexpr int messageStaticHeight = 20;

			inline constexpr int cancelButtonWidth = 200;
			inline constexpr int cancelButtonHeight = 20;
		}

		namespace uploadFilePopupWindow
		{
			inline const std::wstring title = L"Загрузка файла";
		}

		namespace downloadFilePopupWindow
		{
			inline const std::wstring title = L"Скачивание файла";
		}

		namespace setFolderNamePopupWindow
		{
			inline const std::wstring title = L"Создание новой папки";

			inline constexpr int saveFolderNameButtonWidth = 200;
			inline constexpr int saveFolderNameButtonHeight = 20;
		}

		namespace waitResponsePopupWindow
		{
			inline const std::wstring title = L"Ожидание...";
			inline const std::wstring message = L"Ожидание ответа от сервера...";
		}
	}

	namespace buttons
	{
		enum toolbarButtons
		{
			refresh = 0xfff,
			back,
			download,
			remove,
			reconnect,
			toolbarButtonsCount
		};

		enum registrationScreenButtons
		{
			registration = toolbarButtonsCount + 1,
			toAuthorizationScreen,
			registrationScreenButtonsCount
		};

		enum authorizationScreenButtons
		{
			authorization = registrationScreenButtonsCount + 1,
			toRegistrationScreen,
			authorizationScreenButtonsCount
		};

		enum setFolderNamePopupWindowButtons
		{
			saveFolderName = authorizationScreenButtonsCount + 1,
			setFolderNamePopupWindowButtonsCount
		};

		enum popupMenuButtons
		{
			createFolder = setFolderNamePopupWindowButtonsCount + 1,
			uploadFiles,
			popupMenuButtonsCount
		};
	}

	namespace events
	{
		enum networkEvents
		{
			getFilesE = buttons::popupMenuButtonsCount + 1,
			uploadFileE,
			multipleUploadE,
			downloadFileE,
			multipleDownloadE,
			setLoginE,
			networkEventsCount
		};

		enum modifyEvents
		{
			updateProgressBarE = networkEventsCount + 1,
			deletePopupWindowE,
			modifyEventsCount
		};

		enum initEvents
		{
			initMainWindowPtrE = modifyEventsCount + 1,
			initPopupWindowE,
			initEventsCount
		};

		enum controlEvents
		{
			openFileFolderE = initEventsCount + 1,
			controlEventsCount
		};
	}
}