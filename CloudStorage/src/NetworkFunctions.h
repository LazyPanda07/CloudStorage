#pragma once

#include <vector>
#include <string>

#include "IOSocketStream.h"
#include "fileData.h"
#include "MainWindow.h"

void getFiles(UI::MainWindow& ref, streams::IOSocketStream<char>& clientStream, std::vector<db::fileDataRepresentation>& fileNames, bool showError);

void uploadFile(UI::MainWindow& ref, streams::IOSocketStream<char>& clientStream, const std::vector<std::wstring>& files, const std::wstring& login);

void uploadFile(UI::MainWindow& ref, streams::IOSocketStream<char>& clientStream, const std::wstring& filePath, const std::wstring& login);

void downloadFile(UI::MainWindow& ref, streams::IOSocketStream<char>& clientStream, const std::vector<db::fileDataRepresentation>& fileNames, const std::wstring& login);

void downloadFile(UI::MainWindow& ref, streams::IOSocketStream<char>& clientStream, const std::wstring& fileName, const std::wstring& login);

void removeFile(UI::MainWindow& ref, streams::IOSocketStream<char>& clientStream, const std::vector<db::fileDataRepresentation>& fileNames, const std::wstring& login);

void removeFile(UI::MainWindow& ref, streams::IOSocketStream<char>& clientStream, const std::wstring& fileName, const std::wstring& login);

std::wstring authorization(UI::MainWindow& ref, streams::IOSocketStream<char>& clientStream);

std::wstring registration(UI::MainWindow& ref, streams::IOSocketStream<char>& clientStream);