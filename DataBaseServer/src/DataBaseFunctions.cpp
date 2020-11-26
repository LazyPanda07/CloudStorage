#include "pch.h"

#include "DataBaseFunctions.h"
#include "UtilityFunctions.h"
#include "Constants.h"

using namespace std;

void authorization(streams::IOSocketStream& clientStream, const db::CloudDataBase& db, string& login)
{
	string password;

	clientStream >> login;
	clientStream >> password;

	if (utility::validation::validationUserData(login) && utility::validation::validationUserData(password))
	{
		if (db.authorization(login, password))
		{
			clientStream << responses::okResponse;
		}
		else
		{
			clientStream << accountResponses::failAuthorization;
		}
	}
	else
	{
		login = "";
		clientStream << accountResponses::incorrectCharacters;
	}
}

void registration(streams::IOSocketStream& clientStream, const db::CloudDataBase& db, string& login)
{
	string password;

	clientStream >> login;
	clientStream >> password;

	if (utility::validation::validationUserData(login) && utility::validation::validationUserData(password))
	{
		if (db.registration(login, password).size())
		{
			clientStream << accountResponses::failRegistration;
		}
		else
		{
			clientStream << accountResponses::successRegistration;
		}
	}
	else
	{
		login = "";
		clientStream << accountResponses::incorrectCharacters;
	}
}

void showAllFilesInFolder(streams::IOSocketStream& clientStream, const db::CloudDataBase& db, const string& login, const filesystem::path& currentPath)
{
	string result;

	vector<db::fileData> data = db.getFiles(login, currentPath.string());

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

void uploadFileData(streams::IOSocketStream& clientStream, const db::CloudDataBase& db, const string& login, const filesystem::path& currentPath)
{
	string fileName;
	string fileExtension;
	intmax_t fileSize;

	clientStream >> fileName;
	clientStream >> fileExtension;
	clientStream >> fileSize;

	db.uploadFileData(login, move(fileName), currentPath.string(), move(fileExtension), fileSize);
}

void removeFileData(streams::IOSocketStream& clientStream, const db::CloudDataBase& db, const std::string& login, const filesystem::path& currentPath)
{
	string fileName;

	clientStream >> fileName;

	db.removeFileData(login, move(fileName), currentPath.string());
}

void nextFolder(streams::IOSocketStream& clientStream, std::filesystem::path& currentPath)
{
	string folder;
	bool ok = true;

	clientStream >> folder;

	currentPath.append(folder);

	clientStream << ok;
}

void prevFolder(streams::IOSocketStream& clientStream, std::filesystem::path& currentPath)
{
	bool ok = true;

	if (currentPath != "Home")
	{
		const string path = currentPath.string();

		currentPath = string(begin(path), begin(path) + path.rfind('\\'));
	}

	clientStream << ok;
}

void setPath(streams::IOSocketStream& clientStream, std::filesystem::path& currentPath)
{
	string newPath;
	bool ok = true;

	clientStream >> newPath;

	currentPath = newPath;

	clientStream << ok;
}

void createFolder(streams::IOSocketStream& clientStream, const db::CloudDataBase& db, const string& login, const std::filesystem::path& currentPath)
{
	string folderName;
	string fileExtension;

	clientStream >> folderName;
	clientStream >> fileExtension;

	db.addFolder(login, move(folderName), currentPath.string(), move(fileExtension));
}