#include "pch.h"

#include "CloudStorageScreen.h"
#include "../UIConstants.h"
#include "../WindowProcedures/CloudStorageScreenProcedure.h"
#include "resource.h"

#include <Richedit.h>
#include <CommCtrl.h>

using namespace std;

LRESULT __stdcall DragAndDrop(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData);

void createColumns(UI::CloudStorageScreen& ref);

namespace UI
{
	void CloudStorageScreen::show()
	{
		ShowWindow(backArrowButton, SW_SHOW);
		ShowWindow(refreshButton, SW_SHOW);
		ShowWindow(downloadButton, SW_SHOW);
		ShowWindow(removeButton, SW_SHOW);
		ShowWindow(reconnectButton, SW_SHOW);
		ShowWindow(list, SW_SHOW);
	}

	void CloudStorageScreen::destroy()
	{

	}

	CloudStorageScreen::CloudStorageScreen(HWND parentWindow) :
		BaseScreen(parentWindow, screens::cloudStorageScreenName, CloudStorageScreenProcedure)
	{
		HMODULE resourceDll = nullptr;
		HICON icon = nullptr;
		RECT wrapperSizes;

		GetClientRect(wrapper, &wrapperSizes);

#ifndef PRODUCTION
		resourceDll = LoadLibraryW(L"..\\Resources\\Resources.dll");
#else
		resourceDll = LoadLibraryW(L"Resources.dll");
#endif // PRODUCTION

		icon = LoadIconW(resourceDll, MAKEINTRESOURCEW(BACK_ARROW_ICON));

		const LONG width = wrapperSizes.right - wrapperSizes.left;
		const LONG height = wrapperSizes.bottom - wrapperSizes.top;

		backArrowButton = CreateWindowExW
		(
			NULL,
			L"BUTTON",
			nullptr,
			WS_CHILDWINDOW | BS_ICON,
			0,
			0,
			toolbar::toolbarBackButtonWidth,
			toolbar::toolbarBackButtonHeight,
			BaseScreen::wrapper,
			HMENU(buttons::back),
			nullptr,
			nullptr
		);

		refreshButton = CreateWindowExW
		(
			NULL,
			L"BUTTON",
			L"Обновить список файлов",
			WS_CHILDWINDOW,
			toolbar::toolbarBackButtonWidth,
			0,
			toolbar::toolbarButtonWidth,
			toolbar::toolbarButtonHeight,
			BaseScreen::wrapper,
			HMENU(buttons::refresh),
			nullptr,
			nullptr
		);

		downloadButton = CreateWindowExW
		(
			NULL,
			L"BUTTON",
			L"Скачать файл",
			WS_CHILDWINDOW,
			toolbar::toolbarButtonWidth + toolbar::toolbarBackButtonWidth,
			0,
			toolbar::toolbarButtonWidth,
			toolbar::toolbarButtonHeight,
			BaseScreen::wrapper,
			HMENU(buttons::download),
			nullptr,
			nullptr
		);

		removeButton = CreateWindowExW
		(
			NULL,
			L"BUTTON",
			L"Удалить файл",
			WS_CHILDWINDOW,
			toolbar::toolbarButtonWidth * 2 + toolbar::toolbarBackButtonWidth,
			0,
			toolbar::toolbarButtonWidth,
			toolbar::toolbarButtonHeight,
			BaseScreen::wrapper,
			HMENU(buttons::remove),
			nullptr,
			nullptr
		);

		reconnectButton = CreateWindowExW
		(
			NULL,
			L"BUTTON",
			L"Переподключиться",
			WS_CHILDWINDOW,
			toolbar::toolbarButtonWidth * 3 + toolbar::toolbarBackButtonWidth,
			0,
			toolbar::toolbarButtonWidth,
			toolbar::toolbarButtonHeight,
			BaseScreen::wrapper,
			HMENU(buttons::reconnect),
			nullptr,
			nullptr
		);

		list = CreateWindowExW
		(
			WS_EX_CLIENTEDGE,
			WC_LISTVIEW,
			nullptr,
			WS_CHILDWINDOW | LVS_REPORT,
			0,
			toolbar::toolbarHeight,
			width,
			height - toolbar::toolbarHeight,
			BaseScreen::wrapper,
			HMENU(),
			nullptr,
			nullptr
		);

		DragAcceptFiles(list, true);
		SetWindowSubclass(list, DragAndDrop, 1, NULL);

		createColumns(*this);

		SendMessageW(list, LVM_SETEXTENDEDLISTVIEWSTYLE, LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);

		if (icon)
		{
			SendMessageW(backArrowButton, BM_SETIMAGE, IMAGE_ICON, reinterpret_cast<LPARAM>(icon));
		}

		if (resourceDll)
		{
			FreeLibrary(resourceDll);
		}
	}

	void CloudStorageScreen::resize()
	{
		RECT sizes;

		GetClientRect(BaseScreen::wrapper, &sizes);

		LONG width = sizes.right - sizes.left;
		LONG height = sizes.bottom - sizes.top;

		SetWindowPos(list, HWND_BOTTOM, 0, toolbar::toolbarHeight, width, height - toolbar::toolbarHeight, SWP_SHOWWINDOW);
	}

	const string CloudStorageScreen::getName() const
	{
		return "CloudStorageScreen";
	}

	HWND CloudStorageScreen::getBackArrowButton() const
	{
		return backArrowButton;
	}

	HWND CloudStorageScreen::getRefreshButton() const
	{
		return refreshButton;
	}

	HWND CloudStorageScreen::getDownloadButton() const
	{
		return downloadButton;
	}

	HWND CloudStorageScreen::getRemoveButton() const
	{
		return removeButton;
	}

	HWND CloudStorageScreen::getReconnectButton() const
	{
		return reconnectButton;
	}

	HWND CloudStorageScreen::getList() const
	{
		return list;
	}
}

LRESULT __stdcall DragAndDrop(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
{
	switch (msg)
	{
	case WM_DROPFILES:
	{
		vector<wstring> data;
		HDROP drop = reinterpret_cast<HDROP>(wparam);

		UINT count = DragQueryFileW(drop, 0xFFFFFFFF, nullptr, NULL);

		data.resize(count);

		for (UINT i = 0; i < count; i++)
		{
			data[i].resize(DragQueryFileW(drop, i, nullptr, static_cast<UINT>(data[i].size())) + 1);

			DragQueryFileW(drop, i, data[i].data(), static_cast<UINT>(data[i].size()));

			data[i].pop_back();
		}

		SendMessageW(GetParent(hwnd), UI::events::uploadFileE, reinterpret_cast<WPARAM>(&data), NULL);
	}

	return 0;

	default:
		return DefSubclassProc(hwnd, msg, wparam, lparam);
	}
}

void createColumns(UI::CloudStorageScreen& ref)
{
	array<LVCOLUMNW, UI::mainWindowUI::columnsInList> columns = {};
	RECT wrapperSizes;
	RECT listSizes;

	GetClientRect(ref.getWrapper(), &wrapperSizes);
	GetClientRect(ref.getList(), &listSizes);

	LONG width = wrapperSizes.right - wrapperSizes.left;

	const array<LONG, UI::mainWindowUI::columnsInList> columnsSizes =
	{
		static_cast<LONG>(width * UI::mainWindowUI::nameColumnCoefficientWidth),
		static_cast<LONG>(width * UI::mainWindowUI::dateColumnCoefficientWidth),
		static_cast<LONG>(width * UI::mainWindowUI::typeColumnCoefficientWidth),
		static_cast<LONG>(width * UI::mainWindowUI::sizeColumnCoefficientWidth)
	};

	columns[0].pszText = const_cast<wchar_t*>(L"Имя");
	columns[1].pszText = const_cast<wchar_t*>(L"Дата изменения");
	columns[2].pszText = const_cast<wchar_t*>(L"Тип");
	columns[3].pszText = const_cast<wchar_t*>(L"Размер");

	for (size_t i = 0; i < columns.size(); i++)
	{
		columns[i].mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
		columns[i].iSubItem = static_cast<int>(i);
		columns[i].fmt = LVCFMT_LEFT;
		columns[i].cx = columnsSizes[i];

		SendMessageW(ref.getList(), LVM_INSERTCOLUMN, static_cast<WPARAM>(i), reinterpret_cast<LPARAM>(&columns[i]));
	}

	InvalidateRect(ref.getList(), &listSizes, true);
}