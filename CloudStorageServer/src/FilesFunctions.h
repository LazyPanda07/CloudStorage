#pragma once

#include <filesystem>

#include "IOSocketStream.h"

void uploadFile(streams::IOSocketStream<char>& clientStream, const std::filesystem::path& currentPath);

void downloadFile(streams::IOSocketStream<char>& clientStream, const std::filesystem::path& currentPath);

void removeFile(streams::IOSocketStream<char>& clientStream, const std::filesystem::path& currentPath);

void cancelOperation(streams::IOSocketStream<char>& clientStream, const std::filesystem::path& currentPath);

void nextFolder(streams::IOSocketStream<char>& clientStream, std::filesystem::path& currentPath);

void prevFolder(streams::IOSocketStream<char>& clientStream, std::filesystem::path& currentPath);

void setPath(streams::IOSocketStream<char>& clientStream, std::filesystem::path& currentPath, const std::string& login);