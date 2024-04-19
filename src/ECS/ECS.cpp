#include "ECS.h"
#include <algorithm>

#include "../Logger/Logger.h"


//////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////// Entity ///////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
int Entity::GetId() const {
    return id;
}

//////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////// Component ////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
int IComponent::nextId = 0;


//////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////// System ///////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////

//////// Entities ////////

void System::AddEntityToSystem(Entity entity) {
    entities.push_back(entity);
}

void System::RemoveEntityFromSystem(Entity entity) {
    auto b = entities.begin();
    auto e = entities.end();
    // entities.erase(std::remove_if(b, e, entity));
    entities.erase(std::remove_if(b, e, [&entity](Entity other) {
        return entity.GetId() == other.GetId();
    }));
}

const std::vector<Entity>& System::GetSystemEntities() const {
    return entities;
}

//////// Components ////////

const Signature& System::GetComponentSignature() const {
    return componentSignature;
}

////////////////////////////////////////////////////////////////////////
/////////////////////////////// Registry ///////////////////////////////
////////////////////////////////////////////////////////////////////////

Registry::~Registry() {
    Logger::Err("Registry destructor called!");;
}

/**
 * Creates a Entity and adds it to the queue of entities to be spawned
*/
Entity Registry::SpawnEntity() {
    u_int8_t entityId;
    entityId = entityCount++;

    Entity newEntity(entityId);
    entitiesToBeSpawned.insert(newEntity);

    if (entityId >= entityComponentSignatures.size()) {
        entityComponentSignatures.resize(entityId + 1);
    }

    Logger::Success("Entity created with ID = " + std::to_string(newEntity.GetId()));

    return newEntity;
}


/**
 * DESTROYS a Entity and adds it to the queue of entities to be killed
*/
void Registry::KillEntity(Entity entity) {
    entitiesToBeKilled.insert(entity);
    entityCount--;
}

/**
 * We check the component signature of the given entity (that was slowly designed via
 * calls to Registry::AddComponentToEntity) and then we add this entity to all the systems
 * that match the component signature (which were specified via System::RequireComponent)
 * 
 * For example:
 * Component IDs = 0 -> TransformComponent, 1 -> VelocityComponent, 2 -> OtherComponent
 * Let Entity A have the component signature = 001 (from left to right) so it only has "OtherComponent"
 * Let Entity B have the component signature = 101 
 * Let Entity C have the component signature = 111 
 * Let System X have the component signature = 101 (so it requires Transform and Other Component)
 * 
 * Then, only entities B and C will be of interest to system X
 * 
 * 
*/
void Registry::AddEntityToSystems(Entity entity) {
    // Get Entity ID
    auto entityId = entity.GetId();

    // Get Entity Comp. Sig.
    const auto entityComponentSignature = entityComponentSignatures[entityId];

    for (auto& systemEntry: systems) {
        // systemEntry = pair of (key, System*)
        std::shared_ptr<System> system = systemEntry.second;
        // Get System Comp. Sig.
        const auto systemComponentSignature = system->GetComponentSignature();

        // Match the SystemComponentSignature <=> EntityComponentSignature, so
        // that the current system only cares if the entity contains all of the 
        // required components of the system
        bool isInterested = (entityComponentSignature & systemComponentSignature) == systemComponentSignature;

        // If this system is interested in the entity, we add it
        if (isInterested) {
            system->AddEntityToSystem(entity);
        }
    }   
}

/**
 * Create or Destroy Entities that are waiting on the queues
*/
void Registry::Update() {

    // Waiting to be created
    for (auto entity: entitiesToBeSpawned) {
        AddEntityToSystems(entity);
    }

    entitiesToBeSpawned.clear();
    
    // Waiting to be destroyed
    for (auto entity: entitiesToBeKilled) {

    }
}