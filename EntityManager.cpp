#include "EntityManager.h"
#include "Entity.h"
#include <string>
#include <memory>
#include <iostream>
#include <algorithm>


EntityManager::EntityManager()
{
    EntityVec player;
    EntityVec enemy;
    EntityVec bullet;

    m_entityMap.insert(std::pair<std::string, EntityVec>("Player", player));
    m_entityMap.insert(std::pair<std::string, EntityVec>("Enemy", enemy));
    m_entityMap.insert(std::pair<std::string, EntityVec>("SmallEnemy", enemy));
    m_entityMap.insert(std::pair<std::string, EntityVec>("Bullet", bullet));
}

void EntityManager::update()
{
    for (auto e : m_entitiesToAdd)
    {
        m_entities.push_back(e);
        m_entityMap[(e->tag())].push_back(e); //e->tag() should return a memory address, most likely error
    }

    m_entitiesToAdd.clear();

    removeDeadEntities(m_entities);

    //remove dead entities from each vector in the map
    //looping through key and value

    for(auto & [tag, entityVec] : m_entityMap)
    {
        removeDeadEntities(entityVec);
    }
}

void EntityManager::removeDeadEntities(EntityVec & vec)
{
    for(auto & e : vec)
    {
        //delete from the vector/list
        vec.erase(
        std::remove_if(vec.begin(), vec.end(), [](std::shared_ptr<Entity> const &  i) {
                       return !i->isActive();
                       }),
        vec.end()
        );
    }
}

std::shared_ptr<Entity> EntityManager::addEntity (const std::string & tag)
{
    auto entity = std::shared_ptr<Entity>(new Entity(m_totalEntities++, tag));
    m_entitiesToAdd.push_back(entity);
    return entity;
}

const EntityVec & EntityManager::getEntities()
{
    return m_entities;
}

const EntityVec & EntityManager::getEntities(const std::string & tag)
{
    return m_entityMap[tag];
}
