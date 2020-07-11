#include "pch.h"

#include "ScreenFunctions.h"
#include "CloudStorageScreen.h"
#include "RegistrationScreen.h"
#include "AuthorizationScreen.h"
#include "../UIConstants.h"
#include "../WindowProcedures/AuthorizationScreenProcedure.h"
#include "../WindowProcedures/RegistrationScreenProcedure.h"
#include "../WindowProcedures/CloudStorageScreenProcedure.h"

#include <commctrl.h>

using namespace std;

void initCloudStorageScreen(UI::MainWindow& ref)
{
	UI::BaseScreen* ptr = ref.getCurrentScreen();

	if (ptr)
	{
		ptr->pubDestroy();
	}

	ref.setCurrentScreen(new UI::CloudStorageScreen(ref.getMainWindow(), L"CloudStorage", CloudStorageScreenProcedure));
}

void initRegistrationScreen(UI::MainWindow& ref)
{
	UI::BaseScreen* ptr = ref.getCurrentScreen();

	if (ptr)
	{
		ptr->pubDestroy();
	}

	ref.setCurrentScreen(new UI::RegistrationScreen(ref.getMainWindow(), L"Registration", RegistrationScreenProcedure));
}

void initAuthorizationScreen(UI::MainWindow& ref)
{
	UI::BaseScreen* ptr = ref.getCurrentScreen();

	if (ptr)
	{
		ptr->pubDestroy();
	}

	ref.setCurrentScreen(new UI::AuthorizationScreen(ref.getMainWindow(), L"Authorization", AuthorizationScreenProcedure));
}

void updateNameColumn(UI::MainWindow& ref, const vector<wstring>& data)
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
		lvi.pszText = const_cast<wchar_t*>(data[i].data());
		lvi.iItem = i;
		lvi.iSubItem = UI::mainWindowUI::nameColumnIndex;

		SendMessageW(ref.getList(), LVM_INSERTITEM, NULL, reinterpret_cast<LPARAM>(&lvi));
	}

	InvalidateRect(ref.getList(), &rect, TRUE);
}