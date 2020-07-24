#include "pch.h"

#include "FilesFunctions.h"
#include "Constants.h"
#include "UtilityFunctions.h"

using namespace std;

void uploadFile(streams::IOSocketStream<char>& clientStream, const filesystem::path& currentPath)
{
	string fileName;
	intmax_t offset;
	string data;
	bool needResponse;

	clientStream >> fileName;
	clientStream >> offset;
	clientStream >> data;
	clientStream >> needResponse;

	ofstream out(filesystem::path(currentPath).append(fileName), ios::binary | ios::app);

	out.seekp(offset);

	out.write(data.data(), data.size());

	out.close();

	if (needResponse)
	{
		intmax_t uploadSize;
		intmax_t downloadSize = filesystem::file_size(filesystem::path(currentPath).append(fileName));

		clientStream >> uploadSize;

		if (uploadSize == downloadSize)
		{
			clientStream << responses::okResponse;
			clientStream << filesResponses::successUploadFile;

			SHFILEINFOW extensionInfo = {};
			
			SHGetFileInfoW(filesystem::path(currentPath).append(fileName).wstring().data(), NULL, &extensionInfo, sizeof(extensionInfo), SHGFI_TYPENAME);
			
			string extension
			(
				utility::conversion::to_string
				(
					wstring(extensionInfo.szTypeName, extensionInfo.szTypeName + wcslen(extensionInfo.szTypeName))
				)
			);

			clientStream << extension;
		}
		else
		{
			clientStream << responses::failResponse;
			clientStream << filesResponses::failUploadFile;
		}
	}
}

void downloadFile(streams::IOSocketStream<char>& clientStream, const filesystem::path& currentPath)
{
	string fileName;
	intmax_t offset;
	intmax_t size;
	string data;
	bool isLast;

	clientStream >> fileName;
	clientStream >> offset;

	filesystem::path filePath(filesystem::path(currentPath).append(fileName));
	size = filesystem::file_size(filePath);

	ifstream in(filePath, ios::binary);

	in.seekg(offset);

	if (size - offset >= filePacketSize)
	{
		data.resize(filePacketSize);
		isLast = false;
	}
	else
	{
		data.resize(size - offset);
		isLast = true;
	}

	in.read(data.data(), data.size());

	in.close();

	clientStream << data;
	clientStream << isLast;

	if (isLast)
	{
		clientStream << size;
	}
}

void removeFile(streams::IOSocketStream<char>& clientStream, const filesystem::path& currentPath)
{
	string fileName;

	clientStream >> fileName;

	if (filesystem::remove(filesystem::path(currentPath).append(fileName)))
	{
		clientStream << responses::okResponse;
	}
	else
	{
		clientStream << responses::failResponse;
	}
}

void cancelOperation(streams::IOSocketStream<char>& clientStream, const filesystem::path& currentPath)
{
	string fileName;
	string operationType;
	
	clientStream >> fileName;
	clientStream >> operationType;

	if (operationType == filesRequests::uploadFile)
	{
		filesystem::remove(filesystem::path(currentPath).append(fileName));
	}
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

	if (currentPath.filename().string() != usersFolder)
	{
		const string path = currentPath.string();

		currentPath = string(begin(path), begin(path) + path.rfind('\\'));
	}

	clientStream << ok;
}

void setPath(streams::IOSocketStream<char>& clientStream, std::filesystem::path& currentPath, const string& login)
{
	string newPath;
	bool ok = true;

	clientStream >> newPath;

	if (newPath == "Home")
	{
		currentPath = filesystem::current_path().append(usersFolder).append(login);
	}
	else
	{
		currentPath = filesystem::current_path().append(usersFolder).append(login);

		string tem;

		for (size_t i = newPath.find('\\') + 1; i < newPath.size(); i++)
		{
			if (newPath[i] == '\\')
			{
				currentPath.append(tem);

				tem.clear();
			}
			else
			{
				tem += newPath[i];
			}
		}
	}

	clientStream << ok;
}

void createFolder(streams::IOSocketStream<char>& clientStream, const filesystem::path& currentPath)
{
	string folderName;

	clientStream >> folderName;

	const filesystem::path tem(filesystem::path(currentPath).append(folderName));

	filesystem::create_directory(tem);

	SHFILEINFOW extensionInfo = {};

	SHGetFileInfoW(tem.wstring().data(), NULL, &extensionInfo, sizeof(extensionInfo), SHGFI_TYPENAME);

	string extension
	(
		utility::conversion::to_string
		(
			wstring(extensionInfo.szTypeName, extensionInfo.szTypeName + wcslen(extensionInfo.szTypeName))
		)
	);

	clientStream << extension;
}