#include "Types.h"
#include "Component.h"
#include <memory>
#include <unordered_map>
#include <cassert>


template <std::size_t MaxEntites, std::size_t MaxComponents>
class ComponentManager{
public:

    template <typename T>
    void RegisterComponent() {

    }


    template <typename T>
    ComponentID GetComponentID() {
        return getComponentID<T>();
    }

    template <typename T>
    void AddComponent(){

    }

    template <typename T>
    void RemoveComponent(){

    }

    template <typename T> 
    T& GetComponent(){

    }

    void EntityDestroyed(){

    }
private:

    template <typename T>
    std::shared_ptr<ComponentArray<T, MaxEntities>> GetComponentArray(){
        ComponentID componentID = getComponentID<T>();
        assert(m_component_arrays.find(componentID) != m_component_arrays.end() && "Component not registered before use")

        return std::static_pointer_cast
    }

    template <typename T>
    static ComponentID GetComponentID(){
        static ComponentID id = m_next_component_id++;
        assert(id < MaxComponents && "Exceeded the maximum number of registered components.");
        return id;
    }

    std::unordered_map<ComponentID, std::shared_ptr<IComponentArray>> m_component_arrays{};

    inline static ComponentID m_next_component_id{};

};

