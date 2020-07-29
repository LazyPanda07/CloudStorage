#include "pch.h"

#include "Clients.h"

using namespace std;

clientData::clientData(string&& ip, SOCKET clientSocket) noexcept :
	ip(move(ip)),
	clientSocket(clientSocket)
{

}

void Clients::insert(const thread::id& id, string&& ip, SOCKET clientSocket) noexcept
{
	readWriteLock.lock();

	data.insert(make_pair(id, clientData(move(ip), clientSocket)));

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

vector<pair<thread::id, clientData>> Clients::getClients() noexcept
{
	vector<pair<thread::id, clientData>> result;

	readWriteLock.lock();

	result.reserve(data.size());

	for (const auto& i : data)
	{
		result.emplace_back(i.first, i.second);
	}

	readWriteLock.unlock();

	return result;
}