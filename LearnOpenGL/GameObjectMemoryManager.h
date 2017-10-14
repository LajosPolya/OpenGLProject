#pragma once

#include <unordered_map>

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
	static std::unordered_map<void *, AddressProperties> manager;
};

