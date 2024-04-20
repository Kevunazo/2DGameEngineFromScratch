#ifndef ECS_H
#define ECS_H

#include <bitset>
#include <vector>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <typeindex>
#include <memory>
#include "Pool.h"
#include "../Logger/Logger.h"

// Registry forward decaration to be used by Entity
class Registry;

/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////// Signature ///////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
// We use a Signature in order to keep track of which components any Entity contains.
// A System will also have a Component signature of which components it cares about.
// With them, we will be able to quickly get all the entities that a particular
//   system cares about. For an entity to be considered, it should have in its signature
//   all of the components listed in the system signature.
/////////////////////////////////////////////////////////////////////////////////////
const unsigned int MAX_COMPONENTS = 32;
typedef std::bitset<MAX_COMPONENTS> Signature;


//////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////// Entity ///////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////

class Entity {
    private:
        int id;
 
    public:
        Registry* registry;

        Entity(int id): id(id) {};
        int GetId() const;

        // Operators
        Entity& operator =(const Entity& other) = default;
        bool operator ==(const Entity& other) const { return id == other.id; }
        bool operator !=(const Entity& other) const { return id != other.id; }
        bool operator >(const Entity& other) const { return id > other.id; }
        bool operator <(const Entity& other) const { return id < other.id; }

        std::string toString() const {
            return std::to_string(GetId());
        }

        // Component Management
        template <typename TComponent, typename ...TArgs> void AddComponent(TArgs&& ...args);
        template <typename TComponent> void RemoveComponent();
        template <typename TComponent> bool HasComponent() const;
        template <typename TComponent> TComponent& GetComponent() const;

};

//////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////// Component ////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////

struct IComponent {
    protected:
        static int nextId;
};

// We do this whole IComponent and Template Component so that we can have
// a unique ID per Component Type aka. Component<T>
// From ChatGPT:
//// The first time GetId() is called for a specific component type T, 
//// the nextId value will be assigned to the id variable and incremented. 
//// On subsequent calls to GetId() for the same component type T, the same id value will be
//// returned because id retains its value between function calls.
template <typename TComponent>
class Component: public IComponent {
    public:
        // We use `static` here to retain the value across all calls of all instances
        // of the same type of Component
        static int GetId() {
            static auto id = nextId++;
            return id;
        }
};

//////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////// System ///////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
class System {
    private:
        Signature componentSignature;
        std::vector<Entity> entities;

    public:
        System() = default; // Default constructor
        ~System() = default; // Default destructor

        //////// Entities ////////
        void AddEntityToSystem(Entity entity);
        void RemoveEntityFromSystem(Entity entity);
        const std::vector<Entity>& GetSystemEntities() const;

        //////// Components ////////
        const Signature& GetComponentSignature() const;
        template <typename TComponent> void RequireComponent();
};

/**
 * Defines the component type that the entities have to be for them to be
 * considered by the system.
*/
template <typename TComponent>
void System::RequireComponent() {
    const auto componentId = Component<TComponent>::GetId();
    componentSignature.set(componentId);
};

////////////////////////////////////////////////////////////////////////
/////////////////////////////// Registry ///////////////////////////////
////////////////////////////////////////////////////////////////////////
/// The Registry is the one in charge of creation and destruction of
/// entities, adding systems and adding components to the systems.
////////////////////////////////////////////////////////////////////////
class Registry {
    private:
        /////////////////
        // Entity MGMT //
        /////////////////
        int entityCount = 0;
        // Entities awaiting to be created/deleted un the next Registry Update() call.
        std::set<Entity> entitiesToBeSpawned;
        std::set<Entity> entitiesToBeKilled;

        ///////////////////////////
        // Component Pool vector //
        ///////////////////////////
        // Each Pool Contains all the data for a certain component type
        // [ComponentId1 => [EntityId => {ComponentId1 data for this entity}, ...], ...]
        std::vector<std::shared_ptr<IPool>> componentPools;

        ////////////////////////////////
        // Component Signature vector //
        ////////////////////////////////
        // [entityId => ComponentSignature]
        std::vector<Signature> entityComponentSignatures;

        ////////////////
        // System Map //
        ////////////////
        std::unordered_map<std::type_index, std::shared_ptr<System>> systems;


    public:
        Registry() = default;
        ~Registry();

        //////// Entities ////////
        Entity SpawnEntity();
        void KillEntity(Entity entity);

        //////// Entity-Components ////////
        template <typename TComponent, typename ...TArgs> void AddComponentToEntity(Entity entity, TArgs&& ...args);
        template <typename TComponent> void RemoveComponentFromEntity(Entity entity);
        template <typename TComponent> bool EntityHasComponent(Entity entity) const;
        template <typename TComponent> TComponent& GetComponentFromEntity(Entity entity) const;

        //////// Entities-Systems ////////
        void AddEntityToSystems(Entity entity);

        //////// Systems ////////
        template <typename TSystem, typename ...TArgs> void AddSystem(TArgs&& ...args);
        template <typename TSystem> void RemoveSystem();
        template <typename TSystem> bool HasSystem() const;
        template <typename TSystem> TSystem& GetSystem() const;

        /**
         * Here is where we actually spawn/kill entities that are waiting to be added/removed
         * We want to wait until the end of the frame to these updates so that we don't confuse
         * the Systems with things changing in the middle of the currentframe logic
        */
        void Update();




};


//////// Components ////////

/**
 * Assigns a space for the given Component Type in the component pools
 * and creates a new Component instance with the provided values to 
 * which will correspond to a given Entity
*/
template <typename TComponent, typename ...TArgs> 
void Registry::AddComponentToEntity(Entity entity, TArgs&& ...args) {
    /// 0. Get IDs ///
    const auto componentId = Component<TComponent>::GetId();
    const auto entityId = entity.GetId();

    ///  1. Add values to the corresponding Component Pool for given Entity ///

    // 1A. Expand the component pool vector if it is too short
    if (componentId >= static_cast<int>(componentPools.size())) {
        componentPools.resize(componentId + 1, nullptr);
    }

    // 1B. Check if the component doesn't have an intialized pool yet
    if (!componentPools[componentId]) {
        componentPools[componentId] = std::make_shared<Pool<TComponent>>();
    }

    // 1C. Get ComponentPool for the Component type
    std::shared_ptr<Pool<TComponent>> componentPool = std::static_pointer_cast<Pool<TComponent>>(componentPools[componentId]);

    // 1D. Expand the ComponentPool if it is too short
    if (entityId >= componentPool->GetSize()) {
        componentPool->Resize(entityId + 1);
    }

    // 1E. Create a new Component instance that contains the args data, so we forward it
    TComponent newComponent(std::forward<TArgs>(args)...);

    // 1F. Assign the new component to the correct location of the component Pool.
    componentPool->Set(entityId, newComponent);

    /// 2. Set the component Signature of the Entity ///
    entityComponentSignatures[entityId].set(componentId);

    Logger::Log("Component of ID = " + std::to_string(Component<TComponent>::GetId()) + "  has been added to Entity of ID = " + std::to_string(entity.GetId()));

}

/**
 * Removes Component from Entity Component Signature.
 * TODO: Deallocate the actual memory of the Componentfrom the Component Pool
*/
template <typename TComponent> 
void Registry::RemoveComponentFromEntity(Entity entity) {
    // Get IDs
    const auto componentId = Component<TComponent>::GetId();
    const auto entityId = entity.GetId();
    
    // Turn off the bit for the component signature of the entity
    entityComponentSignatures[entityId].set(componentId, false);

    Logger::Log("Component of ID = " + std::to_string(Component<TComponent>::GetId()) + "  has been removed from Entity of ID = " + std::to_string(entity.GetId()));
}

/**
 * Check whether the given entity has the Component type in the 
 * Entity Component Signature.
*/
template <typename TComponent> 
bool Registry::EntityHasComponent(Entity entity) const {
    // Get IDs
    const auto componentId = Component<TComponent>::GetId();
    const auto entityId = entity.GetId();

    // Check on the Component Signature for whether the entity has a component
    return entityComponentSignatures[entityId].test(componentId);
};

/**
 * Get Compoenent Struct from a given entity
*/
template <typename TComponent> 
TComponent& Registry::GetComponentFromEntity(Entity entity) const {
    // Get IDs
    const auto componentId = Component<TComponent>::GetId();
    const auto entityId = entity.GetId();

    // Find Component from Component Pools
    std::shared_ptr<Pool<TComponent>> componentPool = std::static_pointer_cast<Pool<TComponent>>(componentPools[componentId]);

    // Return the Component Data for given entity
    return componentPool->Get(entityId);
};


//////// Systems ////////
/**
 * Creates a new System instance and adds it to the System Set
*/
template <typename TSystem, typename ...TArgs> 
void Registry::AddSystem(TArgs&& ...args) {
    // Create new System instance
    std::shared_ptr<TSystem> newSystem = std::make_shared<TSystem>(std::forward<TArgs>(args)...);

    if (newSystem == nullptr) {
        Logger::Err("Issue creating new system");
    } else {
        Logger::Success("New System Created");

        newSystem->Update(1);
    }

    systems.insert(std::make_pair(std::type_index(typeid(TSystem)), newSystem));

    Logger::Log("Size of systems is now = " + std::to_string(systems.size()));
    Logger::Log(std::type_index(typeid(TSystem)).name());
}

/**
 * Removes a System from the Set
*/
template <typename TSystem> 
void Registry::RemoveSystem() {
    auto system = systems.find(std::type_index(typeid(TSystem)));
    systems.erase(system);
}

/**
 * Checks if we have this type of systems
*/
template <typename TSystem>
bool Registry::HasSystem() const {
    return systems.find(std::type_index(typeid(TSystem))) != systems.end();
}

/**
 * Returns a reference to the specified System type
*/
template <typename TSystem> 
TSystem& Registry::GetSystem() const {
    // Get System Entry (key,val) pair
    auto systemEntry = systems.find(std::type_index(typeid(TSystem)));
    // Return the value but cast it to the TSystem first
    return *(std::static_pointer_cast<TSystem>(systemEntry->second));
}

template <typename TComponent, typename ...TArgs> void Entity::AddComponent(TArgs&& ...args) {
    // Funny enough passing an integer as the first param to AddComponentToEntity() (or any of the similar methods) 
    // Like so: registry->AddComponentToEntity<TComponent>(123, std::forward<TArgs>(args)...); doesn't result
    // in a compilation error. From what I understand it's because template argument deduction is flexible.
    // If I were to pass a string, it does complain though.
    registry->AddComponentToEntity<TComponent>(*this, std::forward<TArgs>(args)...);
}

template <typename TComponent> void Entity::RemoveComponent() {
    registry->RemoveComponentFromEntity<TComponent>(*this);
}
template <typename TComponent> bool Entity::HasComponent() const {
    return registry->EntityHasComponent<TComponent>(*this);
}
template <typename TComponent> TComponent& Entity::GetComponent() const {
    return registry->GetComponentFromEntity<TComponent>(*this);
}

#endif