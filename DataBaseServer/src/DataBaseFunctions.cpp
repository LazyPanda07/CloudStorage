#include "pch.h"

#include "DataBaseFunctions.h"
#include "Constants.h"

using namespace std;

void authorization(streams::IOSocketStream<char>& clientStream, const db::CloudDataBase& db, string& login)
{
	string password;

	clientStream >> login;
	clientStream >> password;

	if (db.authorization(login, password))
	{
		clientStream << responses::okResponse;
	}
	else
	{
		clientStream << accountResponses::failAuthorization;
	}
}

void registration(streams::IOSocketStream<char>& clientStream, const db::CloudDataBase& db, string& login)
{
	string password;

	clientStream >> login;
	clientStream >> password;

	if (db.registration(login, password).size())
	{
		clientStream << accountResponses::failRegistration;
	}
	else
	{
		clientStream << accountResponses::successRegistration;
	}
}

void showAllFilesInFolder(streams::IOSocketStream<char>& clientStream, const db::CloudDataBase& db, const string& login, const filesystem::path& currentPath)
{
	string result;

	vector<db::fileData> data = db.getFiles(login);

	for (const auto& i : data)
	{
		result += i;
	}

	if (data.empty())
	{
		clientStream << responses::failResponse;

		clientStream << filesResponses::emptyFolder;
	}
	else
	{
		clientStream << responses::okResponse;

		clientStream << result;
	}
}

void uploadFileData(streams::IOSocketStream<char>& clientStream, const db::CloudDataBase& db, const string& login, const filesystem::path& currentPath)
{
	string fileName;
	string fileExtension;
	intmax_t fileSize;

	clientStream >> fileName;
	clientStream >> fileExtension;
	clientStream >> fileSize;

	db.uploadFileData(login, move(fileName), currentPath.string(), move(fileExtension), fileSize);
}

void removeFileData(streams::IOSocketStream<char>& clientStream, const db::CloudDataBase& db, const std::string& login, const filesystem::path& currentPath)
{
	string fileName;

	clientStream >> fileName;

	db.removeFileData(login, move(fileName), currentPath.string());
}

void nextFolder(streams::IOSocketStream<char>& clientStream, std::filesystem::path& currentPath)
{
	string folder;
	bool ok = true;

	clientStream >> folder;

	currentPath.append(folder);

	clientStream << ok;
}

void prevFolder(streams::IOSocketStream<char>& clientStream, std::filesystem::path& currentPath)
{
	bool ok = true;

	if (currentPath != "Home")
	{
		const string path = currentPath.string();

		currentPath = string(begin(path), begin(path) + path.rfind('\\'));
	}

	clientStream << ok;
}

void setPath(streams::IOSocketStream<char>& clientStream, std::filesystem::path& currentPath)
{
	string newPath;
	bool ok = true;

	clientStream >> newPath;

	currentPath = newPath;

	clientStream << ok;
}