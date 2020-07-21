#include "pch.h"

#include "PopupWindowFunctions.h"
#include "UploadFilePopupWindow.h"
#include "DownloadFilePopupWindow.h"
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
			L"Загрузка файла",
			UploadFilePopupWindowProcedure,
			message
		)
	);
}

void initDownloadFilePopupWindow(UI::MainWindow& ref, const wstring& message)
{
	ref.setCurrentPopupWindow
	(
		new UI::DownloadFilePopupWindow
		(
			ref.getMainWindow(),
			L"Скачивание файла",
			UploadFilePopupWindowProcedure,
			message
		)
	);
}