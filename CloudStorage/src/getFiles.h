#pragma once

#include <memory>

#include "IOSocketStream.h"
#include "fileData.h"
#include "MainWindow.h"

void getFiles(UI::MainWindow& ref, std::unique_ptr<streams::IOSocketStream<char>>& clientStream, std::vector<db::fileDataRepresentation>& fileNames, bool showError, bool& isCancel, bool isDetach = true);