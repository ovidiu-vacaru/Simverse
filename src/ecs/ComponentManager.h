#pragma once

#include "Types.h"
#include "Component.h"
#include <memory>
#include <vector>
#include <cassert>

// This helper class is a private implementation detail of the ComponentManager.
class ComponentIDGenerator {
private:
    static inline ComponentID s_next_component_id = 0;
public:
    template<typename T>
    static ComponentID GetTypeID(){
        static ComponentID s_type_id = s_next_component_id++;
        return s_type_id;
    }
};


template <std::size_t MaxEntites, std::size_t MaxComponents>
class ComponentManager{
public:

    template <typename T>
    void RegisterComponent() {
        ComponentID id = ComponentIDGenerator::GetTypeID<T>();

        if(m_components_arrays.size() <= id) {
            m_components_arrays.resize(id + 1);
        }

        std::shared_ptr<IComponentArray> component = std::make_shared<ComponentArray<T, MaxEntites>>();
        
        m_components_arrays[id] = component;
    }

    template <typename T>
    void AddComponent(Entity entity, T component){
        GetComponentArray<T>()->InsertData(entity, component);
    }

    template <typename T>
    void RemoveComponent(Entity entity) {
        GetComponentArray<T>()->RemoveData(entity);
    }

    template <typename T>
    T& GetComponent(Entity entity) {
        return GetComponentArray<T>()->GetData(entity);
    }

    // This function is called by the Registry whenever an entity is destroyed.
    // Its job is to notify every single ComponentArray so they can remove any
    // data associated with that entity, preventing orphaned components.
    void EntityDestroyed(Entity entity) {
        // We iterate through our vector of component storage arrays.
        // Each element is a std::shared_ptr<IComponentArray>.
        for (auto const& componentArray : m_components_arrays) {
            // It's possible to have gaps in our vector if component IDs are not
            // registered sequentially, which would leave nullptrs in the vector.
            // This is a crucial safety check to prevent a crash from trying to
            // access a member of a null pointer.
            if (componentArray != nullptr) {
                // This is a polymorphic call.
                // We are calling the virtual EntityDestroyed() method on the base class
                // pointer (IComponentArray). C++'s v-table mechanism ensures that the
                // correct, specific implementation (e.g., in ComponentArray<Position>)
                // is executed for each different component type.
                componentArray->EntityDestroyed(entity);
            }
        }
    }


private:

    template <typename T>
    std::shared_ptr<ComponentArray<T, MaxEntites>> GetComponentArray(){
        ComponentID id = ComponentIDGenerator::GetTypeID<T>();
        
        assert(id < m_components_arrays.size() && "Component not registered before use.");

        std::shared_ptr<IComponentArray> genericPtr = m_components_arrays[id];
        
        return std::static_pointer_cast<ComponentArray<T, MaxEntites>>(genericPtr);
    }

    std::vector<std::shared_ptr<IComponentArray>> m_components_arrays;
};

