#pragma once

#include <memory>

#include "BaseIOSocketStream.h"
#include "fileData.h"
#include "MainWindow.h"

void removeFile(UI::MainWindow& ref, std::unique_ptr<streams::IOSocketStream>& clientStream, std::vector<db::fileDataRepresentation>& fileNames, bool& isCancel);

void removeFile(UI::MainWindow& ref, std::unique_ptr<streams::IOSocketStream>& clientStream, const std::wstring& fileName, bool& isCancel, bool showError = true);

void asyncRemoveFile(UI::MainWindow& ref, std::unique_ptr<streams::IOSocketStream>& clientStream, std::vector<db::fileDataRepresentation>& fileNames, bool& isCancel);