#include "Entity.h"
#include <string>
#include <iostream>

Entity::Entity(const size_t id, const std::string& tag)
    :m_id(id), m_tag(tag)
{
}

bool Entity::isActive() const
{
    return m_active;
}

const size_t Entity::id() const
{
    return m_id;
}

const std::string & Entity::tag() const
{
    return m_tag;
}

//set active to false. this might be not the function used for.
void Entity::destroy()
{
    m_active = false;
    //std::cerr << "Destroy() executed \n";
}
