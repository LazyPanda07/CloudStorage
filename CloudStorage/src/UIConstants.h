#pragma once

#include <Windows.h>

namespace UI
{
	namespace mainWindowUI
	{
		inline constexpr int_fast32_t mainWindowWidth = 1280;
		inline constexpr int_fast32_t mainWindowHeight = 800;
	}

	namespace toolbar
	{
		inline constexpr int_fast32_t toolbarWidth = mainWindowUI::mainWindowWidth;
		inline constexpr int_fast32_t toolbarHeight = 20;
		inline constexpr int_fast32_t toolbarButtonWidth = 150;
		inline constexpr int_fast32_t toolbarButtonHeight = toolbarHeight;
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
			getFiles = buttons::authorizationScreenButtonsCount + 1,
			uploadFile,
			uploadFiles,
			downloadFile,
			downLoadFiles,
			networkEventsCount
		};
	}
}