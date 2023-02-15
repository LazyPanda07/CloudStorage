#include "pch.h"

#include "ScreenFunctions.h"
#include "CloudStorageScreen.h"
#include "RegistrationScreen.h"
#include "AuthorizationScreen.h"
#include "UtilityFunctions.h"
#include "../UIConstants.h"

#include <CommCtrl.h>

using namespace std;

void clearColumns(UI::MainWindow& ref);

void updateNameColumn(UI::MainWindow& ref, const vector<db::fileDataRepresentation>& data);

void updateDateColumn(UI::MainWindow& ref, const vector<db::fileDataRepresentation>& data);

void updateTypeColumn(UI::MainWindow& ref, const vector<db::fileDataRepresentation>& data);

void updateSizeColumn(UI::MainWindow& ref, const vector<db::fileDataRepresentation>& data);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void initCloudStorageScreen(UI::MainWindow& ref)
{
	ref.setCurrentScreen(new UI::CloudStorageScreen(ref.getMainWindow()));
}

void initRegistrationScreen(UI::MainWindow& ref)
{
	ref.setCurrentScreen(new UI::RegistrationScreen(ref.getMainWindow()));
}

void initAuthorizationScreen(UI::MainWindow& ref)
{
	ref.setCurrentScreen(new UI::AuthorizationScreen(ref.getMainWindow()));
}

bool removeFileDialog(UI::MainWindow& ref, const wstring& fileName)
{
	int dialog = IDYES;

	if (ref.getCurrentPopupWindow())
	{
		dialog = MessageBoxW
		(
			ref.getPopupWindow(),
			wstring(L"Вы действительно хотите удалить " + fileName).data(),
			L"Удаление файла",
			MB_YESNO | MB_ICONWARNING
		);
	}
	else
	{
		dialog = MessageBoxW
		(
			ref.getMainWindow(),
			wstring(L"Вы действительно хотите удалить " + fileName).data(),
			L"Удаление файла",
			MB_YESNO | MB_ICONWARNING
		);
	}

	return dialog == IDYES;
}

void updateColumns(UI::MainWindow& ref, const vector<db::fileDataRepresentation>& data)
{
	clearColumns(ref);

	updateNameColumn(ref, data);

	updateDateColumn(ref, data);

	updateTypeColumn(ref, data);

	updateSizeColumn(ref, data);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void clearColumns(UI::MainWindow& ref)
{
	bool success = false;

	do
	{
		LVITEMW item = {};
		item.iSubItem = UI::mainWindowUI::nameColumnIndex;

		success = SendMessageW(ref.getList(), LVM_GETITEMW, NULL, reinterpret_cast<LPARAM>(&item));

		SendMessageW(ref.getList(), LVM_DELETEITEM, item.iItem, NULL);
	} while (success);
}

void updateNameColumn(UI::MainWindow& ref, const vector<db::fileDataRepresentation>& data)
{
	LVITEMW item = {};

	RECT rect;

	GetClientRect(ref.getList(), &rect);

	item.mask = LVIF_TEXT;

	for (size_t i = 0; i < data.size(); i++)
	{
		item.pszText = const_cast<wchar_t*>(data[i].fileName.data());
		item.iItem = static_cast<int>(i);
		item.iSubItem = UI::mainWindowUI::nameColumnIndex;

		SendMessageW(ref.getList(), LVM_INSERTITEM, NULL, reinterpret_cast<LPARAM>(&item));
	}

	InvalidateRect(ref.getList(), &rect, TRUE);
}

void updateDateColumn(UI::MainWindow& ref, const vector<db::fileDataRepresentation>& data)
{
	LVITEMW item = {};

	RECT rect;

	GetClientRect(ref.getList(), &rect);

	item.mask = LVIF_TEXT;

	for (size_t i = 0; i < data.size(); i++)
	{
		item.pszText = const_cast<wchar_t*>(data[i].dateOfChange.data());
		item.iItem = static_cast<int>(i);
		item.iSubItem = UI::mainWindowUI::dateColumnIndex;

		SendMessageW(ref.getList(), LVM_SETITEMW, NULL, reinterpret_cast<LPARAM>(&item));
	}

	InvalidateRect(ref.getList(), &rect, TRUE);
}

void updateTypeColumn(UI::MainWindow& ref, const vector<db::fileDataRepresentation>& data)
{
	LVITEMW item = {};

	RECT rect;

	GetClientRect(ref.getList(), &rect);

	item.mask = LVIF_TEXT;

	for (size_t i = 0; i < data.size(); i++)
	{
		item.pszText = const_cast<wchar_t*>(data[i].fileExtension.data());
		item.iItem = static_cast<int>(i);
		item.iSubItem = UI::mainWindowUI::typeColumnIndex;

		SendMessageW(ref.getList(), LVM_SETITEMW, NULL, reinterpret_cast<LPARAM>(&item));
	}

	InvalidateRect(ref.getList(), &rect, TRUE);
}

void updateSizeColumn(UI::MainWindow& ref, const vector<db::fileDataRepresentation>& data)
{
	LVITEMW item = {};

	RECT rect;

	GetClientRect(ref.getList(), &rect);

	item.mask = LVIF_TEXT;

	for (size_t i = 0; i < data.size(); i++)
	{
		item.pszText = const_cast<wchar_t*>(data[i].fileSizeS.data());
		item.iItem = static_cast<int>(i);
		item.iSubItem = UI::mainWindowUI::sizeColumnIndex;

		SendMessageW(ref.getList(), LVM_SETITEMW, NULL, reinterpret_cast<LPARAM>(&item));
	}

	InvalidateRect(ref.getList(), &rect, TRUE);
}