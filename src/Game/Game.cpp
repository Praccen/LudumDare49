#define NOMINMAX
#include "Game.hpp"

#include "Engine/Rendering.hpp"

#include <algorithm>

#include "Engine/ECS/Entity.hpp"
#include "Engine/ECS/Components/PlayerComponent.hpp"
#include "Engine/ECS/Components/PositionComponent.hpp"

Game::Game(GLFWwindow* window):
	gameState(GameState::Menu), m_ECSManager(&ECSManager::getInstance()) {
	Rendering::getInstance().getQuadManager()->getTexture(0).loadFromFile("resources/Textures/instanced.png");
	Rendering::getInstance().getQuadManager()->getTexture(1).loadFromFile("resources/Textures/bg-blurr.png");
	Rendering::getInstance().getQuadManager()->getTexture(2).loadFromFile("resources/Textures/platta2.png");
	Rendering::getInstance().getQuadManager()->getTexture(3).loadFromFile("resources/Textures/sun.png");
	Rendering::getInstance().getQuadManager()->getTexture(4).loadFromFile("resources/Textures/Stable.png");
	Rendering::getInstance().getCamera()->setZoom(1.0f/(0.5f * 15.f, 15.f)); // Zoom out so that the whole map is visible
	Rendering::getInstance().getCamera()->setPosition(0.5f * 15.f, 0.5f * 15.f);
	
	setupEntities(window);
}

Game::~Game()
{
	m_ECSManager->reset();
	//Rendering::getInstance().~Rendering();
}

void Game::reset(GLFWwindow *window) {
	Rendering::getInstance().reset();
	m_ECSManager->reset();

	setupEntities(window);
}	

void Game::update(float dt) {
	m_ECSManager->update(dt);
	Entity* playerEntity = m_ECSManager->getEntity(playerEntityId);
	Entity* cameraEntity = m_ECSManager->getEntity(cameraEntityId);
	Entity* sunEntity = m_ECSManager->getEntity(sunEntityId);

	MovementComponent* playerMovComp = static_cast<MovementComponent*>(playerEntity->getComponent(ComponentTypeEnum::MOVEMENT));
	PlayerComponent* playerComp = static_cast<PlayerComponent*>(m_ECSManager->getEntity(playerEntityId)->getComponent(ComponentTypeEnum::PLAYER));
	MovementComponent* cameraMovComp = static_cast<MovementComponent*>(cameraEntity->getComponent(ComponentTypeEnum::MOVEMENT));
	PositionComponent* cameraPosComp = static_cast<PositionComponent*>(cameraEntity->getComponent(ComponentTypeEnum::POSITION));
	PositionComponent* sunPosComp = static_cast<PositionComponent*>(sunEntity->getComponent(ComponentTypeEnum::POSITION));

	if (cameraMovComp->velocity.x > 4.0f) {
		cameraMovComp->constantAcceleration.x = 0.1f;
	}

	playerMovComp->wantedVelocity = cameraMovComp->velocity;
	playerMovComp->maxAcceleration.x = playerMovComp->wantedVelocity.x;

	sunPosComp->position.x = cameraPosComp->position.x - 2.5f;

	// Update score
	m_score = playerComp->score;

	if (playerComp->alive == false) {
		gameState = GameState::GameOver;
	}
}

void Game::setupEntities(GLFWwindow *window) {
	playerEntityId = m_ECSManager->createPlayerEntity(7.f, 4.f, window);
	cameraEntityId = m_ECSManager->createCameraEntity();
	sunEntityId = m_ECSManager->createSunEntity();
	m_ECSManager->createStableEntity();
}