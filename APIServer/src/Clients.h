#pragma once

#include <thread>
#include <mutex>
#include <unordered_map>
#include <vector>

#include <WinSock2.h>

struct clientData
{
	std::string ip;
	SOCKET clientSocket;

	clientData(std::string&& ip, SOCKET clientSocket) noexcept;
};

class Clients
{
private:
	std::unordered_map<std::thread::id, clientData> data;
	std::mutex readWriteLock;

public:
	Clients() = default;

	void insert(const std::thread::id& id, std::string&& ip, SOCKET clientSocket) noexcept;

	void erase(const std::thread::id& id) noexcept;

	std::vector<std::pair<std::thread::id, clientData>> getClients() noexcept;

	~Clients() = default;
};