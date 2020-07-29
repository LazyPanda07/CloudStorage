#pragma once

#include <vector>
#include <string>

#include "getFiles.h"
#include "removeFile.h"
#include "IOSocketStream.h"
#include "fileData.h"
#include "MainWindow.h"

void uploadFile(UI::MainWindow& ref, streams::IOSocketStream<char>& clientStream, const std::wstring& filePath, std::vector<db::fileDataRepresentation>& fileNames, bool& isCancel);

int downloadFile(UI::MainWindow& ref, streams::IOSocketStream<char>& clientStream, const std::vector<db::fileDataRepresentation>& fileNames, bool& isCancel, int searchId);

void reconnect(UI::MainWindow& ref, streams::IOSocketStream<char>& clientStream, std::string&& currentPath, const std::wstring& login, const std::wstring& password, std::vector<db::fileDataRepresentation>& fileNames, bool& isCancel);

void nextFolder(UI::MainWindow& ref, streams::IOSocketStream<char>& clientStream, const std::wstring& folderName, std::vector<db::fileDataRepresentation>& fileNames, bool& isCancel);

void prevFolder(UI::MainWindow& ref, streams::IOSocketStream<char>& clientStream, std::filesystem::path& currentPath, std::vector<db::fileDataRepresentation>& fileNames, bool& isCancel);

void createFolder(UI::MainWindow& ref, streams::IOSocketStream<char>& clientStream, std::vector<db::fileDataRepresentation>& fileNames, bool& isCancel);

void exitFromApplication(UI::MainWindow& ref, streams::IOSocketStream<char>& clientStream);

void authorization(UI::MainWindow& ref, streams::IOSocketStream<char>& clientStream, std::wstring& login, std::wstring& password, bool& isCancel);

void registration(UI::MainWindow& ref, streams::IOSocketStream<char>& clientStream, std::wstring& login, std::wstring& password, bool& isCancel);