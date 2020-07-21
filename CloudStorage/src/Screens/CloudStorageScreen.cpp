#include "pch.h"

#include "CloudStorageScreen.h"
#include "../UIConstants.h"
#include "../WindowProcedures/CloudStorageScreenProcedure.h"

#include <Richedit.h>
#include <commctrl.h>

using namespace std;

LRESULT __stdcall DragAndDrop(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData);

void createColumns(UI::CloudStorageScreen& ref);

namespace UI
{
	void CloudStorageScreen::show()
	{
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
		RECT wrapperSizes;

		GetClientRect(wrapper, &wrapperSizes);

		const LONG width = wrapperSizes.right - wrapperSizes.left;
		const LONG height = wrapperSizes.bottom - wrapperSizes.top;

		refreshButton = CreateWindowExW
		(
			NULL,
			L"BUTTON",
			L"Обновить список файлов",
			WS_CHILDWINDOW,
			0,
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
			toolbar::toolbarButtonWidth,
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
			toolbar::toolbarButtonWidth * 2,
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
			toolbar::toolbarButtonWidth * 3,
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
			WS_CHILDWINDOW | LVS_EDITLABELS | LVS_REPORT,
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

		for (size_t i = 0; i < count; i++)
		{
			data[i].resize(DragQueryFileW(drop, i, nullptr, data[i].size()) + 1);

			DragQueryFileW(drop, i, data[i].data(), data[i].size());

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
		width * UI::mainWindowUI::nameColumnCoefficientWidth,
		width * UI::mainWindowUI::dateColumnCoefficientWidth,
		width * UI::mainWindowUI::typeColumnCoefficientWidth,
		width * UI::mainWindowUI::sizeColumnCoefficientWidth
	};

	columns[0].pszText = const_cast<wchar_t*>(L"Имя");
	columns[1].pszText = const_cast<wchar_t*>(L"Дата изменения");
	columns[2].pszText = const_cast<wchar_t*>(L"Тип");
	columns[3].pszText = const_cast<wchar_t*>(L"Размер");

	for (size_t i = 0; i < columns.size(); i++)
	{
		columns[i].mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
		columns[i].iSubItem = i;
		columns[i].fmt = LVCFMT_LEFT;
		columns[i].cx = columnsSizes[i];

		SendMessageW(ref.getList(), LVM_INSERTCOLUMN, static_cast<WPARAM>(i), reinterpret_cast<LPARAM>(&columns[i]));
	}

	InvalidateRect(ref.getList(), &listSizes, true);
}