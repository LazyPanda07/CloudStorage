#include "pch.h"

#include "ErrorHandling.h"

using namespace std;

namespace UI
{
	int customError(MainWindow& ref, const std::wstring& message, const std::wstring& title)
	{
		int ok;

		if (ref.getCurrentPopupWindow())
		{
			ok = MessageBoxW(ref.getPopupWindow(), message.data(), title.data(), MB_OK);
		}
		else
		{
			ok = MessageBoxW(ref.getMainWindow(), message.data(), title.data(), MB_OK);
		}

		return ok;
	}

	int serverResponseError(MainWindow& ref)
	{
		return customError(ref, L"Ошибка при получении ответа от сервера", L"Ошибка соединения");
	}

	int serverRequestError(MainWindow& ref)
	{
		return customError(ref, L"Ошибка при отправке запроса на сервер", L"Ошибка соединения");
	}

	int noConnectionWithServer(MainWindow& ref)
	{
		return customError(ref, L"Нет соединения с сервером", L"Ошибка соединения");
	}
}