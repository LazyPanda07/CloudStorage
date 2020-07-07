#include "pch.h"

#include "Log.h"
#include "DataBaseServer.h"
#include "INIParser.h"

#pragma comment (lib, "INIParser.lib")

using namespace std;

int main(int argc, char** argv)
{
	Log::init(false);

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