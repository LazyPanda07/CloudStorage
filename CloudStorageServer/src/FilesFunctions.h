#pragma once

#include <filesystem>

#include "BaseIOSocketStream.h"

void uploadFile(streams::IOSocketStream& clientStream, const std::filesystem::path& currentPath);

void downloadFile(streams::IOSocketStream& clientStream, const std::filesystem::path& currentPath);

void removeFile(streams::IOSocketStream& clientStream, const std::filesystem::path& currentPath);

void cancelOperation(streams::IOSocketStream& clientStream, const std::filesystem::path& currentPath);

void nextFolder(streams::IOSocketStream& clientStream, std::filesystem::path& currentPath);

void prevFolder(streams::IOSocketStream& clientStream, std::filesystem::path& currentPath);

void setPath(streams::IOSocketStream& clientStream, std::filesystem::path& currentPath, const std::string& login);

void createFolder(streams::IOSocketStream& clientStream, const std::filesystem::path& currentPath);