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
					filesStream = make_unique<streams::IOSocketStream>(new buffers::IOSocketBuffer(new FilesNetwork()));
				}
				catch (const web::WebException&)
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
					dataBaseStream = make_unique<streams::IOSocketStream>(new buffers::IOSocketBuffer(new DataBaseNetwork()));
				}
				catch (const web::WebException&)
				{

				}
			}
		).detach();
	}
}