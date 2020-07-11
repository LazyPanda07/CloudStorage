#pragma once

#include "../MainWindow.h"

#include <string>
#include <vector>

void initCloudStorageScreen(UI::MainWindow& ref);

void initRegistrationScreen(UI::MainWindow& ref);

void initAuthorizationScreen(UI::MainWindow& ref);

void updateNameColumn(UI::MainWindow& ref, const std::vector<std::wstring>& data);