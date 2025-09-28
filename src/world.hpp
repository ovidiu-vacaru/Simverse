// world.hpp
#include <vector>
#include <unordered_map>
#include "types.hpp"
#include "components.hpp"
#include <any>
#include <typeindex>

class World {
public:

    // Create a new entity and return its ID
    Entity create_entity();

    // Attach a component for an entity
   template<typename T>
   void add_component(Entity entity, T component) {

    auto& component_store = get_component_store<T>();
    std::size_t new_index = component_store.data.size();
    component_store.data.push_back(component);
    component_store.map[entity] = new_index;
    }

    
    
    template<typename T>
    T& get_component(Entity entity){
        auto& component_store = get_component_store<T>();
        std::size_t index_of_component = component_store.map.at(entity);
        return component_store.data[index_of_component];
    }

private:
    Entity m_next_entity_id = 0;

    template<typename T>
    struct ComponentStore {
        std::vector<T> data;
        std::unordered_map<Entity, std::size_t> map;
    };


    std::unordered_map<std::type_index, std::any> m_component_stores;

    template<typename T>
    ComponentStore<T>& get_component_store(){
        auto type_id = std::type_index(typeid(T));

        if (m_component_stores.find(type_id) == m_component_stores.end()){
            // this component hasn't been found, so we are creating it.
            m_component_stores[type_id] = ComponentStore<T> {};
        }

        // return a reference to the store.

        return std::any_cast<ComponentStore<T>&>(m_component_stores.at(type_id));

    }

};