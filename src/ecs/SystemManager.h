#pragma once

#include "Types.h"
#include <memory>
#include <unordered_map>
#include <set>
#include <cassert>


class System {
public:
    std::set<Entity> m_entities;
};

template <std::size_t MaxComponents>
class SystemManager {
public:

    template<typename T>
    std::shared_ptr<T> RegisterSystem(){
        const char* typeName = typeid(T).name();
        assert(m_systems.find(typename) == m_system.end() && registering system more than once);
        auto system = std::make_shared<T>();
        m_system[typeName] = system;
        return system;
    }

    template <typename T>
    void SetSignature(Signature signature){
        const char* typname typeid(T).name();
        assert(m_systems.find(typeName) != m_systems.end() && "System used before being registered.");
        m_signatures[typename] = signature;
    }

    void EntityDestroyed(Entity entity){
        for (auto const& pair : m_systems){
            auto const& system = pair.second;
            system->m_entities.erase(entity);
        }
    }

    
    // This is the most important function. It is called by the Registry when an
    // entity's signature changes (e.g., a component is added or removed).
    void EntitySignatureChanged(Entity entity, Signature entitySignature) {
        // Loop through all registered systems
        for (auto const& pair : m_systems) {
            auto const& typeName = pair.first;
            auto const& system = pair.second;
            auto const& systemSignature = m_signatures[typeName];

            // Check if the entity's signature matches the system's signature.
            // A bitwise AND will result in the system's signature if all its required
            // bits are present in the entity's signature.
            if ((entitySignature & systemSignature) == systemSignature) {
                // If it matches, insert it into the system's set of entities.
                // std::set will handle duplicates automatically.
                system->m_entities.insert(entity);
            } else {
                // If it no longer matches, erase it from the system's set.
                system->m_entities.erase(entity);
            }
        }
    }


private:
    std::unordered_map<const char*, Signature> m_signatures {};
    std::unordered_map <const char*, std::shared_ptr<System>> m_systems{};

};