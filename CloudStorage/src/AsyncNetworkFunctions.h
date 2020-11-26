#pragma once

#include <string>
#include <vector>

#include "BaseIOSocketStream.h"
#include "fileData.h"
#include "MainWindow.h"

std::string cancelUploadFile(const std::string& fileName);

std::string asyncFolderControlMessages(UI::MainWindow& ref, std::unique_ptr<streams::IOSocketStream>& clientStream, const std::string_view& controlRequest, std::vector<db::fileDataRepresentation>& fileNames, bool& isCancel, std::string&& data = "");

void asyncUploadFile(UI::MainWindow& ref, std::unique_ptr<streams::IOSocketStream>& clientStream, const std::wstring& filePath, std::vector<db::fileDataRepresentation>& fileNames, bool& isCancel, bool removeBeforeUpload = false);

void asyncDownloadFile(UI::MainWindow& ref, std::unique_ptr<streams::IOSocketStream>& clientStream, const std::wstring& fileName, intmax_t fileSize, bool& isCancel);

void asyncReconnect(UI::MainWindow& ref, std::unique_ptr<streams::IOSocketStream>& clientStream, std::string&& currentPath, const std::wstring& login, const std::wstring& password, std::vector<db::fileDataRepresentation>& fileNames, bool& isCancel);

void asyncCreateFolder(UI::MainWindow& ref, std::unique_ptr<streams::IOSocketStream>& clientStream, std::wstring&& wFolderName, std::vector<db::fileDataRepresentation>& fileNames, bool& isCancel);

void asyncRegistration(UI::MainWindow& ref, std::unique_ptr<streams::IOSocketStream>& clientStream, std::wstring& login, std::wstring& password, bool& isCancel);

void asyncAuthorization(UI::MainWindow& ref, std::unique_ptr<streams::IOSocketStream>& clientStream, std::wstring& login, std::wstring& password, bool& isCancel);

void asyncFirstConnect(UI::MainWindow& ref, std::unique_ptr<streams::IOSocketStream>& clientStream, bool& isCancel);