#pragma once

#include <string>

#include "IOSocketStream.h"
#include "CloudDataBase.h"

//login initialize in this function
void authorization(streams::IOSocketStream<char>& clientStream, const db::CloudDataBase& db, std::string& login);

//login initialize in this function
void registration(streams::IOSocketStream<char>& clientStream, const db::CloudDataBase& db, std::string& login);

void showAllFilesInDirectory(streams::IOSocketStream<char>& clientStream, const db::CloudDataBase& db, const std::string& login);

void uploadFileData(streams::IOSocketStream<char>& clientStream, const db::CloudDataBase& db, const std::string& login);

void removeFileData(streams::IOSocketStream<char>& clientStream, const db::CloudDataBase& db, const std::string& login);