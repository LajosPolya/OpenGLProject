#pragma once

#include <map>

class GameObjectMemoryManager
{
public:
	GameObjectMemoryManager();
	~GameObjectMemoryManager();

	static void add(void * ptr);
	static void add(void * ptr, bool ownedByClass);
	static int decrement(void * ptr);

private:
	struct AddressProperties {
		int count;
		bool ownedByClass;
	};
	static std::map<void *, AddressProperties> manager;
};

