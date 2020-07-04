#include "pch.h"

#include "APIServer.h"
#include "Log.h"

using namespace std;

int main(int argc, char** argv)
{
	Log::init(false);
	SetConsoleOutputCP(1251);

	try
	{
		web::APIServer server;
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