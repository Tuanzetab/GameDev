#include "EntityManager.h"
#include <algorithm>

EntityManager::EntityManager() {}

std::shared_ptr<Entity> EntityManager::addEntity(const std::string& tag) {
    auto e = std::shared_ptr<Entity>(new Entity(m_totalEntities++, tag));
    m_toAdd.push_back(e);
    std::cout << "Vec ToAdd: " << m_toAdd.size() << "\n";
    std::cout << "Vector: " << m_entities.size() << "\n";
    std::cout << "map: " << m_entitiesMap.size() << "\n";

    return e;
}

void EntityManager::update() {
    for (auto& e : m_toAdd)
    {
        m_entities.push_back(e);
        m_entitiesMap[e->tag()].push_back(e);
    }

    removeDeadEntities(m_entities);

    for (auto& pair : m_entitiesMap) {
        removeDeadEntities(pair.second);
    }
    m_toAdd.clear();
}

EntityVec& EntityManager::getEntities() {
    return m_entities;
}

EntityVec& EntityManager::getEntities(const std::string& tag) {
    return m_entitiesMap[tag];
}

void EntityManager::removeDeadEntities(EntityVec& vec) {
    auto it = vec.begin();
    while (it != vec.end()) {
        if (!(*it)->is_active()) {
            it = vec.erase(it);
        }
        else {
            ++it;
        }
    }
}
