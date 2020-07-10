#include "pch.h"

#include "CloudStorageScreen.h"
#include "../UIConstants.h"

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
		ShowWindow(list, SW_SHOW);
	}

	void CloudStorageScreen::destroy()
	{
		DestroyWindow(refreshButton);
		DestroyWindow(downloadButton);
		DestroyWindow(list);
	}

	CloudStorageScreen::CloudStorageScreen(HWND parentWindow, const std::wstring& wrapperClassName, WNDPROC procedure) :
		BaseScreen(parentWindow,wrapperClassName,procedure)
	{
		RECT parentWindowSizes;

		GetClientRect(parentWindow, &parentWindowSizes);

		const LONG width = parentWindowSizes.right - parentWindowSizes.left;
		const LONG height = parentWindowSizes.bottom - parentWindowSizes.top;

		refreshButton = CreateWindowExW
		(
			NULL,
			L"BUTTON",
			L"�������� ������ ������",
			WS_CHILDWINDOW,
			0, 0,
			toolbar::toolbarButtonWidth, toolbar::toolbarButtonHeight,
			parentWindow,
			HMENU(buttons::refresh),
			nullptr,
			nullptr
		);

		downloadButton = CreateWindowExW
		(
			NULL,
			L"BUTTON",
			L"������� ����",
			WS_CHILDWINDOW,
			toolbar::toolbarButtonWidth, 0,
			toolbar::toolbarButtonWidth, toolbar::toolbarButtonHeight,
			parentWindow,
			HMENU(buttons::download),
			nullptr,
			nullptr
		);

		list = CreateWindowExW
		(
			WS_EX_CLIENTEDGE,
			WC_LISTVIEW,
			nullptr,
			WS_CHILDWINDOW | LVS_EDITLABELS | LVS_REPORT,
			0, toolbar::toolbarHeight,
			width, height - toolbar::toolbarHeight,
			parentWindow,
			HMENU(),
			nullptr,
			nullptr
		);

		DragAcceptFiles(list, true);
		SetWindowSubclass(list, &DragAndDrop, 1, NULL);

		createColumns(*this);

		SendMessageW(list, LVM_SETEXTENDEDLISTVIEWSTYLE, LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);
	}

	void CloudStorageScreen::resize()
	{
		RECT sizes;

		GetClientRect(BaseScreen::parentWindow, &sizes);

		LONG width = sizes.right - sizes.left;
		LONG height = sizes.bottom - sizes.top;

		SetWindowPos(list, HWND_BOTTOM, 0, toolbar::toolbarHeight, width, height - toolbar::toolbarHeight, SWP_SHOWWINDOW);
	}

	HWND CloudStorageScreen::getRefreshButton() const
	{
		return refreshButton;
	}

	HWND CloudStorageScreen::getDownloadButton() const
	{
		return downloadButton;
	}

	HWND CloudStorageScreen::getList() const
	{
		return list;
	}

	CloudStorageScreen::~CloudStorageScreen()
	{
		this->destroy();
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
	RECT parentWindowSizes;
	RECT listSizes;

	GetClientRect(ref.getParent(), &parentWindowSizes);
	GetClientRect(ref.getList(), &listSizes);

	LONG width = parentWindowSizes.right - parentWindowSizes.left;

	const array<LONG, UI::mainWindowUI::columnsInList> columnsSizes =
	{
		width * UI::mainWindowUI::nameColumnCoefficientWidth,
		width * UI::mainWindowUI::dateColumnCoefficientWidth,
		width * UI::mainWindowUI::typeColumnCoefficientWidth,
		width * UI::mainWindowUI::sizeColumnCoefficientWidth
	};

	columns[0].pszText = const_cast<wchar_t*>(L"���");
	columns[1].pszText = const_cast<wchar_t*>(L"���� ���������");
	columns[2].pszText = const_cast<wchar_t*>(L"���");
	columns[3].pszText = const_cast<wchar_t*>(L"������");

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