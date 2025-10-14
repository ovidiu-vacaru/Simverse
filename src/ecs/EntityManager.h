#pragma once

#include "Types.h"
#include <queue>
#include <array>
#include <cassert>


template <std::size_t MaxEntities>
class EntityManager {
public:

    EntityManager(){
        for (Entity entity = 0; entity < MaxEntities; ++entity){
            m_available_entities.push(entity);
        }
    }

    Entity CreateEntity() {
        assert(m_living_entity_count < MaxEntities && "Maximum number of entities execeeded.");

        Entity id = m_available_entities.front();
        m_available_entities.pop();
        ++m_living_entity_count;
        return id;
    }

    void DestroyEntity(Entity entity){
        assert(entity < MaxEntities && "Entity out of range");
        m_signatures[entity].reset();
        m_available_entities.push(entity);
      
        --m_living_entity_count;
    }

    void SetSignature(Entity entity, Signature signature){
        assert(entity < MaxEntities && "Entity out of range.");
        m_signatures[entity] = signature;
    }

    Signature GetSignature(Entity entity) {
        assert(entity < MaxEntities && "Entity out of range.");
        return m_signatures[entity];
    }

private:
    std::queue<Entity> m_available_entities;
    std::array<Signature, MaxEntities> m_signatures;
    std::uint32_t m_living_entity_count = 0;
};