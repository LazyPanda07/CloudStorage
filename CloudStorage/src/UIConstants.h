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
		inline constexpr int toolbarHeight = 20;
		inline constexpr int toolbarButtonWidth = 200;
		inline constexpr int toolbarButtonHeight = toolbarHeight;
		inline constexpr int toolbarTopOffset = 10;
	}

	namespace buttons
	{
		enum toolbarButtons
		{
			refresh = 0xfff,
			toolbarButtonsCount
		};

		enum registrationScreenButtons
		{
			registration = toolbarButtonsCount + 1,
			registrationScreenButtonsCount
		};

		enum authorizationScreenButtons
		{
			authorization = registrationScreenButtonsCount + 1,
			authorizationScreenButtonsCount
		};
	}

	namespace events
	{
		enum networkEvents
		{
			getFilesE = buttons::authorizationScreenButtonsCount + 1,
			uploadFileE,
			downLoadFilesE,
			networkEventsCount
		};

		enum initEvents
		{
			initMainWindowPtrE = networkEventsCount + 1,
			initEventsCount
		};
	}
}