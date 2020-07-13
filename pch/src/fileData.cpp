#include "pch.h"

#include "fileData.h"
#include "Constants.h"

using namespace std;

namespace db
{
	fileData::fileData(string&& fileName, string&& filePath, string&& fileExtension, string&& uploadDate, string&& dateOfChange, unsigned int fileSize) noexcept :
		fileName(move(fileName)),
		filePath(move(filePath)),
		fileExtension(move(fileExtension)),
		uploadDate(move(uploadDate)),
		dateOfChange(move(dateOfChange)),
		fileSize(fileSize)
	{

	}

	fileData::operator string () const
	{
		string result;

		result.
			append(fileName).
			append("|").
			append(filePath).
			append("|").
			append(fileExtension).
			append("|").
			append(uploadDate).
			append("|").
			append(dateOfChange).
			append("|").
			append(to_string(fileSize)).
			append(delimiter);

		return result;
	}

	wFileData::wFileData(wstring&& fileName, wstring&& filePath, wstring&& fileExtension, wstring&& uploadDate, wstring&& dateOfChange, unsigned int fileSize) noexcept :
		fileName(move(fileName)),
		filePath(move(filePath)),
		fileExtension(move(fileExtension)),
		uploadDate(move(uploadDate)),
		dateOfChange(move(dateOfChange)),
		fileSize(fileSize)
	{

	}
}