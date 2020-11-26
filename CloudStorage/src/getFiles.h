#pragma once

#include <memory>

#include "BaseIOSocketStream.h"
#include "fileData.h"
#include "MainWindow.h"

void getFiles(UI::MainWindow& ref, std::unique_ptr<streams::IOSocketStream>& clientStream, std::vector<db::fileDataRepresentation>& fileNames, bool showError, bool& isCancel, bool isDetach = true);