#pragma once

#include <vector>
#include <map>
#include <string>
#include "Entity.h"

typedef std::vector<std::shared_ptr<Entity>> EntityVec;
typedef std::map<std::string, EntityVec> EntityMap;

class EntityManager {
public:
    EntityManager();
    void update();
    std::shared_ptr<Entity> addEntity(const std::string& tag);
    EntityVec& getEntities();
    EntityVec& getEntities(const std::string& tag);



private:
    EntityVec m_entities;
    EntityVec m_toAdd;
    EntityMap m_entitiesMap;
    size_t m_totalEntities = 0;

    void removeDeadEntities(EntityVec& vec);
};
