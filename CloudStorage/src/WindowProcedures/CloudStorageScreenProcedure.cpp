#include "pch.h"

#include "CloudStorageScreenProcedure.h"
#include "../UIConstants.h"
#include "Constants.h"

#include <CommCtrl.h>

using namespace std;

void getFilesFromExplorer(HWND mainWindow);

LRESULT __stdcall CloudStorageScreenProcedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	static NMITEMACTIVATE* listViewHdr;
	static HMENU popupMenu = nullptr;

	switch (msg)
	{
	case WM_COMMAND:
		switch (wparam)
		{
		case UI::buttons::back:
			SendMessageW(GetParent(hwnd), WM_COMMAND, UI::buttons::back, NULL);

			break;

		case UI::buttons::refresh:
			SendMessageW(GetParent(hwnd), WM_COMMAND, UI::buttons::refresh, NULL);

			break;

		case UI::buttons::download:
			SendMessageW(GetParent(hwnd), WM_COMMAND, UI::buttons::download, NULL);

			break;

		case UI::buttons::remove:
			SendMessageW(GetParent(hwnd), WM_COMMAND, UI::buttons::remove, NULL);

			break;
			
		case UI::buttons::reconnect:
			SendMessageW(GetParent(hwnd), WM_COMMAND, UI::buttons::reconnect, NULL);

			break;

		case UI::buttons::createFolder:
			SendMessageW(GetParent(hwnd), WM_COMMAND, UI::buttons::createFolder, NULL);

			break;

		case UI::buttons::uploadFiles:
			getFilesFromExplorer(GetParent(hwnd));

			break;
		}

		return 0;

	case UI::events::uploadFileE:
		SendMessageW(GetParent(hwnd), UI::events::uploadFileE, wparam, NULL);

		return 0;

	case WM_NOTIFY:
		switch (reinterpret_cast<LPNMHDR>(lparam)->code)
		{
		case NM_DBLCLK:
			listViewHdr = reinterpret_cast<NMITEMACTIVATE*>(lparam);

			if (listViewHdr->iItem != -1)
			{
				SendMessageW(GetParent(hwnd), UI::events::openFileFolderE, static_cast<WPARAM>(listViewHdr->iItem), NULL);
			}

			return 0;

		case NM_RCLICK:
			
			listViewHdr = reinterpret_cast<NMITEMACTIVATE*>(lparam);

			if (listViewHdr->iItem != -1)
			{
				if (popupMenu)
				{
					DestroyMenu(popupMenu);
				}

				POINT cursorPos;

				popupMenu = CreatePopupMenu();

				GetCursorPos(&cursorPos);

				InsertMenuW(popupMenu, -1, MF_BYPOSITION | MF_STRING, UI::buttons::download, L"Скачать");
				InsertMenuW(popupMenu, -1, MF_BYPOSITION | MF_STRING, UI::buttons::remove, L"Удалить");

				TrackPopupMenu(popupMenu, TPM_TOPALIGN | TPM_LEFTALIGN, cursorPos.x, cursorPos.y, NULL, hwnd, nullptr);
			}
			else
			{
				if (popupMenu)
				{
					DestroyMenu(popupMenu);
				}

				POINT cursorPos;

				popupMenu = CreatePopupMenu();

				GetCursorPos(&cursorPos);

				InsertMenuW(popupMenu, -1, MF_BYPOSITION | MF_STRING, UI::buttons::createFolder, L"Новая папка");
				InsertMenuW(popupMenu, -1, MF_BYPOSITION | MF_STRING, UI::buttons::uploadFiles, L"Загрузить файлы");

				TrackPopupMenu(popupMenu, TPM_TOPALIGN | TPM_LEFTALIGN, cursorPos.x, cursorPos.y, NULL, hwnd, nullptr);
			}

			return 0;
		}

		return 0;

	default:
		return DefWindowProcW(hwnd, msg, wparam, lparam);
	}
}

void getFilesFromExplorer(HWND mainWindow)
{
	wstring fileNames;
	vector<wstring> paths;

	fileNames.resize(FILENAME_MAX * maxFilesFromExplorer);

	OPENFILENAMEW file = {};

	file.lStructSize = sizeof(file);
	file.hwndOwner = mainWindow;
	file.lpstrFilter = L"Все файлы\0*.*\0\0";
	file.nFilterIndex = 1;
	file.lpstrFile = fileNames.data();
	file.nMaxFile = fileNames.size();
	file.Flags = OFN_ALLOWMULTISELECT | OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_EXPLORER;

	GetOpenFileNameW(&file);

	wstring tem;
	wstring pathToFolder;

	for (size_t i = 0; i < fileNames.size(); i++)
	{
		if (fileNames[i] == L'\0')
		{
			if (i == fileNames.size() - 1 || fileNames[i + 1] == L'\0')
			{
				break;
			}
			else if (pathToFolder.empty())
			{
				pathToFolder = move(tem) + L'\\';
			}
			else
			{
				paths.push_back(pathToFolder + move(tem));
			}
		}
		else
		{
			tem += fileNames[i];
		}
	}

	paths.push_back(move(pathToFolder) + move(tem));

	SendMessageW(mainWindow, UI::events::uploadFileE, reinterpret_cast<WPARAM>(&paths), NULL);
}