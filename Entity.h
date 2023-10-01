#pragma once
#include <string>
#include <memory>
#include "Components.h"

class Entity
{
    friend class EntityManager;

    std::string m_tag = "default";
    bool m_active = true;
    size_t m_id = 0;

    Entity(const size_t m_id, const std::string& tag);

public:
    std::shared_ptr<CTransform> cTransform;
    std::shared_ptr<CShape> cShape;
    std::shared_ptr<CCollision> cCollision;
    std::shared_ptr<CInput> cInput;
    std::shared_ptr<CScore> cScore;
    std::shared_ptr<CLifespan> cLifespan;

    bool isActive() const;
    const std::string & tag() const;
    const size_t id() const;
    void destroy();
};
