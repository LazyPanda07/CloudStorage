#include "pch.h"

#include "fileData.h"
#include "Constants.h"

using namespace std;

namespace db
{
	fileData::fileData(string&& fileName, string&& filePath, string&& fileExtension, string&& uploadDate, string&& dateOfChange, intmax_t fileSize) noexcept :
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
			append(dataPartDelimiter).
			append(filePath).
			append(dataPartDelimiter).
			append(fileExtension).
			append(dataPartDelimiter).
			append(uploadDate).
			append(dataPartDelimiter).
			append(dateOfChange).
			append(dataPartDelimiter).
			append(to_string(fileSize)).
			append(dataDelimiter);

		return result;
	}

	fileDataRepresentation::fileDataRepresentation(wstring&& fileName, wstring&& filePath, wstring&& fileExtension, wstring&& uploadDate, wstring&& dateOfChange, intmax_t fileSize) noexcept :
		fileName(move(fileName)),
		filePath(move(filePath)),
		fileExtension(move(fileExtension)),
		uploadDate(move(uploadDate)),
		dateOfChange(move(dateOfChange)),
		fileSizeS(this->fileExtension == L"Папка с файлами" ? L"" : to_wstring(fileSize) + L" Б"),
		fileSize(fileSize)
	{

	}
}