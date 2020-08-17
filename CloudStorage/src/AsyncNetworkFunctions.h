#pragma once

#include <string>
#include <vector>

#include "IOSocketStream.h"
#include "fileData.h"
#include "MainWindow.h"

std::string cancelUploadFile(const std::string& fileName);

std::string asyncFolderControlMessages(UI::MainWindow& ref, streams::IOSocketStream<char>& clientStream, const std::string_view& controlRequest, std::vector<db::fileDataRepresentation>& fileNames, bool& isCancel, std::string&& data = "");

void asyncUploadFile(UI::MainWindow& ref, streams::IOSocketStream<char>& clientStream, const std::wstring& filePath, std::vector<db::fileDataRepresentation>& fileNames, bool& isCancel);

void asyncDownloadFile(UI::MainWindow& ref, streams::IOSocketStream<char>& clientStream, const std::wstring& fileName, intmax_t fileSize, bool& isCancel);

void asyncReconnect(UI::MainWindow& ref, streams::IOSocketStream<char>& clientStream, std::string&& currentPath, const std::wstring& login, const std::wstring& password, std::vector<db::fileDataRepresentation>& fileNames, bool& isCancel);

void asyncCreateFolder(UI::MainWindow& ref, streams::IOSocketStream<char>& clientStream, std::vector<db::fileDataRepresentation>& fileNames, bool& isCancel);

void asyncRegistration(UI::MainWindow& ref, streams::IOSocketStream<char>& clientStream, std::wstring& login, std::wstring& password, bool& isCancel);

void asyncAuthorization(UI::MainWindow& ref, streams::IOSocketStream<char>& clientStream, std::wstring& login, std::wstring& password, bool& isCancel);