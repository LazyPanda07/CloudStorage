#pragma once

#include <string>

#include "BaseIOSocketStream.h"
#include "CloudDataBase.h"

//login initialize in this function
void authorization(streams::IOSocketStream& clientStream, const db::CloudDataBase& db, std::string& login);

//login initialize in this function
void registration(streams::IOSocketStream& clientStream, const db::CloudDataBase& db, std::string& login);

void showAllFilesInFolder(streams::IOSocketStream& clientStream, const db::CloudDataBase& db, const std::string& login, const std::filesystem::path& currentPath);

void uploadFileData(streams::IOSocketStream& clientStream, const db::CloudDataBase& db, const std::string& login, const std::filesystem::path& currentPath);

void removeFileData(streams::IOSocketStream& clientStream, const db::CloudDataBase& db, const std::string& login, const std::filesystem::path& currentPath);

void nextFolder(streams::IOSocketStream& clientStream, std::filesystem::path& currentPath);

void prevFolder(streams::IOSocketStream& clientStream, std::filesystem::path& currentPath);

void setPath(streams::IOSocketStream& clientStream, std::filesystem::path& currentPath);

void createFolder(streams::IOSocketStream& clientStream, const db::CloudDataBase& db, const std::string& login, const std::filesystem::path& currentPath);