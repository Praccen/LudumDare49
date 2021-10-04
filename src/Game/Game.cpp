#define NOMINMAX
#include "Game.hpp"

#include "Engine/Rendering.hpp"

#include <algorithm>

#include "Engine/ECS/Entity.hpp"
#include "Engine/ECS/Components/PlayerComponent.hpp"
#include "Engine/ECS/Components/PositionComponent.hpp"
#include "Engine/ECS/Components/InputComponent.hpp"
#include "Engine/ECS/Components/GraphicsComponent.hpp"

Game::Game(GLFWwindow* window):
	gameState(GameState::Menu), m_window(window), m_ECSManager(&ECSManager::getInstance()), introRunning(true), introTimer(0.0f) {

	Rendering::getInstance().getQuadManager()->getTexture(0).loadFromFile("resources/Textures/Horse.png");
	Rendering::getInstance().getQuadManager()->getTexture(1).loadFromFile("resources/Textures/bg-blurr.png");
	Rendering::getInstance().getQuadManager()->getTexture(2).loadFromFile("resources/Textures/platta2.png");
	Rendering::getInstance().getQuadManager()->getTexture(3).loadFromFile("resources/Textures/sun.png");
	Rendering::getInstance().getQuadManager()->getTexture(4).loadFromFile("resources/Textures/Stable.png");
	Rendering::getInstance().getQuadManager()->getTexture(5).loadFromFile("resources/Textures/Farmer.png");
	Rendering::getInstance().getCamera()->setZoom(1.0f/(0.5f * 15.f)); // Zoom out so that the whole map is visible
	Rendering::getInstance().getCamera()->setPosition(0.5f * 15.f, 0.5f * 15.f);
	
	setupEntities();
}

Game::~Game()
{
	m_ECSManager->reset();
	//Rendering::getInstance().~Rendering();
}

void Game::reset() {
	Rendering::getInstance().reset();
	m_ECSManager->reset();

	setupEntities();
}	

void Game::introInit() {

}

void Game::introUpdate(float dt) {
	introTimer += dt;

	if (introTimer > 5.0f) {
		finishIntro();
	}
}

void Game::finishIntro() {
	Entity* playerEntity =  m_ECSManager->getEntity(playerEntityId);
	m_ECSManager->addComponent(*playerEntity, new InputComponent(m_window));

	Entity* cameraEntity = m_ECSManager->getEntity(cameraEntityId);
	MovementComponent* cameraMovComp = static_cast<MovementComponent*>(cameraEntity->getComponent(ComponentTypeEnum::MOVEMENT));
	cameraMovComp->constantAcceleration = glm::vec3(0.7f, 0.0f, 0.0f);

	introRunning = false;
}

void Game::update(float dt) {
	m_ECSManager->update(dt);

	if (introRunning) {
		if (introTimer == 0.0f) {
			introInit();
		}
		
		introUpdate(dt);
		return;
	}

	Entity* playerEntity = m_ECSManager->getEntity(playerEntityId);
	Entity* cameraEntity = m_ECSManager->getEntity(cameraEntityId);
	Entity* backgroundEntity = m_ECSManager->getEntity(backgroundEntityId);
	Entity* sunEntity = m_ECSManager->getEntity(sunEntityId);

	MovementComponent* playerMovComp = static_cast<MovementComponent*>(playerEntity->getComponent(ComponentTypeEnum::MOVEMENT));
	PlayerComponent* playerComp = static_cast<PlayerComponent*>(m_ECSManager->getEntity(playerEntityId)->getComponent(ComponentTypeEnum::PLAYER));
	MovementComponent* cameraMovComp = static_cast<MovementComponent*>(cameraEntity->getComponent(ComponentTypeEnum::MOVEMENT));
	PositionComponent* cameraPosComp = static_cast<PositionComponent*>(cameraEntity->getComponent(ComponentTypeEnum::POSITION));
	PositionComponent* backgroundPosComp = static_cast<PositionComponent*>(backgroundEntity->getComponent(ComponentTypeEnum::POSITION));
	PositionComponent* sunPosComp = static_cast<PositionComponent*>(sunEntity->getComponent(ComponentTypeEnum::POSITION));

	if (cameraMovComp->velocity.x > 4.0f) {
		cameraMovComp->constantAcceleration.x = 0.1f;
	}

	// Animation
	GraphicsComponent* playerGraphComp = static_cast<GraphicsComponent*>(playerEntity->getComponent(ComponentTypeEnum::GRAPHICS));
	if (playerMovComp->velocity.x < 3.0f) {
		playerGraphComp->advanceBy = {1.0f, 0.0f};
		playerGraphComp->startingTile = {0.0f, 1.0f};
		playerGraphComp->modAdvancement = {8.0f, 1.0f};
		playerGraphComp->updateInterval = 0.2f;
		playerGraphComp->movementMultiplier = 0.5f;
	} else {
		playerGraphComp->advanceBy = {1.0f, 0.0f};
		playerGraphComp->startingTile = {0.0f, 0.0f};
		playerGraphComp->modAdvancement = {6.0f, 1.0f};
		playerGraphComp->updateInterval = 0.2f;
		playerGraphComp->movementMultiplier = 0.35f;
	}

	playerMovComp->wantedVelocity = cameraMovComp->velocity;
	playerMovComp->maxAcceleration.x = playerMovComp->wantedVelocity.x;

	sunPosComp->position.x = cameraPosComp->position.x - 2.5f;
	backgroundPosComp->position.x = cameraPosComp->position.x;

	// Update score
	m_score = playerComp->score;

	if (playerComp->alive == false) {
		gameState = GameState::GameOver;
	}
}

void Game::setupEntities() {
	playerEntityId = m_ECSManager->createPlayerEntity(11.f, 4.f, m_window);
	cameraEntityId = m_ECSManager->createCameraEntity();
	backgroundEntityId = m_ECSManager->createBackgroundEntity();
	sunEntityId = m_ECSManager->createSunEntity();
	m_ECSManager->createStableEntity();
	farmerEntityId = m_ECSManager->createFarmerEntity();
}