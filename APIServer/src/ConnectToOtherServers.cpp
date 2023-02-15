#include "pch.h"

#include "ConnectToOtherServers.h"
#include "FilesNetwork.h"
#include "DataBaseNetwork.h"

using namespace std;

namespace web
{
	void connectToFilesServer(unique_ptr<streams::IOSocketStream>& filesStream)
	{
		thread
		(
			[&filesStream]()
			{
				try
				{
					filesStream = make_unique<streams::IOSocketStream>(make_unique<FilesNetwork>());
				}
				catch (const web::exceptions::WebException&)
				{

				}
			}
		).detach();
	}

	void connectToDataBaseServer(unique_ptr<streams::IOSocketStream>& dataBaseStream)
	{
		thread
		(
			[&dataBaseStream]()
			{
				try
				{
					dataBaseStream = make_unique<streams::IOSocketStream>(make_unique<DataBaseNetwork>());
				}
				catch (const web::exceptions::WebException&)
				{

				}
			}
		).detach();
	}
}