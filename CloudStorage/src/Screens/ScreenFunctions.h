#pragma once

#include <string>
#include <vector>

#include "../MainWindow.h"
#include "fileData.h"

void initCloudStorageScreen(UI::MainWindow& ref);

void initRegistrationScreen(UI::MainWindow& ref);

void initAuthorizationScreen(UI::MainWindow& ref);

bool removeFileDialog(UI::MainWindow& ref, const std::wstring& fileName);

void updateColumns(UI::MainWindow& ref, const std::vector<db::wFileData>& data);