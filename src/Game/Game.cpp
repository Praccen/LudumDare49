#define NOMINMAX
#include "Game.hpp"

#include "Engine/Rendering.hpp"

#include <algorithm>

#include "Engine/ECS/Entity.hpp"

Game::Game(GLFWwindow* window):
	gameState(GameState::Menu), m_ECSManager(&ECSManager::getInstance())
{
	Rendering::getInstance().getQuadManager()->getTexture().loadFromFile("resources/Textures/instanced.png");
	Rendering::getInstance().getCamera()->setZoom(1.0f/(0.5f * 15.f, 15.f)); // Zoom out so that the whole map is visible
	Rendering::getInstance().getCamera()->setPosition(0.5f * 15.f, 0.5f * 15.f);
	
	playerEntityId = m_ECSManager->createPlayerEntity(7.f, 4.f, window);
	cameraEntityId = m_ECSManager->createCameraEntity();
	}	

void Game::update(float dt) {
	m_ECSManager->update(dt);
	Entity* playerEntity = m_ECSManager->getEntity(playerEntityId);
	Entity* cameraEntity = m_ECSManager->getEntity(cameraEntityId);

	MovementComponent* playerMovComp = static_cast<MovementComponent*>(playerEntity->getComponent(ComponentTypeEnum::MOVEMENT));
	MovementComponent* cameraMovComp = static_cast<MovementComponent*>(cameraEntity->getComponent(ComponentTypeEnum::MOVEMENT));
	playerMovComp->wantedVelocity = cameraMovComp->velocity;
	playerMovComp->maxAcceleration.x = playerMovComp->wantedVelocity.x;
}
