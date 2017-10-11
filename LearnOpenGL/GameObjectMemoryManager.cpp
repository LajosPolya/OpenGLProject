#include "GameObjectMemoryManager.h"

/* static member must be defiend in the .cpp file */
std::map<void *, int> GameObjectMemoryManager::manager;

GameObjectMemoryManager::GameObjectMemoryManager()
{
}


GameObjectMemoryManager::~GameObjectMemoryManager()
{
}

void GameObjectMemoryManager::add(void * ptr)
{
	std::map<void *, int>::iterator it = manager.find(ptr);

	if (it == manager.end()) {
		manager[ptr] = 1;
	}
	else {
		manager[ptr] = ++it->second;
	}
}
