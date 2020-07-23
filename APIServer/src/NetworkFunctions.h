#pragma once

#include <string>
#include <tuple>
#include <map>

#include "IOSocketStream.h"

bool checkHTTP(const std::string& request);

void showAllFilesInFolder(streams::IOSocketStream<char>& clientStream, streams::IOSocketStream<char>& dataBaseStream, const std::string& directory);

void uploadFile(streams::IOSocketStream<char>& clientStream, streams::IOSocketStream<char>& filesStream, streams::IOSocketStream<char>& dataBaseStream, const std::string& data, const std::map<std::string, std::string>& headers);

void downloadFile(streams::IOSocketStream<char>& clientStream, streams::IOSocketStream<char>& filesStream, const std::map<std::string, std::string>& headers);

void removeFile(streams::IOSocketStream<char>& clientStream, streams::IOSocketStream<char>& filesStream, streams::IOSocketStream<char>& dataBaseStream, const std::map<std::string, std::string>& headers);

void cancelUploadFile(streams::IOSocketStream<char>& clientStream, streams::IOSocketStream<char>& filesStream, const std::map<std::string, std::string>& headers);

void authorization(streams::IOSocketStream<char>& clientStream, streams::IOSocketStream<char>& dataBaseStream, const std::string& data);

void registration(streams::IOSocketStream<char>& clientStream, streams::IOSocketStream<char>& dataBaseStream, const std::string& data);

std::tuple<std::string, std::string> userDataParse(const std::string& data);