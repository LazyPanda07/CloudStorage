#pragma once

#include <thread>
#include <mutex>
#include <unordered_map>
#include <vector>

class Clients
{
private:
	std::unordered_map<std::thread::id, std::string> data;
	std::mutex readWriteLock;

public:
	Clients() = default;

	void insert(const std::thread::id& id, std::string&& ip) noexcept;

	void erase(const std::thread::id& id) noexcept;

	std::vector<std::pair<std::thread::id, std::string>> getClients() noexcept;

	~Clients() = default;
};