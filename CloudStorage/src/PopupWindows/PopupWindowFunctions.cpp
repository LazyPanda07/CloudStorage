#include "pch.h"

#include "PopupWindowFunctions.h"
#include "UploadFilePopupWindow.h"
#include "../WindowProcedures/UploadFilePopupWindowProcedure.h"
#include "../UIConstants.h"

using namespace std;

void initUploadFilePopupWindow(UI::MainWindow& ref, const wstring& message)
{
	ref.setCurrentPopupWindow
	(
		new UI::UploadFilePopupWindow
		(
			ref.getMainWindow(),
			UI::popupWindows::uploadFilePopupWindow,
			L"Загрузка файла",
			UploadFilePopupWindowProcedure,
			message
		)
	);
}