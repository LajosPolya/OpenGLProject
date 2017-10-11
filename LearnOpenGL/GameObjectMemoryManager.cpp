#include "GameObjectMemoryManager.h"


/* static member must be defiend in the .cpp file */
std::map<void *, GameObjectMemoryManager::AddressProperties> GameObjectMemoryManager::manager;

GameObjectMemoryManager::GameObjectMemoryManager() {}

GameObjectMemoryManager::~GameObjectMemoryManager() {
	// TODO: This never gets called
	for (auto const prop : manager) {
		delete prop.first;
	}

}

void GameObjectMemoryManager::add(void * ptr)
{
	GameObjectMemoryManager::add(ptr, true);
}

void GameObjectMemoryManager::add(void * ptr, bool ownedByClass)
{
	std::map<void *, GameObjectMemoryManager::AddressProperties>::iterator it = manager.find(ptr);

	if (it == manager.end()) {
		GameObjectMemoryManager::AddressProperties prop;
		prop.count = 1;
		prop.ownedByClass = ownedByClass;
		manager[ptr] = prop;
	}
	else {
		++it->second.count;
		manager[ptr] = it->second;
	}
}

int GameObjectMemoryManager::decrement(void * ptr)
{
	std::map<void *, GameObjectMemoryManager::AddressProperties>::iterator it = manager.find(ptr);

	if (it == manager.end()) {
		return -1;
	}
	else {
		if (it->second.ownedByClass == false) {
			return 1;
		}
		--it->second.count;
		manager[ptr] = it->second;
		return it->second.count;
	}
}
