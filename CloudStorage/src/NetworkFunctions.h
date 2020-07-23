#pragma once

#include <vector>
#include <tuple>
#include <string>

#include "IOSocketStream.h"
#include "fileData.h"
#include "MainWindow.h"

void getFiles(UI::MainWindow& ref, streams::IOSocketStream<char>& clientStream, std::vector<db::fileDataRepresentation>& fileNames, bool showError);

void uploadFile(UI::MainWindow& ref, streams::IOSocketStream<char>& clientStream, const std::wstring& filePath, const std::wstring& login, bool& isCancel);

int downloadFile(UI::MainWindow& ref, streams::IOSocketStream<char>& clientStream, const std::vector<db::fileDataRepresentation>& fileNames, const std::wstring& login, bool& isCancel, int searchId);

void removeFile(UI::MainWindow& ref, streams::IOSocketStream<char>& clientStream, const std::vector<db::fileDataRepresentation>& fileNames, const std::wstring& login);

void removeFile(UI::MainWindow& ref, streams::IOSocketStream<char>& clientStream, const std::wstring& fileName, const std::wstring& login);

void reconnect(UI::MainWindow& ref, streams::IOSocketStream<char>& clientStream);

void createFolder(UI::MainWindow& ref, streams::IOSocketStream<char>& clientStream, const std::filesystem::path& currentPath, const std::wstring& folderName);

void exitFromApplication(UI::MainWindow& ref, streams::IOSocketStream<char>& clientSream);

std::tuple<std::wstring, std::wstring> authorization(UI::MainWindow& ref, streams::IOSocketStream<char>& clientStream, std::wstring&& inLogin = L"", std::wstring&& inPassword = L"");

std::tuple<std::wstring, std::wstring> registration(UI::MainWindow& ref, streams::IOSocketStream<char>& clientStream);