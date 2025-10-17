#pragma once

#include "Types.h"
#include <memory> // For std::unique_ptr
#include <cassert>

// --- IMPLEMENTATION INCLUDES ---
// Because Registry is a template class, its full implementation must be visible
// where it's used. Therefore, we must include the full definitions of the
// managers it owns and orchestrates.
#include "EntityManager.h"
#include "ComponentManager.h"
// #include "SystemManager.h" // We will add this back later


// The Registry is the main public-facing class for the entire ECS.
// It is a Facade that delegates all work to its internal manager subsystems.
// The game logic should only ever need to interact with this single class.
template <std::size_t MaxEntities, std::size_t MaxComponents>
class Registry {
public:
    // --- Constructor & Destructor ---
    // In the constructor, we create our internal subsystems.
    Registry() {
        // Create the managers that the Registry will orchestrate.
        m_entity_manager = std::make_unique<EntityManager<MaxEntities>>();
        m_component_manager = std::make_unique<ComponentManager<MaxEntities, MaxComponents>>();
        // m_system_manager = std::make_unique<SystemManager<MaxComponents>>(); // We will add this back later
    }

    // Default destructor is perfect because std::unique_ptr automatically handles
    // deleting the managers, preventing memory leaks.
    ~Registry() = default;


    // --- Public API ---
    // These are the simple "buttons" on our engine's control panel.

    // === Entity Methods ===
    // These just forward the call to the EntityManager. Implemented in the header
    // to encourage compiler inlining for better performance.
    Entity CreateEntity() {
        return m_entity_manager->CreateEntity();
    }

    void DestroyEntity(Entity entity) {
        // This is a coordinating call. The Registry tells all subsystems
        // that the entity is gone so they can perform cleanup.
        m_entity_manager->DestroyEntity(entity);
        m_component_manager->EntityDestroyed(entity);
        // m_system_manager->EntityDestroyed(entity); // We will add this back later
    }


    // === Component Methods ===
    // These are the most important coordinating functions.

    // Used during the setup phase to tell the engine about a new component type.
    template <typename T>
    void RegisterComponent() {
        m_component_manager->template RegisterComponent<T>();
    }

    // Used during the game loop to attach a component to an entity.
    template <typename T>
    void AddComponent(Entity entity, T component) {
        // 1. Tell the ComponentManager to store the new data.
        m_component_manager->template AddComponent<T>(entity, component);

        // 2. Get the entity's signature from the EntityManager.
        auto signature = m_entity_manager->GetSignature(entity);

        // 3. Update the signature to reflect that it now has this component.
        signature.set(ComponentIDGenerator::GetTypeID<T>());
        m_entity_manager->SetSignature(entity, signature);

        // 4. Notify the SystemManager that this entity's "shape" has changed.
        // m_system_manager->EntitySignatureChanged(entity, signature); // We will add this back later
    }

    template <typename T>
    void RemoveComponent(Entity entity) {
        // 1. Tell the ComponentManager to remove the component's data.
        m_component_manager->template RemoveComponent<T>(entity);

        // 2. Get the signature.
        auto signature = m_entity_manager->GetSignature(entity);

        // 3. Update the signature to reflect that the component is gone.
        signature.reset(ComponentIDGenerator::GetTypeID<T>());
        m_entity_manager->SetSignature(entity, signature);

        // 4. Notify the SystemManager of the change.
        // m_system_manager->EntitySignatureChanged(entity, signature); // We will add this back later
    }

    // Used frequently in the game loop to get a component's data for reading or writing.
    template <typename T>
    T& GetComponent(Entity entity) {
        return m_component_manager->template GetComponent<T>(entity);
    }


    // === System Methods ===
    // We will implement these once the SystemManager is built.
    /*
    template <typename T>
    std::shared_ptr<T> RegisterSystem() {
        return m_system_manager->template RegisterSystem<T>();
    }

    template <typename T>
    void SetSystemSignature(Signature signature) {
        m_system_manager->template SetSignature<T>(signature);
    }
    */


private:
    // --- Private Members ---
    // The Registry HOLDS POINTERS to its subsystems. This is the architectural
    // best practice for high-level "owner" classes to prevent dependency problems
    // and keep compile times fast.
    std::unique_ptr<EntityManager<MaxEntities>> m_entity_manager;
    std::unique_ptr<ComponentManager<MaxEntities, MaxComponents>> m_component_manager;
    // std::unique_ptr<SystemManager<MaxComponents>> m_system_manager; // We will add this back later
};

