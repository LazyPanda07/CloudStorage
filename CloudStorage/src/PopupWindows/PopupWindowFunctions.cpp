#include "pch.h"

#include "PopupWindowFunctions.h"
#include "UploadFilePopupWindow.h"
#include "DownloadFilePopupWindow.h"
#include "SetFolderNamePopupWindow.h"
#include "WaitResponsePopupWindow.h"
#include "../UIConstants.h"

using namespace std;

void initUploadFilePopupWindow(UI::MainWindow& ref, const wstring& message)
{
	ref.setCurrentPopupWindow
	(
		new UI::UploadFilePopupWindow
		(
			ref.getMainWindow(),
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
			message
		)
	);
}

void initSetFolderNamePopupWidow(UI::MainWindow& ref)
{
	ref.setCurrentPopupWindow(new UI::SetFolderNamePopupWindow(ref.getMainWindow()));
}

void initWaitResponsePopupWindow(UI::MainWindow& ref)
{
	ref.setCurrentPopupWindow(new UI::WaitResponsePopupWindow(ref.getMainWindow()));

	if (ref.getCurrentPopupWindow())
	{
		static_cast<UI::WaitResponsePopupWindow*>(ref.getCurrentPopupWindow())->startAnimateProgressBar();
	}
}