#include "pch.h"

#include "Log.h"
#include "DataBaseServer.h"
#include "INIParser.h"
#include "Constants.h"

#pragma comment (lib, "INIParser.lib")

using namespace std;

void showAllCommands();

int main(int argc, char** argv)
{
	Log::init();
	SetConsoleOutputCP(1251);
	SetConsoleCP(1251);

	utility::INIParser parser(settingsFile);

	web::DataBaseServer::setDataBaseServerPort(parser.getKeyValue("DataBaseServer", "ServerPort"));

	showAllCommands();

	try
	{
		web::DataBaseServer server;
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
				cout << (server.serverState() ? "������ ��������" : "������ �� ��������") << endl;
			}
			else if (command == "help")
			{
				showAllCommands();
			}
			else
			{
				cout << "����������� ������� - " << command << endl;
			}
		}
	}
	catch (const web::WebException& e)
	{
		cout << e.what() << endl;
	}

	return 0;
}

void showAllCommands()
{
	const static string commandsList = "������ ��������� ������ DataBaseServer\n";

	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO consoleInfo;

	GetConsoleScreenBufferInfo(console, &consoleInfo);
	SHORT width = consoleInfo.srWindow.Right - consoleInfo.srWindow.Left + 1;

	COORD pos = { (width - commandsList.size()) / 2 , consoleInfo.dwCursorPosition.Y };

	SetConsoleCursorPosition(console, pos);

	WriteConsoleA(console, commandsList.data(), commandsList.size(), nullptr, NULL);

	cout << "cls - �������� �������" << endl
		<< "clients - ������ ����������� � ������� ip ����� - �����" << endl
		<< "cls_all - ��������� �������� �������" << endl
		<< "start - ������ �������" << endl
		<< "stop - ������ ��������� ��������� ����������, �� ������� ���������� ��������" << endl
		<< "state - ������� ��������� �������" << endl
		<< "help - ������ ������" << endl;
}