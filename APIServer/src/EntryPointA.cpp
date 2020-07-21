#include "pch.h"

#include "APIServer.h"
#include "Log.h"
#include "INIParser.h"
#include "Constants.h"
#include "FilesNetwork.h"
#include "DataBaseNetwork.h"

#pragma comment (lib, "INIParser.lib")

using namespace std;

int main(int argc, char** argv)
{
	Log::init(false);
	SetConsoleOutputCP(1251);

	utility::INIParser parser(settingsFile);

	web::APIServer::setAPIServerPort(parser.getKeyValue("APIServer", "ServerPort"));

	web::FilesNetwork::setCloudStorageServerIp(parser.getKeyValue("CloudStorageServer", "ServerIp"));
	web::FilesNetwork::setCloudStorageServerPort(parser.getKeyValue("CloudStorageServer", "ServerPort"));

	web::DataBaseNetwork::setDataBaseServerIp(parser.getKeyValue("DataBaseServer", "ServerIp"));
	web::DataBaseNetwork::setDataBaseServerPort(parser.getKeyValue("DataBaseServer", "ServerPort"));

	try
	{
		web::APIServer server;	//TODO: add user interface
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
					cout << i.first << "	" << i.second << endl;
				}
			}
			else if (command == "cls")
			{
				system("cls");
			}
		}
	}
	catch (const web::WebException& e)
	{
		cout << e.what() << endl;
	}

	return 0;
}