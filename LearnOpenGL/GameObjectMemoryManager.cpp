#include "GameObjectMemoryManager.h"


/* static member must be defiend in the .cpp file */
unordered_map<void *, GameObjectMemoryManager::AddressProperties> GameObjectMemoryManager::manager;

GameObjectMemoryManager::GameObjectMemoryManager() {}

GameObjectMemoryManager::~GameObjectMemoryManager() {}

void GameObjectMemoryManager::add(void * ptr) {
	GameObjectMemoryManager::add(ptr, true);
}

void GameObjectMemoryManager::add(void * ptr, bool ownedByClass) {
	auto it = manager.find(ptr);

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

int GameObjectMemoryManager::decrement(void * ptr) {
	auto it = manager.find(ptr);

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

void GameObjectMemoryManager::deleteSharedPointers() {
	for (auto it = manager.begin(); it != manager.end();) {
		if (it->second.ownedByClass == false) {
			delete it->first;
			manager.erase(it++);
		}
		else {
			it++;
		}
	}
}
