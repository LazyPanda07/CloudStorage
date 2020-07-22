#include "pch.h"

#include "CloudStorageServer.h"
#include "Log.h"
#include "INIParser.h"
#include "Constants.h"

#pragma comment (lib, "INIParser.lib")

using namespace std;

int main(int argc, char** argv)
{
	Log::init();
	SetConsoleOutputCP(1251);

	utility::INIParser parser(settingsFile);

	web::CloudStorageServer::setCloudStorageServerPort(parser.getKeyValue("CloudStorageServer", "ServerPort"));

	try
	{
		web::CloudStorageServer server;	//TODO: add user interface
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