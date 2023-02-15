#include "pch.h"

#include "APIServer.h"
#include "Log.h"
#include "INIParser.h"
#include "Constants.h"
#include "FilesNetwork.h"
#include "DataBaseNetwork.h"

#pragma comment (lib, "INIParser.lib")

using namespace std;

void showAllCommands();

int main(int argc, char** argv)
{
	Log::init();
	SetConsoleOutputCP(1251);
	SetConsoleCP(1251);

	utility::INIParser parser(settingsFile);

	web::APIServer::setAPIServerPort(parser.getKeyValueData("APIServer", "ServerPort"));

	web::FilesNetwork::setCloudStorageServerIp(parser.getKeyValueData("CloudStorageServer", "ServerIp"));
	web::FilesNetwork::setCloudStorageServerPort(parser.getKeyValueData("CloudStorageServer", "ServerPort"));

	web::DataBaseNetwork::setDataBaseServerIp(parser.getKeyValueData("DataBaseServer", "ServerIp"));
	web::DataBaseNetwork::setDataBaseServerPort(parser.getKeyValueData("DataBaseServer", "ServerPort"));

	showAllCommands();

	try
	{
		web::APIServer server;
		string command;

		server.start();

		while (true)
		{
			cin >> command;

			if (command == "clients")
			{
				auto clients = server.getClients();

				for (const auto& i : clients)
				{
					cout << i.first << "	- " << i.second << endl;
				}
			}
			else if (command == "cls")
			{
				system("cls");
				showAllCommands();
			}
			else if (command == "cls_all")
			{
				system("cls");
			}
			else if (command == "start")
			{
				server.start();
			}
			else if (command == "stop")
			{
				server.stop();
			}
			else if (command == "state")
			{
				cout << (server.serverState() ? "Сервер работает" : "Сервер не работает") << endl;
			}
			else if (command == "help")
			{
				showAllCommands();
			}
			else if (command == "disconnect")
			{
				string ip;

				cin >> ip;

				server.pubDisconnect(ip);
			}
			else
			{
				cout << "Неизвестная команда - " << command << endl;
			}
		}
	}
	catch (const web::exceptions::WebException& e)
	{
		cout << e.what() << endl;
	}

	return 0;
}

void showAllCommands()
{
	const static string commandsList = "Список доступных команд APIServer\n";

	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO consoleInfo;

	GetConsoleScreenBufferInfo(console, &consoleInfo);
	SHORT width = consoleInfo.srWindow.Right - consoleInfo.srWindow.Left + 1;

	COORD pos = { (width - commandsList.size()) / 2 , consoleInfo.dwCursorPosition.Y };

	SetConsoleCursorPosition(console, pos);

	WriteConsoleA(console, commandsList.data(), commandsList.size(), nullptr, NULL);

	cout << "clients - Список подключений в формате ip адрес - сокет" << endl
		<< "disconnect - Отключает выбранного пользователя по его ip адресу" << endl
		<< "cls - Очистить консоль" << endl
		<< "cls_all - Полностью очистить консоль" << endl
		<< "start - Запуск сервера" << endl
		<< "stop - Сервер перестает принимать соединения, но текущие соединения остаются" << endl
		<< "state - Текущее состояние сервера" << endl
		<< "help - Список команд" << endl;
}