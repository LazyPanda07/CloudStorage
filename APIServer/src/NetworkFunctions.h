#pragma once

#include <string>
#include <tuple>
#include <unordered_map>
#include <memory>

#include "BaseIOSocketStream.h"

bool checkHTTP(const std::string& request);

void showAllFilesInFolder(streams::IOSocketStream& clientStream, std::unique_ptr<streams::IOSocketStream>& dataBaseStream);

void uploadFile(streams::IOSocketStream& clientStream, std::unique_ptr<streams::IOSocketStream>& filesStream, std::unique_ptr<streams::IOSocketStream>& dataBaseStream, const std::string& data, const std::unordered_map<std::string, std::string>& headers);

void downloadFile(streams::IOSocketStream& clientStream, std::unique_ptr<streams::IOSocketStream>& filesStream, const std::unordered_map<std::string, std::string>& headers);

void removeFile(streams::IOSocketStream& clientStream, std::unique_ptr<streams::IOSocketStream>& filesStream, std::unique_ptr<streams::IOSocketStream>& dataBaseStream, const std::unordered_map<std::string, std::string>& headers);

void cancelUploadFile(streams::IOSocketStream& clientStream, std::unique_ptr<streams::IOSocketStream>& filesStream, const std::unordered_map<std::string, std::string>& headers);

void setLogin(streams::IOSocketStream& clientStream, std::unique_ptr<streams::IOSocketStream>& filesStream, std::unique_ptr<streams::IOSocketStream>& dataBaseStream, const std::string& data);

void nextFolder(streams::IOSocketStream& clientStream, std::unique_ptr<streams::IOSocketStream>& filesStream, std::unique_ptr<streams::IOSocketStream>& dataBaseStream, const std::string& data);

void prevFolder(streams::IOSocketStream& clientStream, std::unique_ptr<streams::IOSocketStream>& filesStream, std::unique_ptr<streams::IOSocketStream>& dataBaseStream);

void setPath(streams::IOSocketStream& clientStream, std::unique_ptr<streams::IOSocketStream>& filesStream, std::unique_ptr<streams::IOSocketStream>& dataBaseStream, const std::string& data);

void createFolder(streams::IOSocketStream& clientStream, std::unique_ptr<streams::IOSocketStream>& filesStream, std::unique_ptr<streams::IOSocketStream>& dataBaseStream, const std::string& data);

void authorization(streams::IOSocketStream& clientStream, std::unique_ptr<streams::IOSocketStream>& filesStream, std::unique_ptr<streams::IOSocketStream>& dataBaseStream, const std::string& data);

void registration(streams::IOSocketStream& clientStream, std::unique_ptr<streams::IOSocketStream>& filesStream, std::unique_ptr<streams::IOSocketStream>& dataBaseStream, const std::string& data);

std::tuple<std::string, std::string> userDataParse(const std::string& data);