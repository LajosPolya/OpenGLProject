#pragma once

#include <map>

class GameObjectMemoryManager
{
public:
	GameObjectMemoryManager();
	~GameObjectMemoryManager();

	static void add(void * ptr);
	static int decrement(void * ptr);

private:
	static std::map<void *, int> manager;
};

