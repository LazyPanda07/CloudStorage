#include "pch.h"

#include "CloudStorageServer.h"
#include "Log.h"

using namespace std;

int main(int argc, char** argv)
{
	Log::init();
	SetConsoleOutputCP(1251);

	try
	{
		web::CloudStorageServer server;
		string command;

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