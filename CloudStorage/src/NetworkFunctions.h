#pragma once

#include <vector>
#include <string>

#include "getFiles.h"
#include "removeFile.h"
#include "BaseIOSocketStream.h"
#include "fileData.h"
#include "MainWindow.h"

void uploadFile(UI::MainWindow& ref, std::unique_ptr<streams::IOSocketStream>& clientStream, const std::wstring& filePath, std::vector<db::fileDataRepresentation>& fileNames, bool& isCancel);

LRESULT downloadFile(UI::MainWindow& ref, std::unique_ptr<streams::IOSocketStream>& clientStream, const std::vector<db::fileDataRepresentation>& fileNames, bool& isCancel, LRESULT searchId);

void reconnect(UI::MainWindow& ref, std::unique_ptr<streams::IOSocketStream>& clientStream, std::string&& currentPath, const std::wstring& login, const std::wstring& password, std::vector<db::fileDataRepresentation>& fileNames, bool& isCancel);

void nextFolder(UI::MainWindow& ref, std::unique_ptr<streams::IOSocketStream>& clientStream, const std::wstring& folderName, std::vector<db::fileDataRepresentation>& fileNames, bool& isCancel);

void prevFolder(UI::MainWindow& ref, std::unique_ptr<streams::IOSocketStream>& clientStream, std::filesystem::path& currentPath, std::vector<db::fileDataRepresentation>& fileNames, bool& isCancel);

void createFolder(UI::MainWindow& ref, std::unique_ptr<streams::IOSocketStream>& clientStream, std::vector<db::fileDataRepresentation>& fileNames, bool& isCancel);

void exitFromApplication(UI::MainWindow& ref, std::unique_ptr<streams::IOSocketStream>& clientStream);

void authorization(UI::MainWindow& ref, std::unique_ptr<streams::IOSocketStream>& clientStream, std::wstring& login, std::wstring& password, bool& isCancel);

void registration(UI::MainWindow& ref, std::unique_ptr<streams::IOSocketStream>& clientStream, std::wstring& login, std::wstring& password, bool& isCancel);

void firstConnect(UI::MainWindow& ref, std::unique_ptr<streams::IOSocketStream>& clientStream, bool& isCancel);