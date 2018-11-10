#pragma once

#include <unordered_map>

using namespace std;

class GameObjectMemoryManager
{
public:
	GameObjectMemoryManager();
	~GameObjectMemoryManager();

	static void add(void * ptr);
	static void add(void * ptr, bool ownedByClass);
	static int decrement(void * ptr);
	static void deleteSharedPointers();

private:
	struct AddressProperties {
		int count;
		bool ownedByClass;
	};
	static unordered_map<void *, AddressProperties> manager;
};

