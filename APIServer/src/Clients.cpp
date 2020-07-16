#include "pch.h"

#include "Clients.h"

using namespace std;

void Clients::insert(const thread::id& id, string&& ip) noexcept
{
	readWriteLock.lock();

	data.insert(make_pair(id, move(ip)));

	readWriteLock.unlock();
}

void Clients::erase(const thread::id& id) noexcept
{
	readWriteLock.lock();

	if (data.find(id) != end(data))
	{
		data.erase(id);
	}

	readWriteLock.unlock();
}

vector<pair<thread::id, string>> Clients::getClients() noexcept
{
	vector<pair<thread::id, string>> result;

	readWriteLock.lock();

	result.reserve(data.size());

	for (const auto& i : data)
	{
		result.emplace_back(i.first, i.second);
	}

	readWriteLock.unlock();

	return result;
}