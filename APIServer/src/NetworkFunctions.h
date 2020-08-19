#pragma once

#include <string>
#include <tuple>
#include <unordered_map>
#include <memory>

#include "IOSocketStream.h"

bool checkHTTP(const std::string& request);

void showAllFilesInFolder(streams::IOSocketStream<char>& clientStream, std::unique_ptr<streams::IOSocketStream<char>>& dataBaseStream);

void uploadFile(streams::IOSocketStream<char>& clientStream, std::unique_ptr<streams::IOSocketStream<char>>& filesStream, std::unique_ptr<streams::IOSocketStream<char>>& dataBaseStream, const std::string& data, const std::unordered_map<std::string, std::string>& headers);

void downloadFile(streams::IOSocketStream<char>& clientStream, std::unique_ptr<streams::IOSocketStream<char>>& filesStream, const std::unordered_map<std::string, std::string>& headers);

void removeFile(streams::IOSocketStream<char>& clientStream, std::unique_ptr<streams::IOSocketStream<char>>& filesStream, std::unique_ptr<streams::IOSocketStream<char>>& dataBaseStream, const std::unordered_map<std::string, std::string>& headers);

void cancelUploadFile(streams::IOSocketStream<char>& clientStream, std::unique_ptr<streams::IOSocketStream<char>>& filesStream, const std::unordered_map<std::string, std::string>& headers);

void setLogin(streams::IOSocketStream<char>& clientStream, std::unique_ptr<streams::IOSocketStream<char>>& filesStream, std::unique_ptr<streams::IOSocketStream<char>>& dataBaseStream, const std::string& data);

void nextFolder(streams::IOSocketStream<char>& clientStream, std::unique_ptr<streams::IOSocketStream<char>>& filesStream, std::unique_ptr<streams::IOSocketStream<char>>& dataBaseStream, const std::string& data);

void prevFolder(streams::IOSocketStream<char>& clientStream, std::unique_ptr<streams::IOSocketStream<char>>& filesStream, std::unique_ptr<streams::IOSocketStream<char>>& dataBaseStream);

void setPath(streams::IOSocketStream<char>& clientStream, std::unique_ptr<streams::IOSocketStream<char>>& filesStream, std::unique_ptr<streams::IOSocketStream<char>>& dataBaseStream, const std::string& data);

void createFolder(streams::IOSocketStream<char>& clientStream, std::unique_ptr<streams::IOSocketStream<char>>& filesStream, std::unique_ptr<streams::IOSocketStream<char>>& dataBaseStream, const std::string& data);

void authorization(streams::IOSocketStream<char>& clientStream, std::unique_ptr<streams::IOSocketStream<char>>& filesStream, std::unique_ptr<streams::IOSocketStream<char>>& dataBaseStream, const std::string& data);

void registration(streams::IOSocketStream<char>& clientStream, std::unique_ptr<streams::IOSocketStream<char>>& filesStream, std::unique_ptr<streams::IOSocketStream<char>>& dataBaseStream, const std::string& data);

std::tuple<std::string, std::string> userDataParse(const std::string& data);