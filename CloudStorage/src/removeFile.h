#pragma once

#include "IOSocketStream.h"
#include "fileData.h"
#include "MainWindow.h"

void removeFile(UI::MainWindow& ref, streams::IOSocketStream<char>& clientStream, std::vector<db::fileDataRepresentation>& fileNames, bool& isCancel);

void asyncRemoveFile(UI::MainWindow& ref, streams::IOSocketStream<char>& clientStream, std::vector<db::fileDataRepresentation>& fileNames, bool& isCancel);