#pragma once

#include "Types.h"
#include <array>
#include <unordered_map>
#include <cassert>

// --- The "Shelf Label" ---
// This is a pure virtual interface. Its purpose is to allow the ComponentManager
// to hold a list of pointers to different types of ComponentArrays (e.g.,
// a ComponentArray<Position> and a ComponentArray<Velocity>) in a single container.
class IComponentArray {
public:
    // Virtual destructor is crucial for a polymorphic base class.
    virtual ~IComponentArray() = default;

    // A virtual method that must be implemented by derived classes.
    // This is called when an entity is destroyed to ensure its component data is cleaned up.
    virtual void EntityDestroyed(Entity entity) = 0;
};

// --- The "Shelf" ---
// This is the actual storage for a single type of component.
// It is a template class, so we will have a separate ComponentArray for each
// component type (e.g., ComponentArray<Position>, ComponentArray<Velocity>).
template <typename T, std::size_t MaxEntities>
class ComponentArray : public IComponentArray {
public:
    // --- Public Methods ---

    // Attaches a component to an entity.
    void InsertData(Entity entity, T component) {
        assert(m_entity_to_index_map.find(entity) == m_entity_to_index_map.end() && "Component added to same entity more than once.");

        // Put new entry at end of packed array
        size_t newIndex = m_size;
        m_entity_to_index_map[entity] = newIndex;
        m_index_to_entity_map[newIndex] = entity;
        m_component_array[newIndex] = component;
        ++m_size;
    }

    // Removes an entity's component.
    // This is a high-performance "swap-and-pop" operation.
    void RemoveData(Entity entity) {
        assert(m_entity_to_index_map.find(entity) != m_entity_to_index_map.end() && "Removing non-existent component.");

        // To keep the array tightly packed, we will move the last element
        // into the slot of the element being removed.
        size_t indexOfRemovedEntity = m_entity_to_index_map[entity];
        size_t indexOfLastElement = m_size - 1;

        // Copy data from last element into the deleted element's slot
        m_component_array[indexOfRemovedEntity] = m_component_array[indexOfLastElement];

        // Update the maps to point to the new spot
        Entity entityOfLastElement = m_index_to_entity_map[indexOfLastElement];
        m_entity_to_index_map[entityOfLastElement] = indexOfRemovedEntity;
        m_index_to_entity_map[indexOfRemovedEntity] = entityOfLastElement;

        m_entity_to_index_map.erase(entity);
        --m_size;
    }

    // Retrieves an entity's component.
    T& GetData(Entity entity) {
        assert(m_entity_to_index_map.find(entity) != m_entity_to_index_map.end() && "Retrieving non-existent component.");
        return m_component_array[m_entity_to_index_map[entity]];
    }

    // Implementation of the virtual method from the interface.
    void EntityDestroyed(Entity entity) override {
        // If the entity has a component in this array, remove it.
        if (m_entity_to_index_map.find(entity) != m_entity_to_index_map.end()) {
            RemoveData(entity);
        }
    }

private:
    // --- Private Member Variables ---

    // The packed array of components. This is the key to our performance.
    // The data for all components of this type is stored contiguously in memory.
    std::array<T, MaxEntities> m_component_array;

    // Maps from an entity ID to an array index.
    std::unordered_map<Entity, size_t> m_entity_to_index_map;

    // Maps from an array index to an entity ID.
    std::unordered_map<size_t, Entity> m_index_to_entity_map;

    // Total number of valid components in the array.
    size_t m_size = 0;
};
