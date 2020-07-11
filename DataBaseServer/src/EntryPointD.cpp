#include "pch.h"

#include "Log.h"
#include "DataBaseServer.h"
#include "INIParser.h"
#include "Constants.h"

#pragma comment (lib, "INIParser.lib")

using namespace std;

int main(int argc, char** argv)
{
	Log::init(false);
	SetConsoleOutputCP(1251);

	utility::INIParser parser(settingsFile);

	web::DataBaseServer::setDataBaseServerPort(parser.getKeyValue("DataBaseServer", "ServerPort"));

	try
	{
		web::DataBaseServer server;
		string command;

		server.start();

		while (true)
		{
			cin >> command;


		}
	}
	catch (const web::WebException& e)
	{
		cout << e.what() << endl;
	}

	return 0;
}