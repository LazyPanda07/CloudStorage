#include "pch.h"

#include "FilesFunctions.h"
#include "Constants.h"
#include "UtilityFunctions.h"

using namespace std;

void uploadFile(streams::IOSocketStream<char>& clientStream, const filesystem::path& currentPath)
{
	string fileName;
	uintmax_t offset;
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
		uintmax_t uploadSize;
		uintmax_t downloadSize = filesystem::file_size(filesystem::path(currentPath).append(fileName));

		clientStream >> uploadSize;

		if (uploadSize == downloadSize)
		{
			clientStream << responses::okResponse;
			clientStream << filesResponses::successUploadFile;

			SHFILEINFOW extensionInfo = {};
			
			SHGetFileInfoW(filesystem::path(currentPath).append(fileName).wstring().data(), NULL, &extensionInfo, sizeof(extensionInfo), SHGFI_TYPENAME);
			
			string extension
			(
				utility::to_string
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
	uintmax_t offset;
	uintmax_t size;
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