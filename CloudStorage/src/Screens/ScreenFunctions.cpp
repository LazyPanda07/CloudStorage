#include "pch.h"

#include "ScreenFunctions.h"
#include "CloudStorageScreen.h"
#include "RegistrationScreen.h"
#include "AuthorizationScreen.h"
#include "UtilityFunctions.h"
#include "../UIConstants.h"
#include "../WindowProcedures/AuthorizationScreenProcedure.h"
#include "../WindowProcedures/RegistrationScreenProcedure.h"
#include "../WindowProcedures/CloudStorageScreenProcedure.h"

#include <commctrl.h>

using namespace std;

void updateNameColumn(UI::MainWindow& ref, const vector<db::wFileData>& data);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void initCloudStorageScreen(UI::MainWindow& ref)
{
	ref.setCurrentScreen(new UI::CloudStorageScreen(ref.getMainWindow(), UI::screens::cloudStorageScreen, CloudStorageScreenProcedure));
}

void initRegistrationScreen(UI::MainWindow& ref)
{
	ref.setCurrentScreen(new UI::RegistrationScreen(ref.getMainWindow(), UI::screens::registrationScreen, RegistrationScreenProcedure));
}

void initAuthorizationScreen(UI::MainWindow& ref)
{
	ref.setCurrentScreen(new UI::AuthorizationScreen(ref.getMainWindow(), UI::screens::authorizationScreen, AuthorizationScreenProcedure));
}

bool removeFileDialog(UI::MainWindow& ref, const std::wstring& fileName)
{
	int dialog = MessageBoxW
	(
		ref.getMainWindow(),
		wstring(L"Вы действительно хотите удалить " + fileName).data(),
		L"Удаление файла",
		MB_YESNO | MB_ICONWARNING
	);

	return dialog == IDYES;
}

void updateColumns(UI::MainWindow& ref, const vector<db::wFileData>& data)
{
	updateNameColumn(ref, data);

	//TODO: update other columns
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void updateNameColumn(UI::MainWindow& ref, const vector<db::wFileData>& data)
{
	bool success = false;

	do
	{
		LVITEMW item = {};
		item.iSubItem = UI::mainWindowUI::nameColumnIndex;

		success = SendMessageW(ref.getList(), LVM_GETITEMW, NULL, reinterpret_cast<LPARAM>(&item));

		SendMessageW(ref.getList(), LVM_DELETEITEM, item.iItem, NULL);
	} while (success);

	LVITEMW lvi = {};

	RECT rect;

	GetClientRect(ref.getList(), &rect);

	lvi.mask = LVIF_TEXT;

	for (size_t i = 0; i < data.size(); i++)
	{
		lvi.pszText = const_cast<wchar_t*>(data[i].fileName.data());
		lvi.iItem = i;
		lvi.iSubItem = UI::mainWindowUI::nameColumnIndex;

		SendMessageW(ref.getList(), LVM_INSERTITEM, NULL, reinterpret_cast<LPARAM>(&lvi));
	}

	InvalidateRect(ref.getList(), &rect, TRUE);
}