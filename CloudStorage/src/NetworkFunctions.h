#pragma once

#include <vector>
#include <string>

#include "IOSocketStream.h"
#include "MainWindow.h"

void getFiles(UI::MainWindow& ref, streams::IOSocketStream<char>& clientStream, std::vector<std::wstring>& filesNames, bool showError, const std::wstring& login);

void uploadFile(streams::IOSocketStream<char>& clientStream, const std::vector<std::wstring>& files, const std::wstring& login);

void uploadFile(streams::IOSocketStream<char>& clientStream, const std::wstring& filePath, const std::wstring& login);

void downloadFile(UI::MainWindow& ref, streams::IOSocketStream<char>& clientStream, const std::vector<std::wstring>& filesNames, const std::wstring& login);

void downloadFile(streams::IOSocketStream<char>& clientStream, const std::wstring& fileName, const std::wstring& login);

std::wstring authorization(UI::MainWindow& ref, streams::IOSocketStream<char>& clientStream);

std::wstring registration(UI::MainWindow& ref, streams::IOSocketStream<char>& clientStream);