#include "pch.h"

#include "ErrorHandling.h"

using namespace std;

namespace UI
{
	void customError(MainWindow& ref, const std::wstring& message, const std::wstring& title)
	{
		MessageBoxW(ref.getMainWindow(), message.data(), title.data(), MB_OK);
	}

	void serverResponseError(MainWindow& ref)
	{
		customError(ref, L"Ошибка при получении ответа от сервера", L"Ошибка соединения");
	}

	void serverRequestError(MainWindow& ref)
	{
		customError(ref, L"Ошибка при отправке запроса на сервер", L"Ошибка соединения");
	}
}