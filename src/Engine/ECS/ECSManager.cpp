#include "ECSManager.hpp"
#include "Components/PositionComponent.hpp"
#include "Components/MovementComponent.hpp"
#include "Components/InputComponent.hpp"
#include "Components/CollisionComponent.hpp"
#include "Components/HealthComponent.hpp"
#include "Components/DamageComponent.hpp"
#include "Components/GraphicsComponent.hpp"
#include "Components/WeaponComponent.hpp"
#include "Components/SeeingComponent.hpp"
#include "Components/CameraFocusComponent.hpp"
#include "Components/PlayerComponent.hpp"
#include "Systems/MapSystem.hpp"
#include "Rendering.hpp"

std::vector<Entity*> ECSManager::m_entities;

ECSManager::ECSManager()
	:m_idCounter(1), m_systems(),
		m_addEntities(), m_addComponents(), m_removeEntities(), m_removeComponents()
{
	initializeSystems();
	m_startingPositions.push_back(glm::vec2(2, 2));
	m_startingPositions.push_back(glm::vec2(28, 2));
	m_startingPositions.push_back(glm::vec2(2, 28));
	m_startingPositions.push_back(glm::vec2(28, 28));
}

ECSManager::~ECSManager()
{
	//Delete all entities and systems
	for (auto& e : m_entities) {
		delete e;
	}
}

void ECSManager::initializeSystems() {
	m_systems["INPUT"] = std::make_shared<InputSystem>(InputSystem(this));
	m_systems["MOVEMENT"] = std::make_shared<MovementSystem>(MovementSystem(this));;
	m_systems["COLLISION"] = std::make_shared<CollisionSystem>(CollisionSystem(this));
	m_systems["SEEING"] = std::make_shared<SeeingSystem>(SeeingSystem(this));
	m_systems["HEALTH"] = std::make_shared<HealthSystem>(HealthSystem(this));
	m_systems["GRAPHICS"] = std::make_shared<GraphicsSystem>(GraphicsSystem(this));
	m_systems["WEAPON"] = std::make_shared<WeaponSystem>(WeaponSystem(this));
	m_systems["CAMERAFOCUS"] = std::make_shared<CameraSystem>(CameraSystem(this));
	m_systems["MAP"] = std::make_shared<MapSystem>(MapSystem(this));
	m_systems["ANIMATION"] = std::make_shared<AnimationSystem>(AnimationSystem(this));
	m_systems["PLAYER"] = std::make_shared<PlayerSystem>(PlayerSystem(this));

	m_systems["MAP"]->initialize();
}

void ECSManager::update(float dt)
{
	//for all entities, remove/add components
    //remove/add entities from systems
	addEntities();
	addComponents();
	removeEntities();
	removeComponents();

	//update all systems
	m_systems["INPUT"]->update(dt);
	m_systems["MOVEMENT"]->update(dt);
	m_systems["COLLISION"]->update(dt);
	m_systems["SEEING"]->update(dt);
	m_systems["HEALTH"]->update(dt);
	m_systems["GRAPHICS"]->update(dt);
	m_systems["WEAPON"]->update(dt);
	m_systems["CAMERAFOCUS"]->update(dt);
	m_systems["MAP"]->update(dt);
	m_systems["PLAYER"]->update(dt);
}

void ECSManager::updateRenderingSystems(float dt) {
	m_systems["ANIMATION"]->update(dt);
}

void ECSManager::reset() {
	//Delete all entities
	for (auto& e : m_entities) {
		delete e;
	}
	m_entities.clear();
	m_idCounter = 0;

	//re-init systems
	initializeSystems();
}

Entity& ECSManager::createEntity()
{
	Entity* e = new Entity(m_idCounter++);
	m_addEntities.push_back(e);
	return *e;
}

void ECSManager::addEntity(Entity* entity)
{
	m_addEntities.push_back(entity);
}

void ECSManager::addComponent(Entity& entity, Component* component)
{
	m_addComponents.push_back(addComponent_t{ entity, component });
}

void ECSManager::removeEntity(int entityID)
{
	m_removeEntities.push_back(entityID);
}

void ECSManager::removeComponent(Entity& entity, ComponentTypeEnum component)
{
	m_removeComponents.push_back(removeComponent_t{ entity, component });
}

Entity* ECSManager::getEntity(int entityID)
{
	for (auto& entity : m_entities) {
		if (entity->getID() == entityID) {
			return entity;
		}
	}
	return nullptr;
}

//PRIVATE

void ECSManager::addEntities()
{
	for (auto& newEntity : m_addEntities) {
		//add to manager
		m_entities.push_back(newEntity);


		//add to systems
		for (auto& system : m_systems) {
			system.second->addEntity(m_entities.back());
		}
	}
	m_addEntities.clear();
}

void ECSManager::addComponents()
{
	for (auto& components : m_addComponents) {

		//if enitity does not already have component, proceed
		if (components.ent.addComponent(components.cmp)) {

			for (auto& system : m_systems) {
				//if entity is not already belonging to the system, try and add it
				if (!system.second->containsEntity(components.ent.getID())) {
					system.second->addEntity(&components.ent);
				}
			}
		}
	}
	m_addComponents.clear();
}

void ECSManager::removeEntities()
{
	for (auto &i : m_removeEntities) {

		//delete in systems
		for (auto& system : m_systems) {
			system.second->removeEntity(i);
		}

		//delete in manager
		for (unsigned int j = 0; j < m_entities.size(); j++) {
			if (m_entities[j]->getID() == i) {
				delete m_entities[j];
				m_entities.erase(m_entities.begin() + j);
			}
		}
	}
	m_removeEntities.clear();
}

void ECSManager::removeComponents()
{
	for (auto& components : m_removeComponents) {
		components.ent.removeComponent(components.cmp);
		for (auto& system : m_systems) {
			system.second->removeFaultyEntity(components.ent.getID());
		}
	}
	m_removeComponents.clear();
}

const int ECSManager::createPlayerEntity(float x, float y, GLFWwindow* window) {

	Entity &playerEntity = createEntity();
	playerEntity.setName("Player");
	// Add components to player
	PositionComponent* posComp = new PositionComponent(x, y);
	posComp->scale = glm::vec3(-2.0f, (34.0f/60.0f) * 2.0f, 1.0f);
	addComponent(playerEntity, posComp);
	MovementComponent *movComp = new MovementComponent();
	movComp->constantAcceleration = glm::vec3(0.0f, -9.82f, 0.0f);
	movComp->wantedVelocity = glm::vec3(4.0f, 0.0f, 0.0f);
	addComponent(playerEntity, movComp);
	addComponent(playerEntity, new InputComponent(window));
	addComponent(playerEntity, new CollisionComponent());
	addComponent(playerEntity, new DamageComponent());
	GraphicsComponent *graphComp = new GraphicsComponent();
	graphComp->quad->setTextureIndex(0);
	graphComp->quad->setNrOfSprites(6.0f, 1.0f);
	graphComp->quad->setCurrentSprite(0.0f, 1.0f);
	graphComp->animate = true;
	graphComp->advanceBy = {1.0f, 0.0f};
	graphComp->startingTile = {0.0f, 0.0f};
	graphComp->modAdvancement = {6.0f, 1.0f};
	graphComp->updateInterval = 0.2f;
	graphComp->movementMultiplier = 0.35f;
	addComponent(playerEntity, graphComp);
	addComponent(playerEntity, new WeaponComponent());
	addComponent(playerEntity, new PlayerComponent());
	addComponent(playerEntity, new DamageComponent());
	return playerEntity.getID();
}

const int ECSManager::createCameraEntity() {
	Entity &cameraEntity = createEntity();
	
	PositionComponent* posComp = new PositionComponent();
	posComp->position.z = 0.0f;
	posComp->scale = {40.0f, 20.0f, 1.0f};
	addComponent(cameraEntity, posComp);
	MovementComponent *movComp = new MovementComponent();
	movComp->constantAcceleration = glm::vec3(0.1f, 0.0f, 0.0f);
	movComp->velocity = glm::vec3(4.0f, 0.0f, 0.0f);
	addComponent(cameraEntity, movComp);
	addComponent(cameraEntity, new CameraFocusComponent());
	GraphicsComponent* graphComp = new GraphicsComponent();
	graphComp->quad->setTextureIndex(1);
	graphComp->quad->setNrOfSprites(1.0f, 1.0f);
	graphComp->animate = true;
	graphComp->advanceBy = {0.00001f, 0.0f};
	graphComp->startingTile = {0.0f, 0.0f};
	graphComp->modAdvancement = {1.0f, 1.0f};
	graphComp->updateInterval = 0.001f;
	graphComp->movementMultiplier = 0.01f;
	Rendering::getInstance().getQuadManager()->getTexture(1).setTexParameters(GL_TEXTURE_WRAP_S, GL_REPEAT);
	addComponent(cameraEntity, graphComp);

	return cameraEntity.getID();
}

const int ECSManager::createSunEntity() {
	Entity &sunEntity = createEntity();
	PositionComponent* posComp = new PositionComponent();
	posComp->position.z = 0.01f;
	posComp->scale = {20.0f, 10.0f, 1.0f};
	addComponent(sunEntity, posComp);
	MovementComponent *movComp = new MovementComponent();
	movComp->constantAcceleration = glm::vec3(0.1f, 0.0f, 0.0f);
	movComp->velocity = glm::vec3(4.0f, 0.0f, 0.0f);
	addComponent(sunEntity, movComp);
	addComponent(sunEntity, new CameraFocusComponent());
	GraphicsComponent* graphComp = new GraphicsComponent();
	graphComp->quad->setTextureIndex(3);
	graphComp->quad->setNrOfSprites(1.0f, 1.0f);
	addComponent(sunEntity, graphComp);

	return sunEntity.getID();
}