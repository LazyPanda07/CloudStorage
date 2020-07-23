#pragma once

#include <vector>
#include <tuple>
#include <string>

#include "IOSocketStream.h"
#include "fileData.h"
#include "MainWindow.h"

void getFiles(UI::MainWindow& ref, streams::IOSocketStream<char>& clientStream, std::vector<db::fileDataRepresentation>& fileNames, bool showError);

void uploadFile(UI::MainWindow& ref, streams::IOSocketStream<char>& clientStream, const std::wstring& filePath, bool& isCancel);

int downloadFile(UI::MainWindow& ref, streams::IOSocketStream<char>& clientStream, const std::vector<db::fileDataRepresentation>& fileNames, bool& isCancel, int searchId);

void removeFile(UI::MainWindow& ref, streams::IOSocketStream<char>& clientStream, const std::vector<db::fileDataRepresentation>& fileNames);

void removeFile(UI::MainWindow& ref, streams::IOSocketStream<char>& clientStream, const std::wstring& fileName);

void reconnect(UI::MainWindow& ref, streams::IOSocketStream<char>& clientStream);

void nextFolder(streams::IOSocketStream<char>& clientStream, const std::wstring& folderName);

void prevFolder(streams::IOSocketStream<char>& clientStream);

void setPath(streams::IOSocketStream<char>& clientStream, std::string&& path);

void createFolder(UI::MainWindow& ref, streams::IOSocketStream<char>& clientStream, const std::filesystem::path& currentPath);

void setLogin(streams::IOSocketStream<char>& clientStream, const std::wstring& login, const std::wstring password);

void exitFromApplication(UI::MainWindow& ref, streams::IOSocketStream<char>& clientSream);

std::tuple<std::wstring, std::wstring> authorization(UI::MainWindow& ref, streams::IOSocketStream<char>& clientStream);

std::tuple<std::wstring, std::wstring> registration(UI::MainWindow& ref, streams::IOSocketStream<char>& clientStream);